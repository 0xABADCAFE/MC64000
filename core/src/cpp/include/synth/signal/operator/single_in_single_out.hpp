#ifndef MC64K_SYNTH_SIGNAL_SINGLE_IN_SINGLE_OUT_HPP
    #define MC64K_SYNTH_SIGNAL_SINGLE_IN_SINGLE_OUT_HPP

/**
 *   888b     d888  .d8888b.   .d8888b.      d8888  888    d8P
 *   8888b   d8888 d88P  Y88b d88P  Y88b    d8P888  888   d8P
 *   88888b.d88888 888    888 888          d8P 888  888  d8P
 *   888Y88888P888 888        888d888b.   d8P  888  888d88K
 *   888 Y888P 888 888        888P "Y88b d88   888  8888888b
 *   888  Y8P  888 888    888 888    888 8888888888 888  Y88b
 *   888   "   888 Y88b  d88P Y88b  d88P       888  888   Y88b
 *   888       888  "Y8888P"   "Y8888P"        888  888    Y88b
 *
 *    - 64-bit 680x0-inspired Virtual Machine and assembler -
 */

#include <cmath>
#include <synth/signal.hpp>

namespace MC64K::Synth::Audio::Signal::Operator {

/**
 * Abstract base class for simple inline operators that consume one input stream and produce one output.
 */
class SingleInSingleOut : public virtual IStream, public TStreamCommon, protected TPacketIndexAware {
    protected:
        IStream::Ptr    oSourceInputPtr;
        Packet::Ptr     oLastPacketPtr;
        IStream*        poSourceInput;

    public:
        SingleInSingleOut(IStream& roSourceInput);
        SingleInSingleOut(IStream::Ptr const& roSourceInputPtr);
        ~SingleInSingleOut();

        /**
         * @inheritDoc
         */
        bool canEnable() const noexcept override {
            return poSourceInput != nullptr;
        }

        /**
         * @inheritDoc
         */
        SingleInSingleOut* reset() noexcept override;

        SingleInSingleOut* enable() noexcept override {
            TStreamCommon::enable();
            if (bEnabled && poSourceInput) {
                poSourceInput->enable();
            }
            return this;
        }

        /**
         * @inheritDoc
         */
        Packet::ConstPtr emit(size_t uIndex = 0) noexcept override;

        size_t getPosition() const noexcept override {
            if (poSourceInput) {
                return poSourceInput->getPosition();
            }
            return uSamplePosition;
        }

        SingleInSingleOut* setInputStream(IStream& roNewSource) noexcept {
            poSourceInput = &roNewSource;
            if (bEnabled) {
                bEnabled = (poSourceInput != nullptr);
            }
            std::fprintf(stderr, "SingleInSingleOut::setSourceInput(%p)\n", poSourceInput);
            return this;
        }

        SingleInSingleOut* setInputStream(IStream::Ptr const& roNewSourcePtr) noexcept {
            oSourceInputPtr = roNewSourcePtr;
            poSourceInput   = oSourceInputPtr.get();
            if (bEnabled) {
                bEnabled = (poSourceInput != nullptr);
            }
            std::fprintf(stderr, "SingleInSingleOut::setSourceInput(%p)\n", poSourceInput);
            return this;
        }

    protected:
        /**
         * Generate a new packet. This is called by emit() when it is determined that
         * the packet we are being asked for is not the one we last calculated.
         *
         * @return Packet::ConstPtr
         */
        Packet::ConstPtr emitNew() noexcept;
};

} // namespace

#endif

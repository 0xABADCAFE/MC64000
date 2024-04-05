#ifndef MC64K_SYNTH_SIGNAL_LEVELADJUST_HPP
    #define MC64K_SYNTH_SIGNAL_LEVELADJUST_HPP

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
 * Fixed level adjustment, for attenuation or amplification.
 */
class LevelAdjust : public TStreamCommon, protected TPacketIndexAware {
    private:
        IStream::Ptr    oSourceInputPtr;
        Packet::Ptr     oLastPacketPtr;
        IStream*        poSourceInput;

        float32     fOutputLevel;
        float32     fOutputBias;
        bool        bMuted;

    public:
        LevelAdjust(IStream& roSourceInput, float32 fOutputLevel = 1.0f, float32 fOutputBias = 0.0f);
        LevelAdjust(IStream::Ptr const& roSourceInputPtr, float32 fOutputLevel = 1.0f, float32 fOutputBias = 0.0f);
        ~LevelAdjust();

        /**
         * @inheritDoc
         */
        bool canEnable() const noexcept override {
            return poSourceInput != nullptr;
        }

        /**
         * @inheritDoc
         */
        LevelAdjust* reset() noexcept override;

        LevelAdjust* enable() noexcept override {
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


        /**
         * Returns the current output level.
         *
         * @return float32
         */
        float32 getOutputLevel() const noexcept {
            return fOutputLevel;
        }

        float32 getOutputBias() const noexcept {
            return fOutputBias;
        }

        LevelAdjust* setOutputLevel(float32 fNewOutputLevel) noexcept {
            bMuted = std::fabs( (fOutputLevel = fNewOutputLevel) ) < 1e-5f;
            return this;
        }

        LevelAdjust* setSourceInput(IStream& roNewSource) noexcept {
            poSourceInput = &roNewSource;
            if (bEnabled) {
                bEnabled = (poSourceInput != nullptr);
            }
            std::fprintf(stderr, "LevelAdjust::setSourceInput(%p)\n", poSourceInput);
            return this;
        }

        LevelAdjust* setSourceInput(IStream::Ptr const& roNewSourcePtr) noexcept {
            oSourceInputPtr = roNewSourcePtr;
            poSourceInput   = oSourceInputPtr.get();
            if (bEnabled) {
                bEnabled = (poSourceInput != nullptr);
            }
            std::fprintf(stderr, "LevelAdjust::setSourceInput(%p)\n", poSourceInput);
            return this;
        }

        typedef std::shared_ptr<LevelAdjust> Ptr;
        typedef std::shared_ptr<LevelAdjust const> ConstPtr;


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

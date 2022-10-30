#ifndef MC64K_STANDARD_TEST_HOST_SYNTH_SIGNAL_WAVE_SQUARE_HPP
    #define MC64K_STANDARD_TEST_HOST_SYNTH_SIGNAL_WAVE_SQUARE_HPP

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

#include "constants.hpp"

namespace MC64K::Synth::Audio::Signal::Waveform {

/**
 * Basic square wave
 */
class Square : public IWaveform {
    public:
        ~Square();

        /**
         * @inheritDoc
         */
        float32 getPeriod() const {
            return TWO;
        }

        /**
         * @inheritDoc
         */
        Packet::Ptr map(Packet::ConstPtr pInput);

        /**
         * @inheritDoc
         */
        float32 value(float32 fTime) const {
            union {
                int32   iResult;
                float32 fResult;
            };
            iResult = ONE_IEEE_32 | ((int32)std::floor(fTime) & 1) << 31;
            return fResult;
        };

};

/**
 * Basic fixed duty cycle PWM
 */
class FixedPWM : public IWaveform {
    private:
        float32 fWidth;

    public:
        static constexpr float32 const MIN_WIDTH = (float32)(1.0/16.0);
        static constexpr float32 const MAX_WIDTH = (float32)(15.0/16.0);

        ~FixedPWM();

        /**
         * @inheritDoc
         */
        float32 getPeriod() const {
            return ONE;
        }

        /**
         * Returns the current PWM duty cycle.
         */
        float32 getWidth() const {
            return fWidth;
        }

        /**
         * Sets the new PWN duty cycle
         */
        void setWidth(float32 fNewWidth) {
            fWidth = fNewWidth < MIN_WIDTH ? MIN_WIDTH : (fNewWidth > MAX_WIDTH ? MAX_WIDTH : fNewWidth);
        }

        /**
         * @inheritDoc
         */
        Packet::Ptr map(Packet::ConstPtr pInput);

        /**
         * @inheritDoc
         */
        float32 value(float32 fTime) const {
            union {
                int32   iResult;
                float32 fResult;
            };
            fResult = std::ceil(fTime) - fTime - fWidth;
            iResult = ONE_IEEE_32 | (iResult & 0x80000000);
            return fResult;
        };

        FixedPWM(float32 fWidth) {
            setWidth(fWidth);
        }

};

}

#endif

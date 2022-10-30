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
#include <cstdio>
#include <synth/signal.hpp>


#include <synth/signal/waveform/sine.hpp>
namespace MC64K::Synth::Audio::Signal::Waveform {

Packet::Ptr SineGold::map(Packet::ConstPtr pInput) {
    auto pOutput = Packet::create();
    float32* pDest      = pOutput->aSamples;
    float32 const* pSrc = pInput->aSamples;
    for (unsigned i = 0; i < PACKET_SIZE; ++i) {
        pDest[i] = std::sin(pSrc[i]);
    }
    return pOutput;
}

/**
 * @inheritDoc
 */
SineGold::~SineGold() {
    std::printf("Destroyed SineGold\n");
}

/**
 * @inheritDoc
 *
 * Branchless techniques used here to improve throughput.
 */
Packet::Ptr SineFast::map(Packet::ConstPtr pInput) {
    auto pOutput = Packet::create();
    float32* pDest      = pOutput->aSamples;
    float32 const* pSrc = pInput->aSamples;

    union {
        int32   iPi;
        float32 fPi;
    };

    for (unsigned i = 0; i < PACKET_SIZE; ++i) {
        float32 fTime  = pSrc[i] - HALF;
        float32 fFloor = std::floor(fTime);

        // Branchless selection of pi/-pi
        iPi = PI_IEEE_32|(((int32)fFloor) & 1) << 31; // set sign bit when odd
        pDest[i] = OUTPUT_ADJUST * sinApprox(fPi * (fTime - fFloor - HALF));
    }
    return pOutput;
}

/**
 * @inheritDoc
 */
SineFast::~SineFast() {
    std::printf("Destroyed SineFast\n");
}
}


///////////////////////////////////////////////////////////////////////////////////////////////////

#include <synth/signal/waveform/triangle.hpp>
namespace MC64K::Synth::Audio::Signal::Waveform {

/**
 * @inheritDoc
 */
Packet::Ptr Triangle::map(Packet::ConstPtr pInput) {
    auto pOutput = Packet::create();
    float32* pDest      = pOutput->aSamples;
    float32 const* pSrc = pInput->aSamples;

    union {
        int32   iTwo;
        float32 fTwo;
    };

    for (unsigned i = 0; i < PACKET_SIZE; ++i) {
        float32 fTime  = pSrc[i] - HALF;
        float32 fFloor = std::floor(fTime);
        iTwo = TWO_IEEE_32 | (((int32)fFloor) & 1) << 31;
        pDest[i] = -fTwo * (fTime - fFloor - HALF);
    }
    return pOutput;
}

/**
 * @inheritDoc
 */
Triangle::~Triangle() {
    std::printf("Destroyed Triangle\n");
}
}

///////////////////////////////////////////////////////////////////////////////////////////////////

#include <synth/signal/waveform/saw.hpp>
namespace MC64K::Synth::Audio::Signal::Waveform {

/**
 * @inheritDoc
 */
Packet::Ptr SawDown::map(Packet::ConstPtr pInput) {
    auto pOutput = Packet::create();
    float32* pDest      = pOutput->aSamples;
    float32 const* pSrc = pInput->aSamples;
    for (unsigned i = 0; i < PACKET_SIZE; ++i) {
        float32 fSrc = pSrc[i] + HALF;
        pDest[i] = TWO * ((float32)std::ceil(fSrc) - fSrc - HALF);
    }
    return pOutput;
}

/**
 * @inheritDoc
 */
SawDown::~SawDown() {
    std::printf("Destroyed SawDown\n");
}

/**
 * @inheritDoc
 */
Packet::Ptr SawUp::map(Packet::ConstPtr pInput) {
    auto pOutput = Packet::create();
    float32* pDest      = pOutput->aSamples;
    float32 const* pSrc = pInput->aSamples;
    for (unsigned i = 0; i < PACKET_SIZE; ++i) {
        float32 fSrc = pSrc[i] + HALF;
        pDest[i] = TWO * (fSrc - (float32)std::floor(fSrc) - HALF);
    }
    return pOutput;
}

/**
 * @inheritDoc
 */
SawUp::~SawUp() {
    std::printf("Destroyed SawUp\n");
}

}

///////////////////////////////////////////////////////////////////////////////////////////////////

#include <synth/signal/waveform/square.hpp>
namespace MC64K::Synth::Audio::Signal::Waveform {

/**
 * @inheritDoc
 *
 * Branchless techniques used here to improve throughput.
 */
Packet::Ptr Square::map(Packet::ConstPtr pInput) {
    auto pOutput = Packet::create();
    int32* pDest = (int32*)pOutput->aSamples;
    float32 const* pSrc = pInput->aSamples;
    for (unsigned i = 0; i < PACKET_SIZE; ++i) {
        pDest[i] = ONE_IEEE_32 | ((int32)std::floor(pSrc[i]) & 1) << 31;
    }
    return pOutput;
}

/**
 * @inheritDoc
 */
Square::~Square() {
   std::printf("Destroyed Square\n");
}

/**
 * @inheritDoc
 */
Packet::Ptr FixedPWM::map(Packet::ConstPtr pInput) {
    auto pOutput = Packet::create();
    int32* pDest = (int32*)pOutput->aSamples;
    float32 const* pSrc = pInput->aSamples;
    union {
        int32   iResult;
        float32 fResult;
    };
    for (unsigned i = 0; i < PACKET_SIZE; ++i) {
        fResult = std::ceil(pSrc[i]) - pSrc[i] - fWidth;
        pDest[i] = ONE_IEEE_32 | (iResult & 0x80000000);
    }
    return pOutput;
}

/**
 * @inheritDoc
 */
FixedPWM::~FixedPWM() {
   std::printf("Destroyed FixedPWM\n");
}

}

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace MC64K::Synth::Audio::Signal::Waveform {

/**
 * Transformer
 */
class XForm : public IWaveform {

    private:
        static constexpr float32 const PERIOD = 4.0f;

        enum {
            PHASE_MUL = 0,
            PHASE_ADD = 1,
            LEVEL_MUL = 2,
            LEVEL_ADD = 3
        };

        float32 aTransform[16] __attribute__ ((aligned (16))) = {
            // phase multiplier, phase displacement, scale multiplier, bias
           1.0f, 0.0f, 1.0f, 0.0f,  // First  Quadrant
           1.0f, 1.0f, 1.0f, 0.0f,  // Second Quadrant
           1.0f, 2.0f, 1.0f, 0.0f,  // Third  Quadrant
           1.0f, 3.0f, 1.0f, 0.0f,  // Fourth Quadrant

        };

        Ptr     pSourceWaveform;
        float32 fPeriodAdjust;
    public:
        XForm(Ptr pSourceWaveform, float32 const* pTransform);
        ~XForm();

        /**
         * @inheritDoc
         */
        float32 getPeriod() const {
            return PERIOD;
        }

        /**
         * @inheritDoc
         */
        Packet::Ptr map(Packet::ConstPtr pInput);

        /**
         * @inheritDoc
         */
        float32 value(float32 fTime) const {
            return fTime; // todo
        };

};

XForm::XForm(IWaveform::Ptr pSourceWaveform, float32 const* pCustomTransform) : pSourceWaveform(pSourceWaveform) {
    fPeriodAdjust = pSourceWaveform->getPeriod() / PERIOD;
    if (pCustomTransform) {
        for (unsigned i = 0; i < 16; ++i) {
            aTransform[i] = pCustomTransform[i];
        }
    }
}

XForm::~XForm() {
    std::printf("Destroyed XForm\n");
}

/**
 * @inheritDoc
 */
Packet::Ptr XForm::map(Packet::ConstPtr pInput) {
    auto pReshaped = Packet::create();
    float32* pDest      = pReshaped->aSamples;
    float32 const* pSrc = pInput->aSamples;

    // Apply input phase modification
    for (unsigned i = 0; i < PACKET_SIZE; ++i) {
        float32 fVal   = pSrc[i];
        float32 fFloor = std::floor(fVal);
        int32   iPhase = (int32)fFloor;
        float32 const* aQuadrant = aTransform + ((iPhase & 3) << 2);

        pDest[i] = fPeriodAdjust * (
            aQuadrant[PHASE_MUL] * (fVal-fFloor) + aQuadrant[PHASE_ADD]
        );
    }

    pReshaped = pSourceWaveform->map(pReshaped);

    // Apply the output amplitude modification
    pDest = pReshaped->aSamples;
    for (unsigned i = 0; i < PACKET_SIZE; ++i) {
        float32 const* aQuadrant = aTransform + ((((int32)std::floor(pSrc[i])) & 3) << 2);
        pDest[i] = (pDest[i] * aQuadrant[LEVEL_MUL]) + aQuadrant[LEVEL_ADD];
    }

    return pReshaped;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

class NoopDeleter {
    public:
        void operator()(IWaveform* pWaveform) const {
            std::printf("Not deleting global waveform at %p\n", pWaveform);
        }
};

NoopDeleter oNoDelete;

///////////////////////////////////////////////////////////////////////////////////////////////////

// Fixed waveforms that can be allocated once
SineFast    oSine;
Triangle    oTriangle;
SawDown     oSawDown;
SawUp       oSawUp;
Square      oSquare;
FixedPWM    oPWM10(0.1f);
FixedPWM    oPWM20(0.2f);
FixedPWM    oPWM30(0.3f);
FixedPWM    oPWM40(0.4f);


// Custom waveforms based on transformations
float32 const aPokey[16] = {
    1.0f, 2.0f, 0.25f, 1.0f,  // First  Quadrant
    1.0f, 1.0f, 0.05f, 0.7f,  // Second Quadrant
    1.0f, 2.0f, -0.25f, -1.0f,  // Third  Quadrant
    1.0f, 1.0f, -0.05f, -0.7f,  // Fourth Quadrant
};
XForm oPokey(IWaveform::Ptr(&Waveform::oSine, Waveform::oNoDelete), aPokey);

// float32 const aCut[16] = {
//     1.0f, 0.0f, -1.0f, 0.0f,  // First  Quadrant
//     1.0f, 1.0f, -1.0,  0.0f,  // Second Quadrant
//     1.0f, 2.0f, 1.0f,  0.0f,  // Third  Quadrant
//     1.0f, 3.0f, 1.0f,  0.0f,  // Fourth Quadrant
// };
// XForm oSineCut(IWaveform::Ptr(&Waveform::oSine, Waveform::oNoDelete), aCut);

}

///////////////////////////////////////////////////////////////////////////////////////////////////


namespace MC64K::Synth::Audio::Signal {

IWaveform::Ptr IWaveform::get(IWaveform::FixedShape eShape) {

    switch (eShape) {
        default:
        case IWaveform::SINE:
            return IWaveform::Ptr(&Waveform::oSine, Waveform::oNoDelete);
        case IWaveform::TRIANGLE:
            return IWaveform::Ptr(&Waveform::oTriangle, Waveform::oNoDelete);
        case IWaveform::SAW_DOWN:
            return IWaveform::Ptr(&Waveform::oSawDown, Waveform::oNoDelete);
        case IWaveform::SAW_UP:
            return IWaveform::Ptr(&Waveform::oSawUp, Waveform::oNoDelete);
        case IWaveform::SQUARE:
            return IWaveform::Ptr(&Waveform::oSquare, Waveform::oNoDelete);
        case IWaveform::PULSE_10:
            return IWaveform::Ptr(&Waveform::oPWM10, Waveform::oNoDelete);
        case IWaveform::PULSE_20:
            return IWaveform::Ptr(&Waveform::oPWM20, Waveform::oNoDelete);
        case IWaveform::PULSE_30:
            return IWaveform::Ptr(&Waveform::oPWM30, Waveform::oNoDelete);
        case IWaveform::PULSE_40:
            return IWaveform::Ptr(&Waveform::oPWM40, Waveform::oNoDelete);
        case IWaveform::POKEY:
            return IWaveform::Ptr(&Waveform::oPokey, Waveform::oNoDelete);
    }
}

}




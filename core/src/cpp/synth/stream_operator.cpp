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
#include <synth/signal/operator/mixer.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////

#include <synth/signal/operator/mixer.hpp>
namespace MC64K::Synth::Audio::Signal::Operator {

SimpleMixer::SimpleMixer(float32 fOutputLevel): fOutputLevel(fOutputLevel) {
    std::fprintf(stderr, "Created SimpleMixer at %p\n", this);
}

SimpleMixer::~SimpleMixer() {
    std::fprintf(stderr, "Destroyed SimpleMixer at %p\n", this);
}

/**
 * @inheritDoc
 *
 * Zero out
 */
SimpleMixer* SimpleMixer::reset() {
    uLastIndex = 0;
    uSamplePosition = 0;
    if ( auto p = pLastPacket.get() ) {
        p->clear();
    }
    std::fprintf(stderr, "SimpleMixer %p reset()\n", this);
    for (auto pPair = oChannels.begin(); pPair != oChannels.end(); ++pPair) {
        pPair->second.pSource->reset();
        std::fprintf(
            stderr,
            "\tResetting input ID:%lu [%p]\n",
            pPair->first,
            pPair->second.pSource.get()
        );
    }
    return this;
}

/**
 * @inheritDoc
 */
Packet::ConstPtr SimpleMixer::emit(size_t uIndex) {
    if (!bEnabled || 0 == oChannels.size()) {
        return Packet::getSilence();
    }
    if (useLast(uIndex)) {
        return pLastPacket;
    }
    return emitNew();
}

Packet::ConstPtr SimpleMixer::emitNew() {
    if (!pLastPacket.get()) {
        pLastPacket = Packet::create();
    }
    Packet* pOutput = pLastPacket.get();
    pOutput->clear();
    for (auto pPair = oChannels.begin(); pPair != oChannels.end(); ++pPair) {
        if (pPair->second.pSource->isEnabled()) {
            pOutput->accumulate(
                pPair->second.pSource->emit(uLastIndex),
                pPair->second.fLevel
            );
        }
    }
    pOutput->scaleBy(fOutputLevel);
    return pLastPacket;
}

/**
 * Set the current output level.
 *
 * @param  float32 fLevel
 * @return this
 */
SimpleMixer* SimpleMixer::setOutputLevel(float32 fLevel) {
    fOutputLevel = fLevel;
    return this;
}

/**
 * Attach (or replace) an input stream. If the stream pointer is empty
 * no action is taken.
 *
 * @param  ChannelID uID
 * @param  IStream::Ptr pSource
 * @param  float32 fLevel
 * @return this
 */
SimpleMixer* SimpleMixer::addInputStream(SimpleMixer::ChannelID uID, IStream::Ptr pSource, float32 fLevel) {
    if (pSource.get()) {
        std::fprintf(stderr, "SimpleMixer %p addInputStream() setting %p [ID:%lu]\n", this, pSource.get(), uID);
        Channel& roChannel = oChannels[uID];
        roChannel.pSource  = pSource;
        roChannel.fLevel   = fLevel;
    } else {
        std::fprintf(stderr, "SimpleMixer %p addInputStream() not adding empty stream [ID:%lu]\n", this, uID);
    }
    return this;
}

/**
 *  Removes an input stream, if it is attached.
 */
SimpleMixer* SimpleMixer::removeIputStream(SimpleMixer::ChannelID uID) {
    oChannels.erase(uID);
    return this;
}

/**
 * Get the level for an input. Returns zero for any unrecognised input ID.
 *
 * @param  ChannelID uChannelID
 * @return float32
 */
float32 SimpleMixer::getInputLevel(SimpleMixer::ChannelID uID) const {
    auto pChannel = oChannels.find(uID);
    if (pChannel != oChannels.end()) {
        return pChannel->second.fLevel;
    }
    return 0.0f;
}

/**
 * Set the level for an input. If the input channel ID is unrecognised, no
 * action is taken.
 *
 * @param  ChannelID uChannelID
 * @return float32
 */
SimpleMixer* SimpleMixer::setInputLevel(SimpleMixer::ChannelID uID, float32 fLevel) {
    auto pChannel = oChannels.find(uID);
    if (pChannel != oChannels.end()) {
        pChannel->second.fLevel = fLevel;
    }
    return this;
}

} // namespace

///////////////////////////////////////////////////////////////////////////////////////////////////

#include <synth/signal/operator/automute.hpp>
namespace MC64K::Synth::Audio::Signal::Operator {

/**
 * @inheritDoc
 */
AutoMuteSilence::AutoMuteSilence(
    IStream::Ptr pInput,
    float32 fDuration,
    float32 fRMSLevel
) {
    setStream(pInput);
    setDuration(fDuration);
    setThreshold(fRMSLevel);
    std::fprintf(stderr, "Created AutoMuteSilence at %p\n", this);
}

/**
 * @inheritDoc
 */
AutoMuteSilence::~AutoMuteSilence() {
    std::fprintf(stderr, "Destroyed AutoMuteSilence at %p\n", this);
}

/**
 * @inheritDoc
 */
size_t AutoMuteSilence::getPosition() {
    return pSource.get() ? pSource->getPosition() : 0;
}

/**
 * @inheritDoc
 */
bool AutoMuteSilence::isEnabled() {
    return bEnabled;
}

/**
 * @inheritDoc
 */
AutoMuteSilence* AutoMuteSilence::enable() {
    if (pSource.get()) {
        fLastTotalSquared  = 0.0;
        uSilentPacketCount = 0;
        bEnabled = true;
    } else {
        bEnabled = false;
    }
    return this;
}

/**
 * @inheritDoc
 */
AutoMuteSilence* AutoMuteSilence::disable() {
    bEnabled = false;
    return this;
}

/**
 * @inheritDoc
 */
AutoMuteSilence* AutoMuteSilence::reset() {
    if (pSource.get()) {
        pSource->reset();
        fLastTotalSquared  = 0.0;
        uSilentPacketCount = 0;
        bEnabled = true;
    }
    return this;
}

/**
 * @inheritDoc
 */
Packet::ConstPtr AutoMuteSilence::emit(size_t uIndex) {
    if (bEnabled) {
        auto pPacket = pSource->emit(uIndex);
        float64 fTotalSquared = 0.0;

        float32 const* aSamples = pPacket->aSamples;
        for (unsigned u = 0; u < PACKET_SIZE; ++u) {
            float64 fSample = aSamples[u];
            fTotalSquared += fSample * fSample;
        }
        fTotalSquared *= RMS_SCALE;

        if (fTotalSquared > fLastTotalSquared) {
            // If the total is rising, keep the gate open
            fLastTotalSquared =  fTotalSquared;
            uSilentPacketCount = 0;
        } else if (fTotalSquared < fThresholdSquared) {
            // If the total is not rising and is below the threshold, start closing the gate
            if (++uSilentPacketCount > uSilentPacketLimit) {
                // Closed
                bEnabled          = false;
                fLastTotalSquared = 0.0;
            }
        } else {
            // Not rising but above the threshold, keep gate open
            uSilentPacketCount = 0;
        }
        return pPacket;
    } else {
        return Packet::getSilence();
    }
}

/**
 * @inheritDoc
 */
AutoMuteSilence* AutoMuteSilence::setDuration(float32 fSeconds) {
    uSilentPacketLimit = (fSeconds > 0.0f) ? (uint32)(fSeconds / PACKET_PERIOD) : 0;
    std::fprintf(
        stderr,
        "AutoMuteSilence::setDuration() %.03fs (%u consecutive packets)\n",
        fSeconds,
        uSilentPacketLimit
    );
    return this;
}

/**
 * @inheritDoc
 */
AutoMuteSilence* AutoMuteSilence::setThreshold(float32 fRMSLevel) {
    fThresholdSquared = (float64)fRMSLevel;
    fThresholdSquared *= fThresholdSquared;
    std::fprintf(
        stderr,
        "AutoMuteSilence::setThreshold() RMS: %.6g\n",
        fRMSLevel
    );
    return this;
}

/**
 * @inheritDoc
 */
AutoMuteSilence* AutoMuteSilence::setStream(IStream::Ptr pInput) {
    pSource = pInput;
    if (!pSource.get()) {
        disable();
    }
    return this;
}

}



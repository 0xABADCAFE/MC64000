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

namespace MC64K::Synth::Audio::Signal {

uint64 Packet::uPacketsCreated   = 0;
uint64 Packet::uPacketsDestroyed = 0;
uint64 Packet::uPeakPacketsInUse = 0;

class Packet::Deleter {
    public:
        void operator()(Packet* pPacket) const {
            Packet::destroy(pPacket);
        }
};

Packet::Ptr Packet::create() {
    Packet* pPacket = new Packet();
    uint64 uPacketsInUse = ++uPacketsCreated - uPacketsDestroyed;
    if (uPacketsInUse > uPeakPacketsInUse) {
        uPeakPacketsInUse = uPacketsInUse;
    }
    return Ptr(pPacket, Deleter());
}

void Packet::destroy(Packet* pPacket) {
    if (pPacket) {
        ++uPacketsDestroyed;
        delete pPacket;
    }
}

void Packet::dumpStats() {
    std::printf(
        "Packet statistics:\n"
        "\tCreated     : %lu\n"
        "\tDestroyed   : %lu\n"
        "\tPeak In Use : %lu\n",
        uPacketsCreated,
        uPacketsDestroyed,
        uPeakPacketsInUse
    );
}

}

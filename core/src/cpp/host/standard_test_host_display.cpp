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

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#include <host/standard_test_host_mem.hpp>
#include <host/standard_test_host_display.hpp>
#include <host/display/context.hpp>
#include <machine/register.hpp>

using MC64K::Machine::Interpreter;

namespace MC64K::StandardTestHost::Display {

char const* aFormatNames[] = {
    "8-bit CLUT",
    "32-bit ARGB",
};

Interpreter::Status runEventLoop() {
    Context* poContext = Interpreter::gpr<ABI::PTR_REG_0>().address<Context>();
    if (!poContext || !poContext->poManager) {
        Interpreter::gpr<ABI::INT_REG_0>().value<uint64>() = ABI::ERR_NULL_PTR;
        return Interpreter::RUNNING;
    }
    poContext->poManager->runEventLoop();
    return Interpreter::INITIALISED;
}

void updateDisplay() {
    Context* poContext = Interpreter::gpr<ABI::PTR_REG_0>().address<Context>();
    if (!poContext || !poContext->poManager) {
        Interpreter::gpr<ABI::INT_REG_0>().value<uint64>() = ABI::ERR_NULL_PTR;
    }
    poContext->poManager->updateDisplay();
}

void openDisplay() {
    PackedParams oParams;

    oParams.u64 = Interpreter::gpr<ABI::INT_REG_0>().value<uint64>();

    uint16 uWidth  = oParams.u16[0];
    uint16 uHeight = oParams.u16[1];
    uint16 uFlags  = oParams.u16[2];
    uint8  uFormat = oParams.u8[6];
    uint8  uRateHz = oParams.u8[7];

    std::printf(
        "openDisplay w:%d h:%04X flags:%d fmt:%d, hz:%d\n",
        (int)uWidth,
        (unsigned)uHeight,
        (int)uFlags,
        (int)uFormat,
        (int)uRateHz
    );

    if (uFormat >= PXL_MAX) {
        Interpreter::gpr<ABI::INT_REG_0>().value<uint64>()    = ERR_INVALID_FMT;
        Interpreter::gpr<ABI::PTR_REG_0>().pAny = nullptr;
        return;
    }

    if (uWidth < WIDTH_MIN || uWidth > WIDTH_MAX) {
        Interpreter::gpr<ABI::INT_REG_0>().value<uint64>() = ERR_INVALID_WIDTH;
        Interpreter::gpr<ABI::PTR_REG_0>().pAny = nullptr;
        return;
    }

    if (uHeight < HEIGHT_MIN || uHeight > HEIGHT_MAX) {
        Interpreter::gpr<ABI::INT_REG_0>().value<uint64>() = ERR_INVALID_HEIGHT;
        Interpreter::gpr<ABI::PTR_REG_0>().pAny = nullptr;
        return;
    }

    try {
        // Obtain the appropriate manager for the host. This should use RAII semantics
        Manager* poManager = createManager(
            uWidth,
            uHeight,
            uFlags,
            uFormat,
            uRateHz
        );
        Interpreter::gpr<ABI::PTR_REG_0>().pAny = poManager->getContext();
    } catch (Error& roError) {
        Interpreter::gpr<ABI::INT_REG_0>().value<uint64>() = Mem::ERR_NO_MEM;
        Interpreter::gpr<ABI::PTR_REG_0>().pAny = nullptr;
    } catch (std::bad_alloc&) {
        Interpreter::gpr<ABI::INT_REG_0>().value<uint64>() = Mem::ERR_NO_MEM;
        Interpreter::gpr<ABI::PTR_REG_0>().pAny = nullptr;
    }
}

void closeDisplay() {
    if (Context* poContext = Interpreter::gpr<ABI::PTR_REG_0>().address<Context>()) {
        delete poContext->poManager;
        poContext = nullptr;
        Interpreter::gpr<ABI::PTR_REG_0>().pAny = nullptr;
        Interpreter::gpr<ABI::INT_REG_0>().value<uint64>() = ABI::ERR_NONE;
    } else {
        Interpreter::gpr<ABI::INT_REG_0>().value<uint64>() = ABI::ERR_NULL_PTR;
    }
}

/**
 * Display::hostVector(uint8 uFunctionID)
 */
Interpreter::Status hostVector(uint8 uFunctionID) {
    Call iOperation = (Call) uFunctionID;
    switch (iOperation) {
        case INIT:
        case DONE:
            break;
        case OPEN:  openDisplay();  break;
        case CLOSE: closeDisplay(); break;
        case BEGIN:
            return runEventLoop();
            break;
        case UPDATE: updateDisplay(); break;
        default:
            std::fprintf(stderr, "Unknown Display operation %d\n", iOperation);
            return Interpreter::UNKNOWN_HOST_CALL;
            break;
    }

    return Interpreter::RUNNING;
}

} // namespace


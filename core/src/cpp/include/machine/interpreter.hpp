#ifndef __MC64K_MACHINE_INTERPETER_HPP__
#   define __MC64K_MACHINE_INTERPETER_HPP__

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

#include "scalar.hpp"
#include "register.hpp"

namespace MC64K {
namespace Machine {

/**
 * Static
 *
 * Static, single threaded interpreter model.
 */
class Interpreter {

    public:
        typedef enum {
            UNINITIALISED = 0,
            INITIALISED,
            RUNNING,
            COMPLETED,
            UNIMPLEMENTED_OPCODE
        } Status;

        enum DumpFlags {
            STATE_GPR = 1,
            STATE_FPR = 2,
            STATE_TMP = 4
        };

        static void        dumpState(const int iFlags);
        static GPRegister& gpr(const unsigned int uReg);
        static FPRegister& fpr(const unsigned int uReg);
        static void        setProgramCounter(const uint8* pNewProgramCounter);
        static void        run();

    private:
        static GPRegister   aGPR[GPRegister::MAX];
        static FPRegister   aFPR[FPRegister::MAX];
        static const uint8* pProgramCounter;
        static void*        pDstEA;
        static void*        pSrcEA;
        static void*        pTmpEA;
        static int          iCallDepth;

        static enum  OperationSize {
            SIZE_BYTE = 1,
            SIZE_WORD = 2,
            SIZE_LONG = 4,
            SIZE_QUAD = 8
        } eOperationSize;

        static Status eStatus;

        static void* decodeEffectiveAddress();
};

}} // namespace
#endif

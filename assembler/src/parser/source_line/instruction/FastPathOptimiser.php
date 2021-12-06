<?php

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

declare(strict_types = 1);

namespace ABadCafe\MC64K\Parser\SourceLine\Instruction;

use ABadCafe\MC64K;
use ABadCafe\MC64K\State;
use ABadCafe\MC64K\Defs;
use ABadCafe\MC64K\Defs\Mnemonic\IDataMove;
use ABadCafe\MC64K\Defs\Mnemonic\ILogical;
use ABadCafe\MC64K\Defs\Mnemonic\IArithmetic;
use ABadCafe\MC64K\Defs\Mnemonic\IControl;
use ABadCafe\MC64K\Tokeniser;
use ABadCafe\MC64K\Parser\EffectiveAddress;
use ABadCafe\MC64K\Utils\Log;
use ABadCafe\MC64K\Defs\EffectiveAddress\IRegisterDirect;
use ABadCafe\MC64K\Defs\EffectiveAddress\IOther;

use function \strlen, \ord, \chr, \substr, \reset, \unpack, \pack;

/**
 * FastPathOptimiser
 *
 * Identifies fast path replacements for selected bytecode sequences. Fast path replacements rearrange the opcode
 * for selected instructions that have an alternative representation in the runtime interpreter, typically skipping
 * unnecessary steps, e.g. effective address decoding, etc. A limited number of intructions support such fast path
 * optimisation, generally for dyadic register to register oeprations.
 */
class FastPathOptimiser {

    const
        FAST_INT_PREFIX   = 254,
        FAST_FLOAT_PREFIX = 255
    ;

    const DYADIC_FAST_PATH = [
        IDataMove::MOVE_L   => self::FAST_INT_PREFIX,
        IDataMove::MOVE_Q   => self::FAST_INT_PREFIX,
        ILogical::AND_L     => self::FAST_INT_PREFIX,
        ILogical::AND_Q     => self::FAST_INT_PREFIX,
        ILogical::OR_L      => self::FAST_INT_PREFIX,
        ILogical::OR_Q      => self::FAST_INT_PREFIX,
        IArithmetic::EXTB_L => self::FAST_INT_PREFIX,
        IArithmetic::EXTB_Q => self::FAST_INT_PREFIX,
        IArithmetic::EXTW_L => self::FAST_INT_PREFIX,
        IArithmetic::EXTW_Q => self::FAST_INT_PREFIX,
        IArithmetic::ADD_L  => self::FAST_INT_PREFIX,
        IArithmetic::ADD_Q  => self::FAST_INT_PREFIX,
        IArithmetic::SUB_L  => self::FAST_INT_PREFIX,
        IArithmetic::SUB_Q  => self::FAST_INT_PREFIX,
        IArithmetic::MULS_L => self::FAST_INT_PREFIX,
        IArithmetic::MULS_Q => self::FAST_INT_PREFIX,
        IArithmetic::MULU_L => self::FAST_INT_PREFIX,
        IArithmetic::MULU_Q => self::FAST_INT_PREFIX,

        IDataMove::FMOVE_S  => self::FAST_FLOAT_PREFIX,
        IDataMove::FMOVE_D  => self::FAST_FLOAT_PREFIX,
        IArithmetic::FADD_S => self::FAST_FLOAT_PREFIX,
        IArithmetic::FADD_D => self::FAST_FLOAT_PREFIX,
        IArithmetic::FSUB_S => self::FAST_FLOAT_PREFIX,
        IArithmetic::FSUB_D => self::FAST_FLOAT_PREFIX,
        IArithmetic::FNEG_S => self::FAST_FLOAT_PREFIX,
        IArithmetic::FNEG_D => self::FAST_FLOAT_PREFIX,
        IArithmetic::FMUL_S => self::FAST_FLOAT_PREFIX,
        IArithmetic::FMUL_D => self::FAST_FLOAT_PREFIX,
        IArithmetic::FDIV_S => self::FAST_FLOAT_PREFIX,
        IArithmetic::FDIV_D => self::FAST_FLOAT_PREFIX,
    ];

    const SPECIAL_CASES = [
        IControl::DBNZ => 'handleDBNZ',
    ];

    const OPERANDS = [
        IRegisterDirect::R0_DIR   => 1,
        IRegisterDirect::R1_DIR   => 1,
        IRegisterDirect::R2_DIR   => 1,
        IRegisterDirect::R3_DIR   => 1,
        IRegisterDirect::R4_DIR   => 1,
        IRegisterDirect::R5_DIR   => 1,
        IRegisterDirect::R6_DIR   => 1,
        IRegisterDirect::R7_DIR   => 1,
        IRegisterDirect::R8_DIR   => 1,
        IRegisterDirect::R9_DIR   => 1,
        IRegisterDirect::R10_DIR  => 1,
        IRegisterDirect::R11_DIR  => 1,
        IRegisterDirect::R12_DIR  => 1,
        IRegisterDirect::R13_DIR  => 1,
        IRegisterDirect::R14_DIR  => 1,
        IRegisterDirect::R15_DIR  => 1,
        IRegisterDirect::FP0_DIR  => 1,
        IRegisterDirect::FP1_DIR  => 1,
        IRegisterDirect::FP2_DIR  => 1,
        IRegisterDirect::FP3_DIR  => 1,
        IRegisterDirect::FP4_DIR  => 1,
        IRegisterDirect::FP5_DIR  => 1,
        IRegisterDirect::FP6_DIR  => 1,
        IRegisterDirect::FP7_DIR  => 1,
        IRegisterDirect::FP8_DIR  => 1,
        IRegisterDirect::FP9_DIR  => 1,
        IRegisterDirect::FP10_DIR => 1,
        IRegisterDirect::FP11_DIR => 1,
        IRegisterDirect::FP12_DIR => 1,
        IRegisterDirect::FP13_DIR => 1,
        IRegisterDirect::FP14_DIR => 1,
        IRegisterDirect::FP15_DIR => 1,
    ];

    /**
     * Attempt to find a fast path code fold option for the current opcode / encoded operand string. If a code fold is
     * found, a CodeFoldException for the complete statement is raised. Otheriwise the vanilla bytecode is returned.
     *
     * @param  int    $iOpcode
     * @param  string $sOperandByteCode
     * @return string
     */
    public function attempt(int $iOpcode, string $sOperandByteCode): string {
        if (State\Coordinator::get()->getOptions()->isEnabled(Defs\Project\IOptions::OPT_USE_FAST_PATH)) {
            if (
                isset(self::DYADIC_FAST_PATH[$iOpcode]) &&
                2 == strlen($sOperandByteCode)
            ) {
                // Extract the operands to check if they are full on register to reister
                $iDstOperand = ord($sOperandByteCode[0]);
                $iSrcOperand = ord($sOperandByteCode[1]);
                if ($iSrcOperand == IOther::SAME_AS_DEST) {
                    $iSrcOperand = $iDstOperand;
                }

                if (
                    isset(self::OPERANDS[$iDstOperand]) &&
                    isset(self::OPERANDS[$iSrcOperand])
                ) {
                    // We could just return the code, but throwing as a code fold allows us to log it
                    throw new CodeFoldException(
                        "Register to register dyadic fast path",
                        chr(self::DYADIC_FAST_PATH[$iOpcode]) .
                        chr(
                            ($iDstOperand & 0x0F) |
                            (($iSrcOperand & 0x0F) << 4)
                        ) .
                        chr($iOpcode)
                    );
                }
            } else if (isset(self::SPECIAL_CASES[$iOpcode])) {
                $cHandler = [$this, self::SPECIAL_CASES[$iOpcode]];
                $cHandler($iOpcode, $sOperandByteCode);
            }
        }
        return chr($iOpcode) . $sOperandByteCode;
    }

    /**
     * Special case handler for dbnz rX, #displacement
     *
     * Fast path prefixed code is 1 byte longer so backwards displacements require an adjustment.
     */
    private function handleDBNZ(int $iOpcode, string $sOperandByteCode): void {
        $aDisplacement = unpack(Defs\IIntLimits::LONG_BIN_FORMAT, substr($sOperandByteCode, 1));
        if (false === $aDisplacement) {
            throw new \Exception('Unexpected bytecode during dbnz expansion');
        }
        $iDisplacement = reset($aDisplacement);

        if ($iDisplacement > Defs\IIntLimits::LONG_MAX_SIGNED) {
            $iDisplacement -= (Defs\IIntLimits::LONG_MAX_UNSIGNED + 2);
        }

        // We could just return the code, but throwing as a code fold allows us to log it
        throw new CodeFoldException(
            "dbnz register decrement fast path",
            chr(self::FAST_INT_PREFIX) .
            chr(ord($sOperandByteCode[0]) & 0x0F) .
            chr($iOpcode) .
            pack(Defs\IIntLimits::LONG_BIN_FORMAT, $iDisplacement)
        );
    }
}

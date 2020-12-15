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

namespace ABadCafe\MC64K\Defs\EffectiveAddress;

/**
 * Effective Adddress Modes
 *
 * Enumerates the expected bytecode for an effective address format.
 */
interface IByteCodeNames extends IRegisterDirect, IRegisterIndirect, IRegisterIndirectIndexed, IOther {

    /**
     * Mode names. The effective address mode byte mapped to a human readable description.
     */
    const MAP = [
        // @see IRegisterDirect
        self::R0_DIR             => "GPR 0 Direct",
        self::R1_DIR             => "GPR 1 Direct",
        self::R2_DIR             => "GPR 2 Direct",
        self::R3_DIR             => "GPR 3 Direct",
        self::R4_DIR             => "GPR 4 Direct",
        self::R5_DIR             => "GPR 5 Direct",
        self::R6_DIR             => "GPR 6 Direct",
        self::R7_DIR             => "GPR 7 Direct",
        self::R8_DIR             => "GPR 8 Direct",
        self::R9_DIR             => "GPR 9 Direct",
        self::R10_DIR            => "GPR 10 Direct",
        self::R11_DIR            => "GPR 11 Direct",
        self::R12_DIR            => "GPR 12 Direct",
        self::R13_DIR            => "GPR 13 Direct",
        self::R14_DIR            => "GPR 14 Direct",
        self::R15_DIR            => "GPR 15 Direct",
        self::FP0_DIR            => "FPR 0 Direct",
        self::FP1_DIR            => "FPR 1 Direct",
        self::FP2_DIR            => "FPR 2 Direct",
        self::FP3_DIR            => "FPR 3 Direct",
        self::FP4_DIR            => "FPR 4 Direct",
        self::FP5_DIR            => "FPR 5 Direct",
        self::FP6_DIR            => "FPR 6 Direct",
        self::FP7_DIR            => "FPR 7 Direct",
        self::FP8_DIR            => "FPR 8 Direct",
        self::FP9_DIR            => "FPR 9 Direct",
        self::FP10_DIR           => "FPR 10 Direct",
        self::FP11_DIR           => "FPR 11 Direct",
        self::FP12_DIR           => "FPR 12 Direct",
        self::FP13_DIR           => "FPR 13 Direct",
        self::FP14_DIR           => "FPR 14 Direct",
        self::FP15_DIR           => "FPR 15 Direct",

        // @see IRegisterIndirect
        self::R0_IND            => "GPR x Indirect",
        self::R1_IND            => "GPR x Indirect",
        self::R2_IND            => "GPR x Indirect",
        self::R3_IND            => "GPR x Indirect",
        self::R4_IND            => "GPR x Indirect",
        self::R5_IND            => "GPR x Indirect",
        self::R6_IND            => "GPR x Indirect",
        self::R7_IND            => "GPR x Indirect",
        self::R8_IND            => "GPR x Indirect",
        self::R9_IND            => "GPR x Indirect",
        self::R10_IND           => "GPR x Indirect",
        self::R11_IND           => "GPR x Indirect",
        self::R12_IND           => "GPR x Indirect",
        self::R13_IND           => "GPR x Indirect",
        self::R14_IND           => "GPR x Indirect",
        self::R15_IND           => "GPR x Indirect",
        self::R0_IND_POST_INC   => "GPR 0 Indirect, Post Increment",
        self::R1_IND_POST_INC   => "GPR 1 Indirect, Post Increment",
        self::R2_IND_POST_INC   => "GPR 2 Indirect, Post Increment",
        self::R3_IND_POST_INC   => "GPR 3 Indirect, Post Increment",
        self::R4_IND_POST_INC   => "GPR 4 Indirect, Post Increment",
        self::R5_IND_POST_INC   => "GPR 5 Indirect, Post Increment",
        self::R6_IND_POST_INC   => "GPR 6 Indirect, Post Increment",
        self::R7_IND_POST_INC   => "GPR 7 Indirect, Post Increment",
        self::R8_IND_POST_INC   => "GPR 8 Indirect, Post Increment",
        self::R9_IND_POST_INC   => "GPR 9 Indirect, Post Increment",
        self::R10_IND_POST_INC  => "GPR 10 Indirect, Post Increment",
        self::R11_IND_POST_INC  => "GPR 11 Indirect, Post Increment",
        self::R12_IND_POST_INC  => "GPR 12 Indirect, Post Increment",
        self::R13_IND_POST_INC  => "GPR 13 Indirect, Post Increment",
        self::R14_IND_POST_INC  => "GPR 14 Indirect, Post Increment",
        self::R15_IND_POST_INC  => "GPR 15 Indirect, Post Increment",
        self::R0_IND_POST_DEC   => "GPR 0 Indirect, Post Decrement",
        self::R1_IND_POST_DEC   => "GPR 1 Indirect, Post Decrement",
        self::R2_IND_POST_DEC   => "GPR 2 Indirect, Post Decrement",
        self::R3_IND_POST_DEC   => "GPR 3 Indirect, Post Decrement",
        self::R4_IND_POST_DEC   => "GPR 4 Indirect, Post Decrement",
        self::R5_IND_POST_DEC   => "GPR 5 Indirect, Post Decrement",
        self::R6_IND_POST_DEC   => "GPR 6 Indirect, Post Decrement",
        self::R7_IND_POST_DEC   => "GPR 7 Indirect, Post Decrement",
        self::R8_IND_POST_DEC   => "GPR 8 Indirect, Post Decrement",
        self::R9_IND_POST_DEC   => "GPR 9 Indirect, Post Decrement",
        self::R10_IND_POST_DEC  => "GPR 10 Indirect, Post Decrement",
        self::R11_IND_POST_DEC  => "GPR 11 Indirect, Post Decrement",
        self::R12_IND_POST_DEC  => "GPR 12 Indirect, Post Decrement",
        self::R13_IND_POST_DEC  => "GPR 13 Indirect, Post Decrement",
        self::R14_IND_POST_DEC  => "GPR 14 Indirect, Post Decrement",
        self::R15_IND_POST_DEC  => "GPR 15 Indirect, Post Decrement",
        self::R0_IND_PRE_INC    => "GPR 0 Indirect, Pre Increment",
        self::R1_IND_PRE_INC    => "GPR 1 Indirect, Pre Increment",
        self::R2_IND_PRE_INC    => "GPR 2 Indirect, Pre Increment",
        self::R3_IND_PRE_INC    => "GPR 3 Indirect, Pre Increment",
        self::R4_IND_PRE_INC    => "GPR 4 Indirect, Pre Increment",
        self::R5_IND_PRE_INC    => "GPR 5 Indirect, Pre Increment",
        self::R6_IND_PRE_INC    => "GPR 6 Indirect, Pre Increment",
        self::R7_IND_PRE_INC    => "GPR 7 Indirect, Pre Increment",
        self::R8_IND_PRE_INC    => "GPR 8 Indirect, Pre Increment",
        self::R9_IND_PRE_INC    => "GPR 9 Indirect, Pre Increment",
        self::R10_IND_PRE_INC   => "GPR 10 Indirect, Pre Increment",
        self::R11_IND_PRE_INC   => "GPR 11 Indirect, Pre Increment",
        self::R12_IND_PRE_INC   => "GPR 12 Indirect, Pre Increment",
        self::R13_IND_PRE_INC   => "GPR 13 Indirect, Pre Increment",
        self::R14_IND_PRE_INC   => "GPR 14 Indirect, Pre Increment",
        self::R15_IND_PRE_INC   => "GPR 15 Indirect, Pre Increment",
        self::R0_IND_PRE_DEC    => "GPR 0 Indirect, Pre Decrement",
        self::R1_IND_PRE_DEC    => "GPR 1 Indirect, Pre Decrement",
        self::R2_IND_PRE_DEC    => "GPR 2 Indirect, Pre Decrement",
        self::R3_IND_PRE_DEC    => "GPR 3 Indirect, Pre Decrement",
        self::R4_IND_PRE_DEC    => "GPR 4 Indirect, Pre Decrement",
        self::R5_IND_PRE_DEC    => "GPR 5 Indirect, Pre Decrement",
        self::R6_IND_PRE_DEC    => "GPR 6 Indirect, Pre Decrement",
        self::R7_IND_PRE_DEC    => "GPR 7 Indirect, Pre Decrement",
        self::R8_IND_PRE_DEC    => "GPR 8 Indirect, Pre Decrement",
        self::R9_IND_PRE_DEC    => "GPR 9 Indirect, Pre Decrement",
        self::R10_IND_PRE_DEC   => "GPR 10 Indirect, Pre Decrement",
        self::R11_IND_PRE_DEC   => "GPR 11 Indirect, Pre Decrement",
        self::R12_IND_PRE_DEC   => "GPR 12 Indirect, Pre Decrement",
        self::R13_IND_PRE_DEC   => "GPR 13 Indirect, Pre Decrement",
        self::R14_IND_PRE_DEC   => "GPR 14 Indirect, Pre Decrement",
        self::R15_IND_PRE_DEC   => "GPR 15 Indirect, Pre Decrement",
        self::R0_IND_DSP        => "GPR 0 Indirect with Displacement",
        self::R1_IND_DSP        => "GPR 1 Indirect with Displacement",
        self::R2_IND_DSP        => "GPR 2 Indirect with Displacement",
        self::R3_IND_DSP        => "GPR 3 Indirect with Displacement",
        self::R4_IND_DSP        => "GPR 4 Indirect with Displacement",
        self::R5_IND_DSP        => "GPR 5 Indirect with Displacement",
        self::R6_IND_DSP        => "GPR 6 Indirect with Displacement",
        self::R7_IND_DSP        => "GPR 7 Indirect with Displacement",
        self::R8_IND_DSP        => "GPR 8 Indirect with Displacement",
        self::R9_IND_DSP        => "GPR 9 Indirect with Displacement",
        self::R10_IND_DSP       => "GPR 10 Indirect with Displacement",
        self::R11_IND_DSP       => "GPR 11 Indirect with Displacement",
        self::R12_IND_DSP       => "GPR 12 Indirect with Displacement",
        self::R13_IND_DSP       => "GPR 13 Indirect with Displacement",
        self::R14_IND_DSP       => "GPR 14 Indirect with Displacement",
        self::R15_IND_DSP       => "GPR 15 Indirect with Displacement",

        // @see IRegisterIndirectIndexed
        self::REG_IND_IDXB       => "GPR Indirect with 8-bit Index",
        self::REG_IND_IDXW       => "GPR Indirect with 16-bit Index",
        self::REG_IND_IDXL       => "GPR Indirect with 32-bit Index",
        self::REG_IND_IDXQ       => "GPR Indirect with 64-bit Index",
        self::REG_IND_IDXB_2     => "GPR Indirect with 16-bit Scaled 8-bit Index",
        self::REG_IND_IDXW_2     => "GPR Indirect with 16-bit Scaled 16-bit Index",
        self::REG_IND_IDXL_2     => "GPR Indirect with 16-bit Scaled 32-bit Index",
        self::REG_IND_IDXQ_2     => "GPR Indirect with 16-bit Scaled 64-bit Index",
        self::REG_IND_IDXB_4     => "GPR Indirect with 32-bit Scaled 8-bit Index",
        self::REG_IND_IDXW_4     => "GPR Indirect with 32-bit Scaled 16-bit Index",
        self::REG_IND_IDXL_4     => "GPR Indirect with 32-bit Scaled 32-bit Index",
        self::REG_IND_IDXQ_4     => "GPR Indirect with 32-bit Scaled 64-bit Index",
        self::REG_IND_IDXB_8     => "GPR Indirect with 64-bit Scaled 8-bit Index",
        self::REG_IND_IDXW_8     => "GPR Indirect with 64-bit Scaled 16-bit Index",
        self::REG_IND_IDXL_8     => "GPR Indirect with 64-bit Scaled 32-bit Index",
        self::REG_IND_IDXQ_8     => "GPR Indirect with 64-bit Scaled 64-bit Index",
        self::REG_IND_IDXB_DSP   => "GPR Indirect with 8-bit Index and Displacement",
        self::REG_IND_IDXW_DSP   => "GPR Indirect with 16-bit Index and Displacement",
        self::REG_IND_IDXL_DSP   => "GPR Indirect with 32-bit Index and Displacement",
        self::REG_IND_IDXQ_DSP   => "GPR Indirect with 64-bit Index and Displacement",
        self::REG_IND_IDXB_2_DSP => "GPR Indirect with 16-bit Scaled 8-bit Index and Displacement",
        self::REG_IND_IDXW_2_DSP => "GPR Indirect with 16-bit Scaled 16-bit Index and Displacement",
        self::REG_IND_IDXL_2_DSP => "GPR Indirect with 16-bit Scaled 32-bit Index and Displacement",
        self::REG_IND_IDXQ_2_DSP => "GPR Indirect with 16-bit Scaled 64-bit Index and Displacement",
        self::REG_IND_IDXB_4_DSP => "GPR Indirect with 32-bit Scaled 8-bit Index and Displacement",
        self::REG_IND_IDXW_4_DSP => "GPR Indirect with 32-bit Scaled 16-bit Index and Displacement",
        self::REG_IND_IDXL_4_DSP => "GPR Indirect with 32-bit Scaled 32-bit Index and Displacement",
        self::REG_IND_IDXQ_4_DSP => "GPR Indirect with 32-bit Scaled 64-bit Index and Displacement",
        self::REG_IND_IDXB_8_DSP => "GPR Indirect with 64-bit Scaled 8-bit Index and Displacement",
        self::REG_IND_IDXW_8_DSP => "GPR Indirect with 64-bit Scaled 16-bit Index and Displacement",
        self::REG_IND_IDXL_8_DSP => "GPR Indirect with 64-bit Scaled 32-bit Index and Displacement",
        self::REG_IND_IDXQ_8_DSP => "GPR Indirect with 64-bit Scaled 64-bit Index and Displacement",
        self::PC_IND_IDXB        => "PC with 8-bit Index",
        self::PC_IND_IDXW        => "PC with 16-bit Index",
        self::PC_IND_IDXL        => "PC with 32-bit Index",
        self::PC_IND_IDXQ        => "PC with 64-bit Index",
        self::PC_IND_IDXB_2      => "PC with 16-bit Scaled 8-bit Index",
        self::PC_IND_IDXW_2      => "PC with 16-bit Scaled 16-bit Index",
        self::PC_IND_IDXL_2      => "PC with 16-bit Scaled 32-bit Index",
        self::PC_IND_IDXQ_2      => "PC with 16-bit Scaled 64-bit Index",
        self::PC_IND_IDXB_4      => "PC with 32-bit Scaled 8-bit Index",
        self::PC_IND_IDXW_4      => "PC with 32-bit Scaled 16-bit Index",
        self::PC_IND_IDXL_4      => "PC with 32-bit Scaled 32-bit Index",
        self::PC_IND_IDXQ_4      => "PC with 32-bit Scaled 64-bit Index",
        self::PC_IND_IDXB_8      => "PC with 64-bit Scaled 8-bit Index",
        self::PC_IND_IDXW_8      => "PC with 64-bit Scaled 16-bit Index",
        self::PC_IND_IDXL_8      => "PC with 64-bit Scaled 32-bit Index",
        self::PC_IND_IDXQ_8      => "PC with 64-bit Scaled 64-bit Index",
        self::PC_IND_IDXB_DSP    => "PC with 8-bit Index and Displacement",
        self::PC_IND_IDXW_DSP    => "PC with 16-bit Index and Displacement",
        self::PC_IND_IDXL_DSP    => "PC with 32-bit Index and Displacement",
        self::PC_IND_IDXQ_DSP    => "PC with 16-bit Index and Displacement",
        self::PC_IND_IDXB_2_DSP  => "PC with 16-bit Scaled 8-bit Index and Displacement",
        self::PC_IND_IDXW_2_DSP  => "PC with 16-bit Scaled 16-bit Index and Displacement",
        self::PC_IND_IDXL_2_DSP  => "PC with 16-bit Scaled 32-bit Index and Displacement",
        self::PC_IND_IDXQ_2_DSP  => "PC with 16-bit Scaled 64-bit Index and Displacement",
        self::PC_IND_IDXB_4_DSP  => "PC with 32-bit Scaled 8-bit Index and Displacement",
        self::PC_IND_IDXW_4_DSP  => "PC with 32-bit Scaled 16-bit Index and Displacement",
        self::PC_IND_IDXL_4_DSP  => "PC with 32-bit Scaled 32-bit Index and Displacement",
        self::PC_IND_IDXQ_4_DSP  => "PC with 32-bit Scaled 64-bit Index and Displacement",
        self::PC_IND_IDXB_8_DSP  => "PC with 64-bit Scaled 8-bit Index and Displacement",
        self::PC_IND_IDXW_8_DSP  => "PC with 64-bit Scaled 16-bit Index and Displacement",
        self::PC_IND_IDXL_8_DSP  => "PC with 64-bit Scaled 32-bit Index and Displacement",
        self::PC_IND_IDXQ_8_DSP  => "PC with 64-bit Scaled 64-bit Index and Displacement",

        // Immediate small integer encoded directly, no extension byte.
        self::INT_SMALL_0        => "Small Immediate 0",
        self::INT_SMALL_1        => "Small Immediate 1",
        self::INT_SMALL_2        => "Small Immediate 2",
        self::INT_SMALL_3        => "Small Immediate 3",
        self::INT_SMALL_4        => "Small Immediate 4",
        self::INT_SMALL_5        => "Small Immediate 5",
        self::INT_SMALL_6        => "Small Immediate 6",
        self::INT_SMALL_7        => "Small Immediate 7",
        self::INT_SMALL_8        => "Small Immediate 8",

        // Immediate integer value encoded into extension bytes.
        self::INT_IMM_BYTE       => "Integer Immediate Byte",
        self::INT_IMM_WORD       => "Integer Immediate Word",
        self::INT_IMM_LONG       => "Integer Immediate Long",
        self::INT_IMM_QUAD       => "Integer Immediate Quad",

        // Immediate floating point value encoded into extension bytes.
        self::FLT_IMM_SINGLE     => "Float Immediate Single",
        self::FLT_IMM_DOUBLE     => "Float Immediate Double",

        // @see IOther
        self::PC_IND_DSP         => "PC with Displacement",

        self::SAME_AS_DEST       => "[Source Same As Destination]"
    ];
}

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

namespace ABadCafe\MC64K\Parser\SourceLine\Instruction\OperandSet;
use ABadCafe\MC64K\Parser\SourceLine\Instruction\CodeFoldException;
use ABadCafe\MC64K\Parser\SourceLine\Instruction\Operand;
use ABadCafe\MC64K\Parser\EffectiveAddress;
use ABadCafe\MC64K\Parser;
use ABadCafe\MC64K\State;
use ABadCafe\MC64K\Defs\Mnemonic\IControl;
use ABadCafe\MC64K\Defs;

/**
 * MonadicBranch
 *
 * Common behaviours for single operand with branch displacement
 */
abstract class MonadicBranch extends Monadic {

    use TBranching;

    const
        OPERAND_TARGET    = 1,
        MIN_OPERAND_COUNT = 2,
        FIXED_LENGTH      = 4
    ;

    /**
     * @inheritDoc
     */
    public function parse(int $iOpcode, array $aOperands, array $aSizes = []) : string {
        $this->assertMinimumOperandCount($aOperands, self::MIN_OPERAND_COUNT);

        $iSrcIndex     = $this->getSourceOperandIndex();
        $sSrcBytecode  = $this->oSrcParser
            ->setOperationSize($aSizes[$iSrcIndex] ?? self::DEFAULT_SIZE)
            ->parse($aOperands[$iSrcIndex]);

        if (null === $sSrcBytecode) {
            throw new \UnexpectedValueException(
                $aOperands[$iSrcIndex] . ' not a valid comparison operand'
            );
        }

        $oState = State\Common::get();
        $oState->setCurrentStatementLength(
            Defs\IOpcodeLimits::SIZE +
            Defs\IBranchLimits::DISPLACEMENT_SIZE +
            strlen($sSrcBytecode)
        );

        $sDisplacement = $this->parseBranchDisplacement(
            $aOperands[self::OPERAND_TARGET],
            $this->oSrcParser->hasSideEffects()
        );

        $sBytecode = $sSrcBytecode . $sDisplacement;
        $this->checkBranchDisplacement($sBytecode);

        if ($this->oSrcParser->wasImmediate()) {
            $sFoldFunc = static::OPCODES[$iOpcode];
            $cCallback = [$this, $sFoldFunc];
            $sFolded   =  $cCallback(
                $this->oSrcParser->getImmediate(),
                $this->oTgtParser->getLastDisplacement(),
                strlen($sBytecode)
            );

            // If the folded code is not empty, make sure we handle any unresolved branch targets
            if ($sFolded && $this->oTgtParser->wasUnresolved()) {
                $oState
                    ->setCurrentStatementLength(strlen($sFolded))
                    ->addUnresolvedLabel($aOperands[self::OPERAND_TARGET]);
            }

            throw new CodeFoldException(
                'SrcEA #' . $this->oSrcParser->getImmediate() .
                ' using ' . $sFoldFunc,
                $sFolded
            );
        }

        if ($this->oTgtParser->wasUnresolved()) {
            $oState->addUnresolvedLabel($aOperands[self::OPERAND_TARGET]);
        }

        return $sBytecode;
    }

}

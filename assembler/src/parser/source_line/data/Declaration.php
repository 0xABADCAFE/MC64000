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

namespace ABadCafe\MC64K\Parser\SourceLine\Data;
use ABadCafe\MC64K\Parser\SourceLine;
use ABadCafe\MC64K\State;
use ABadCafe\MC64K\Defs;

use function \rtrim, \preg_match, \stripcslashes;

/**
 * Declaration
 *
 * Handles in-line constant data declarations.
 */
class Declaration implements SourceLine\IParser {

    const BASIC_LINE_MATCH  = '/^\s+dc\.([bwlqsd])\s+(.*?)$/';
    const IDX_TYPE = 1;
    const IDX_DATA = 2;

    private array $aMatches = [];
    private array $aProcessHook = [
        'b' => 'processBytes',
        'w' => 'processWords',
        'l' => 'processLongs',
        'q' => 'processQuads',
        's' => 'processSingles',
        'd' => 'processDoubles'
    ];

    /**
     * @implements IParser::checkLine(string $sLine): bool
     */
    public function checkLine(string $sLine): bool {
        $bMatch = (bool)preg_match(self::BASIC_LINE_MATCH, $sLine, $this->aMatches);
        if (!$bMatch) {
            $this->aMatches = [];
        }
        return $bMatch;
    }

    /**
     * @inheritDoc
     */
    public function parse(string $sSource): ?string {
        if (!isset($this->aProcessHook[$this->aMatches[self::IDX_TYPE]])) {
            throw new \Exception();
        }
        $cProcessHook = [$this, $this->aProcessHook[$this->aMatches[self::IDX_TYPE]]];
        return $cProcessHook($this->aMatches[self::IDX_DATA]);
    }

    /**
     * Process a list of discrete byte values. This can be a comma separated list or a quote delimited string of
     * characters. This is not explicitly null terminated.
     *
     * @param  string $sData
     * @return string
     */
    private function processBytes(string $sData): string {
        if (preg_match('/^"(.*?)"$/', $sData, $aMatches)) {
            return stripcslashes($aMatches[1]);
        }
        $aData = $this->getValues($sData);
        return pack(Defs\IIntLimits::BYTE_BIN_FORMAT . '*', ...$aData);
    }

    /**
     * Process a comma separated list of discrete word values.
     *
     * @param  string $sData
     * @return string
     */
    private function processWords(string $sData): string {
        $aData = $this->getValues($sData);
        return pack(Defs\IIntLimits::WORD_BIN_FORMAT . '*', ...$aData);
    }

    /**
     * Process a comma separated list of discrete values.
     *
     * @param  string $sData
     * @return string
     */
    private function processLongs(string $sData): string {
        $aData = $this->getValues($sData);
        return pack(Defs\IIntLimits::LONG_BIN_FORMAT . '*', ...$aData);
    }

    /**
     * Process a comma separated list of quad values.
     *
     * @param  string $sData
     * @return string
     */
    private function processQuads(string $sData): string {
        $aData = $this->getValues($sData);
        return pack(Defs\IIntLimits::QUAD_BIN_FORMAT . '*', ...$aData);
    }

    /**
     * Process a comma separated list of single precision float values.
     *
     * @param  string $sData
     * @return string
     */
    private function processSingles(string $sData): string {
        $aData = $this->getValues($sData);
        return pack('g*', ...$aData);
    }

    /**
     * Process a comma separated list of double precision float values.
     *
     * @param  string $sData
     * @return string
     */
    private function processDoubles(string $sData): string {
        $aData = $this->getValues($sData);
        return pack('e*', ...$aData);
    }

    /**
     * Process a comma separated list of single precision float values.
     *
     * @param  string $sData
     * @return string[]
     */
    private function getValues(string $sData) : array {
        return array_map(
            'trim',
            explode(',', $sData)
        );
    }
}



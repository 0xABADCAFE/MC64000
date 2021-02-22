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

namespace ABadCafe\MC64K\State;

use ABadCafe\MC64K\Utils\Log;
use ABadCafe\MC64K\Defs;
use ABadCafe\MC64K\IO;

/**
 * DSG Antipattern: Dirty Great Singleton
 *
 * It may not be the right tool for most things, but here, it makes some sense.
 */
class Coordinator {

    const
        I_NAME = 3,
        I_FILE = 2,
        I_LINE = 1,
        I_POSN = 0
    ;

    private static ?self $oInstance = null; // Singleton

    private IO\ISourceFile $oCurrentFile;
    private LabelLocation  $oLabelLocation;
    private Output         $oOutput;

    private array
        $aGlobalLabels     = [],
        $aLocalLabels      = [],
        $aUnresolvedLabels = []
    ;


    private int
        $iCurrentStatementPosition = 0,
        $iCurrentStatementLength   = 0
    ;

    /**
     * Constructor
     */
    private function __construct() {
        $this->oCurrentFile   = new IO\SourceString('', 'none');
        $this->oLabelLocation = new LabelLocation();
        $this->oOutput        = new Output;
    }

    /**
     * Singleton accessor
     *
     * @return self
     */
    public static function get() : self {
        if (null === self::$oInstance) {
            self::$oInstance = new self;
        }
        return self::$oInstance;
    }

    /**
     * Set the file to work on.
     *
     * @param  IO\ISourceFile $oFile
     * @return self
     */
    public function setCurrentFile(IO\ISourceFile $oFile) : self {
        if ($oFile !== $this->oCurrentFile) {
            $sFilename = $oFile->getFilename();
//             if (isset($this->aFileList[$sFilename])) {
//                 throw new \Exception('File ' . $sFilename . ' already processed');
//             }
            $this->oCurrentFile                  = $oFile;
            $this->iCurrentStatementLength       = 0;
            $this->aLocalLabels[$sFilename]      = [];
            $this->aUnresolvedLabels[$sFilename] = [];
        }
        return $this;
    }

    public function getCurrentFile() : IO\ISourceFile {
        return $this->oCurrentFile;
    }

    public function getOutput() : Output {
        return $this->oOutput;
    }

    /**
     * Simple trampoline
     */
    public function setCurrentStatementLength(int $iLength) : self {
        $this->oOutput->setCurrentStatementLength($iLength);
        return $this;
    }

    /**
     * Add a global label to the registry. A global label can be declared only once.
     *
     * @param  string $sLabel
     * @return self
     * @throws Exception
     */
    public function addGlobalLabel(string $sLabel) : self {

        $this->oLabelLocation->addGlobal(
            $this->oCurrentFile,
            $sLabel,
            $this->oOutput->getCurrentStatementPosition()
        );

        if (isset($this->aGlobalLabels[$sLabel])) {
            throw new \Exception(
                'Duplicate global: '    . $sLabel .
                ' already declared in ' . $this->aGlobalLabels[$sLabel][self::I_FILE] .
                ' on line ' . $this->aGlobalLabels[$sLabel][self::I_LINE]
            );
        }
        $sCurrentFilename          = $this->oCurrentFile->getFilename();
        $iCurrentLineNumber        = $this->oCurrentFile->getLineNumber();
        $iCurrentStatementPosition = $this->oOutput->getCurrentStatementPosition();
        $this->aGlobalLabels[$sLabel] = [
            self::I_FILE => $sCurrentFilename,
            self::I_LINE => $iCurrentLineNumber,
            self::I_POSN => $iCurrentStatementPosition
        ];
        return $this;
    }

    /**
     * Add a local label to the registry. A local label can be declared only once in the current file.
     *
     * @param  string $sLabel
     * @return self
     * @throws Exception
     */
    public function addLocalLabel(string $sLabel) : self {

        $this->oLabelLocation->addLocal(
            $this->oCurrentFile,
            $sLabel,
            $this->oOutput->getCurrentStatementPosition()
        );


        $sCurrentFilename = $this->oCurrentFile->getFilename();
        if (isset($this->aLocalLabels[$sCurrentFilename][$sLabel])) {
            throw new \Exception(
                'Duplicate local: '     . $sLabel .
                ' already declared in ' . $sCurrentFilename .
                ' on line '             . $this->aLocalLabels[$sCurrentFilename][$sLabel][self::I_LINE]
            );
        }
        $iCurrentLineNumber        = $this->oCurrentFile->getLineNumber();
        $iCurrentStatementPosition = $this->oOutput->getCurrentStatementPosition();

        $this->aLocalLabels[$sCurrentFilename][$sLabel] = [
            self::I_LINE => $iCurrentLineNumber,
            self::I_POSN => $iCurrentStatementPosition
        ];
//         Log::printf(
//             "Added local label '%s' on line %d, code position %d",
//             $sLabel,
//             $sCurrentFilename,
//             $iCurrentStatementPosition
//         );
        return $this;
    }

    /**
     * Returns the global symbol table.
     *
     * @return array
     */
    public function getGlobals() : array {
        return $this->aGlobalLabels;
    }

    /**
     * @param  string $sLabel
     * @return int
     */
    public function getBranchDisplacementForLabel(string $sLabel) : int {
        $iPosition = $this->getPositionForLabel($sLabel);
        if (null !== $iPosition) {
            $iDisplacement = $this->oOutput->getDisplacmentForPosition($iPosition);
//             Log::printf(
//                 "Resolved %s to displacement %d [%d - %d - %d]",
//                 $sLabel,
//                 $iDisplacement,
//                 $iPosition,
//                 $this->iCurrentStatementPosition,
//                 $this->iCurrentStatementLength
//             );
            return $iDisplacement;
        }
        return Defs\IBranchLimits::UNRESOLVED_DISPLACEMENT;
    }

    /**
     * @param  string $sLabel
     * @return int|null
     */
    public function getPositionForLabel(string $sLabel) : ?int {
        $sCurrentFilename = $this->oCurrentFile->getFilename();
        $aLabels = ('.' === $sLabel[0]) ?
            $this->aLocalLabels[$sCurrentFilename] :
            $this->aGlobalLabels;
        if (isset($aLabels[$sLabel])) {
            Log::printf(
                "Resolved label '%s' to bytecode position %d",
                $sLabel,
                $aLabels[$sLabel][self::I_POSN]
            );
            return $aLabels[$sLabel][self::I_POSN];
        }
        return null;
    }

    /**
     * @param  string $sLabel
     * @return self
     */
    public function addUnresolvedLabel(string $sLabel) : self {

        $iLocation          = $this->oOutput->getCurrentOffset() - Defs\IBranchLimits::DISPLACEMENT_SIZE;
        $sCurrentFilename   = $this->oCurrentFile->getFilename();
        $iCurrentLineNumber = $this->oCurrentFile->getLineNumber();

        if (isset($this->aUnresolvedLabels[$sCurrentFilename][$sLabel][$iCurrentLineNumber])) {
            throw new \Exception("Duplicate unresolved label reference to same line in same file");
        }

        Log::printf(
            "Recorded reference to unresolved label '%s' at bytecode position %d",
            $sLabel,
            $iLocation
        );

        $this->aUnresolvedLabels[$sCurrentFilename][$sLabel][$iCurrentLineNumber] = $iLocation;
        return $this;
    }

    /**
     *
     */
    public function dumpUnresolvedLabels() : void {
        echo "\nFirst Pass unresolved label dump:\n";
        foreach ($this->aUnresolvedLabels as $sFilename => &$aUnresolved) {
            echo "\tIn source file '", $sFilename, "':\n";
            foreach ($aUnresolved as $sLabel => &$aReferences) {
                foreach ($aReferences as $iLineNumber => $iLocation) {
                    echo "\t\tLine ", $iLineNumber, " (bytecode position ", $iLocation, ") => '", $sLabel, "'\n";
                }
            }
        }
    }
}

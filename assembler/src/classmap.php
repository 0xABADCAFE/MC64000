<?php

namespace ABadCafe\MC64K;

const CLASS_MAP = [
  'ABadCafe\\MC64K\\IParser' => '/IParser.php',
  'ABadCafe\\MC64K\\Parser\\SourceError' => '/parser/SourceError.php',
  'ABadCafe\\MC64K\\Parser\\Utils\\TSignedDisplacementAware' => '/parser/utils/TSignedDisplacementAware.php',
  'ABadCafe\\MC64K\\Parser\\Utils\\Hex' => '/parser/utils/Hex.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\TParser' => '/parser/source_line/TParser.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\IParser' => '/parser/source_line/IParser.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Processor' => '/parser/source_line/Processor.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Directive\\IProcessor' => '/parser/source_line/directive/IProcessor.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Directive\\Statement' => '/parser/source_line/directive/Statement.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Directive\\Processor\\Export' => '/parser/source_line/directive/processor/Export.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Directive\\Processor\\Flag' => '/parser/source_line/directive/processor/Flag.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Directive\\Processor\\Undefine' => '/parser/source_line/directive/processor/Undefine.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Directive\\Processor\\Define' => '/parser/source_line/directive/processor/Define.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Directive\\Processor\\Import' => '/parser/source_line/directive/processor/Import.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Label\\Exported' => '/parser/source_line/label/Exported.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Label\\Local' => '/parser/source_line/label/Local.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Instruction\\CodeFoldException' => '/parser/source_line/instruction/CodeFoldException.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Instruction\\IOperandSetParser' => '/parser/source_line/instruction/IOperandSetParser.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Instruction\\UnhandledCodeFoldException' => '/parser/source_line/instruction/UnhandledCodeFoldException.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Instruction\\Statement' => '/parser/source_line/instruction/Statement.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Instruction\\Operand\\FixedInteger' => '/parser/source_line/instruction/operand/FixedInteger.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Instruction\\Operand\\BranchDisplacement' => '/parser/source_line/instruction/operand/BranchDisplacement.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Instruction\\OperandSet\\IntegerMonadicBranch' => '/parser/source_line/instruction/operand_set/IntegerMonadicBranch.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Instruction\\OperandSet\\CustomDyadic' => '/parser/source_line/instruction/operand_set/CustomDyadic.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Instruction\\OperandSet\\IntegerMonadic' => '/parser/source_line/instruction/operand_set/IntegerMonadic.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Instruction\\OperandSet\\IFixedBranch' => '/parser/source_line/instruction/operand_set/IFixedBranch.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Instruction\\OperandSet\\FloatToIntegerDyadic' => '/parser/source_line/instruction/operand_set/FloatToIntegerDyadic.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Instruction\\OperandSet\\FloatDyadic' => '/parser/source_line/instruction/operand_set/FloatDyadic.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Instruction\\OperandSet\\BranchDisplacementOnly' => '/parser/source_line/instruction/operand_set/BranchDisplacementOnly.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Instruction\\OperandSet\\FloatDyadicBranch' => '/parser/source_line/instruction/operand_set/FloatDyadicBranch.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Instruction\\OperandSet\\CustomMonadic' => '/parser/source_line/instruction/operand_set/CustomMonadic.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Instruction\\OperandSet\\IntegerDyadicBranch' => '/parser/source_line/instruction/operand_set/IntegerDyadicBranch.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Instruction\\OperandSet\\None' => '/parser/source_line/instruction/operand_set/None.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Instruction\\OperandSet\\FloatMonadicBranch' => '/parser/source_line/instruction/operand_set/FloatMonadicBranch.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Instruction\\OperandSet\\IntegerDyadic' => '/parser/source_line/instruction/operand_set/IntegerDyadic.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Instruction\\OperandSet\\CustomMonadicBranch' => '/parser/source_line/instruction/operand_set/CustomMonadicBranch.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Instruction\\OperandSet\\IntegerToFloatDyadic' => '/parser/source_line/instruction/operand_set/IntegerToFloatDyadic.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Instruction\\OperandSet\\IntegerMonadicAddress' => '/parser/source_line/instruction/operand_set/IntegerMonadicAddress.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Instruction\\OperandSet\\PackedFPRPair' => '/parser/source_line/instruction/operand_set/PackedFPRPair.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Instruction\\OperandSet\\PackedGPRPair' => '/parser/source_line/instruction/operand_set/PackedGPRPair.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Instruction\\OperandSet\\TBranching' => '/parser/source_line/instruction/operand_set/abstract/TBranching.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Instruction\\OperandSet\\Monadic' => '/parser/source_line/instruction/operand_set/abstract/Monadic.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Instruction\\OperandSet\\MonadicBranch' => '/parser/source_line/instruction/operand_set/abstract/MonadicBranch.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Instruction\\OperandSet\\Dyadic' => '/parser/source_line/instruction/operand_set/abstract/Dyadic.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Instruction\\OperandSet\\DyadicBranch' => '/parser/source_line/instruction/operand_set/abstract/DyadicBranch.php',
  'ABadCafe\\MC64K\\Parser\\SourceLine\\Instruction\\OperandSet\\PackedRegisterPair' => '/parser/source_line/instruction/operand_set/abstract/PackedRegisterPair.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\IntegerImmediate' => '/parser/effective_address/IntegerImmediate.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\GPRIndirectUpdating' => '/parser/effective_address/GPRIndirectUpdating.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\AllFloatReadable' => '/parser/effective_address/AllFloatReadable.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\AllFloatWriteable' => '/parser/effective_address/AllFloatWriteable.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\GPRIndirectDisplacement' => '/parser/effective_address/GPRIndirectDisplacement.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\GPRDirect' => '/parser/effective_address/GPRDirect.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\PCIndirectDisplacement' => '/parser/effective_address/PCIndirectDisplacement.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\AllIntegerWriteable' => '/parser/effective_address/AllIntegerWriteable.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\GPRIndirectIndexed' => '/parser/effective_address/GPRIndirectIndexed.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\FPRDirect' => '/parser/effective_address/FPRDirect.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\IParser' => '/parser/effective_address/IParser.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\ARDirect' => '/parser/effective_address/ARDirect.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\TOperationSizeAware' => '/parser/effective_address/TOperationSizeAware.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\AllControlAddressing' => '/parser/effective_address/AllControlAddressing.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\GPRIndirectIndexedDisplacement' => '/parser/effective_address/GPRIndirectIndexedDisplacement.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\FloatImmediate' => '/parser/effective_address/FloatImmediate.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\AllIntegerReadable' => '/parser/effective_address/AllIntegerReadable.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\PCIndirectIndexedDisplacement' => '/parser/effective_address/PCIndirectIndexedDisplacement.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\Custom' => '/parser/effective_address/Custom.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\GPRIndirect' => '/parser/effective_address/GPRIndirect.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\PCIndirectIndexed' => '/parser/effective_address/PCIndirectIndexed.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\GlobalReference' => '/parser/effective_address/GlobalReference.php',
  'ABadCafe\\MC64K\\Parser\\EffectiveAddress\\Composite' => '/parser/effective_address/Composite.php',
  'ABadCafe\\MC64K\\IO\\ISourceFile' => '/io/ISourceFile.php',
  'ABadCafe\\MC64K\\IO\\SourceFile' => '/io/SourceFile.php',
  'ABadCafe\\MC64K\\IO\\SourceString' => '/io/SourceString.php',
  'ABadCafe\\MC64K\\IO\\Output\\ExportList' => '/io/output/ExportList.php',
  'ABadCafe\\MC64K\\IO\\Output\\Binary' => '/io/output/Binary.php',
  'ABadCafe\\MC64K\\IO\\Output\\ChunkList' => '/io/output/ChunkList.php',
  'ABadCafe\\MC64K\\IO\\Output\\ImportList' => '/io/output/ImportList.php',
  'ABadCafe\\MC64K\\IO\\Output\\IBinaryChunk' => '/io/output/IBinaryChunk.php',
  'ABadCafe\\MC64K\\Process\\SecondPass' => '/process/SecondPass.php',
  'ABadCafe\\MC64K\\Utils\\Binary' => '/utils/Binary.php',
  'ABadCafe\\MC64K\\Utils\\Log' => '/utils/Log.php',
  'ABadCafe\\MC64K\\Project\\Definition' => '/project/Definition.php',
  'ABadCafe\\MC64K\\Tokeniser\\Instruction' => '/tokeniser/Instruction.php',
  'ABadCafe\\MC64K\\State\\Output' => '/state/Output.php',
  'ABadCafe\\MC64K\\State\\LabelLocation' => '/state/LabelLocation.php',
  'ABadCafe\\MC64K\\State\\Options' => '/state/Options.php',
  'ABadCafe\\MC64K\\State\\Coordinator' => '/state/Coordinator.php',
  'ABadCafe\\MC64K\\State\\DefinitionSet' => '/state/DefinitionSet.php',
  'ABadCafe\\MC64K\\Application\\Assembler' => '/application/Assembler.php',
  'ABadCafe\\MC64K\\Defs\\IOpcodeLimits' => '/defs/IOpcodeLimits.php',
  'ABadCafe\\MC64K\\Defs\\IBranchLimits' => '/defs/IBranchLimits.php',
  'ABadCafe\\MC64K\\Defs\\ILabel' => '/defs/ILabel.php',
  'ABadCafe\\MC64K\\Defs\\IIdentifier' => '/defs/IIdentifier.php',
  'ABadCafe\\MC64K\\Defs\\IIntLimits' => '/defs/IIntLimits.php',
  'ABadCafe\\MC64K\\Defs\\Register\\INames' => '/defs/register/INames.php',
  'ABadCafe\\MC64K\\Defs\\Register\\Enumerator' => '/defs/register/Enumerator.php',
  'ABadCafe\\MC64K\\Defs\\Project\\IOptions' => '/defs/project/IOptions.php',
  'ABadCafe\\MC64K\\Defs\\Mnemonic\\IDataMove' => '/defs/mnemonic/IDataMove.php',
  'ABadCafe\\MC64K\\Defs\\Mnemonic\\IControl' => '/defs/mnemonic/IControl.php',
  'ABadCafe\\MC64K\\Defs\\Mnemonic\\IByteCodeGroups' => '/defs/mnemonic/IByteCodeGroups.php',
  'ABadCafe\\MC64K\\Defs\\Mnemonic\\IArithmetic' => '/defs/mnemonic/IArithmetic.php',
  'ABadCafe\\MC64K\\Defs\\Mnemonic\\ILogical' => '/defs/mnemonic/ILogical.php',
  'ABadCafe\\MC64K\\Defs\\Mnemonic\\IMatches' => '/defs/mnemonic/IMatches.php',
  'ABadCafe\\MC64K\\Defs\\Mnemonic\\IOperandSizes' => '/defs/mnemonic/IOperandSizes.php',
  'ABadCafe\\MC64K\\Defs\\EffectiveAddress\\IRegisterDirect' => '/defs/effective_address/IRegisterDirect.php',
  'ABadCafe\\MC64K\\Defs\\EffectiveAddress\\IByteCodeGroups' => '/defs/effective_address/IByteCodeGroups.php',
  'ABadCafe\\MC64K\\Defs\\EffectiveAddress\\IByteCodeNames' => '/defs/effective_address/IByteCodeNames.php',
  'ABadCafe\\MC64K\\Defs\\EffectiveAddress\\IRegisterIndirect' => '/defs/effective_address/IRegisterIndirect.php',
  'ABadCafe\\MC64K\\Defs\\EffectiveAddress\\IMatches' => '/defs/effective_address/IMatches.php',
  'ABadCafe\\MC64K\\Defs\\EffectiveAddress\\ISameAsDestination' => '/defs/effective_address/ISameAsDestination.php',
  'ABadCafe\\MC64K\\Defs\\EffectiveAddress\\IRegisterIndirectIndexed' => '/defs/effective_address/IRegisterIndirectIndexed.php',
  'ABadCafe\\MC64K\\Defs\\EffectiveAddress\\IOther' => '/defs/effective_address/IOther.php',
];
# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: recoFilterTest --filein /store/user/appelte1/HYDJET_MinBias_5020GeV_HIRun2015Development_7_2_0_pre6_v2/HYDJET_MinBias_5020GeV_HIRun2015Development_7_2_0_pre6_v2_RAW/f38a9144f6c496f7ce214f85fb96c198/RAW_3_1_rFu.root --fileout file:RECO.root --datatier GEN-SIM-RECO --conditions auto:starthi_HIon -s RAW2DIGI,L1Reco,RECO --scenario HeavyIons --eventcontent RECOSIM -n 10 --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process('RECO')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContentHeavyIons_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.ReconstructionHeavyIons_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(50)
)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring('/store/user/appelte1/HYDJET_MinBias_5020GeV_HIRun2015Development_7_2_0_pre6_v2/HYDJET_MinBias_5020GeV_HIRun2015Development_7_2_0_pre6_v2_HLTDEBUG/1cd0ca6c61f855318bc29f97df0adbcb/RAW_3_1_Bju.root')
)

process.options = cms.untracked.PSet(
wantSummary = cms.untracked.bool(True)
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.19 $'),
    annotation = cms.untracked.string('recoFilterTest nevts:10'),
    name = cms.untracked.string('Applications')
)

# Output definition

process.RECOSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.RECOSIMEventContent.outputCommands,
    fileName = cms.untracked.string('file:RECO.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM-RECO'),
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('raw2digi_step')
    )
)

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:starthi_HIon', '')

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstructionHeavyIons)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOSIMoutput_step = cms.EndPath(process.RECOSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.endjob_step,process.RECOSIMoutput_step)

# Add Ncoll Filter
process.load('Appeltel.HIRun2015Ana.NcollFilter_cfi')
process.ncFilter = process.ncollFilter.clone(
    ncollmax = 3000
)
process.RandomNumberGeneratorService.ncFilter = cms.PSet(
    initialSeed = cms.untracked.uint32(2015),
    engineName = cms.untracked.string('TRandom3')
)
for path in process.paths:
	getattr(process,path)._seq = process.ncFilter * getattr(process,path)._seq 

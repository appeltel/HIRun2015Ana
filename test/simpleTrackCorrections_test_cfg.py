import FWCore.ParameterSet.Config as cms

process = cms.Process('TRACKANA')
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
#process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.ReconstructionHeavyIons_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
#process.load('Configuration.EventContent.EventContentHeavyIons_cff')
process.load('Appeltel.HIRun2015Ana.SimpleTrackingCorrections_cfi')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('trackCorrections.root')
)

process.load("SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi")

process.tpRecoAssocGeneralTracks = process.trackingParticleRecoTrackAsssociation.clone()
process.tpRecoAssocGeneralTracks.label_tr = cms.InputTag("hiGeneralTracks")

process.load("SimTracker.TrackAssociatorProducers.quickTrackAssociatorByHits_cfi")
process.quickTrackAssociatorByHits.SimToRecoDenominator = cms.string('reco')

process.load("SimTracker.TrackerHitAssociation.clusterTpAssociationProducer_cfi")

# Input source
process.source = cms.Source("PoolSource",
    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
    fileNames =  cms.untracked.vstring(
#'file:step3_RAW2DIGI_L1Reco_RECO.root'
'/store/user/mnguyen/PyquenUnquenched_Dijet_pthat80_740pre6_GEN-SIM/PyquenUnquenched_Dijet_pthat80_740pre6_MCHI2_74_V0_RECO/35189fe3832afd77f4f2029b245352e2/step3_RAW2DIGI_L1Reco_RECO_100_1_0f3.root'
    )
)

#process.load("HLTrigger.HLTfilters.hltHighLevel_cfi")
#process.hltSingleTrigger = process.hltHighLevel.clone()
#process.hltSingleTrigger.HLTPaths = ["HLT_PAZeroBiasPixel_SingleTrack_v1"]

process.GlobalTag.globaltag = 'MCHI2_74_V0::All'

process.p = cms.Path( 
                      process.tpClusterProducer *
                      process.quickTrackAssociatorByHits *
                      process.tpRecoAssocGeneralTracks *
                      process.SimpleTrkCorr 
)

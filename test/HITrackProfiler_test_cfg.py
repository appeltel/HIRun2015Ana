import FWCore.ParameterSet.Config as cms

process = cms.Process('TRACKANA')
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.ReconstructionHeavyIons_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Appeltel.HIRun2015Ana.HITrackProfiler_cfi')


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('trackProfiler.root')
)

process.load("SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi")

process.tpRecoAssocGeneralTracks = process.trackingParticleRecoTrackAsssociation.clone()
process.tpRecoAssocGeneralTracks.label_tr = cms.InputTag("TrackRefitter")

process.load("SimTracker.TrackAssociatorProducers.quickTrackAssociatorByHits_cfi")
process.quickTrackAssociatorByHits.SimToRecoDenominator = cms.string('reco')
process.quickTrackAssociatorByHits.Cut_RecoToSim = 0.02
process.quickTrackAssociatorByHits.Quality_SimToReco = 0.02
process.quickTrackAssociatorByHits.Purity_SimToReco = 0.02

process.load("SimTracker.TrackerHitAssociation.clusterTpAssociationProducer_cfi")

# Input source
process.source = cms.Source("PoolSource",
    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
    fileNames =  cms.untracked.vstring(
'/store/user/mnguyen/PyquenUnquenched_Dijet_pthat80_740pre6_GEN-SIM/PyquenUnquenched_Dijet_pthat80_740pre8_MCHI2_74_V0_RECO/5e9447e73d909a826044ea40afa73dc6/step3_RAW2DIGI_L1Reco_RECO_100_1_nLj.root'
#'file:/scratch/appelte1/step3_RAW2DIGI_L1Reco_RECO_100_1_nLj.root'
    )
)


#centrality
process.load("RecoHI.HiCentralityAlgos.CentralityBin_cfi") 
process.centralityBin.Centrality = cms.InputTag("hiCentrality")
process.centralityBin.centralityVariable = cms.string("HFtowers")
process.centralityBin.nonDefaultGlauberModel = cms.string("HydjetDrum5")
process.GlobalTag.toGet.extend([
   cms.PSet(record = cms.string("HeavyIonRcd"),
      tag = cms.string("CentralityTable_HFtowers200_HydjetDrum5_v740x01_mc"),
      connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_PHYSICSTOOLS"),
      label = cms.untracked.string("HFtowersHydjetDrum5")
   ),
])


process.load("RecoTracker.TrackProducer.TrackRefitter_cfi")
process.TrackRefitter.src = cms.InputTag("hiGeneralTracks")

process.hiTrackProfiler.useCentrality = True
process.hiTrackProfiler.centralitySrc = cms.InputTag("centralityBin","HFtowers")


process.hiTrackProfiler.applyTrackCuts = False
process.hiTrackProfiler.fillNTuples = True
process.hiTrackProfiler.trackSrc = "TrackRefitter"

process.GlobalTag.globaltag = 'MCHI2_74_V0::All'

process.p = cms.Path( 
                      process.siPixelRecHits *
                      process.siStripMatchedRecHits *
                      process.MeasurementTrackerEvent *
                      process.TrackRefitter * 
                      process.tpClusterProducer *
                      process.quickTrackAssociatorByHits *
                      process.tpRecoAssocGeneralTracks *
                      process.centralityBin *
                      process.hiTrackProfiler
)

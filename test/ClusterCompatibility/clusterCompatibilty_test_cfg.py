import FWCore.ParameterSet.Config as cms

process = cms.Process('ANA')

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

process.source = cms.Source("PoolSource",
    fileNames =  cms.untracked.vstring(
'/store/user/appeltel/HIRECO_2011data_75X_CCtest2/HIMinBiasUPC/HIRECO_2011data_75X_CCtest2/150430_014904/0000/step2_RAW2DIGI_L1Reco_RECO_10.root',
'/store/user/appeltel/HIRECO_2011data_75X_CCtest2/HIMinBiasUPC/HIRECO_2011data_75X_CCtest2/150430_014904/0000/step2_RAW2DIGI_L1Reco_RECO_11.root',
'/store/user/appeltel/HIRECO_2011data_75X_CCtest2/HIMinBiasUPC/HIRECO_2011data_75X_CCtest2/150430_014904/0000/step2_RAW2DIGI_L1Reco_RECO_12.root',
'/store/user/appeltel/HIRECO_2011data_75X_CCtest2/HIMinBiasUPC/HIRECO_2011data_75X_CCtest2/150430_014904/0000/step2_RAW2DIGI_L1Reco_RECO_13.root',
'/store/user/appeltel/HIRECO_2011data_75X_CCtest2/HIMinBiasUPC/HIRECO_2011data_75X_CCtest2/150430_014904/0000/step2_RAW2DIGI_L1Reco_RECO_14.root',
'/store/user/appeltel/HIRECO_2011data_75X_CCtest2/HIMinBiasUPC/HIRECO_2011data_75X_CCtest2/150430_014904/0000/step2_RAW2DIGI_L1Reco_RECO_15.root',
'/store/user/appeltel/HIRECO_2011data_75X_CCtest2/HIMinBiasUPC/HIRECO_2011data_75X_CCtest2/150430_014904/0000/step2_RAW2DIGI_L1Reco_RECO_16.root',
'/store/user/appeltel/HIRECO_2011data_75X_CCtest2/HIMinBiasUPC/HIRECO_2011data_75X_CCtest2/150430_014904/0000/step2_RAW2DIGI_L1Reco_RECO_2.root',
'/store/user/appeltel/HIRECO_2011data_75X_CCtest2/HIMinBiasUPC/HIRECO_2011data_75X_CCtest2/150430_014904/0000/step2_RAW2DIGI_L1Reco_RECO_3.root',
'/store/user/appeltel/HIRECO_2011data_75X_CCtest2/HIMinBiasUPC/HIRECO_2011data_75X_CCtest2/150430_014904/0000/step2_RAW2DIGI_L1Reco_RECO_4.root',
'/store/user/appeltel/HIRECO_2011data_75X_CCtest2/HIMinBiasUPC/HIRECO_2011data_75X_CCtest2/150430_014904/0000/step2_RAW2DIGI_L1Reco_RECO_5.root',
'/store/user/appeltel/HIRECO_2011data_75X_CCtest2/HIMinBiasUPC/HIRECO_2011data_75X_CCtest2/150430_014904/0000/step2_RAW2DIGI_L1Reco_RECO_6.root',
'/store/user/appeltel/HIRECO_2011data_75X_CCtest2/HIMinBiasUPC/HIRECO_2011data_75X_CCtest2/150430_014904/0000/step2_RAW2DIGI_L1Reco_RECO_8.root',
'/store/user/appeltel/HIRECO_2011data_75X_CCtest2/HIMinBiasUPC/HIRECO_2011data_75X_CCtest2/150430_014904/0000/step2_RAW2DIGI_L1Reco_RECO_9.root'
    )
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('ccgallery.root')
)

process.clusComAna = cms.EDAnalyzer('HIClusterCompatibilityProfiler',
        vertexSrc = cms.InputTag("hiSelectedVertex"),
        cluscomSrc = cms.InputTag("hiClusterCompatibility")
)

process.p = cms.Path(process.clusComAna)

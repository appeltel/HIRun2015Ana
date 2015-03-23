import re

# 
# First read in all raw lines from all files
#

dataFileList = ['reco_pixel.log']
rawData = list()

for dataFile in dataFileList:
  with open(dataFile) as inputData:
    rawData += inputData.read().splitlines()

#
# Count the events
#

nEvent = 0

for line in rawData:
  if( re.match(r'^Begin processing the',line) ): nEvent += 1

print 'There are {} events in the combined samples\n\n'.format(nEvent)

#
# create a dictionary of times per module keyed 
# by module name
#

modules = dict()

for line in rawData:
  if ( re.match(r'^TimeModule> ', line)):
    moduleName = line.split()[3]
    moduleTime = float(line.split()[5])
    if moduleName in modules.keys():
      modules[moduleName] += moduleTime
    else:
      modules[moduleName] = moduleTime

for m in modules:
  modules[m] /= float(nEvent)

#
# Print the sorted times per module
#

totalTime = 0.0

for m in sorted(modules, key=modules.get, reverse=True ):
  totalTime += modules[m]
  print '{}     {}'.format(m,modules[m])

print '\n\nTotal time per event is {}'.format(totalTime)

#
# Make a list of logical groups for the 
# modules, and determine total time for each group
#

groups = {

  'Digi Unpacking and Trigger' : ['csctfDigis',
                      'dttfDigis',
                      'gctDigis',
                      'caloStage1Digis',
                      'caloStage1LegacyFormatDigis',
                      'gtDigis',
                      'gtEvmDigis',
                      'siPixelDigis',
                      'siStripDigis',
                      'ecalDigis',   
                      'ecalPreshowerDigis',
                      'hcalDigis',
                      'muonCSCDigis',
                      'muonDTDigis',
                      'muonRPCDigis',
                      'castorDigis',
                      'scalersRawToDigi',
                      'l1extraParticles',
                      'l1GtTriggerMenuLite',
                      'conditionsInEdm',
                      'l1L1GtObjectMap',
                      'TriggerResults'],

  'Tracker Local Reco' : ['siPixelClusters',
                          'siPixelRecHits',
                          'siStripZeroSuppression',
                          'siStripClusters',
                          'siStripMatchedRecHits',
                          'clusterSummaryProducerNoSplitting',
                          'MeasurementTrackerEvent',
                          'siPixelClusterShapeCache'],

  'Calo Local Reco' : ['ecalMultiFitUncalibRecHit',
                       'ecalDetIdToBeRecovered',
                       'ecalRecHit',
                       'ecalCompactTrigPrim',
                       'ecalTPSkim',
                       'ecalPreshowerRecHit',
                       'hbhereco',
                       'hfreco',
                       'horeco',
                       'zdcreco',
                       'castorreco'],

  'Muon Local Reco' : ['dt1DRecHits',
                       'dt4DSegments',
                       'dt1DCosmicRecHits',
                       'dt4DCosmicSegments',
                       'csc2DRecHits',
                       'cscSegments',
                       'rpcRecHits'],

  'Primary Vertex' : ['offlineBeamSpot',
                      'hiPixelClusterVertex',
                      'PixelLayerTriplets',
                      'hiPixel3ProtoTracks',
                      'hiPixelMedianVertex',
                      'hiSelectedProtoTracks',
                      'hiPixelAdaptiveVertex',
                      'hiBestAdaptiveVertex',
                      'hiSelectedVertex'],

  'Tracking Iter0': ['hiPixel3PrimTracks',
                     'hiPixelTrackSeeds',
                     'hiPrimTrackCandidates',
                     'hiGlobalPrimTracks',
                     'hiInitialStepSelector'],

  'Tracking Iter1': ['hiDetachedTripletStepClusters',
                     'hiDetachedTripletStepSeedLayers',
                     'hiDetachedTripletStepPixelTracks',
                     'hiDetachedTripletStepSeeds',
                     'hiDetachedTripletStepTrackCandidates',
                     'hiDetachedTripletStepTracks',
                     'hiDetachedTripletStepSelector',
                     'hiDetachedTripletStepQual'],

  'Tracking Iter2': ['hiLowPtTripletStepClusters',
                     'hiLowPtTripletStepSeedLayers',
                     'hiLowPtTripletStepPixelTracks',
                     'hiLowPtTripletStepSeeds',
                     'hiLowPtTripletStepTrackCandidates',
                     'hiLowPtTripletStepTracks',
                     'hiLowPtTripletStepSelector',
                     'hiLowPtTripletStepQual'],

  'Tracking Iter3': ['hiPixelPairClusters',
                     'hiPixelPairSeedLayers',
                     'hiPixelPairSeeds',
                     'hiPixelPairTrackCandidates',
                     'hiPixelPairGlobalPrimTracks',
                     'hiPixelPairStepSelector' ],

  'Pixel Tracking': ['hiConformalPixelTracks',
                     'hiHighPtStepSelector',
                     'hiGeneralAndPixelTracks',
                     'hiPixelOnlyStepSelector'],

  'Muon Reco': ['ancientMuonSeed',
                'refittedStandAloneMuons',
                'displacedMuonSeeds',
                'displacedStandAloneMuons',
                'hiRegitMuInitialStepSeedLayers',
                'hiRegitMuInitialStepSeeds',
                'hiRegitMuInitialStepTrackCandidates',
                'hiRegitMuInitialStepTracks',
                'hiRegitMuInitialStepSelector',
                'hiRegitMuDetachedTripletStepClusters',
                'hiRegitMuDetachedTripletStepSeedLayers',
                'hiRegitMuDetachedTripletStepSeeds',
                'hiRegitMuDetachedTripletStepTrackCandidates',
                'hiRegitMuDetachedTripletStepTracks',
                'hiRegitMuDetachedTripletStepSelector',
                'hiRegitMuLowPtTripletStepClusters',
                'hiRegitMuLowPtTripletStepSeedLayers',
                'hiRegitMuLowPtTripletStepSeeds',
                'hiRegitMuLowPtTripletStepTrackCandidates',
                'hiRegitMuLowPtTripletStepTracks',
                'hiRegitMuLowPtTripletStepSelector',
                'hiRegitMuPixelPairStepClusters',
                'hiRegitMuPixelPairStepSeedLayers',
                'hiRegitMuPixelPairStepSeeds',
                'hiRegitMuPixelPairStepTrackCandidates',
                'hiRegitMuPixelPairStepTracks',
                'hiRegitMuPixelPairStepSelector',
                'hiRegitMuMixedTripletStepClusters',
                'hiRegitMuMixedTripletStepSeedLayersA',
                'hiRegitMuMixedTripletStepSeedsA',
                'hiRegitMuMixedTripletStepSeedLayersB',
                'hiRegitMuMixedTripletStepSeedsB',
                'hiRegitMuMixedTripletStepSeeds',
                'hiRegitMuMixedTripletStepTrackCandidates',
                'hiRegitMuMixedTripletStepTracks',
                'hiRegitMuMixedTripletStepSelector',
                'hiRegitMuPixelLessStepClusters',
                'hiRegitMuPixelLessStepSeedLayers',
                'hiRegitMuPixelLessStepSeeds',
                'hiRegitMuPixelLessStepTrackCandidates',
                'hiRegitMuPixelLessStepTracks',
                'hiRegitMuPixelLessStepSelector',
                'hiRegitMuTobTecStepClusters',
                'hiRegitMuTobTecStepSeedLayersTripl',
                'hiRegitMuTobTecStepSeedsTripl',
                'hiRegitMuTobTecStepSeedLayersPair',
                'hiRegitMuTobTecStepSeedsPair',
                'hiRegitMuTobTecStepSeeds',
                'hiRegitMuTobTecStepTrackCandidates',
                'hiRegitMuTobTecStepTracks',
                'hiRegitMuTobTecStepSelector',
                'hiGeneralTracks',
                'globalMuons',
                'standAloneMuons',
                'tevMuons',
                'glbTrackQual',
                'muons',
                'calomuons',
                'muonEcalDetIds',
                'muonShowerInformation',
                'muIsoDepositTk',
                'muIsoDepositCalByAssociatorTowers',
                'muIsoDepositJets'],

  'Calo Clustering' : ['islandBasicClusters',
                       'islandSuperClusters',
                       'correctedIslandBarrelSuperClusters',
                       'correctedIslandEndcapSuperClusters',
                       'cleanedHybridSuperClusters',
                       'uncleanedHybridSuperClusters',
                       'hybridSuperClusters',
                       'correctedHybridSuperClusters',
                       'uncleanedOnlyCorrectedHybridSuperClusters',
                       'multi5x5BasicClustersCleaned',
                       'multi5x5SuperClustersCleaned',
                       'multi5x5BasicClustersUncleaned',
                       'multi5x5SuperClustersUncleaned',
                       'multi5x5SuperClusters',
                       'multi5x5SuperClustersWithPreshower',
                       'correctedMulti5x5SuperClustersWithPreshower',
                       'multi5x5PreshowerClusterShape',
                       'uncleanedOnlyMulti5x5SuperClustersWithPreshower',
                       'uncleanedOnlyCorrectedMulti5x5SuperClustersWithPreshower',
                       'correctedEndcapSuperClustersWithPreshower',
                       'preshowerClusterShape',
                       'towerMaker',
                       'towerMakerWithHO',
                       'caloTowers',
                       'hcalnoise',
                       'CastorTowerReco'],

  'Electrons and Photons' : ['ecalDrivenElectronSeeds',
                             'electronCkfTrackCandidates',
                             'electronGsfTracks',
                             'ecalDrivenGsfElectronCores',
                             'ecalDrivenGsfElectrons',
                             'photonCore',
                             'photons',
                             'electronsWithPresel',
                             'mvaElectrons',
                             'gedGsfElectronCores',
                             'gedGsfElectronsTmp',
                             'gedPhotonCore',
                             'gedPhotonsTmp'],

  'Particle Flow' : ['particleFlowRecHitPS',
                     'particleFlowClusterPS',
                     'particleFlowRecHitECAL',
                     'particleFlowClusterECALUncorrected',
                     'particleFlowClusterECAL',
                     'particleFlowRecHitHBHE',
                     'particleFlowRecHitHF',
                     'particleFlowClusterHBHE',
                     'particleFlowClusterHF',
                     'particleFlowClusterHCAL',
                     'particleFlowRecHitHO',
                     'particleFlowClusterHO',
                     'particleFlowSuperClusterECAL',
                     'pfTrack',
                     'pfTrackElec',
                     'particleFlowBlock',
                     'particleFlowEGamma',
                     'particleFlowTmp',
                     'PFTowers'],

  'Voronoi Subtraction' : ['voronoiBackgroundCalo',
                           'voronoiBackgroundPF'],

  'Jet Reco' : ['iterativeConePu5CaloJets',
                'akPu3CaloJets',
                'akPu4CaloJets',
                'akPu5CaloJets',
                'akVs2CaloJets',
                'akVs3CaloJets',
                'akVs4CaloJets',
                'akVs5CaloJets',
                'akPu3PFJets',
                'akPu4PFJets',
                'akPu5PFJets',
                'akVs3PFJets',
                'akVs4PFJets',
                'akVs5PFJets',
                'ak5CastorJets',
                'ak5CastorJetID',
                'ak7CastorJets',
                'ak7CastorJetID'],

  'Global Observables' : ['hiCentrality','hiEvtPlane'],

  'File Output' : ['MEtoEDMConverter',
                   'RECODEBUGoutput']

}

print '\n\n\n'

accountedTime = 0.0

groupTimes = dict()

for g in groups:
  time = 0.0
  for m in groups[g]:
    time += modules[m]
  groupTimes[g] = time
  accountedTime += time

for g in sorted(groupTimes, key=groupTimes.get, reverse=True ):
  print '{} :    {}'.format(g,groupTimes[g])

print '\n\n{} seconds accounted for out of {} total'.format(accountedTime,totalTime)


#
# report ungrouped modules
#

print '\n\nUngrouped modules:'

ungrouped = list()

for m in modules:
  isGrouped = False
  for g in groups:
    if m in groups[g]: isGrouped = True
  if isGrouped == False: print m



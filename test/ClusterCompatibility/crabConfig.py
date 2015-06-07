from CRABClient.UserUtilities import config
config = config()

config.General.requestName = 'HIRECO_2011data_75X_CCtest2'
config.General.workArea = 'crab_projects3'

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'step2_RAW2DIGI_L1Reco_RECO.py'

config.Data.inputDataset = '/HIMinBiasUPC/HIRun2011-v1/RAW'
config.Data.inputDBS = 'global'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 1
config.Data.lumiMask = 'json.txt'
#config.Data.runRange = '193093-193999' # '193093-194075'
config.Data.outLFN = '/store/user/appeltel/HIRECO_2011data_75X_CCtest2' # or '/store/group/<subdir>'
config.Data.publication = True
config.Data.publishDataName = 'HIRECO_2011data_75X_CCtest2'

config.Site.storageSite = 'T2_US_Vanderbilt'

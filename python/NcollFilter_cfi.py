import FWCore.ParameterSet.Config as cms

ncollFilter  = cms.EDFilter('NcollFilter',
    generators = cms.vstring("generator"),
    ncollmax = cms.int32(3000)
)

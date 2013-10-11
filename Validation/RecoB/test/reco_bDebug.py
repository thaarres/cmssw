# The following comments couldn't be translated into the new config version:

#! /bin/env cmsRun

import FWCore.ParameterSet.Config as cms

process = cms.Process("validation")
process.load("DQMServices.Components.DQMEnvironment_cfi")

#keep the logging output to a nice level
process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.load("DQMServices.Core.DQM_cfg")

# load the full reconstraction configuration, to make sure we're getting all needed dependencies
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")

process.load("PhysicsTools.JetMCAlgos.CaloJetsMCFlavour_cfi")  

process.load("Validation.RecoB.bTagAnalysis_cfi")
process.bTagValidation.jetMCSrc = 'AK5byValAlgo'
process.bTagValidation.allHistograms = True 
#process.bTagValidation.fastMC = True

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(500)
)
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring()
)

process.GlobalTag.globaltag = 'START53_V27::All'

process.load("SimTracker.TrackHistory.TrackClassifier_cff")

process.allBParticles  = cms.EDFilter("GenParticleSelector",
     filter = cms.bool(False),
     src = cms.InputTag("genParticles"),
    cut = cms.string('((abs(pdgId) == 5) || (abs(pdgId) > 500  && abs(pdgId) < 600) || (abs(pdgId) > 5000  && abs(pdgId) < 6000))'),
     stableOnly = cms.bool(False)
)
process.bParticlesFilter  = cms.EDFilter("GenParticleSelector",
     filter = cms.bool(True),
     src = cms.InputTag("genParticles"),
    cut = cms.string('((abs(pdgId) == 5) || (abs(pdgId) > 500  && abs(pdgId) < 600) || (abs(pdgId) > 5000  && abs(pdgId) < 6000)) && pt > 100'),
     stableOnly = cms.bool(False)
)
process.cParticlesFilter  = cms.EDFilter("GenParticleSelector",
     filter = cms.bool(False),
     src = cms.InputTag("genParticles"),
    cut = cms.string('(abs(pdgId) == 4) || (abs(pdgId) > 400  && abs(pdgId) < 500) || (abs(pdgId) > 4000  && abs(pdgId) < 5000)'),
     stableOnly = cms.bool(False)
)

process.bTracksProducer = cms.EDProducer("BTracksProducer",
    trackConfig = cms.PSet(process.trackClassifier),
    simG4 = cms.InputTag("g4SimHits"),
    trackingTruth = cms.untracked.InputTag("mergedtruth","MergedTrackTruth")

)
#process.bTracksProducer.trackConfig.trackProducer = cms.untracked.InputTag("generalTracks")
process.bTracksProducer.trackConfig.enableSimToReco = cms.untracked.bool(True)
#process.bTracksProducer.trackConfig.bestMatchByMaxValue = cms.untracked.bool(True)
#process.bTracksProducer.trackingTruth.signalOnlyTP  = cms.untracked.bool(True)
process.load("RecoVertex.AdaptiveVertexFinder.inclusiveVertexing_cff")


process.p = cms.Path(process.allBParticles * process.bParticlesFilter* process.cParticlesFilter* process.bTracksProducer * process.myPartons* process.AK5Flavour * process.btagging * process.inclusiveVertexing)
process.PoolSource.fileNames = [
'file:/networkdata/cvernier/validation_533/CMSSW_5_3_3_patch2/src/Validation/RecoB/00020B8A-DCF3-E111-B63E-00266CF33340.root',
#file:/networkdata/arizzi/QCD470600/reco/001793DE-DB07-E211-B4CE-003048679162.root'
]

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('btag003.root'),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring("p"))
    
)
process.endpath= cms.EndPath(process.out)


process.load('CondCore.DBCommon.CondDBSetup_cfi')
process.BTauMVAJetTagComputerRecord = cms.ESSource('PoolDBESSource',
    process.CondDBSetup,
    timetype = cms.string('runnumber'),
    toGet = cms.VPSet(cms.PSet(
        record = cms.string('BTauGenericMVAJetTagComputerRcd'),
        tag = cms.string('MVAComputerContainer_Retrained53X_JetTags_v2')
    )),
    connect = cms.string('frontier://FrontierProd/CMS_COND_PAT_000'),
    BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService')
)
process.es_prefer_BTauMVAJetTagComputerRecord = cms.ESPrefer('PoolDBESSource','BTauMVAJetTagComputerRecord')



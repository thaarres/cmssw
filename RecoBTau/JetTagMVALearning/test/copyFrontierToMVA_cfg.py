import FWCore.ParameterSet.Config as cms

process = cms.Process("MVAJetTagsSQLiteSave")

process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.source = cms.Source("EmptySource")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'START53_V25::All'

#process.prefer("BTauMVAJetTagComputerRecord")

process.jetTagMVATrainerSave = cms.EDAnalyzer("JetTagMVATrainerFileSave",
	trained = cms.untracked.bool(False),
	CombinedMVA = cms.string('CombinedMVA.mva'), 
	CombinedSVRecoVertex = cms.string('CombinedSVRecoVertex.mva'), 
	CombinedSVPseudoVertex = cms.string('CombinedSVPseudoVertex.mva'), 
	CombinedSVNoVertex = cms.string('CombinedSVNoVertex.mva'), 
	CombinedSVMVARecoVertex = cms.string('CombinedSVMVARecoVertex.mva'), 
	CombinedSVMVAPseudoVertex = cms.string('CombinedSVMVAPseudoVertex.mva'), 
	CombinedSVMVANoVertex = cms.string('CombinedSVMVANoVertex.mva'),
	GhostTrackRecoVertex = cms.string('GhostTrackRecoVertex.mva'),
        GhostTrackPseudoVertex = cms.string('GhostTrackPseudoVertex.mva'),
        GhostTrackNoVertex = cms.string('GhostTrackNoVertex.mva')
)

process.outpath = cms.EndPath(process.jetTagMVATrainerSave)

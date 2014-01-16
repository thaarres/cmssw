import FWCore.ParameterSet.Config as cms

process = cms.Process("MVAJetTagsSQLiteSave")

process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.source = cms.Source("EmptySource")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

process.calib = cms.ESSource("BTauGenericMVAJetTagComputerFileSource",
	CombinedSVV2RecoVertex = cms.string('CombinedSVRecoVertex.mva'), 
	CombinedSVV2PseudoVertex = cms.string('CombinedSVPseudoVertex.mva'), 
	CombinedSVV2NoVertex = cms.string('CombinedSVNoVertex.mva'), 
)

process.PoolDBOutputService = cms.Service("PoolDBOutputService",
	BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
	DBParameters = cms.PSet( messageLevel = cms.untracked.int32(0) ),
	timetype = cms.untracked.string('runnumber'),
	connect = cms.string('sqlite_file:MVAJetTags.db'),
	toPut = cms.VPSet(cms.PSet(
		record = cms.string('BTauGenericMVAJetTagComputerRcd'),
		tag = cms.string('MVAJetTags')
	))
)

process.jetTagMVATrainerSave = cms.EDAnalyzer("JetTagMVATrainerSave",
	toPut = cms.vstring(),
	toCopy = cms.vstring(
		'CombinedSVV2RecoVertex', 
		'CombinedSVV2PseudoVertex', 
		'CombinedSVV2NoVertex', 
	)
)

process.outpath = cms.EndPath(process.jetTagMVATrainerSave)

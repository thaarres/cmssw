import FWCore.ParameterSet.Config as cms

process = cms.Process("IPTrainer")

process.source = cms.Source("EmptySource")

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1))

process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.CombinedSVV2Trainer = cms.EDAnalyzer("JetTagMVATreeTrainer",
	useCategories		= cms.bool(False),
	calibrationRecord	= cms.string("CombinedSVV2PseudoVertex"),
	ignoreFlavours		= cms.vint32(0, 1, 2, 3, 4, 21),
	signalFlavours		= cms.vint32(9),
	minimumTransverseMomentum = cms.double(15.0),
	minimumPseudoRapidity	= cms.double(0),
	maximumPseudoRapidity	= cms.double(2.5),

	#factor = cms.double(1),
	#bound = cms.double(50),

	fileNames = cms.vstring(
		" /afs/cern.ch/user/t/thaarres/CSVTraining/CMSSW_5_3_14/src/RecoBTau/JetTagMVALearning/test/rootfiles/RadionToHH_4b_M-1000_TuneZ2star_8TeV-Madgraph_pythia6_Summer12_DR53X-PU_S10_START53_V19-v1_AODSIM/RecoVsRecoReco/CombinedSVV2PseudoVertex_B.root",
		" /afs/cern.ch/user/t/thaarres/CSVTraining/CMSSW_5_3_14/src/RecoBTau/JetTagMVALearning/test/rootfiles/RadionToHH_4b_M-1000_TuneZ2star_8TeV-Madgraph_pythia6_Summer12_DR53X-PU_S10_START53_V19-v1_AODSIM/RecoVsRecoReco/CombinedSVV2PseudoVertex_BB.root"
	#),
	#weightFile = cms.string("weights/CombinedSVV2PseudoVertex_BBB_histo.txt"),
	#biasFiles = cms.vstring(
		#"*",
		#"-",
##		"weights/CombinedSVV2PseudoVertex_B_BB_ratio.txt"
		#"PseudoVertex_BB_B.txt"
	)
)

process.looper = cms.Looper("JetTagMVATrainerLooper",
	trainers = cms.VPSet(
		cms.PSet(
			calibrationRecord = cms.string("CombinedSVV2PseudoVertex"),
			trainDescription = cms.untracked.string("Save_Pseudo_B_BB.xml"),
			loadState = cms.untracked.bool(False),
			saveState = cms.untracked.bool(False)
		)
	)
)

process.p = cms.Path(process.CombinedSVV2Trainer)

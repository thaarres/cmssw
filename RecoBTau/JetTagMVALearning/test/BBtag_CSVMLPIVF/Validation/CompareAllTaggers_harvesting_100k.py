import FWCore.ParameterSet.Config as cms

process = cms.Process("harvesting")

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.EDMtoMEAtRunEnd_cff')
process.load('Configuration.StandardSequences.Harvesting_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.MessageLogger.cerr.threshold = 'ERROR'

# for the conditions
from Configuration.AlCa.autoCond import autoCond
process.GlobalTag.globaltag = autoCond['startup']

# Open and read list file
#filename = open('RootFiles/list.list', 'r')
#filelist = cms.untracked.vstring( filename.readlines() )

# Input source
process.source = cms.Source("PoolSource",
  #fileNames = filelist,
  fileNames = cms.untracked.vstring(),
  secondaryFileNames = cms.untracked.vstring(),
  processingMode = cms.untracked.string('RunsAndLumis')
)


process.options = cms.untracked.PSet(
  Rethrow = cms.untracked.vstring('ProductNotFound'),
  fileMode = cms.untracked.string('FULLMERGE')
)

from DQMOffline.RecoB.bTagCommon_cff import*
process.load("DQMOffline.RecoB.bTagCommon_cff")
#process.bTagCommonBlock.ptRecJetMin = cms.double(600.0)
process.bTagCommonBlock.ptRanges = cms.vdouble(0.0,40.0,60.0,90.0, 150.0,400.0,600.0,3000.0)
process.bTagCommonBlock.etaRanges = cms.vdouble(0.0, 1.2, 2.1, 2.4)

###############################################################################################

from Validation.RecoB.bTagAnalysis_cfi import *
process.load("Validation.RecoB.bTagAnalysis_cfi")

process.CustombTagValidation = process.bTagValidation.clone(
    tagConfig = cms.VPSet(
				cms.PSet(
            bTagTrackCountingAnalysisBlock,
            label = cms.InputTag("trackCountingHighEffBJetTags"),
            folder = cms.string("TCHE")
        ), 
        cms.PSet(
            bTagTrackCountingAnalysisBlock,
            label = cms.InputTag("trackCountingHighPurBJetTags"),
            folder = cms.string("TCHP")
        ), 
        cms.PSet(
            bTagProbabilityAnalysisBlock,
            label = cms.InputTag("jetProbabilityBJetTags"),
            folder = cms.string("JP")
        ), 
        cms.PSet(
            bTagBProbabilityAnalysisBlock,
            label = cms.InputTag("jetBProbabilityBJetTags"),
            folder = cms.string("JBP")
        ), 
        cms.PSet(
            bTagSimpleSVAnalysisBlock,
            label = cms.InputTag("simpleSecondaryVertexHighEffBJetTags"),
            folder = cms.string("SSVHE")
        ), 
        cms.PSet(
            bTagSimpleSVAnalysisBlock,
            label = cms.InputTag("simpleSecondaryVertexHighPurBJetTags"),
            folder = cms.string("SSVHP")
        ), 
	#Comment out if you DON'T want to compare with old CSV!!!!!!
        cms.PSet(
				    parameters = cms.PSet(
        			discriminatorStart = cms.double(-0.1),
        			discriminatorEnd = cms.double(1.05),
        			nBinEffPur = cms.int32(200),
        			# the constant b-efficiency for the differential plots versus pt and eta
        			effBConst = cms.double(0.5),
        			endEffPur = cms.double(1.005),
        			startEffPur = cms.double(-0.005)
    				),
            label = cms.InputTag("combinedSecondaryVertexBJetTags"),
            folder = cms.string("oldCSV") # standard CSV for 7 TeV data taking
        ), 
        cms.PSet(
				    parameters = cms.PSet(
        			discriminatorStart = cms.double(-0.1),
        			discriminatorEnd = cms.double(1.05),
        			nBinEffPur = cms.int32(200),
        			# the constant b-efficiency for the differential plots versus pt and eta
        			effBConst = cms.double(0.5),
        			endEffPur = cms.double(1.005),
        			startEffPur = cms.double(-0.005)
    				),
            label = cms.InputTag("combinedSecondaryVertexV2BJetTags"),
            folder = cms.string("CSVV2") # MLP-based CSV
        ),
        cms.PSet(
				    parameters = cms.PSet(
        			discriminatorStart = cms.double(-0.1),
        			discriminatorEnd = cms.double(1.05),
        			nBinEffPur = cms.int32(200),
        			# the constant b-efficiency for the differential plots versus pt and eta
        			effBConst = cms.double(0.5),
        			endEffPur = cms.double(1.005),
        			startEffPur = cms.double(-0.005)
    				),
            label = cms.InputTag("combinedSecondaryVertexIVFBJetTags"),
            folder = cms.string("oldCSVIVF") # standard CSV+IVF for 7 TeV data taking
        ), 
        cms.PSet(
				    parameters = cms.PSet(
        			discriminatorStart = cms.double(-0.1),
        			discriminatorEnd = cms.double(1.05),
        			nBinEffPur = cms.int32(200),
        			# the constant b-efficiency for the differential plots versus pt and eta
        			effBConst = cms.double(0.5),
        			endEffPur = cms.double(1.005),
        			startEffPur = cms.double(-0.005)
    				),
            label = cms.InputTag("combinedSecondaryVertexIVFV2BJetTags"),
            folder = cms.string("CSVIVFV2") # MLP+IVF-based CSV
        ),
	#Use this if you want to add your new tagger!!
	cms.PSet(
				  parameters = cms.PSet(
				discriminatorStart = cms.double(-0.1),
				discriminatorEnd = cms.double(1.05),
				nBinEffPur = cms.int32(200),
				# the constant b-efficiency for the differential plots versus pt and eta
				effBConst = cms.double(0.5),
				endEffPur = cms.double(1.005),
				startEffPur = cms.double(-0.005)
				),
             label = cms.InputTag("combinedSecondaryVertexIVFV2NEWBJetTags"),
             folder = cms.string("CSVIVFV2NEW") # Version 2 MLP+IVF-based CSVIVFV2NEW
        ) 
			)
)

process.dqmEnv.subSystemFolder = 'BTAG'
process.dqmSaver.producer = 'DQM'
#process.dqmSaver.workflow = '/POG/BTAG/BJETtrained20k_eachptetabin_NewTrackSelection_modified16_tighttracksel'
process.dqmSaver.workflow = '/POG/BTAG/CSVV2'
process.dqmSaver.convention = 'Offline'
process.dqmSaver.saveByRun = cms.untracked.int32(-1)
process.dqmSaver.saveAtJobEnd =cms.untracked.bool(True)
process.dqmSaver.forceRunNumber = cms.untracked.int32(1)

# Path and EndPath definitions
process.edmtome_step = cms.Path(process.EDMtoME)
process.bTagValidation_step = cms.Path(process.CustombTagValidation)
process.dqmsave_step = cms.Path(process.DQMSaver)

# Schedule definition
process.schedule = cms.Schedule(
  process.edmtome_step,
  process.bTagValidation_step,
  process.dqmsave_step
)
#process.PoolSource.fileNames = ['file:/afs/cern.ch/user/t/thaarres/removeMe/CMSSW_5_3_14/src/RecoBTau/JetTagMVALearning/test/CSV_MLP/Validation/DQMfile.root']
process.PoolSource.fileNames = ['/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_10_1_aGD.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_29_1_yvs.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_2_1_XXr.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_8_1_GGL.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_22_1_zXD.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_35_1_Lvj.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_18_1_UpI.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_11_1_8Q7.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_24_1_KuW.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_14_1_ua9.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_15_1_Mgu.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_7_1_YW2.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_43_1_pZH.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_25_1_A6L.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_5_1_x8h.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_26_1_cm0.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_86_1_T7w.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_73_1_3mS.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_28_1_faS.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_85_1_TwM.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_92_1_ueP.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_20_1_3NT.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_50_1_9lu.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_12_1_u2a.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_74_1_EL5.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_23_1_wEq.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_53_1_dCQ.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_9_1_0gp.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_87_1_45C.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_46_1_66b.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_4_1_HzK.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_3_1_N0G.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_13_1_rMY.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_30_1_Fdk.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_49_1_OqS.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_57_1_PZZ.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_32_1_PrK.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_56_1_Kgw.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_17_1_f57.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_99_1_mnw.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_93_1_fwq.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_36_1_5xH.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_16_1_X89.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_100_1_5kj.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_1_1_53w.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_27_1_KDZ.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_55_1_5JC.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_62_1_Cm5.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_41_1_buH.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_39_1_daT.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_54_1_bic.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_42_1_7HG.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_6_1_8cS.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_37_1_W0q.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_69_1_nGC.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_66_1_TaL.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_31_1_r2S.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_40_1_Ruw.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_19_1_pSC.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_38_1_Ptm.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_96_1_A6B.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_21_1_TQ2.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_44_1_mDK.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_33_1_xLc.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_78_1_DkW.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_83_1_WpK.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_80_1_OxO.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_52_1_Lyc.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_75_1_jLH.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_98_1_V4X.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_64_1_tfx.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_47_1_EuH.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_48_1_cqe.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_51_1_a80.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_70_1_cZz.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_94_1_UNA.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_34_1_vBP.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_82_1_jDB.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_79_1_djc.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_68_1_KMu.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_90_1_Kct.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_88_1_mfN.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_72_1_COR.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_76_1_Jmi.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_60_1_nJU.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_89_1_iTj.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_58_1_Vt4.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_71_1_uHF.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_61_1_UIm.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_81_1_AcA.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_59_1_0Tt.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_63_1_Gak.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_65_1_2Ar.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_95_1_HYG.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_97_1_mO9.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_45_1_pQa.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_67_1_APN.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_84_1_X1S.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_91_1_u3K.root',
'/store/user/thaarres/DQMfiles_AllwBias020714/DQMfile_AllwBias020714_77_1_mrs.root',
]

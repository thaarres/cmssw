####### Process initialization ##########

import FWCore.ParameterSet.Config as cms

process = cms.Process("Ntuple")
process.load("Configuration.EventContent.EventContent_cff")
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.GlobalTag.globaltag = '80X_mcRun2_asymptotic_v4'
	   
####### Config parser ##########
import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing ('analysis')
options.maxEvents = 10
options.inputFiles = 'dcap://t3se01.psi.ch:22125//pnfs/psi.ch/cms/trivcat/store/mc/RunIISpring16MiniAODv2/BulkGravToWW_narrow_M-1000_13TeV-madgraph/MINIAODSIM/PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/00000/E234D164-B425-E611-BA38-0025905C94D2.root'
options.parseArguments()
process.options  = cms.untracked.PSet( 
                     wantSummary = cms.untracked.bool(False),
                     SkipEvent = cms.untracked.vstring('ProductNotFound'),
                     allowUnscheduled = cms.untracked.bool(True)
                     )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(options.inputFiles)
                            )   
							
							                  
####### Redo Jet clustering sequences ##########
fatjet_ptmin = 170.0

from RecoJets.Configuration.RecoPFJets_cff import ak8PFJetsPuppiSoftDrop    
from EXOVVNtuplizerRunII.Ntuplizer.redoPatJets_cff import patJetCorrFactorsAK8, patJetsAK8, selectedPatJetsAK8 # Redo pat jets from gen AK8
from RecoJets.JetProducers.ak8PFJetsPuppi_groomingValueMaps_cfi import ak8PFJetsPuppiSoftDropMass
                                                                                         
process.load('CommonTools/PileupAlgos/Puppi_cff')
process.puppi.useExistingWeights = True
process.puppi.candName = cms.InputTag('packedPFCandidates')
process.puppi.vertexName = cms.InputTag('offlineSlimmedPrimaryVertices')  

#Re-running AK8 PUPPI
process.load('RecoJets.JetProducers.ak8PFJetsPuppi_cfi')
process.ak8PFJetsPuppi.doAreaFastjet = True
process.NjettinessAK8Puppi = cms.EDProducer("NjettinessAdder",
             src = cms.InputTag("ak8PFJetsPuppi"),
             Njets = cms.vuint32(1, 2),
             # variables for measure definition :
             measureDefinition = cms.uint32( 0 ), 	# CMS default is normalized measure ==0, unnormalized measure==1
             beta = cms.double(1.0),        		# CMS default is 1
             R0 = cms.double( 0.8 ),        		# CMS default is jet cone size
             Rcutoff = cms.double( 999.0),      	# not used by default
             # variables for axes definition :
             axesDefinition = cms.uint32( 1 ),    	# CMS default is 1-pass KT axes --> 3 is WTA kT axes!
             nPass = cms.int32(999),       			# not used by default
             akAxesR0 = cms.double(-999.0)      	# not used by default
             )
									 
process.AK8PFJetsPuppi = patJetsAK8.clone( jetSource = 'ak8PFJetsPuppi' )
process.AK8PFJetsPuppi.userData.userFloats.src = [cms.InputTag("NjettinessAK8Puppi:tau1"), cms.InputTag("NjettinessAK8Puppi:tau2")]
process.AK8PFJetsPuppi.addJetCorrFactors = cms.bool(False)
process.selectedAK8PFJetsPuppi = selectedPatJetsAK8.clone( src = 'AK8PFJetsPuppi', cut = cms.string('pt > 200') )




#Beta==0 jets --> TAGGED, used for Tau1
process.ak8PuppiJetsSoftDropBeta0 = ak8PFJetsPuppiSoftDrop.clone(jetPtMin = fatjet_ptmin, beta = cms.double(0.0)  )
process.NjettinessAK8PuppiSoftdropBeta0 = cms.EDProducer("NjettinessAdder",
             src = cms.InputTag("ak8PuppiJetsSoftDropBeta0"),
             Njets = cms.vuint32(1, 2),
             # variables for measure definition :
             measureDefinition = cms.uint32( 0 ), 	# CMS default is normalized measure ==0, unnormalized measure==1
             beta = cms.double(1.0),        		# CMS default is 1
             R0 = cms.double( 0.8 ),        		# CMS default is jet cone size
             Rcutoff = cms.double( 999.0),      	# not used by default
             # variables for axes definition :
             axesDefinition = cms.uint32( 1 ),    	# CMS default is 1-pass KT axes --> 3 is WTA kT axes!
             nPass = cms.int32(999),       			# not used by default
             akAxesR0 = cms.double(-999.0)      	# not used by default
             )
# process.ak8PFJetsPuppiSoftDropMass = cms.EDProducer("RecoJetDeltaRValueMapProducer",
#                                          src = cms.InputTag("ak8PuppiJetsSoftDrop"),
#                                          matched = cms.InputTag("ak8PuppiJetsSoftDrop"),       #match to itself, nonsense but just a test :)
#                                          distMax = cms.double(0.8),
#                                          value = cms.string('mass')
# 										 )
process.AK8PuppiJetsSoftDropB0 = patJetsAK8.clone( jetSource = 'ak8PuppiJetsSoftDropBeta0' )
process.AK8PuppiJetsSoftDropB0.userData.userFloats.src = [cms.InputTag("NjettinessAK8PuppiSoftdropBeta0:tau1"), cms.InputTag("NjettinessAK8PuppiSoftdropBeta0:tau2")]
# process.AK8PuppiJetsSoftDrop.userData.userFloats.src += ['ak8PFJetsPuppiSoftDropMass']
process.AK8PuppiJetsSoftDropB0.addJetCorrFactors = cms.bool(False)
process.selectedAK8PuppiJetsSoftDropB0 = selectedPatJetsAK8.clone( src = 'AK8PuppiJetsSoftDropB0', cut = cms.string('pt > 200') )



#Beta==2 jets --> GROOMED, used for Tau2
process.ak8PuppiJetsSoftDropBeta2 = ak8PFJetsPuppiSoftDrop.clone(jetPtMin = fatjet_ptmin, beta =  cms.double(2.0)  )
process.NjettinessAK8PuppiSoftdropBeta2 = cms.EDProducer("NjettinessAdder",
             src = cms.InputTag("ak8PuppiJetsSoftDropBeta2"),
             Njets = cms.vuint32(1, 2),
             # variables for measure definition :
             measureDefinition = cms.uint32( 0 ), # CMS default is normalized measure ==0, unnormalized measure==1
             beta = cms.double(1.0),        # CMS default is 1
             R0 = cms.double( 0.8 ),        # CMS default is jet cone size
             Rcutoff = cms.double( 999.0),      # not used by default
             # variables for axes definition :
             axesDefinition = cms.uint32( 1 ),    # CMS default is 1-pass KT axes --> 3 is WTA kT axes!
             nPass = cms.int32(999),       # not used by default
             akAxesR0 = cms.double(-999.0)      # not used by default
             )
process.AK8PuppiJetsSoftDropB2 = patJetsAK8.clone( jetSource = 'ak8PuppiJetsSoftDropBeta2' )
process.AK8PuppiJetsSoftDropB2.userData.userFloats.src = [cms.InputTag("NjettinessAK8PuppiSoftdropBeta2:tau1"), cms.InputTag("NjettinessAK8PuppiSoftdropBeta2:tau2")]
process.AK8PuppiJetsSoftDropB2.addJetCorrFactors = cms.bool(False)
process.selectedAK8PuppiJetsSoftDropB2 = selectedPatJetsAK8.clone( src = 'AK8PuppiJetsSoftDropB2', cut = cms.string('pt > 200') )




# Adding energy correlation functions

# For Puppi jets
from RecoJets.JetProducers.ECF_cfi import ECF 
rangeECF = range(1,3+1)
process.ak8PFJetsPuppiECF = ECF.clone(
								src = cms.InputTag( "ak8PFJetsPuppi" ),
								Njets = cms.vuint32( rangeECF ),
								beta = cms.double( 1.0 )
								)
process.AK8PFJetsPuppi.userData.userFloats.src += ['ak8PFJetsPuppiECF:ecf1','ak8PFJetsPuppiECF:ecf2','ak8PFJetsPuppiECF:ecf3']

# For Softdrop Beta==0 jets
from RecoJets.JetProducers.ECF_cfi import ECF 
rangeECF = range(1,3+1)
process.ak8PuppiJetsSoftDropBeta0ECF = ECF.clone(
								src = cms.InputTag( "ak8PuppiJetsSoftDropBeta0" ),
								Njets = cms.vuint32( rangeECF ),
								beta = cms.double( 1.0 )
								)
process.AK8PuppiJetsSoftDropB0.userData.userFloats.src += ['ak8PuppiJetsSoftDropBeta0ECF:ecf1','ak8PuppiJetsSoftDropBeta0ECF:ecf2','ak8PuppiJetsSoftDropBeta0ECF:ecf3']

# For Softdrop Beta==2 jets
from RecoJets.JetProducers.ECF_cfi import ECF 
rangeECF = range(1,3+1)
process.ak8PuppiJetsSoftDropBeta2ECF = ECF.clone(
								src = cms.InputTag( "ak8PuppiJetsSoftDropBeta2" ),
								Njets = cms.vuint32( rangeECF ),
								beta = cms.double( 1.0 )
								)
process.AK8PuppiJetsSoftDropB2.userData.userFloats.src += ['ak8PuppiJetsSoftDropBeta2ECF:ecf1','ak8PuppiJetsSoftDropBeta2ECF:ecf2','ak8PuppiJetsSoftDropBeta2ECF:ecf3']



#Assign everything to PUPPI jet
process.SoftDropMassBeta0 = cms.EDProducer("RecoJetToPatJetDeltaRValueMapProducer",
                                        src = cms.InputTag("ak8PFJetsPuppi"),
                                        matched = cms.InputTag("AK8PuppiJetsSoftDropB0"),                                         
                                        distMax = cms.double(0.8),
                                        values = cms.vstring([
                                            'userFloat("NjettinessAK8PuppiSoftdropBeta0:tau1")',
                                            'userFloat("NjettinessAK8PuppiSoftdropBeta0:tau2")',
											'userFloat("ak8PuppiJetsSoftDropBeta0ECF:ecf1")',
											'userFloat("ak8PuppiJetsSoftDropBeta0ECF:ecf2")',
											'userFloat("ak8PuppiJetsSoftDropBeta0ECF:ecf3")',
                                            'pt','eta','phi','mass'
                                        ]),
                                        valueLabels = cms.vstring( [
                                            'NjettinessAK8PuppiSoftdropBeta0Tau1',
                                            'NjettinessAK8PuppiSoftdropBeta0Tau2',
											'ak8PuppiJetsSoftDropBeta0ECF1',
											'ak8PuppiJetsSoftDropBeta0ECF2',
											'ak8PuppiJetsSoftDropBeta0ECF3',
                                            'pt','eta','phi','mass'
                                        ])
                    )
process.SoftDropMassBeta2 = cms.EDProducer("RecoJetToPatJetDeltaRValueMapProducer",
                                        src = cms.InputTag("ak8PFJetsPuppi"),
                                        matched = cms.InputTag("AK8PuppiJetsSoftDropB2"),                                         
                                        distMax = cms.double(0.8),
                                        values = cms.vstring([
                                            'userFloat("NjettinessAK8PuppiSoftdropBeta2:tau1")',
                                            'userFloat("NjettinessAK8PuppiSoftdropBeta2:tau2")',
											'userFloat("ak8PuppiJetsSoftDropBeta2ECF:ecf1")',
											'userFloat("ak8PuppiJetsSoftDropBeta2ECF:ecf2")',
											'userFloat("ak8PuppiJetsSoftDropBeta2ECF:ecf3")',
                                            'pt','eta','phi','mass'
                                        ]),
                                        valueLabels = cms.vstring( [
                                            'NjettinessAK8PuppiSoftdropBeta2Tau1',
                                            'NjettinessAK8PuppiSoftdropBeta2Tau2',
											'ak8PuppiJetsSoftDropBeta2ECF1',
											'ak8PuppiJetsSoftDropBeta2ECF2',
											'ak8PuppiJetsSoftDropBeta2ECF3',
                                            'pt','eta','phi','mass'
                                        ])
                    )
					
					
					

#Some control histograms
# process.TFileService = cms.Service("TFileService",
#     fileName = cms.string("histo.root")
# )
#
# process.plotPUPPI = cms.EDAnalyzer('CandViewHistoAnalyzer',
#     src = cms.InputTag("AK8PuppiJetsSoftDropB2"),
#     histograms = cms.VPSet(	cms.PSet(
#         						itemsToPlot = cms.untracked.int32(2),
#         						min = cms.untracked.double(0.),
#         						max = cms.untracked.double(250.),
#         						nbins = cms.untracked.int32(50),
#         						description = cms.untracked.string('jet %d PUPPI+softdrop mass [GeV]'),
#         						name = cms.untracked.string('jet_%d_mass'),
#         						plotquantity = cms.untracked.string('mass'),
# 								),
# 							)
# 						)
#
# process.plotB0 = cms.EDAnalyzer('CandViewHistoAnalyzer',
#     src = cms.InputTag("AK8PuppiJetsSoftDropB0"),
#     histograms = cms.VPSet(	cms.PSet(
#         						itemsToPlot = cms.untracked.int32(2),
#         						min = cms.untracked.double(0.),
#         						max = cms.untracked.double(250.),
#         						nbins = cms.untracked.int32(50),
#         						description = cms.untracked.string('jet %d PUPPI+softdrop mass [GeV]'),
#         						name = cms.untracked.string('jet_%d_mass'),
#         						plotquantity = cms.untracked.string('mass'),
# 								),
# 							)
# 						)
# process.plotB2 = cms.EDAnalyzer('CandViewHistoAnalyzer',
#     src = cms.InputTag("AK8PuppiJetsSoftDropB2"),
#     histograms = cms.VPSet(	cms.PSet(
#         						itemsToPlot = cms.untracked.int32(2),
#         						min = cms.untracked.double(0.),
#         						max = cms.untracked.double(250.),
#         						nbins = cms.untracked.int32(50),
#         						description = cms.untracked.string('jet %d PUPPI+softdrop mass [GeV]'),
#         						name = cms.untracked.string('jet_%d_mass'),
#         						plotquantity = cms.untracked.string('mass'),
# 								),
# 							)
# 						)



  
  
# process.p = cms.Path()
# process.p = process.chs*process.ak8CHSJetsSoftDrop*process.NjettinessAK8Puppi

process.out = cms.OutputModule("PoolOutputModule",
		fileName=cms.untracked.string("substructure.root"),
        outputCommands = cms.untracked.vstring('drop *',
   					   'keep patJets_*_*_*',
					   # 'keep *_NjettinessAK8_*_*',
				   #   					   'keep *_NjettinessAK8Puppi_*_*',
				   # 					   'keep *_NjettinessAK8PuppiSoftdropBeta0_*_*',
				   # 					   'keep *_NjettinessAK8PuppiSoftdropBeta2_*_*',
				   # 					   'keep *_ak8PFJetsPuppiECF_*_*',
				   # 					   'keep *_ak8PuppiJetsSoftDropBeta0ECF_*_*',
				   # 					   'keep *_ak8PuppiJetsSoftDropBeta2ECF_*_*',
					   'keep *_SoftDropMassBeta0_*_*',
					   'keep *_SoftDropMassBeta2_*_*',
   					   ),
)


# process.outpath = cms.EndPath(process.out*process.plotPUPPI*process.plotB0*process.plotB2)
process.outpath = cms.EndPath(process.out)

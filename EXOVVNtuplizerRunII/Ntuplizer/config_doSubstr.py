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
options.maxEvents = 2000
# options.inputFiles = 'dcap://t3se01.psi.ch:22125//pnfs/psi.ch/cms/trivcat/store/mc/RunIISpring16MiniAODv2/WprimeToWZToWhadZhad_narrow_M-2000_13TeV-madgraph/MINIAODSIM/PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/00000/74356808-EF40-E611-9CA9-D4AE52651C5F.root'
options.inputFiles = 'dcap://t3se01.psi.ch:22125//pnfs/psi.ch/cms/trivcat/store/mc/RunIISpring16MiniAODv2/QCD_Pt-15to7000_TuneCUETHS1_Flat_13TeV_herwigpp/MINIAODSIM/PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/60000/F2FDF3C3-031C-E611-A090-0050560210EA.root'
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
fatjet_ptmin = 200.0

from RecoJets.Configuration.RecoPFJets_cff import ak8PFJetsPuppiSoftDrop    
from EXOVVNtuplizerRunII.Ntuplizer.redoPatJets_cff import patJetCorrFactorsAK8, patJetsAK8, selectedPatJetsAK8 # Redo pat jets from gen AK8
from RecoJets.JetProducers.ak8PFJetsPuppi_groomingValueMaps_cfi import ak8PFJetsPuppiSoftDropMass
                                                                                         



#Re-running AK8 PUPPI
process.load('CommonTools/PileupAlgos/Puppi_cff')
process.puppi.useExistingWeights = True
process.puppi.candName = cms.InputTag('packedPFCandidates')
process.puppi.vertexName = cms.InputTag('offlineSlimmedPrimaryVertices')


from PhysicsTools.PatAlgos.tools.jetTools import addJetCollection
from PhysicsTools.PatAlgos.producersLayer1.jetProducer_cfi import _patJets as patJetsDefault

process.load('RecoJets.JetProducers.ak8PFJetsPuppi_cfi')
process.ak8PFJetsPuppi.doAreaFastjet = True # even for standard ak8PFJets this is overwritten in RecoJets/Configuration/python/RecoPFJets_cff
from RecoJets.JetAssociationProducers.j2tParametersVX_cfi import j2tParametersVX
process.ak8PFJetsPuppiTracksAssociatorAtVertex = cms.EDProducer("JetTracksAssociatorAtVertex",
    j2tParametersVX,
    jets = cms.InputTag("ak8PFJetsPuppi")
)
process.patJetAK8PuppiCharge = cms.EDProducer("JetChargeProducer",
    src = cms.InputTag("ak8PFJetsPuppiTracksAssociatorAtVertex"),
    var = cms.string('Pt'),
    exp = cms.double(1.0)
)
    
addJetCollection(process, labelName = 'AK8Puppi',
                 jetSource = cms.InputTag('ak8PFJetsPuppi'),
                 algo= 'AK', rParam = 0.8,
				 pvSource = cms.InputTag('offlineSlimmedPrimaryVertices'),
				 genParticles =  cms.InputTag('prunedGenParticles'),
                 jetCorrections = ('AK8PFPuppi', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'None'),
                 genJetCollection = cms.InputTag('slimmedGenJetsAK8')
                 )


process.load('RecoJets.JetProducers.nJettinessAdder_cfi')
process.NjettinessAK8Puppi 						= process.Njettiness.clone()
process.NjettinessAK8Puppi.src 					= cms.InputTag("ak8PFJetsPuppi")
process.NjettinessAK8Puppi.cone 				= cms.double(0.8)
process.NjettinessAK8Puppi.Njets				=cms.vuint32(1,2)
process.patJetsAK8Puppi.userData.userFloats.src = []
process.patJetsAK8Puppi.userData.userFloats.src += ['NjettinessAK8Puppi:tau1','NjettinessAK8Puppi:tau2']				 
process.selectedPatJetsAK8Puppi.cut = cms.string("pt > 200 && mass > 0.1")




#Beta==0 jets --> TAGGED, used for Tau1
process.ak8PuppiJetsSoftDropBeta0 = ak8PFJetsPuppiSoftDrop.clone(jetPtMin = fatjet_ptmin, beta = cms.double(0.0)  )
addJetCollection(process, labelName = 'AK8PuppiMMDT',
                 jetSource = cms.InputTag('ak8PuppiJetsSoftDropBeta0'),
                 algo= 'AK', rParam = 0.8,
				 pvSource = cms.InputTag('offlineSlimmedPrimaryVertices'),
				 genParticles =  cms.InputTag('prunedGenParticles'),
                 jetCorrections = ('AK8PFPuppi', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'None'),
                 genJetCollection = cms.InputTag('slimmedGenJetsAK8'),
                 )
				 
				 
process.NjettinessAK8PuppiSoftdropBeta0 						= process.NjettinessAK8Puppi.clone()
process.NjettinessAK8PuppiSoftdropBeta0.src 					= cms.InputTag("ak8PuppiJetsSoftDropBeta0")
process.patJetsAK8PuppiMMDT.userData.userFloats.src = []
process.patJetsAK8PuppiMMDT.userData.userFloats.src += ['NjettinessAK8PuppiSoftdropBeta0:tau1','NjettinessAK8PuppiSoftdropBeta0:tau2']				 
process.selectedPatJetsAK8Puppi.cut = cms.string("pt > 200 && mass > 0.1")



#Beta==2 jets --> GROOMED, used for Tau2
process.ak8PuppiJetsSoftDropBeta2 = ak8PFJetsPuppiSoftDrop.clone(jetPtMin = fatjet_ptmin, beta = cms.double(2.0)  )
addJetCollection(process, labelName = 'AK8PuppiSD',
                 jetSource = cms.InputTag('ak8PuppiJetsSoftDropBeta2'),
                 algo= 'AK', rParam = 0.8,
				 pvSource = cms.InputTag('offlineSlimmedPrimaryVertices'),
				 genParticles =  cms.InputTag('prunedGenParticles'),
                 jetCorrections = ('AK8PFPuppi', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'None'),
                 genJetCollection = cms.InputTag('slimmedGenJetsAK8'),
                 )
				 
				 
process.NjettinessAK8PuppiSoftdropBeta2 						= process.NjettinessAK8Puppi.clone()
process.NjettinessAK8PuppiSoftdropBeta2.src 					= cms.InputTag("ak8PuppiJetsSoftDropBeta2")
process.patJetsAK8PuppiSD.userData.userFloats.src = []
process.patJetsAK8PuppiSD.userData.userFloats.src += ['NjettinessAK8PuppiSoftdropBeta2:tau1','NjettinessAK8PuppiSoftdropBeta2:tau2']				 
process.selectedPatJetsAK8PuppiSD.cut = cms.string("pt > 200 && mass > 0.1")




# Adding energy correlation functions

# For Puppi jets
from RecoJets.JetProducers.ECF_cfi import ECF 
rangeECF = range(1,3+1)
process.ak8PFJetsPuppiECF = ECF.clone(
								src = cms.InputTag( "ak8PFJetsPuppi" ),
								Njets = cms.vuint32( rangeECF ),
								beta = cms.double( 1.0 )
								)
process.patJetsAK8Puppi.userData.userFloats.src += ['ak8PFJetsPuppiECF:ecf1','ak8PFJetsPuppiECF:ecf2','ak8PFJetsPuppiECF:ecf3']

# For Softdrop Beta==0 jets
from RecoJets.JetProducers.ECF_cfi import ECF 
rangeECF = range(1,3+1)
process.ak8PuppiJetsSoftDropBeta0ECF = ECF.clone(
								src = cms.InputTag( "ak8PuppiJetsSoftDropBeta0" ),
								Njets = cms.vuint32( rangeECF ),
								beta = cms.double( 1.0 )
								)
process.patJetsAK8PuppiMMDT.userData.userFloats.src += ['ak8PuppiJetsSoftDropBeta0ECF:ecf1','ak8PuppiJetsSoftDropBeta0ECF:ecf2','ak8PuppiJetsSoftDropBeta0ECF:ecf3']

# For Softdrop Beta==2 jets
from RecoJets.JetProducers.ECF_cfi import ECF 
rangeECF = range(1,3+1)
process.ak8PuppiJetsSoftDropBeta2ECF = ECF.clone(
								src = cms.InputTag( "ak8PuppiJetsSoftDropBeta2" ),
								Njets = cms.vuint32( rangeECF ),
								beta = cms.double( 1.0 )
								)
process.patJetsAK8PuppiSD.userData.userFloats.src += ['ak8PuppiJetsSoftDropBeta2ECF:ecf1','ak8PuppiJetsSoftDropBeta2ECF:ecf2','ak8PuppiJetsSoftDropBeta2ECF:ecf3']



#Assign everything to PUPPI jet
process.SoftDropBeta0ValueMap = cms.EDProducer("RecoJetToPatJetDeltaRValueMapProducer",
                                        src = cms.InputTag("ak8PFJetsPuppi"),
                                        matched = cms.InputTag("patJetsAK8PuppiMMDT"),                                         
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
                                            'Tau1',
                                            'Tau2',
											'ECF1',
											'ECF2',
											'ECF3',
                                            'pt','eta','phi','mass'
                                        ])
                    )
process.SoftDropBeta2ValueMap = cms.EDProducer("RecoJetToPatJetDeltaRValueMapProducer",
                                        src = cms.InputTag("ak8PFJetsPuppi"),
                                        matched = cms.InputTag("patJetsAK8PuppiSD"),                                         
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
                                            'Tau1',
                                            'Tau2',
											'ECF1',
											'ECF2',
											'ECF3',
                                            'pt','eta','phi','mass'
                                        ])
                    )
process.patJetsAK8Puppi.userData.userFloats.src += [cms.InputTag('SoftDropBeta0ValueMap','Tau1'),
                                                   cms.InputTag('SoftDropBeta0ValueMap','Tau2'),
                                                   cms.InputTag('SoftDropBeta0ValueMap','ECF1'),
												   cms.InputTag('SoftDropBeta0ValueMap','ECF2'),
												   cms.InputTag('SoftDropBeta0ValueMap','ECF3'),
                                                   cms.InputTag('SoftDropBeta0ValueMap','pt'),
                                                   cms.InputTag('SoftDropBeta0ValueMap','eta'),
                                                   cms.InputTag('SoftDropBeta0ValueMap','phi'),
                                                   cms.InputTag('SoftDropBeta0ValueMap','mass'),
                                                  ]
process.patJetsAK8Puppi.userData.userFloats.src += [cms.InputTag('SoftDropBeta2ValueMap','Tau1'),
                                                   cms.InputTag('SoftDropBeta2ValueMap','Tau2'),
                                                   cms.InputTag('SoftDropBeta2ValueMap','ECF1'),
												   cms.InputTag('SoftDropBeta2ValueMap','ECF2'),
												   cms.InputTag('SoftDropBeta2ValueMap','ECF3'),
                                                   cms.InputTag('SoftDropBeta2ValueMap','pt'),
                                                   cms.InputTag('SoftDropBeta2ValueMap','eta'),
                                                   cms.InputTag('SoftDropBeta2ValueMap','phi'),
                                                   cms.InputTag('SoftDropBeta2ValueMap','mass'),
                                                  ]
                                                                                



                                           
					

#Some control histograms
# process.TFileService = cms.Service("TFileService",
#     fileName = cms.string("histo.root")
# )
#
# process.plotPUPPI = cms.EDAnalyzer('CandViewHistoAnalyzer',
#     src = cms.InputTag("patJetsAK8PuppiSD"),
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
#     src = cms.InputTag("patJetsAK8PuppiMMDT"),
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
#     src = cms.InputTag("patJetsAK8PuppiSD"),
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
		fileName=cms.untracked.string("substructure_bkg.root"),
        outputCommands = cms.untracked.vstring('drop *',
   					   'keep patPackedGenParticles_*_*_*',
   					   'keep recoGenParticles_*_*_*',
					   'keep patJets_*_*_*',
					   'keep recoGenJets_*_*_*',
					   'keep *_NjettinessAK8Puppi_*_*',
					   'keep *_SoftDropBeta0ValueMap_*_*',
					   'keep *_SoftDropBeta2ValueMap_*_*',
   					   ),
)


# process.outpath = cms.EndPath(process.out*process.plotPUPPI*process.plotB0*process.plotB2)
process.outpath = cms.EndPath(process.out)

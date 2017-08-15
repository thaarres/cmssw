# import ROOT in batch mode
import sys, os
import ROOT, array
ROOT.gROOT.SetBatch(True)

	
f = ROOT.TFile("dichroic.root", "recreate")
f.cd()
t = ROOT.TTree("tree", "tree")
print "Writing to outfile " , f.GetName()



def isAncestor(a,p) :
	if a == p :
		return True
	for i in xrange(0,p.numberOfMothers()) :
		if isAncestor(a,p.mother(i)) : return True
	return False
				
				
				
jsd 		= array.array( 'f', [ 0 ] )
je 			= array.array( 'f', [ 0 ] )
jpt 		= array.array( 'f', [ 0 ] )
jeta 		= array.array( 'f', [ 0 ] )
jphi 		= array.array( 'f', [ 0 ] )
jtau1 	= array.array( 'f', [ 0 ] )
jtau2 	= array.array( 'f', [ 0 ] )




t.Branch('jsd' 			, jsd, 'msoftdrop/f')
t.Branch('je' 			, je, 'je/f')
t.Branch('jpt' 			, jpt, 'jpt/f')
t.Branch('jeta' 		, jeta, 'jeta/f')
t.Branch('jphi' 		, jphi, 'jphi/f')
t.Branch('jtau1' 		, jtau1, 'jtau1/f')
t.Branch('jtau2' 		, jtau2, 'jtau2/f')


# load FWlite python libraries
from DataFormats.FWLite import Handle, Events

#Handles
AK8PuppiJetsSoftDropB0  , AK8PuppiJetsSoftDropB0Label      	= Handle("std::vector<pat::Jet>"), "selectedAK8PuppiJetsSoftDropB0"
AK8PuppiJetsSoftDropB2  , AK8PuppiJetsSoftDropB2Label      	= Handle("std::vector<pat::Jet>"), "selectedAK8PuppiJetsSoftDropB2"
AK8PFJetsPuppi   		, AK8PFJetsPuppiLabel 				= Handle("std::vector<pat::Jet>"), "selectedAK8PFJetsPuppi"
slimmedJetsAK8			, slimmedJetsAK8Label   			= Handle("std::vector<pat::Jet>"), "slimmedJetsAK8"


histsTau1 = []
histsTau2 = []
histsSD   = []


histotau1_b0=ROOT.TH1F("histotau1_b0",""		, 100, 0., 1) 
histotau1_b2=ROOT.TH1F("histotau1_b2",""		, 100, 0., 1) 
histotau1_std=ROOT.TH1F("histotau1_std",""	, 100, 0., 1) 
histotau2_b0=ROOT.TH1F("histotau2_b0",""		, 100, 0., 1) 
histotau2_b2=ROOT.TH1F("histotau2_b2",""		, 100, 0., 1) 
histotau2_std=ROOT.TH1F("histotau2_std",""	, 100, 0., 1) 
histo_sd_b0=ROOT.TH1F("histo_sd_b0",""		, 100, 0., 200) 
histo_sd_b2=ROOT.TH1F("histo_sd_b2",""		, 100, 0., 200) 
histo_sd_std=ROOT.TH1F("histo_sd_std",""		, 100, 0., 200) 

events = Events('substructure.root')
for iev,event in enumerate(events):
	print "-------------NEW EVENT:----------------"
	# if iev >= 10: break
	# if iev % 1000 == 0:print "Event", iev
										 
	event.getByLabel(AK8PuppiJetsSoftDropB0Label, AK8PuppiJetsSoftDropB0)
	for i,j in enumerate(AK8PuppiJetsSoftDropB0.product()):
		if j.pt() < 200: continue

		
		beta0_tau1         = j.userFloat("NjettinessAK8PuppiSoftdropBeta0:tau1")
		beta0_tau2         = j.userFloat("NjettinessAK8PuppiSoftdropBeta0:tau2")
		beta0_sd		   = j.mass()
		
		histotau1_b0.Fill(beta0_tau1)
		histotau2_b0.Fill(beta0_tau2)
		histo_sd_b0.Fill(beta0_sd)
		
		
		print "%i) Softdrop Beta0 AK8 jet   : tau1 = %5.3f , tau2= % 5.3f  , softdrop = %5.3f" % (i, beta0_tau1, beta0_tau2,beta0_sd)
		
	event.getByLabel(AK8PuppiJetsSoftDropB2Label, AK8PuppiJetsSoftDropB2)
	for ii,jj in enumerate(AK8PuppiJetsSoftDropB2.product()):
		if jj.pt() < 200: continue

		beta2_tau1         = jj.userFloat("NjettinessAK8PuppiSoftdropBeta2:tau1")
		beta2_tau2         = jj.userFloat("NjettinessAK8PuppiSoftdropBeta2:tau2")
		beta2_sd 		   = jj.mass()
		
		histotau1_b2.Fill(beta0_tau1)
		histotau2_b2.Fill(beta0_tau2)
		histo_sd_b2.Fill(beta2_sd)



		print "%i) Softdrop Beta2 AK8 jet   : tau1 = %5.3f , tau2= % 5.3f  , softdrop = %5.3f" % (ii, beta2_tau1, beta2_tau2,beta2_sd)


	
	event.getByLabel(slimmedJetsAK8Label, slimmedJetsAK8)
	for iii,jjj in enumerate(slimmedJetsAK8.product()):
		if jjj.pt() < 200: continue
		
		slimmed_tau1         = jjj.userFloat("NjettinessAK8:tau1")
		slimmed_tau2         = jjj.userFloat("NjettinessAK8:tau2")
		
		puppi_softdrop, puppi_softdrop_subjet = ROOT.TLorentzVector() ,ROOT.TLorentzVector() 
		wSubjets = jjj.subjets('SoftDropPuppi')
		for iw,wsub in enumerate( wSubjets ) :
			puppi_softdrop_subjet.SetPtEtaPhiM(wsub.correctedP4(0).pt(),wsub.correctedP4(0).eta(),wsub.correctedP4(0).phi(),wsub.correctedP4(0).mass())
			puppi_softdrop+=puppi_softdrop_subjet
		
		slimmed_jsd 		 = puppi_softdrop.M()

		
		histotau1_std.Fill(slimmed_tau1)
		histotau2_std.Fill(slimmed_tau2)
		histo_sd_std.Fill(slimmed_jsd)
		
		
		print "%i) Slimmed AK8 jet          : tau1 = %5.3f , tau2= % 5.3f  , softdrop = %5.3f" % (iii, slimmed_tau1, slimmed_tau2,slimmed_jsd)
	
	event.getByLabel(AK8PFJetsPuppiLabel, AK8PFJetsPuppi)
	for iiii,jjjj in enumerate(AK8PFJetsPuppi.product()):
		if jjjj.pt() < 200: continue

		check_tau1         = jjjj.userFloat("NjettinessAK8Puppi:tau1")
		check_tau2         = jjjj.userFloat("NjettinessAK8Puppi:tau2")
	

		print "%i) CHECK AK8 jet            : tau1 = %5.3f , tau2= % 5.3f  " % (ii, check_tau1, check_tau2)
			
	print "-------------DONE----------------"
	#
	# t.Fill()
	#
		

f.Write()
f.Close()   						 
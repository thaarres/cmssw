#include <functional>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <cmath>
#include <map>

#include <boost/shared_ptr.hpp>

#include <TRandom.h>
#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TLeaf.h>
#include <TList.h>
#include <TKey.h>

#include <TH1F.h>
#include <TH2D.h>
#include <TFile.h>

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "SimDataFormats/JetMatching/interface/JetFlavourMatching.h"

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/BTauReco/interface/JetTagInfo.h"
#include "DataFormats/BTauReco/interface/TaggingVariable.h"

#include "CondFormats/PhysicsToolsObjects/interface/MVAComputer.h"
#include "CondFormats/DataRecord/interface/BTauGenericMVAJetTagComputerRcd.h"

#include "PhysicsTools/MVATrainer/interface/MVATrainer.h"

#include "RecoBTau/JetTagComputer/interface/JetTagComputerRecord.h"
#include "RecoBTau/JetTagComputer/interface/GenericMVAComputer.h"
#include "RecoBTau/JetTagComputer/interface/GenericMVAComputerCache.h"
#include "RecoBTau/JetTagComputer/interface/TagInfoMVACategorySelector.h"

#include "EventProgress.h"

using namespace reco;
using namespace PhysicsTools;

class Var {
    public:
	Var(char type, TTree *tree, const char *name) :
		type(type), var(getTaggingVariableName(name))
	{
		switch(type) {
		    case 'D':
			tree->SetBranchAddress(name, &D);
			break;
		    case 'I':
			tree->SetBranchAddress(name, &I);
			break;
		    case 'd':
			indirectd = &d;
			tree->SetBranchAddress(name, &indirectd);
			break;
		    case 'i':
			indirecti = &i;
			tree->SetBranchAddress(name, &indirecti);
			break;
		}
	}

	void fill(TaggingVariableList &list)
	{
		switch(type) {
		    case 'D':
			list.insert(var, D, true);
			break;
		    case 'I':
			list.insert(var, I, true);
			break;
		    case 'd':
			for(std::vector<double>::const_iterator p = d.begin();
			    p != d.end(); p++)
				list.insert(var, *p, true);
			break;
		    case 'i':
			for(std::vector<int>::const_iterator p = i.begin();
			    p != i.end(); p++)
				list.insert(var, *p, true);
			break;
		}
	}	

	static bool order(const boost::shared_ptr<Var> &a,
	                  const boost::shared_ptr<Var> &b)
	{ return a->var < b->var; }

    private:
	char			type;
	TaggingVariableName	var;
	double			D;
	int			I;
	std::vector<double>	d;
	std::vector<int>	i;
	std::vector<double>*	indirectd;
	std::vector<int>*	indirecti;
};

class JetTagMVATreeTrainer : public edm::EDAnalyzer {
    public:
	explicit JetTagMVATreeTrainer(const edm::ParameterSet &params);
	~JetTagMVATreeTrainer();

	virtual void beginRun(const edm::Run &run,
	                      const edm::EventSetup &es);

	virtual void analyze(const edm::Event &event,
	                     const edm::EventSetup &es);

    protected:
	bool isSignalFlavour(int flavour) const;
	bool isIgnoreFlavour(int flavour) const;

	std::auto_ptr<TagInfoMVACategorySelector>	categorySelector;
	std::auto_ptr<GenericMVAComputerCache>		computerCache;

	double						minPt;
	double						minEta;
	double						maxEta;

    private:
	std::vector<int>				signalFlavours;
	std::vector<int>				ignoreFlavours;
	int						maxEvents;
	TRandom						rand;

	std::vector<std::string>			fileNames;
	
	//TESTING
//	TH1F* h_JetPt;
//	TH1F* h_JetEta;
	
//	TFile* outfile;	
	
	TH2D* histo_B_lin; 
	TH2D* histo_C_lin;
	TH2D* histo_BB_lin;	
//	TH2D* histo2D_B_reweighted_lin;
//	TH2D* histo2D_C_reweighted_lin;
//	TH2D* histo2D_BB_reweighted_lin;
	
};

JetTagMVATreeTrainer::JetTagMVATreeTrainer(const edm::ParameterSet &params) :
	minPt(params.getParameter<double>("minimumTransverseMomentum")),
	minEta(params.getParameter<double>("minimumPseudoRapidity")),
	maxEta(params.getParameter<double>("maximumPseudoRapidity")),
	signalFlavours(params.getParameter<std::vector<int> >("signalFlavours")),
	ignoreFlavours(params.getParameter<std::vector<int> >("ignoreFlavours")),
	maxEvents(params.getUntrackedParameter<int>("maxEvents", -1)),
	fileNames(params.getParameter<std::vector<std::string> >("fileNames"))
{

	std::cout << "JetTagMVATreeTrainer::JetTagMVATreeTrainer" << std::endl;
	
	std::sort(signalFlavours.begin(), signalFlavours.end());
	std::sort(ignoreFlavours.begin(), ignoreFlavours.end());

	std::vector<std::string> calibrationLabels;
	if (params.getParameter<bool>("useCategories")) {
		categorySelector = std::auto_ptr<TagInfoMVACategorySelector>(
				new TagInfoMVACategorySelector(params));

		calibrationLabels = categorySelector->getCategoryLabels();
	} else {
		std::string calibrationRecord =
			params.getParameter<std::string>("calibrationRecord");

		calibrationLabels.push_back(calibrationRecord);
	}

	computerCache = std::auto_ptr<GenericMVAComputerCache>(
			new GenericMVAComputerCache(calibrationLabels));
	
  //TESTING
//	h_JetPt = new TH1F("h_JetPt","h_JetPt",200,0,1200);
//	h_JetEta = new TH1F("h_JetEta","h_JetEta",200,-2.6,2.6);
	
//	outfile = new TFile("JetTagMVATreeTrainer_outfile.root","RECREATE");
	
	//for non-fit reweighting
	TFile* infile_B = 0;
	TFile* infile_C = 0;
	TFile* infile_BB = 0;
/* 	
	if(params.getParameter<std::string>("calibrationRecord") == "CombinedSVRecoVertex")
	{
	  infile_B = TFile::Open("CombinedSVRecoVertex_B_histo.root");
	  infile_C = TFile::Open("CombinedSVRecoVertex_C_histo.root");
	  infile_BB = TFile::Open("CombinedSVRecoVertex_BB_histo.root");		
	}
	else if(params.getParameter<std::string>("calibrationRecord") == "CombinedSVPseudoVertex")
	{
	  infile_B = TFile::Open("CombinedSVPseudoVertex_B_histo.root");
	  infile_C = TFile::Open("CombinedSVPseudoVertex_C_histo.root");
	  infile_BB = TFile::Open("CombinedSVPseudoVertex_BB_histo.root");
	}
	else if(params.getParameter<std::string>("calibrationRecord") == "CombinedSVNoVertex")
	{
	  infile_B = TFile::Open("CombinedSVNoVertex_B_histo.root");
	  infile_C = TFile::Open("CombinedSVNoVertex_C_histo.root");
	  infile_BB = TFile::Open("CombinedSVNoVertex_BB_histo.root");
	}
	else if(params.getParameter<std::string>("calibrationRecord") == "combinedMVA")
	{
	  //std::cout << "TEST" << std::endl;
		infile_B = TFile::Open("combinedMVA_B_histo.root");
	  infile_C = TFile::Open("combinedMVA_C_histo.root");
	  infile_BB = TFile::Open("combinedMVA_BB_histo.root");
	}
 */

	std::string calibRecords[17] = {"CombinedSVRecoVertexNoSoftLepton","CombinedSVRecoVertexSoftElectron","CombinedSVRecoVertexSoftMuon","CombinedSVPseudoVertexNoSoftLepton","CombinedSVPseudoVertexSoftElectron","CombinedSVPseudoVertexSoftMuon","CombinedSVNoVertexNoSoftLepton","CombinedSVNoVertexSoftElectron","CombinedSVNoVertexSoftMuon", "CombinedSVNoVertex","CombinedSVPseudoVertex","CombinedSVRecoVertex", "CombinedSVV2NoVertex","CombinedSVV2PseudoVertex","CombinedSVV2RecoVertex","CombinedSVV2RecoRecoVertex", "combinedMVA"};
	bool calibRecordFound = false;
	for(int i=0; i<17; i++){ 
//		std::cout << "calibrationRecord " << i << ": " << calibRecords[i] << " and we are looking for " << params.getParameter<std::string>("calibrationRecord")  << std::endl;
		if(params.getParameter<std::string>("calibrationRecord") == calibRecords[i])
		{
		  calibRecordFound = true;
			TString tmp = (TString) calibRecords[i] ;
			infile_B = TFile::Open(tmp+"_B_histo.root");
		  infile_C = TFile::Open(tmp+"_C_histo.root");
		  infile_BB = TFile::Open(tmp+"_BB_histo.root");		
		} 
 		else if(i==16 && calibRecordFound == false)
		{
	   	std::cout<<"JetTagMVATreeTrainer: calibrationRecord " << params.getParameter<std::string>("calibrationRecord") << " NOT FOUND!!!"<<std::endl;
		}
	}
	//flatten in linear scale of pt
	histo_B_lin = (TH2D*) infile_B->Get("jets_lin");
	histo_C_lin = (TH2D*) infile_C->Get("jets_lin");
	histo_BB_lin = (TH2D*) infile_BB->Get("jets_lin");	
//	histo2D_B_reweighted_lin = new TH2D("h_2D_B_reweighted_lin","h_2D_B_reweighted_lin",50, -2.5, 2.5, 40, 15., 1000.);
//	histo2D_C_reweighted_lin = new TH2D("h_2D_C_reweighted_lin","h_2D_C_reweighted_lin",50, -2.5, 2.5, 40, 15., 1000.);
//	histo2D_BB_reweighted_lin = new TH2D("h_2D_BB_reweighted_lin","h_2D_BB_reweighted_lin",50, -2.5, 2.5, 40, 15., 1000.);
	
}

JetTagMVATreeTrainer::~JetTagMVATreeTrainer()
 {
/*  outfile->cd();
	std::cout<<"Writing histograms to files"<<std::endl;
	h_JetPt->Write();
	h_JetEta->Write();
	
	histo2D_B_reweighted_lin->Write();
	histo2D_C_reweighted_lin->Write();
	histo2D_BB_reweighted_lin->Write();
	
	std::cout<<"Done."<<std::endl;
	outfile->Close(); */
}

bool JetTagMVATreeTrainer::isSignalFlavour(int flavour) const
{
	std::vector<int>::const_iterator pos =
		std::lower_bound(signalFlavours.begin(), signalFlavours.end(),
		                 flavour);

	return pos != signalFlavours.end() && *pos == flavour;
}

bool JetTagMVATreeTrainer::isIgnoreFlavour(int flavour) const
{
	std::vector<int>::const_iterator pos =
		std::lower_bound(ignoreFlavours.begin(), ignoreFlavours.end(),
		                 flavour);

	return pos != ignoreFlavours.end() && *pos == flavour;
}

void JetTagMVATreeTrainer::beginRun(const edm::Run& run,
                                    const edm::EventSetup& es)
{
	rand.SetSeed(65539);
}

void JetTagMVATreeTrainer::analyze(const edm::Event& event,
                                   const edm::EventSetup& es)
{
	// retrieve MVAComputer calibration container
	edm::ESHandle<Calibration::MVAComputerContainer> calibHandle;
	es.get<BTauGenericMVAJetTagComputerRcd>().get("trainer", calibHandle);
	const Calibration::MVAComputerContainer *calib = calibHandle.product();

	// check container for changes
	computerCache->update(calib);
	if (computerCache->isEmpty())
		return;

	// cached array containing MVAComputer value list
	std::vector<Variable::Value> values;
	values.push_back(Variable::Value(MVATrainer::kTargetId, 0));
	values.push_back(Variable::Value(MVATrainer::kWeightId, 0));

	int nEvents = 0;
	for(std::vector<std::string>::const_iterator fName = fileNames.begin();
	    fName != fileNames.end(); fName++) {
		if (maxEvents >= 0 && nEvents >= maxEvents)
			break;

		std::auto_ptr<TFile> file(TFile::Open(fName->c_str()));
		if (!file.get())
			continue;
		std::cout << "Opened " << *fName << std::endl;

		TIter next(file->GetListOfKeys());
		TObject *obj;
		while((obj = next())) {
			if (maxEvents >= 0 && nEvents >= maxEvents)
				break;

			TTree *tree = dynamic_cast<TTree*>(file->Get(((TKey*)obj)->GetName()));
			if (!tree)
				continue;
			std::cout << "Tree " << tree->GetName() << std::endl;

			int flavour;
			tree->SetBranchAddress("flavour", &flavour);

			std::vector< boost::shared_ptr<Var> > vars;

			TIter branchIter(tree->GetListOfBranches());
			while((obj = branchIter())) {
				TBranch *branch = dynamic_cast<TBranch*>(obj);
				if (!branch)
					continue;

				TString name = branch->GetName();
				TLeaf *leaf = dynamic_cast<TLeaf*>(
							branch->GetLeaf(name));
				if (!leaf)
					continue;

				TString typeName = leaf->GetTypeName();
				char typeId;
				if (typeName == "Double_t")
					typeId = 'D';
				else if (typeName == "Int_t")
					typeId = 'I';
				else if (typeName == "vector<double>")
					typeId = 'd';
				else if (typeName == "vector<int>")
					typeId = 'i';
				else
					continue;

				if (getTaggingVariableName((const char *)name) ==
						btau::lastTaggingVariable)
					continue;
				vars.push_back(boost::shared_ptr<Var>(
						new Var(typeId, tree, name)));
			}
			std::sort(vars.begin(), vars.end(), &Var::order);

			Long64_t entries = tree->GetEntries();
			std::cout << "Entries " << entries << std::endl;
			EventProgress progress(entries);
			for(Long64_t entry = 0; entry < entries; entry++) {
				if (maxEvents >= 0 && nEvents >= maxEvents)
					break;

				progress.update(entry);
				tree->GetEntry(entry);

				TaggingVariableList variables;
				for(std::vector< boost::shared_ptr<Var> >::const_iterator iter = vars.begin();
				    iter != vars.end(); iter++)
					(*iter)->fill(variables);
				variables.finalize();

				double jetPt = variables[btau::jetPt];
				double jetEta = variables[btau::jetEta];

				// simple jet filter
				if (jetPt < minPt ||
				    std::abs(jetEta) < minEta ||
				    std::abs(jetEta) > maxEta)
						{
						std::cout<<" jet filter not passed by jet with Pt="<<jetPt<<" and  Eta="<<jetEta<<std::endl;
					continue;					
					}

				// do not train with unknown jet flavours
				if (isIgnoreFlavour(flavour))
				{
				  std::cout<<"  isIgnoreFlavour("<<flavour<<") = "<<true<<std::endl;
					continue;					
				}

				// is it a b-jet?
				bool target = isSignalFlavour(flavour);

				// retrieve index of computer in case categories are used
				int index = 0;
				if (categorySelector.get()) {
					index = categorySelector->findCategory(variables);
					if (index < 0)
					{
					  std::cout<<"  index = "<<index<<" < 0"<<std::endl;
						continue;
					}
				}

				GenericMVAComputer const* mvaComputer =
					computerCache->getComputer(index);
				if (!mvaComputer)
				{
				  std::cout<<"  mvaComputer declaration problem "<<std::endl;
					continue;					
				}
				
//				h_JetPt->Fill(jetPt);
//				h_JetEta->Fill(jetEta);
	
	      //non-fit reweighting: jet weight is inverse of bin content of the bin in which the jet resides in the 2D pt,eta histogram
	      double weight = 1;
				float bincontent_B_lin = 0;
				float bincontent_C_lin = 0;
				float bincontent_BB_lin = 0;
				bincontent_B_lin = histo_B_lin->GetBinContent( histo_B_lin->FindBin(jetEta,jetPt) );
				bincontent_C_lin = histo_C_lin->GetBinContent( histo_C_lin->FindBin(jetEta,jetPt) );
				bincontent_BB_lin = histo_BB_lin->GetBinContent( histo_BB_lin->FindBin(jetEta,jetPt) );
				
				if(flavour == 5){
					 weight = 1./bincontent_B_lin;
//					 histo2D_B_reweighted_lin->Fill(jetEta,jetPt,weight);
					//std::cout << "bincontent B: " << bincontent_B_lin << " so that weight is: " << weight << std::endl;
				}
				else if(flavour == 4)
				{
					 weight = 1./bincontent_C_lin;
//					 histo2D_C_reweighted_lin->Fill(jetEta,jetPt,weight);
					//std::cout << "bincontent C: " << bincontent_C_lin << " so that weight is: " << weight << std::endl;
					}
				else
				  if(flavour == 9)
				{				   
					 weight = 1./bincontent_BB_lin;
//					 histo2D_BB_reweighted_lin->Fill(jetEta,jetPt,weight);
					//std::cout << "bincontent BB: " << bincontent_BB_lin << " so that weight is: " << weight << std::endl;
				}
				
				// if weights are too small this might be suboptimal for the training
				//weight = weight * 100;

				// composite full array of MVAComputer values
				values.resize(2 + variables.size());

				std::vector<Variable::Value>::iterator insert = values.begin();
		                (insert++)->setValue(target);
		                (insert++)->setValue(weight);

				std::copy(mvaComputer->iterator(variables.begin()),  mvaComputer->iterator(variables.end()), insert);

				static_cast<MVAComputer const*>(mvaComputer)->eval(values);

				nEvents++;
			}
		}
	}
}

// the main module
DEFINE_FWK_MODULE(JetTagMVATreeTrainer);

//I add 2 extra high pt bins: 400 < PT < 600; PT>600 GeV
//For those I only use 2 eta bins

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "TString.h"
#include "TTree.h"
#include "TFile.h"
#include "TROOT.h"
#include "TObject.h"

using namespace std;

// define name Tree in calcEntries()

void calcEntries(string  flavour, string category, vector<float> & entries, string dir, string fix);

int main(int argc, char **argv){

	string dir = "/afs/cern.ch/user/t/thaarres/CSVTraining/CMSSW_5_3_14/src/RecoBTau/JetTagMVALearning/test/rootfiles/RadionToHH_4b_M-1000_TuneZ2star_8TeV-Madgraph_pythia6_Summer12_DR53X-PU_S10_START53_V19-v1_AODSIM/RecoVsRecoReco";
	string fix = "CombinedSVV2";

	if(argc == 2 || argc == 3) dir = argv[1];
	if(argc == 3) fix = argv[2];
		
	cout << "calculate bias from rootfiles in dir " << dir << endl;

//	string flavour[3] = {"B", "C", "DUSG"};
	string flavour[2] = {"BB", "B"};
//	string cat[3] = {"NoVertex", "PseudoVertex", "RecoVertex"};
	string cat[4] = {"NoVertex", "PseudoVertex", "RecoVertex", "RecoRecoVertex"};

//	vector<float> entries[9];
	vector<float> entries[8];

	int nIter =0;

// 	for(int j=0; j<3; j++){//loop on categories
// 		for(int i =0; i<3; i++){//loop on flavours
	for(int j=0; j<4; j++){//loop on categories
		for(int i =0; i<2; i++){//loop on flavours
			calcEntries(flavour[i], cat[j], entries[nIter], dir, fix);
			//for(int k =0 ; k<entries[nIter].size(); k++) cout<<flavour[i]<<"   "<<cat[j]<<"  "<<entries[nIter][k]<<endl; 
			nIter++;
		}
	}

	//int count = 0;
	//for(int j=0; j<3; j++){//loop on categories
	//	for(int i =0; i<3; i++){//loop on flavours
	//		cout << "for category " << cat[j] << " and jet flavour " << flavour[i] << " there are " << entries[count][0] << " entries"  << endl;
	//		count++;
	//	}
	//}
	//cout << "for jet flavour B there are " << entries[0][0]+entries[3][0]+entries[6][0] << " entries"  << endl;
	//cout << "for jet flavour C there are " << entries[1][0]+entries[4][0]+entries[7][0] << " entries"  << endl;
	//cout << "for jet flavour DUSG there are " << entries[2][0]+entries[5][0]+entries[8][0] << " entries"  << endl;
	
/*For illustration (transverse of entries[9]). 9 entries per eta-pt bin! 19 pt-eta bins (9 columns,19 rows. Content of each column stored in *.txt (19 rows))
      entries^T[9] = [#B    in Reco]	[0]		     
  		     [#C    in Reco]	[1]
		     [#DUSG in Reco]	[2]	 
		     [#B    in Pseudo]	[3]	  
		     [#C    in Pseudo]	[4]
		     [#DUSG in Pseudo]	[5]	     
		     [#B    in No]	[6]	     
		     [#C    in No]	[7]
		     [#DUSG in No]	[8] */	     
		     
  ofstream myfile;
	string filename = "";
// 	for(int j=0; j<3; j++){//loop on categories	
// 		for(int k=1; k<3; k++){//loop on C and light
// 			cout<<"***************   "<<cat[j]<<"_B_"<<flavour[k]<<"   ***************"<<endl;
// 			filename = cat[j]+"_B_"+flavour[k]+".txt";
	for(int j=0; j<4; j++){	
		for(int k=1; k<2; k++){
			cout<<"***************   "<<cat[j]<<"_BB_"<<flavour[k]<<"   ***************"<<endl;
			filename = cat[j]+"_BB_"+flavour[k]+".txt";
  		myfile.open (filename.c_str());
 			for(int l = 0; l<19; l++ ){	// loop on pt/eta bins defined in xml
 			//for(int l = 0; l<1; l++ ){	// loop on pt/eta bins defined in xml
//				int index = j*3; 	// to get #B 	  in Reco[0],Pseudo[3] and No[6]
//				int indexb = k+j*3;	// to get #C/DUSG in Reco[0+k],Pseudo[3+k] and No[6+k]
				int index = j*2; 	
				int indexb = k+j*2;
////				Setting bias manually in case of nan/inf
// 				float SigFrac = (float)(entries[index][l]/(entries[0][l]+entries[3][l]+entries[6][l]+entries[9][l]));
// 				float BkgFrac = (float)(entries[indexb][l]/(entries[k][l]+entries[k+3][l]+entries[k+6][l]+entries[k+9][l]));
// 				if ( entries[indexb][l]== 0 || (entries[k][l]+entries[k+3][l]+entries[k+6][l]+entries[k+9][l]) == 0 ) BkgFrac = 0.01;
// 				float bias = SigFrac/BkgFrac;
// 				cout<<"In pT-eta bin"<<l<<" : SigFrac = "<<SigFrac<<" and BkgFrac = "<<BkgFrac<<endl;
				
//				//Bias computed as 
//				      bias = ( #Sig_inVtxCatX          /             #Sig_AllVtxCat                 )/( #Bkg_inVtxCatX   /            #Bkg_AllVtxCat )
//				float bias = (float)((entries[index][l]/(entries[0][l]+entries[3][l]+entries[6][l]+entries[9][l]))/((entries[indexb][l]/(entries[k][l]+entries[k+3][l]+entries[k+6][l]+entries[k+9][l]))));
				float bias = (float)((entries[index][l]/(entries[0][l]+entries[2][l]+entries[4][l]+entries[6][l]))/((entries[indexb][l]/(entries[k][l]+entries[k+2][l]+entries[k+4][l]+entries[k+6][l]))));
				
				myfile << "<bias>"<<bias<<"</bias>\n";
				cout<<"<bias>"<<bias<<"</bias>"<<endl; 
			}
			cout<<"Storing bias in "<<filename<<"."<<endl;
			myfile.close();
		}
	}
	
	return 0;
}


void calcEntries(string flavour, string  category,  vector<float> & entries, string dir, string fix){	
	TFile * f = TFile::Open((dir+"/"+fix+category+"_"+flavour+".root").c_str());
  
	cout << "opening file: " << (dir+"/"+fix+category+"_"+flavour+".root").c_str() << endl;
	   
	f->cd();
	TTree * t =(TTree*)f->Get((fix+category).c_str());

	//definition of pt and eta bins should be the same as in the Train*xml files!!!
//	entries.push_back(t->GetEntries());
	entries.push_back(t->GetEntries("jetPt>15&&jetPt<40&&TMath::Abs(jetEta)<1.2"));
	entries.push_back(t->GetEntries("jetPt>15&&jetPt<40&&TMath::Abs(jetEta)<2.1&&(!(TMath::Abs(jetEta)<1.2))"));
	entries.push_back(t->GetEntries("jetPt>15&&jetPt<40&&(!(TMath::Abs(jetEta)<2.1))"));
	entries.push_back(t->GetEntries("jetPt>40&&jetPt<60&&TMath::Abs(jetEta)<1.2"));
	entries.push_back(t->GetEntries("jetPt>40&&jetPt<60&&TMath::Abs(jetEta)<2.1&&(!(TMath::Abs(jetEta)<1.2))"));
	entries.push_back(t->GetEntries("jetPt>40&&jetPt<60&&(!(TMath::Abs(jetEta)<2.1))"));
	entries.push_back(t->GetEntries("jetPt>60&&jetPt<90&&TMath::Abs(jetEta)<1.2"));
	entries.push_back(t->GetEntries("jetPt>60&&jetPt<90&&TMath::Abs(jetEta)<2.1&&(!(TMath::Abs(jetEta)<1.2))"));
	entries.push_back(t->GetEntries("jetPt>60&&jetPt<90&&(!(TMath::Abs(jetEta)<2.1))"));
	entries.push_back(t->GetEntries("jetPt>90&&jetPt<150&&TMath::Abs(jetEta)<1.2"));
	entries.push_back(t->GetEntries("jetPt>90&&jetPt<150&&TMath::Abs(jetEta)<2.1&&(!(TMath::Abs(jetEta)<1.2))"));
	entries.push_back(t->GetEntries("jetPt>90&&jetPt<150&&(!(TMath::Abs(jetEta)<2.1))"));
	entries.push_back(t->GetEntries("jetPt>150&&jetPt<400&&TMath::Abs(jetEta)<1.2"));
	entries.push_back(t->GetEntries("jetPt>150&&jetPt<400&&TMath::Abs(jetEta)<2.1&&(!(TMath::Abs(jetEta)<1.2))"));
	entries.push_back(t->GetEntries("jetPt>150&&jetPt<400&&(!(TMath::Abs(jetEta)<2.1))"));
	entries.push_back(t->GetEntries("jetPt>400&&jetPt<600&&TMath::Abs(jetEta)<1.2"));
	entries.push_back(t->GetEntries("jetPt>400&&jetPt<600&&(!(TMath::Abs(jetEta)<1.2))"));
	entries.push_back(t->GetEntries("jetPt>600&&TMath::Abs(jetEta)<1.2"));
	entries.push_back(t->GetEntries("jetPt>600&&(!(TMath::Abs(jetEta)<1.2))"));

	cout << "Number of entries in each pt and eta bin have been stored, calculating bias." << endl;
	    
}




// Usage root -l RankingVariables.C+
// The code uses as input a monitoring rootfile wich contains the output of a ProcMatrix processor with different
//   variables
// The codes prints the list of most discriminating variables (starting from the most one)
//  and then prints the list of the maximum set of variables below a certain level of
//  correlation. It prints as many list as variables are in the set (as the list depends on the
//  seed variable.
// This is done first when the criteria to rank the variables is the surface (the fraction of
//  overlaping distribution, and then when the criteria is Purity X Efficiency
//
// Some information is hardcoded and needs to be adapted:
//  *List of variables 		-> modify "TString VariableNames[NVar] = {"
//  *Number of variables 	-> it should match the number of variables in the previous string
//				   you should modify "#define NVar"
//  *bool AddCorrelation=true;  -> flag for checking the correlation
//  *bool SfromSurf=true;       -> flag for method SfromSurf
//  *bool SfromPTE=true;        -> flag for method PTE
//  *TString procMatrixName 	-> it should match the name of the ProcMatrix processor in the rootfile
// 				   additionaly it is needed to adjust the lines:
//				   S = "input_"+procMatrixName+"...
//				   B = "input_"+procMatrixName+"... in order to read properly the input variables
// 
//  *Double_t MaxCorr=40;	-> maximum level of correlation allowed for the maximum set of variables (in percent)
//  *Input file			-> modify: "TFile* _file1 = TFile::Open("

#include "TH1.h"
#include "TH2F.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "TF1.h"
#include "TKey.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TProfile.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TLine.h"
#include "TArrow.h"
#include "TLatex.h"
#include "TMinuit.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TFormula.h"

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <algorithm> 

//#define NVar 109 //including different track alternatives
#define NVar 49 



void setTDRStyle();


TH1F * hDistMthadbfhadt;
TH1F * hDistMthadbflept;
TH1F * hDistMthadbfother;

//Double_t Corr[93][93];
Double_t Corr[NVar+1][NVar+1];

Int_t NrObs=NVar; //(NVar)
TString procMatrixName = "rotAllDUSG";
//TString procMatrixName = "rotAllC";


//TString VariableNames[92] = {"massHadW","massHadTop","ptHadTop","etaHadTop","phiHadTop","thetaHadTop","ptHadW","etaHadW","phiHadW","thetaHadW","ptHadB","etaHadB","phiHadB","thetaHadB","ptLepB","etaLepB","phiLepB","thetaLepB","ptHhadQ","etaHadQ","phiHadQ","thetaHadQ","ptHadQBar","etaHadQBar","phiHadQBar","thetaHadQBar","ptLepton","etaLepton","phiLepton","thetaLepton","massLepBLepton","ptLepBLepton","etaLepBLepton","phiLepBLepton","thetaLepBLepton","deltaRHadTopHadW","deltaPhiHadTopHadW","deltaThetaHadTopHadW","deltaRHadTopHadB","deltaPhiHadTopHadB","deltaThetaHadTopHadB","deltaRHadWHadB","deltaPhiHadWHadB","deltaThetaHadWHadB","deltaRHadQHadQBar","deltaPhiHadQHadQBar","deltaThetaHadQHadQBar","deltaRLepBLepton","deltaPhiLepBLepton","deltaThetaLepBLepton","deltaRLepBHadTop","deltaPhiLepBHadTop","deltaThetaLepBHadTop","deltaRLepBHadW","deltaPhiLepBHadW","deltaThetaLepBHadW","deltaRLepBHadB","deltaPhiLepBHadB","deltaThetaLepBHadB","deltaRLeptonHadTop","deltaPhiLeptonHadTop","deltaThetaLeptonHadTop","deltaRLeptonHadW","deltaPhiLeptonHadW","deltaThetaLeptonHadW","deltaRLeptonHadB","deltaPhiLeptonHadB","deltaThetaLeptonHadB","deltaRLepBLeptonHadTop","deltaPhiLepBLeptonHadTop","deltaThetaLepBLeptonHadTop","deltaRLepBLeptonHadW","deltaPhiLepBLeptonHadW","deltaThetaLepBLeptonHadW","deltaRLepBLeptonHadB","deltaPhiLepBLeptonHadB","deltaThetaLepBLeptonHadB","relPtHadTop","relPtBJets","sumBTag1HadBLepB","sumBTag2HadBLepB"}; 

/*TString VariableNames[NVar] = {
  "jetPt",
  "jetEta",
  "vertexCategory",
  "trackSip2dSig",
  "trackSip3dSig",
  "trackSip2dVal",
  "trackSip3dVal",
  "trackPtRel",
  "trackPPar",
  "trackEtaRel"
};*/

TString VariableNames[NVar] = {
  "trackSip2dSig_1",
  "trackSip3dSig_1",
  "trackSip2dVal_1",
  "trackSip3dVal_1",
  "trackPtRel_1",
  "trackPPar_1",
  "trackEtaRel_1",
  "trackDeltaR_1",
  "trackPtRatio_1",
  "trackPParRatio_1",
  "trackJetDist_1",
  "trackDecayLenVal_1",
  "jetPt_1",
  "jetEta_1",
  "vertexMass_1",
  "vertexNTracks_1",
  "vertexEnergyRatio_1",
  "trackSip2dSigAboveCharm_1",
  "trackSip3dSigAboveCharm_1",
  "flightDistance2dSig_1",
  "flightDistance3dSig_1",
  "flightDistance2dVal_1",
  "flightDistance3dVal_1",
  "trackSumJetEtRatio_1",
  "jetNSecondaryVertices_1",
  "vertexJetDeltaR_1",
  "trackSumJetDeltaR_1",
  "jetNTracks_1",
  "trackSip2dValAboveCharm_1",
  "trackSip3dValAboveCharm_1",
  "vertexFitProb_1",
  "chargedHadronEnergyFraction_1",
  "neutralHadronEnergyFraction_1",
  "photonEnergyFraction_1",
  "electronEnergyFraction_1",
  "muonEnergyFraction_1",
  "chargedHadronMultiplicity_1",
  "neutralHadronMultiplicity_1",
  "photonMultiplicity_1",
  "electronMultiplicity_1",
  "muonMultiplicity_1",
  "hadronMultiplicity_1",
  "hadronPhotonMultiplicity_1",
  "totalMultiplicity_1",
  "massVertexEnergyFraction_1",
  "vertexBoostOverSqrtJetPt_1",
  "nTracks_1",
  "category_1",
  "count_var1_1"

};


//TString VariableNames[NVar] = {"jetPt","jetEta","vertexCategory","trackSip2dSig","trackSip3dSig","trackSip2dVal","trackSip3dVal","trackPtRel","trackPPar","trackEtaRel"};


bool AddCorrelation=true;
bool SfromSurf=true;
bool SfromPTE=true;
Double_t MaxCorr=40;

//struct to define sorting on the second element of the vector of pairs
struct sort_pred {
  bool operator()(std::pair<Int_t,Double_t> &left, std::pair<Int_t,Double_t> &right) {
    return left.second < right.second;
    //return left.first < right.first;
  }
  };

///int main() {
void RankingVariables() {

  using namespace std;

  setTDRStyle();
  gStyle->SetOptFit(0);
  gStyle->SetOptStat(0);
  
  cout << "settings : " << "correlation allowed for list of variables < " <<  MaxCorr << endl;
  cout << "Read file" << endl;

  TFile* _file1 = TFile::Open("train_monitoring_rotAll.root");
  //TFile* _file1 = TFile::Open("train_monitoring_B_C_rotAll.root");
  
  //_file1 = TFile::Open("train_monitoring_ttbarReconstruction.root");
  //_file1 = TFile::Open("/user/jmmaes/CMSSW/CMSSW_2_2_4_v1/CMSSW_2_2_4/src/TopQuarkAnalysis/TopJetCombination/test/case1/train_monitoring_bug.root");
  //TFile *_file1 = TFile::Open("/user/jmmaes/CMSSW/CMSSW_2_2_4_v1/CMSSW_2_2_4/src/TopQuarkAnalysis/TopJetCombination/test/case1/train_monitoring_v1.root");

  
  
  TString S;
  TString B;
  TString C;

  std::vector < std::pair <Double_t,Int_t> > Surf;
  std::vector < std::pair <Double_t,Int_t> > PTE;
 

  TH1F * SvalueSurf = new TH1F("SvalueSurf","SvalueSurf",10,0.5,1);
  TH1F * SvaluePTE = new TH1F("SvaluePTE","SvaluePTE",50,0.2,.5);
  TH1F * corrHist = new TH1F("corrHist","corrHist",40,-100,100);

  for (Int_t a=0; a <= NVar; a++){
    for (Int_t b=0; b <= NVar; b++){
      Corr[a][b] = 0;
    }
  }
  
  cout << "starting loop" << endl;
  for (Int_t t=0; t < NrObs; t++){
    //std::cout << "t=" << t << std::endl;


    //LOADING HISTO'S
    //S = "input_norm/input_";
    //S+=VariableNames[t];
    
    S = "input_"+procMatrixName+"/procOptDUSG_"+VariableNames[t]+"_sig";
    B = "input_"+procMatrixName+"/procOptDUSG_"+VariableNames[t]+"_bkg";
    //S = "input_"+procMatrixName+"/procOptC_"+VariableNames[t]+"_sig";
    //B = "input_"+procMatrixName+"/procOptC_"+VariableNames[t]+"_bkg";

    //cout << t << ": loading histograms " << S << " and " << B<< endl;
  
    
    TH1F * hDistObs1_S = (TH1F *) _file1->Get(S); 
    TH1F * hDistObs1_B = (TH1F *) _file1->Get(B);
    //std::cout << "  after hDistObs1 VariableNames[" << t << "]=" << VariableNames[t]
    //          << " hDistObs1_S-B.Integral()=" << hDistObs1_S->Integral() << " " << hDistObs1_B->Integral() << std::endl;
    
    
    
    
    //couting number of bins and range of histo's
    //cout << "Sig histo: Nbins: " << hDistObs1_S->GetNbinsX() << " range: " << hDistObs1_S->GetBinCenter(0) << " " <<  hDistObs1_S->GetBinCenter(hDistObs1_S->GetNbinsX()) << endl;
    //cout << "Bkg histo: Nbins: " << hDistObs1_B->GetNbinsX() << " range: " << hDistObs1_B->GetBinCenter(0) << " " <<  hDistObs1_B->GetBinCenter(hDistObs1_B->GetNbinsX()) << endl;
 
    //normalizehistos including underflow and overflow
    Int_t nbinS=hDistObs1_S->GetNbinsX();
    Int_t nbinB=hDistObs1_S->GetNbinsX();
    
    Double_t scaleS=1/hDistObs1_S->Integral(0, nbinS + 1);
    Double_t scaleB=1/hDistObs1_B->Integral(0, nbinB + 1);

    hDistObs1_S->Scale(scaleS);
    hDistObs1_B->Scale(scaleB);

    
    //cout << "Starting Surface calculation" << endl;
    //S-VALUE FROM SURFACE
    Double_t CS=0;
    Double_t bincontS=0;
    Double_t bincontB=0;
    
    //std::cout << "before loop bincontSB" << std::endl;

    for (Int_t b=0; b<=hDistObs1_S->GetNbinsX()+1; b++){
      //std::cout << "  b=" << b << std::endl;
      bincontS = hDistObs1_S->GetBinContent(b);
      bincontB = hDistObs1_B->GetBinContent(b);

      //cout << bincontS << "  " << bincontB << endl;

      if(bincontS<=bincontB) CS += bincontS;
      else CS +=bincontB;
      //cout << CS << endl;
    }

    //std::cout << "after loop bincontSB" << std::endl;
    
    std::pair <Double_t,Int_t> SurfPair;
    //cout << CS << endl;

    SurfPair.first = CS;    
    SurfPair.second = t;   
    Surf.push_back(SurfPair);

    //END:S-VALUE FROM SURFACE
    //cout << "Starting PTE calculation" << endl;
    //S-VALUE FROM PURITY X EFFICIENCY
    Double_t PTEvalue1=0;
    Double_t PTEvalue2=0;
    Double_t PTEvalue=0;
    Double_t effS=0;
    Double_t effB=1;

  

    for (Int_t b=0; b<=hDistObs1_S->GetNbinsX()+1; b++){
      bincontS = hDistObs1_S->GetBinContent(b);
      bincontB = hDistObs1_B->GetBinContent(b);

      effS += bincontS;
      effB -= bincontB;

      if((effS*effB)>PTEvalue1){PTEvalue1=effS*effB;}
    }
    
    effS=0;
    effB=1;
   
    for (Int_t b=hDistObs1_S->GetNbinsX()+1; b>=0; b--){
      bincontS = hDistObs1_S->GetBinContent(b);
      bincontB = hDistObs1_B->GetBinContent(b);
 
      effS += bincontS;
      effB -= bincontB;
      //if(t==28) cout << effS << " x  "<< effB << " = " << PTEvalue2 << endl;


      if((effS*effB)>PTEvalue2){PTEvalue2=effS*effB;}
    }   

    //    std::cout << "PTEvalue1 = " << PTEvalue1<< std::endl;
    //    std::cout << "PTEvalue2 = " << PTEvalue2<< std::endl;
    if(PTEvalue1>PTEvalue2) PTEvalue=PTEvalue1;
    else PTEvalue=PTEvalue2;

    std::pair <Double_t,Int_t> PTEPair;
     
    PTEPair.second = t;    
    PTEPair.first = PTEvalue;     
    
    PTE.push_back(PTEPair);
  
    //END:S-VALUE FROM PURITY X EFFICIENCY
    

  }
  

  
  cout << "starting loop for correlations" << endl;
  //notice first bin in Corr Matrix is 1 and not zero, that's why we define tp and up
  for (Int_t t=0; t <= NVar; t++){
    Int_t tp=t+1; 
  
    for (Int_t u=0; u <= NVar; u++){
      if(t!=u && t<u){  
	Int_t up=u+1;

	C = "ProcMatrix_"+ procMatrixName + "/CorrMatrix";
	TH2F * hDistObs1_corr = (TH2F *) _file1->Get(C);
	
	Int_t corr1 = 0;
	
	if(AddCorrelation) corr1 = (hDistObs1_corr->GetBinContent(up,tp)) *100;
	//cout << "up: " << up << " tp: " << tp << " " << corr1 << " vs. " << (hDistObs1_corr->GetBinContent(up,tp)) *100 << endl;
	Corr[t][u] = corr1;
	Corr[u][t] = corr1;
      }
     
	Corr[t][t] = 100;
    
    }
  }

  
  //PRINTING INFO ON THE SCREEN


  for (Int_t t=0; t < NrObs; t++){
    std::cout << "Observable Nb. " << Surf[t].second << " has S value = " << Surf[t].first << std::endl;
    std::cout << "Observable Nb. " << PTE[t].first << " has S value = " << PTE[t].second << std::endl;
  } 

  std::sort(Surf.begin(), Surf.end()); 
  std::sort(PTE.begin(), PTE.end(), greater<pair<Double_t,Int_t > >() ); 
 

  if(SfromSurf) std::cout << "S-ordered list of variables (for *surface*) " <<std::endl;
 

  for (Int_t t=0; t < NrObs; t++){
    if(SfromSurf)   std::cout << "Observable Nb. " << Surf[t].second +1<< ": "<< VariableNames[Surf[t].second] << " has S value = " << Surf[t].first ;
    std::cout << " " << std::endl;
  } 
   
  std::cout << " " << std::endl;
 

 
  

  if(SfromSurf) std::cout << "S-ordered list of variables taking in acount the correlations <" << MaxCorr << "% (for *surface*) " <<std::endl;
  

  vector<Int_t> indexC;

  if(SfromSurf) {
    
    for (Int_t t=0; t < NrObs; t++){
      //if(t>4) continue;  
      std::cout << "(" << Surf[t].second+1 << ":"<< VariableNames[Surf[t].second] << ") " ;
      
      indexC.push_back(t);
	
	for (Int_t k=0; k < NrObs; k++){  
	  if(k==t) continue;
	  
	  indexC.push_back(k);
	  bool accepted=true;

	  for (Int_t i=0; i < indexC.size(); i++){
	    if(k==indexC[i])continue;
	    if(fabs(Corr[Surf[k].second][Surf[indexC[i]].second])>MaxCorr){	
	      accepted=false;
	      if(accepted==false) continue;
	     
	    }
	  }
	  if(accepted) {
	    std::cout << "(" << Surf[k].second+1 << ":"<< VariableNames[Surf[k].second] << ") " ;
	    //std::cout << " " << std::endl;
	  } else {
	    indexC.pop_back();
	  }
	} 
      std::cout << " " << std::endl;
      std::cout << " " << std::endl;
      indexC.clear();
    }
  }
 

  if(SfromPTE) std::cout << "S-ordered list of variables (for *PTE*) " <<std::endl;
  for (Int_t t=0; t < NrObs; t++){
    if(SfromPTE)   std::cout << "Observable Nb." << PTE[t].second+1 << ": "<< VariableNames[PTE[t].second] << " has S value = " << PTE[t].first ;
     std::cout << " " << std::endl;
  } 
  
  std::cout << " " << std::endl;

  if(SfromPTE) std::cout << "S-ordered list of variables taking in acount the correlations <" << MaxCorr << "% (for *PTE*) " <<std::endl;
  

  //vector<Int_t> indexC;

  if(SfromPTE) {
    
    for (Int_t t=0; t < NrObs; t++){
      //if(t>4) continue;  
      std::cout << "(" << PTE[t].second+1 << ":"<< VariableNames[PTE[t].second] << ") " ;
      
      indexC.push_back(t);
	
	for (Int_t k=0; k < NrObs; k++){  
	  if(k==t) continue;
	  
	  indexC.push_back(k);
	  bool accepted=true;

	  for (Int_t i=0; i < indexC.size(); i++){
	    if(k==indexC[i])continue;
	    if(fabs(Corr[PTE[k].second][PTE[indexC[i]].second])>MaxCorr){	
	      accepted=false;
	      if(accepted==false) continue;
	     
	    }
	  }
	  if(accepted) {
	    std::cout << "(" << PTE[k].second+1 << ":"<< VariableNames[PTE[k].second] << ") " ;
	    //std::cout << " " << std::endl;
	  } else {
	    indexC.pop_back();
	  }
	} 
      std::cout << " " << std::endl;
      std::cout << " " << std::endl;
      indexC.clear();
    }
  }
 



  // for (Int_t t=0; t < 89; t++){
  for (Int_t t=0; t < Surf.size(); t++){
    if(SfromSurf)   SvalueSurf->Fill(Surf[t].first);
    if(SfromPTE)  SvaluePTE->Fill(PTE[t].first);
    //    for (Int_t u=0; u < 89; u++){
    if(AddCorrelation) {
      for (Int_t u=0; u < NrObs; u++){
	corrHist->Fill(Corr[u][t]);
      }
    }
  }
  
  //if(SfromSurf)  SvalueSurf->Draw();
  //if(SfromPTE)  SvaluePTE->Draw();
  if(AddCorrelation) corrHist->Draw();


//     struct sort_pred {
//     bool operator()(const std::pair<Int_t,Double_t> &left, const std::pair<Int_t,Double_t> &right) {
//       return left.second < right.second;
//       //return left.first < right.first;
//     }
//   };
// 
//   std::sort(Calojet10.begin(), Calojet10.end(), greater<pair<double, unsigned Int_t > >() ); 
//   std::sort(Conv.begin(), Conv.end(), sort_pred());

//   typedef std::pair<Int_t, Double_t> my_pair;
//   bool sort_pred(const my_pair& left, const my_pair& right)
//     {
//       return left.second < right.second;
//       }

  // std::sort(v.begin(), v.end(), sort_pred());

  //  std::sort(Conv.begin(), Conv.end(), sort_pred());


  
  //gROOT->ProcessLine(".q"); 
}

//*/
void setTDRStyle() {

TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

// For the canvas:
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetCanvasDefH(600); //Height of canvas
  tdrStyle->SetCanvasDefW(600); //Width of canvas
  tdrStyle->SetCanvasDefX(0);   //POsition on screen
  tdrStyle->SetCanvasDefY(0);

// For the Pad:
  tdrStyle->SetPadBorderMode(0);
  // tdrStyle->SetPadBorderSize(Width_t size = 1);
  tdrStyle->SetPadColor(kWhite);
  tdrStyle->SetPadGridX(false);
  tdrStyle->SetPadGridY(false);
  tdrStyle->SetGridColor(0);
  tdrStyle->SetGridStyle(3);
  tdrStyle->SetGridWidth(1);

// For the frame:
  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetFrameBorderSize(1);
  tdrStyle->SetFrameFillColor(0);
  tdrStyle->SetFrameFillStyle(0);
  tdrStyle->SetFrameLineColor(1);
  tdrStyle->SetFrameLineStyle(1);
  tdrStyle->SetFrameLineWidth(1);

// For the histo:
  // tdrStyle->SetHistFillColor(1);
  // tdrStyle->SetHistFillStyle(0);
  tdrStyle->SetHistLineColor(1);
  tdrStyle->SetHistLineStyle(0);
  tdrStyle->SetHistLineWidth(1);
  // tdrStyle->SetLegoInnerR(Float_t rad = 0.5);
  // tdrStyle->SetNumberContours(Int_t number = 20);

  tdrStyle->SetEndErrorSize(2);
  // tdrStyle->SetErrorMarker(20);
  tdrStyle->SetMarkerStyle(20);
  tdrStyle->SetErrorX(0.);

// For the fit/function:
  tdrStyle->SetOptFit(1);
  tdrStyle->SetFitFormat("5.4g");
  tdrStyle->SetFuncColor(2);
  tdrStyle->SetFuncStyle(1);
  tdrStyle->SetFuncWidth(1);


//For the date:
  tdrStyle->SetOptDate(0);
  // tdrStyle->SetDateX(Float_t x = 0.01);
  // tdrStyle->SetDateY(Float_t y = 0.01);


// For the statistics box:
  tdrStyle->SetOptFile(0);
  tdrStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
  tdrStyle->SetStatColor(kWhite);
  tdrStyle->SetStatFont(42);
  tdrStyle->SetStatFontSize(0.025);
  tdrStyle->SetStatTextColor(1);
  tdrStyle->SetStatFormat("6.4g");
  tdrStyle->SetStatBorderSize(1);
  tdrStyle->SetStatH(0.1);
  tdrStyle->SetStatW(0.15);
  // tdrStyle->SetStatStyle(Style_t style = 1001);
  // tdrStyle->SetStatX(Float_t x = 0);
  // tdrStyle->SetStatY(Float_t y = 0);

// Margins:
  tdrStyle->SetPadTopMargin(0.05);
  tdrStyle->SetPadBottomMargin(0.13);
  tdrStyle->SetPadLeftMargin(0.13);
  tdrStyle->SetPadRightMargin(0.05);

// For the Global title:
  // tdrStyle->SetOptTitle(0);
  tdrStyle->SetTitleFont(42);
  tdrStyle->SetTitleColor(1);
  tdrStyle->SetTitleTextColor(1);
  tdrStyle->SetTitleFillColor(10);
  tdrStyle->SetTitleFontSize(0.05);
  // tdrStyle->SetTitleH(0); // Set the height of the title box
  // tdrStyle->SetTitleW(0); // Set the width of the title box
  // tdrStyle->SetTitleX(0); // Set the position of the title box
  // tdrStyle->SetTitleY(0.985); // Set the position of the title box
  // tdrStyle->SetTitleStyle(Style_t style = 1001);
  // tdrStyle->SetTitleBorderSize(2);

// For the axis titles:
  tdrStyle->SetTitleColor(1, "XYZ");
  tdrStyle->SetTitleFont(42, "XYZ");
  tdrStyle->SetTitleSize(0.06, "XYZ");
  // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // tdrStyle->SetTitleYSize(Float_t size = 0.02);
  tdrStyle->SetTitleXOffset(0.9);
  tdrStyle->SetTitleYOffset(1.05);
  // tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset


// For the axis labels:
  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelFont(42, "XYZ");
  tdrStyle->SetLabelOffset(0.007, "XYZ");
  tdrStyle->SetLabelSize(0.05, "XYZ");

// For the axis:
  tdrStyle->SetAxisColor(1, "XYZ");
  tdrStyle->SetStripDecimals(kTRUE);
  tdrStyle->SetTickLength(0.03, "XYZ");
  tdrStyle->SetNdivisions(510, "XYZ");
  tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  tdrStyle->SetPadTickY(1);

// Change for log plots:
  tdrStyle->SetOptLogx(0);
  tdrStyle->SetOptLogy(0);
  tdrStyle->SetOptLogz(0);

// Postscript options:
  // tdrStyle->SetPaperSize(15.,15.);
  // tdrStyle->SetLineScalePS(Float_t scale = 3);
  // tdrStyle->SetLineStyleString(Int_t i, const char* text);
  // tdrStyle->SetHeaderPS(const char* header);
  // tdrStyle->SetTitlePS(const char* pstitle);

  // tdrStyle->SetBarOffset(Float_t baroff = 0.5);
  // tdrStyle->SetBarWidth(Float_t barwidth = 0.5);
  // tdrStyle->SetPaintTextFormat(const char* format = "g");
  // tdrStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
  // tdrStyle->SetTimeOffset(Double_t toffset);
  // tdrStyle->SetHistMinimumZero(kTRUE);

  tdrStyle->cd();


}



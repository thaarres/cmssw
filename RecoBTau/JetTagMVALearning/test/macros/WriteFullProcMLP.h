
#ifndef WriteFullProcMLP_h
#define WriteFullProcMLP_h

//Not all this includes are really needed
#include <TStyle.h>
#include <TCanvas.h>
#include <TFrame.h>
#include "TH1D.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TCanvas.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include "TLegend.h"
#include "TFile.h"
#include "TROOT.h"
#include "TF1.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <TString.h>
#include <fstream>


class WriteFullProcMLP {
public:

   ofstream xmlFile;
   
   TString VtxCat;
   TString Flavour;
   TString tagVtx;//to distinguish BvsC and BvsDUSG trainings
   TString procCat;//Choose Name of the only ProcCategory of the xml file
   TString procCount;//Choose Name of the only ProcCount of the xml file
   TString procNorm;//Choose Name of the only ProcNormalize of the xml file
   TString procSplit;
   TString procLike;
   TString procOpt;
   TString procMLP;
   TString procBiasNorm;
   TString procLinear;
   TString NIter;
   TString Struct;
   
   std::map<TString, std::map<TString, TString> > mapBias;
   
   
   bool FixRange;
   bool writeSplittersForNonMultiple;
   bool writeMatrices;
   bool varSet;
   
   std::vector<TString> Vname;
   std::vector<TString> Vmultiple;
   std::vector<TString> Voptional;
   std::vector<TString> Vtreatment;
   std::vector<TString> Vsmooth;
   std::vector<TString> Vsize;
   std::vector<TString> Vlower;
   std::vector<TString> Vupper;
   
   std::vector<TString> Oprocess;
   std::vector<TString> Oid;
   
   std::vector<std::pair<TString, TString> > mlp;
   
   WriteFullProcMLP();
   WriteFullProcMLP(TString VtxCat_, TString Flavour_);
   virtual ~WriteFullProcMLP();
   void addVariables();
   void addVariable(TString name,
                    TString multiple,
		    TString optional,
                    TString treatment,
                    TString smooth,
                    TString size,
                    TString lower,
                    TString upper
		    );
		    
   void setVarSet(int x);
   
   void FillMLP(int varset = 0);
   void WriteHeader();
   void WriteInput();
   void WriteCategory();
   void WriteCount(TString VarID, TString parentVarProcess, TString parentVarID);
   void WriteNormalize();
   void WriteSplitters();
   void WriteSplittersForNonMultiple();
   void WriteLRx();
   void WriteLRall();
   void WriteLRsplit();
   void WriteOptional();
   void WriteMatrices();
   void WriteMatrix(TString processID, std::vector<TString> v);
   void WriteProcMLP(std::vector<std::pair<TString, TString> > v_process_id);
   void WriteBiasNorm(TString processID, TString VarID, TString parentVarProcess, TString parentVarID);
   void WriteLRBias(TString processID, TString VarID, TString parentVarProcess, TString parentVarID);
   void WriteLinear();
   void WriteOutput(TString processID, TString VarID);
   
   void Loop();
   
   
   
   
};
#endif


#ifdef WriteFullProcMLP_cxx

//VtxCat = Reco, Pseudo, No
//Flavour = C, DUSG, Combined

WriteFullProcMLP::WriteFullProcMLP(){
  //WriteFullProcMLP("Reco", "DUSG");
}



WriteFullProcMLP::WriteFullProcMLP(TString VtxCat_, TString Flavour_){
  //xmlFile.open("output.xml");
  
  TString xml_flavour_name = "_B_" + Flavour_ + "_";
  if (Flavour_ == "Combined") xml_flavour_name = "_"; 
  xmlFile.open("CSVMVA_"+VtxCat_+"Vertex"+xml_flavour_name+"defaultFromDB.xml");
  
  VtxCat = VtxCat_;
  Flavour = Flavour_;
  varSet = 0;
  
  if (Flavour == "DUSG" || Flavour == "Combined") tagVtx = "DUSG";
  else if (Flavour == "C") tagVtx = "C";
  else {
    std::cout << "Unknown Flavour= " << Flavour << std::endl;
    exit(0);
  }
  procCat = "procCat";
  procCount = "procCount";
  procNorm = "procNorm";
  procSplit = "Spl_";
  procLike = "LR_";
  procOpt = "procOpt";
  procMLP = "procMLP";
  procBiasNorm = "BiasNorm";
  procLinear = "procLinear";
  NIter = "100";
  
  if (VtxCat == "Reco") Struct = "44";
  else if (VtxCat == "Pseudo") Struct = "40";
  else if (VtxCat == "No") Struct = "30";
  else {
    std::cout << "Unknown VtxCat= " << VtxCat << std::endl;
    exit(0);
  }
  
  FixRange = true;
  writeSplittersForNonMultiple = true;//for Procmatrix studies
  writeMatrices = false;
  
  Vname.clear();
  Vmultiple.clear();
  Voptional.clear();
  Vtreatment.clear();
  Vsmooth.clear();
  Vsize.clear();
  Vlower.clear();
  Vupper.clear();
  
  Oprocess.clear();
  Oid.clear();
  mapBias.clear();
  
  mlp.clear();

/*

  
  mapBias["Reco"]["DUSG"] = TString("<bias>139.808</bias>\n") +
TString("<bias>89.3185</bias>\n") +
TString("<bias>50.7531</bias>\n") +
TString("<bias>52.6124</bias>\n") +
TString("<bias>37.4155</bias>\n") +
TString("<bias>24.3036</bias>\n") +
TString("<bias>40.2046</bias>\n") +
TString("<bias>28.9391</bias>\n") +
TString("<bias>19.1818</bias>\n") +
TString("<bias>31.5035</bias>\n") +
TString("<bias>22.5154</bias>\n") +
TString("<bias>14.7495</bias>\n") +
TString("<bias>22.8651</bias>\n") +
TString("<bias>15.0419</bias>\n") +
TString("<bias>9.48608</bias>\n") +
TString("<bias>11.5759</bias>\n") +
TString("<bias>6.72355</bias>\n") +
TString("<bias>7.82546</bias>\n") +
TString("<bias>4.96855</bias>");

  mapBias["Reco"]["C"] = TString("<bias>139.808</bias>\n") +
TString("<bias>89.3185</bias>\n") +
TString("<bias>50.7531</bias>\n") +
TString("<bias>52.6124</bias>\n") +
TString("<bias>37.4155</bias>\n") +
TString("<bias>24.3036</bias>\n") +
TString("<bias>40.2046</bias>\n") +
TString("<bias>28.9391</bias>\n") +
TString("<bias>19.1818</bias>\n") +
TString("<bias>31.5035</bias>\n") +
TString("<bias>22.5154</bias>\n") +
TString("<bias>14.7495</bias>\n") +
TString("<bias>22.8651</bias>\n") +
TString("<bias>15.0419</bias>\n") +
TString("<bias>9.48608</bias>\n") +
TString("<bias>11.5759</bias>\n") +
TString("<bias>6.72355</bias>\n") +
TString("<bias>7.82546</bias>\n") +
TString("<bias>4.96855</bias>");

  mapBias["Pseudo"]["DUSG"] = TString("<bias>139.808</bias>\n") +
TString("<bias>89.3185</bias>\n") +
TString("<bias>50.7531</bias>\n") +
TString("<bias>52.6124</bias>\n") +
TString("<bias>37.4155</bias>\n") +
TString("<bias>24.3036</bias>\n") +
TString("<bias>40.2046</bias>\n") +
TString("<bias>28.9391</bias>\n") +
TString("<bias>19.1818</bias>\n") +
TString("<bias>31.5035</bias>\n") +
TString("<bias>22.5154</bias>\n") +
TString("<bias>14.7495</bias>\n") +
TString("<bias>22.8651</bias>\n") +
TString("<bias>15.0419</bias>\n") +
TString("<bias>9.48608</bias>\n") +
TString("<bias>11.5759</bias>\n") +
TString("<bias>6.72355</bias>\n") +
TString("<bias>7.82546</bias>\n") +
TString("<bias>4.96855</bias>");

  mapBias["Pseudo"]["C"] = TString("<bias>139.808</bias>\n") +
TString("<bias>89.3185</bias>\n") +
TString("<bias>50.7531</bias>\n") +
TString("<bias>52.6124</bias>\n") +
TString("<bias>37.4155</bias>\n") +
TString("<bias>24.3036</bias>\n") +
TString("<bias>40.2046</bias>\n") +
TString("<bias>28.9391</bias>\n") +
TString("<bias>19.1818</bias>\n") +
TString("<bias>31.5035</bias>\n") +
TString("<bias>22.5154</bias>\n") +
TString("<bias>14.7495</bias>\n") +
TString("<bias>22.8651</bias>\n") +
TString("<bias>15.0419</bias>\n") +
TString("<bias>9.48608</bias>\n") +
TString("<bias>11.5759</bias>\n") +
TString("<bias>6.72355</bias>\n") +
TString("<bias>7.82546</bias>\n") +
TString("<bias>4.96855</bias>");

  mapBias["No"]["DUSG"] = TString("<bias>139.808</bias>\n") +
TString("<bias>89.3185</bias>\n") +
TString("<bias>50.7531</bias>\n") +
TString("<bias>52.6124</bias>\n") +
TString("<bias>37.4155</bias>\n") +
TString("<bias>24.3036</bias>\n") +
TString("<bias>40.2046</bias>\n") +
TString("<bias>28.9391</bias>\n") +
TString("<bias>19.1818</bias>\n") +
TString("<bias>31.5035</bias>\n") +
TString("<bias>22.5154</bias>\n") +
TString("<bias>14.7495</bias>\n") +
TString("<bias>22.8651</bias>\n") +
TString("<bias>15.0419</bias>\n") +
TString("<bias>9.48608</bias>\n") +
TString("<bias>11.5759</bias>\n") +
TString("<bias>6.72355</bias>\n") +
TString("<bias>7.82546</bias>\n") +
TString("<bias>4.96855</bias>");

  mapBias["No"]["C"] = TString("<bias>139.808</bias>\n") +
TString("<bias>89.3185</bias>\n") +
TString("<bias>50.7531</bias>\n") +
TString("<bias>52.6124</bias>\n") +
TString("<bias>37.4155</bias>\n") +
TString("<bias>24.3036</bias>\n") +
TString("<bias>40.2046</bias>\n") +
TString("<bias>28.9391</bias>\n") +
TString("<bias>19.1818</bias>\n") +
TString("<bias>31.5035</bias>\n") +
TString("<bias>22.5154</bias>\n") +
TString("<bias>14.7495</bias>\n") +
TString("<bias>22.8651</bias>\n") +
TString("<bias>15.0419</bias>\n") +
TString("<bias>9.48608</bias>\n") +
TString("<bias>11.5759</bias>\n") +
TString("<bias>6.72355</bias>\n") +
TString("<bias>7.82546</bias>\n") +
TString("<bias>4.96855</bias>");
*/


  mapBias["Reco"]["DUSG"] =
    TString("<bias>58.0902</bias>\n") +
    TString("<bias>41.6971</bias>\n") +
    TString("<bias>28.3985</bias>\n") +
    TString("<bias>41.9596</bias>\n") +
    TString("<bias>30.5144</bias>\n") +
    TString("<bias>20.6542</bias>\n") +
    TString("<bias>34.9875</bias>\n") +
    TString("<bias>25.1552</bias>\n") +
    TString("<bias>16.9442</bias>\n") +
    TString("<bias>28.3286</bias>\n") +
    TString("<bias>20.1282</bias>\n") +
    TString("<bias>13.0102</bias>\n") +
    TString("<bias>20.7791</bias>\n") +
    TString("<bias>13.9049</bias>\n") +
    TString("<bias>8.95809</bias>\n") +
    TString("<bias>11.2143</bias>\n") +
    TString("<bias>6.82707</bias>\n") +
    TString("<bias>7.75588</bias>\n") +
    TString("<bias>4.73601</bias>\n");
       
       

  mapBias["Reco"]["C"] = 
    TString("<bias>3.54908</bias>\n") +
    TString("<bias>3.96524</bias>\n") +
    TString("<bias>4.05405</bias>\n") +
    TString("<bias>3.14922</bias>\n") +
    TString("<bias>3.43942</bias>\n") +
    TString("<bias>3.61338</bias>\n") +
    TString("<bias>2.96939</bias>\n") +
    TString("<bias>3.19214</bias>\n") +
    TString("<bias>3.37679</bias>\n") +
    TString("<bias>2.85962</bias>\n") +
    TString("<bias>2.96425</bias>\n") +
    TString("<bias>3.07407</bias>\n") +
    TString("<bias>2.86782</bias>\n") +
    TString("<bias>2.82837</bias>\n") +
    TString("<bias>2.95484</bias>\n") +
    TString("<bias>2.84215</bias>\n") +
    TString("<bias>2.40304</bias>\n") +
    TString("<bias>2.59101</bias>\n") +
    TString("<bias>2.85577</bias>\n");


  mapBias["Pseudo"]["DUSG"] = 
    TString("<bias>15.9876</bias>\n") +
    TString("<bias>18.3107</bias>\n") +
    TString("<bias>16.2624</bias>\n") +
    TString("<bias>7.78643</bias>\n") +
    TString("<bias>8.47539</bias>\n") +
    TString("<bias>7.54226</bias>\n") +
    TString("<bias>5.6141</bias>\n") +
    TString("<bias>5.74704</bias>\n") +
    TString("<bias>4.94291</bias>\n") +
    TString("<bias>4.69452</bias>\n") +
    TString("<bias>4.3728</bias>\n") +
    TString("<bias>3.1419</bias>\n") +
    TString("<bias>3.79953</bias>\n") +
    TString("<bias>3.16405</bias>\n") +
    TString("<bias>2.72256</bias>\n") +
    TString("<bias>2.81606</bias>\n") +
    TString("<bias>3.21926</bias>\n") +
    TString("<bias>2.57965</bias>\n") +
    TString("<bias>3.32301</bias>\n");

  mapBias["Pseudo"]["C"] = 
    TString("<bias>2.09442</bias>\n") +
    TString("<bias>2.70306</bias>\n") +
    TString("<bias>3.10447</bias>\n") +
    TString("<bias>1.45801</bias>\n") +
    TString("<bias>1.82994</bias>\n") +
    TString("<bias>2.00104</bias>\n") +
    TString("<bias>1.23248</bias>\n") +
    TString("<bias>1.49997</bias>\n") +
    TString("<bias>1.69172</bias>\n") +
    TString("<bias>1.20876</bias>\n") +
    TString("<bias>1.29101</bias>\n") +
    TString("<bias>1.3102</bias>\n") +
    TString("<bias>1.2501</bias>\n") +
    TString("<bias>1.19195</bias>\n") +
    TString("<bias>1.20769</bias>\n") +
    TString("<bias>1.24498</bias>\n") +
    TString("<bias>1.0806</bias>\n") +
    TString("<bias>1.08517</bias>\n") +
    TString("<bias>1.31154</bias>\n");

  mapBias["No"]["DUSG"] = 
    TString("<bias>0.494679</bias>\n") +
    TString("<bias>0.603949</bias>\n") +
    TString("<bias>0.72725</bias>\n") +
    TString("<bias>0.309296</bias>\n") +
    TString("<bias>0.407488</bias>\n") +
    TString("<bias>0.552474</bias>\n") +
    TString("<bias>0.245273</bias>\n") +
    TString("<bias>0.332774</bias>\n") +
    TString("<bias>0.482552</bias>\n") +
    TString("<bias>0.216049</bias>\n") +
    TString("<bias>0.298596</bias>\n") +
    TString("<bias>0.454424</bias>\n") +
    TString("<bias>0.237576</bias>\n") +
    TString("<bias>0.32552</bias>\n") +
    TString("<bias>0.466442</bias>\n") +
    TString("<bias>0.392945</bias>\n") +
    TString("<bias>0.470504</bias>\n") +
    TString("<bias>0.458261</bias>\n") +
    TString("<bias>0.52488</bias>\n");

  mapBias["No"]["C"] = 
    TString("<bias>0.576543</bias>\n") +
    TString("<bias>0.667669</bias>\n") +
    TString("<bias>0.774572</bias>\n") +
    TString("<bias>0.39703</bias>\n") +
    TString("<bias>0.488367</bias>\n") +
    TString("<bias>0.622579</bias>\n") +
    TString("<bias>0.330996</bias>\n") +
    TString("<bias>0.416634</bias>\n") +
    TString("<bias>0.559358</bias>\n") +
    TString("<bias>0.300747</bias>\n") +
    TString("<bias>0.387464</bias>\n") +
    TString("<bias>0.539248</bias>\n") +
    TString("<bias>0.328385</bias>\n") +
    TString("<bias>0.422979</bias>\n") +
    TString("<bias>0.551453</bias>\n") +
    TString("<bias>0.508926</bias>\n") +
    TString("<bias>0.615143</bias>\n") +
    TString("<bias>0.610848</bias>\n") +
    TString("<bias>0.625801</bias>\n");
  
}

WriteFullProcMLP::~WriteFullProcMLP(){
  Vname.clear();
  Vmultiple.clear();
  Voptional.clear();
  Vtreatment.clear();
  Vsmooth.clear();
  Vsize.clear();
  Vlower.clear();
  Vupper.clear();
  
  Oprocess.clear();
  Oid.clear();
  mapBias.clear();
  
  mlp.clear();
  

}

void WriteFullProcMLP::addVariable(
                    TString name,
                    TString multiple,
		    TString optional,
                    TString treatment,
                    TString smooth,
                    TString size,
                    TString lower,
                    TString upper) {

  Vname.push_back(name);
  Vmultiple.push_back(multiple);
  Voptional.push_back(optional);
  Vtreatment.push_back(treatment);
  Vsmooth.push_back(smooth);
  Vsize.push_back(size);
  Vlower.push_back(lower);
  Vupper.push_back(upper);

}


void WriteFullProcMLP::addVariables() {

  //They should be written a given order
  
  //treatment:
  //"All"
  //"Input" -> only input
  //"LRx" 
  //"LRsplit"
  //"LRall"
  //
  //addVariable(name, multiple, optional, treatment, smooth, size, lower, upper)
  addVariable("jetPt", "false", "false", "All", "x", "x", "x", "x");
  addVariable("jetEta", "false", "false", "All", "x", "x", "x", "x");
  addVariable("vertexCategory", "false", "false", "Input", "x", "x", "x", "x");
  addVariable("trackSip2dSig", "true", "true", "All", "3", "500", "-80", "150");
  addVariable("trackSip3dSig", "true", "true", "All", "3", "500", "-80", "150");
  addVariable("trackSip2dVal", "true", "true", "All", "3", "500", "-0.2", "0.2");
  addVariable("trackSip3dVal", "true", "true", "All", "3", "500", "-3", "3");
  addVariable("trackPtRel", "true", "true", "All", "3", "500", "0", "10");
  addVariable("trackPPar", "true", "true", "All", "3", "500", "0", "5000");
  addVariable("trackEtaRel", "true", "true", "All", "40", "100", "1.2", "8.5");

  //addVariable("trackDeltaR", "true", "true", "All", "3", "100", "0", "0.5");
  addVariable("trackDeltaR", "true", "true", "All", "3", "500", "0", "0.5");

  //addVariable("trackPtRatio", "true", "true", "All", "3", "100", "0", "0.3");
  addVariable("trackPtRatio", "true", "true", "All", "3", "500", "0", "0.3");

  addVariable("trackPParRatio", "true", "true", "All", "3", "100", "0.95", "1.01");

  //addVariable("trackJetDist", "true", "true", "All", "3", "200", "-0.07", "0");
  addVariable("trackJetDist", "true", "true", "All", "3", "500", "-0.07", "0");

  addVariable("trackDecayLenVal", "true", "true", "All", "3", "500", "0", "40");
  addVariable("vertexMass", "false", "false", "All", "10", "100", "0.2", "500");
  addVariable("vertexNTracks", "false", "false", "All", "0", "100", "1.5", "20.5");
  addVariable("vertexEnergyRatio", "false", "false", "All", "20", "500", "0", "10");
  addVariable("trackSip2dSigAboveCharm", "false", "false", "All", "3", "500", "-50", "80");
  addVariable("trackSip3dSigAboveCharm", "false", "false", "All", "3", "500", "-300", "150");
  addVariable("flightDistance2dSig", "false", "false", "All", "10", "100", "0", "350");
  addVariable("flightDistance3dSig", "false", "false", "All", "10", "100", "0", "350");
  addVariable("flightDistance2dVal", "false", "false", "All", "3", "100", "0.01", "2.5");
  addVariable("flightDistance3dVal", "false", "false", "All", "3", "500", "0", "20");
  addVariable("trackSumJetEtRatio", "false", "false", "All", "3", "500", "0", "6");
  addVariable("jetNSecondaryVertices", "false", "false", "All", "0", "100", "-0.5", "7.5");
  addVariable("vertexJetDeltaR", "false", "false", "All", "20", "100", "0", "0.5");
  addVariable("trackSumJetDeltaR", "false", "false", "All", "3", "100", "0", "4");
  addVariable("jetNTracks", "false", "false", "All", "0", "400", "0", "400");
  addVariable("trackSip2dValAboveCharm", "false", "false", "All", "3", "500", "-1.01", "0.2");
  addVariable("trackSip3dValAboveCharm", "false", "false", "All", "3", "500", "-1.5", "0.7");
  addVariable("vertexFitProb", "false", "false", "All", "3", "500", "0", "500");
  addVariable("chargedHadronEnergyFraction", "false", "false", "All", "3", "100", "-0.01", "1.01");
  addVariable("neutralHadronEnergyFraction", "false", "false", "All", "3", "100", "-0.01", "1.01");
  addVariable("photonEnergyFraction", "false", "false", "All", "3", "100", "-0.01", "1.01");
  addVariable("electronEnergyFraction", "false", "false", "All", "3", "100", "-0.01", "1.01");
  addVariable("muonEnergyFraction", "false", "false", "All", "3", "100", "-0.01", "1.01");
  addVariable("chargedHadronMultiplicity", "false", "false", "All", "0", "351", "-0.5", "350.5");
  addVariable("neutralHadronMultiplicity", "false", "false", "All", "0", "31", "-0.5", "30.5");
  addVariable("photonMultiplicity", "false", "false", "All", "0", "101", "-0.5", "100.5");
  addVariable("electronMultiplicity", "false", "false", "All", "0", "13", "-0.5", "12.5");
  addVariable("muonMultiplicity", "false", "false", "All", "0", "13", "-0.5", "12.5");
  addVariable("hadronMultiplicity", "false", "false", "All", "0", "351", "-0.5", "350.5");
  addVariable("hadronPhotonMultiplicity", "false", "false", "All", "0", "351", "-0.5", "350.5");
  addVariable("totalMultiplicity", "false", "false", "All", "0", "351", "-0.5", "350.5");
  addVariable("massVertexEnergyFraction", "false", "false", "All", "3", "100", "0", "35");
  addVariable("vertexBoostOverSqrtJetPt", "false", "false", "All", "3", "100", "-0.01", "1.01");
  addVariable("nTracks", "false", "false", "All", "0", "51", "-0.5", "50.5");
  addVariable("category", "false", "false", "All", "0", "20", "0", "20");


}

void WriteFullProcMLP::setVarSet(int x) {varSet = x;}

void WriteFullProcMLP::FillMLP(int varset) {
  //somehow it doesn't work to switch varset using setVarSet
  varset = 20;
  ///variable come either from ProcOptional or directly from ProcNormalize: procOpt+tagVtx or procNorm+tagVtx
  mlp.clear();
  
  //std::cout << "  varset = " << varset << std::endl;
  if (varset == 0) {
    if (VtxCat == "Reco") {
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetPt"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetEta"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackPtRel"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackPtRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackJetDist"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackDecayLenVal"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexMass"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexNTracks"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexEnergyRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip2dSigAboveCharm"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "flightDistance2dSig"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetEtRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetNSecondaryVertices"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "count_var1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_3"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_4"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackEtaRel_all"));
    }
    else if (VtxCat == "Pseudo") {
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetPt"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetEta"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackPtRel"));    
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackPtRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackJetDist"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackDecayLenVal"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexMass"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexNTracks"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexEnergyRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip2dSigAboveCharm"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetEtRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "count_var1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_3"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_4"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackEtaRel_all"));
    }
    else if (VtxCat == "No") {
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetPt"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetEta"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackPtRel"));    
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackPtRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackJetDist"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackDecayLenVal"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip2dSigAboveCharm"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetEtRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "count_var1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_3"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_4"));
    }
    else {std::cout << "Wrong VtxCat" << std::endl; exit(0);}
  }
  
  // ************************************
  else if (varset == 1) {
    if (VtxCat == "Reco") {
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetPt"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetEta"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexMass"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexNTracks"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexEnergyRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip2dSigAboveCharm"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "flightDistance2dSig"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetEtRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetNSecondaryVertices"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "count_var1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackPtRel_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackDeltaR_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackPtRatio_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackJetDist_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackDecayLenVal_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_3"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_4"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackEtaRel_all"));
    }
    else if (VtxCat == "Pseudo") {
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetPt"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetEta"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexMass"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexNTracks"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexEnergyRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip2dSigAboveCharm"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetEtRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "count_var1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackPtRel_1"));    
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackDeltaR_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackPtRatio_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackJetDist_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackDecayLenVal_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_3"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_4"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackEtaRel_all"));
    }
    else if (VtxCat == "No") {
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetPt"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetEta"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip2dSigAboveCharm"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetEtRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "count_var1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackPtRel_1"));    
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackDeltaR_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackPtRatio_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackJetDist_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackDecayLenVal_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_3"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_4"));
    }
    else {std::cout << "Wrong VtxCat" << std::endl; exit(0);}
  
  }
  
  // ************************************
  else if (varset == 2) {
    if (VtxCat == "Reco") {
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetPt"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetEta"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexMass"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexNTracks"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexEnergyRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip2dSigAboveCharm"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "flightDistance2dSig"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetEtRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetNSecondaryVertices"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "count_var1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackPtRel_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackDeltaR_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackPtRatio_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackJetDist_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackDecayLenVal_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_3"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_4"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackEtaRel_all"));
    }
    else if (VtxCat == "Pseudo") {
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetPt"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetEta"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexMass"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexNTracks"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexEnergyRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip2dSigAboveCharm"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetEtRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "count_var1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackPtRel_2"));    
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackDeltaR_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackPtRatio_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackJetDist_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackDecayLenVal_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_3"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_4"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackEtaRel_all"));
    }
    else if (VtxCat == "No") {
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetPt"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetEta"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip2dSigAboveCharm"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetEtRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "count_var1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackPtRel_2"));    
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackDeltaR_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackPtRatio_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackJetDist_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackDecayLenVal_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_3"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_4"));
    }
    else {std::cout << "Wrong VtxCat" << std::endl; exit(0);}
  
  }
  
  // ************************************
  else if (varset == 3) {
    if (VtxCat == "Reco") {
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetPt"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetEta"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexMass"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexNTracks"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexEnergyRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip2dSigAboveCharm"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "flightDistance2dSig"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetEtRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetNSecondaryVertices"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "count_var1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_3"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_4"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackPtRel_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackDeltaR_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackPtRatio_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackJetDist_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackDecayLenVal_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackEtaRel_all"));
    }
    else if (VtxCat == "Pseudo") {
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetPt"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetEta"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexMass"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexNTracks"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexEnergyRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip2dSigAboveCharm"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetEtRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "count_var1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_3"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_4"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackPtRel_1"));    
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackDeltaR_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackPtRatio_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackJetDist_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackDecayLenVal_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackEtaRel_all"));
    }
    else if (VtxCat == "No") {
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetPt"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetEta"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip2dSigAboveCharm"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetEtRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "count_var1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_3"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_4"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackPtRel_1"));    
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackDeltaR_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackPtRatio_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackJetDist_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackDecayLenVal_1"));
    }
    else {std::cout << "Wrong VtxCat" << std::endl; exit(0);}
  
  }
  
  // ************************************
  else if (varset == 4) {
    if (VtxCat == "Reco") {
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetPt"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetEta"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexMass"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexNTracks"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexEnergyRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip2dSigAboveCharm"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "flightDistance2dSig"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetEtRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetNSecondaryVertices"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "count_var1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_3"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_4"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackPtRel_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackDeltaR_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackPtRatio_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackJetDist_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackDecayLenVal_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackEtaRel_all"));
    }
    else if (VtxCat == "Pseudo") {
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetPt"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetEta"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexMass"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexNTracks"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexEnergyRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip2dSigAboveCharm"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetEtRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "count_var1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_3"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_4"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackPtRel_2"));    
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackDeltaR_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackPtRatio_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackJetDist_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackDecayLenVal_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackEtaRel_all"));
    }
    else if (VtxCat == "No") {
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetPt"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetEta"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip2dSigAboveCharm"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetEtRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "count_var1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_3"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_4"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackPtRel_2"));    
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackDeltaR_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackPtRatio_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackJetDist_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackDecayLenVal_2"));
    }
    else {std::cout << "Wrong VtxCat" << std::endl; exit(0);}
  
  }
  
  // ************************************
  else if (varset == 5) {
    if (VtxCat == "Reco") {
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetPt"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetEta"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexMass"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexNTracks"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexEnergyRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip2dSigAboveCharm"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "flightDistance2dSig"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetEtRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetNSecondaryVertices"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "count_var1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_3"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_4"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackPtRel_all"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackEtaRel_all"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackDeltaR_all"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackPtRatio_all"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackJetDist_all"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackDecayLenVal_all"));
    }
    else if (VtxCat == "Pseudo") {
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetPt"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetEta"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexMass"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexNTracks"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexEnergyRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip2dSigAboveCharm"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetEtRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "count_var1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_3"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_4"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackPtRel_all"));    
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackEtaRel_all"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackDeltaR_all"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackPtRatio_all"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackJetDist_all"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackDecayLenVal_all"));
    }
    else if (VtxCat == "No") {
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetPt"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetEta"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip2dSigAboveCharm"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetEtRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "count_var1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_3"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_4"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackPtRel_all"));    
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackDeltaR_all"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackPtRatio_all"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackJetDist_all"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackDecayLenVal_all"));
    }
    else {std::cout << "Wrong VtxCat" << std::endl; exit(0);}
  
  }
  
  // ************************************
  else if (varset == 6) {
    if (VtxCat == "Reco") {
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetPt"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetEta"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexMass"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexNTracks"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexEnergyRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip2dSigAboveCharm"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "flightDistance2dSig"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetEtRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetNSecondaryVertices"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "count_var1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_3"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_4"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackEtaRel_all"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackPtRel_split"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackDeltaR_split"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackPtRatio_split"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackJetDist_split"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackDecayLenVal_split"));
    }
    else if (VtxCat == "Pseudo") {
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetPt"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetEta"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexMass"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexNTracks"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexEnergyRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip2dSigAboveCharm"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetEtRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "count_var1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_3"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_4"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackEtaRel_all"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackPtRel_split"));    
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackDeltaR_split"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackPtRatio_split"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackJetDist_split"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackDecayLenVal_split"));
    }
    else if (VtxCat == "No") {
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetPt"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetEta"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip2dSigAboveCharm"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetEtRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "count_var1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_3"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_4"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackPtRel_split"));    
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackDeltaR_split"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackPtRatio_split"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackJetDist_split"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackDecayLenVal_split"));
    }
    else {std::cout << "Wrong VtxCat" << std::endl; exit(0);}
  
  }
  

  if (varset == 10) {
    if (VtxCat == "Reco") {
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetPt"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetEta"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip2dSig"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip3dSig"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip2dVal"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip3dVal"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackPtRel"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackPPar"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackEtaRel"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackPtRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackPParRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackJetDist"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackDecayLenVal"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexMass"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexNTracks"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexEnergyRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip2dSigAboveCharm"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "flightDistance2dSig"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetEtRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetNSecondaryVertices"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "count_var1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_3"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_4"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackEtaRel_all"));
    }
    else if (VtxCat == "Pseudo") {
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetPt"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetEta"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip2dSig"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip3dSig"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip2dVal"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip3dVal"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackPtRel"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackPPar"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackEtaRel"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackPtRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackPParRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackJetDist"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackDecayLenVal"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexMass"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexNTracks"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexEnergyRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip2dSigAboveCharm"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetEtRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "count_var1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_3"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_4"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackEtaRel_all"));
    }
    else if (VtxCat == "No") {
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetPt"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetEta"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip2dSig"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip3dSig"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip2dVal"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip3dVal"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackPtRel"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackPPar"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackPtRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackPParRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackJetDist"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackDecayLenVal"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip2dSigAboveCharm"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetEtRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "count_var1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_3"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_4"));
    }
    else {std::cout << "Wrong VtxCat" << std::endl; exit(0);}
  }

  if (varset == 11) {
    if (VtxCat == "Reco") {
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetPt"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetEta"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexMass"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexNTracks"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexEnergyRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip2dSigAboveCharm"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "flightDistance2dSig"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetEtRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetNSecondaryVertices"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "count_var1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackSip2dSig_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackSip3dSig_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackSip2dVal_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackSip3dVal_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackPtRel_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackPPar_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackEtaRel_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackDeltaR_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackPtRatio_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackPParRatio_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackJetDist_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackDecayLenVal_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_3"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_4"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackEtaRel_all"));
    }
    else if (VtxCat == "Pseudo") {
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetPt"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetEta"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexMass"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexNTracks"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexEnergyRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip2dSigAboveCharm"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetEtRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "count_var1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackSip2dSig_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackSip3dSig_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackSip2dVal_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackSip3dVal_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackPtRel_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackPPar_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackEtaRel_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackDeltaR_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackPtRatio_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackPParRatio_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackJetDist_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackDecayLenVal_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_3"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_4"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackEtaRel_all"));
    }
    else if (VtxCat == "No") {
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetPt"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetEta"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip2dSigAboveCharm"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetEtRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "count_var1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackSip2dSig_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackSip3dSig_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackSip2dVal_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackSip3dVal_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackPtRel_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackPPar_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackDeltaR_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackPtRatio_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackPParRatio_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackJetDist_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackDecayLenVal_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_3"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_4"));
    }
    else {std::cout << "Wrong VtxCat" << std::endl; exit(0);}
  }
  
  //40%
  if (varset == 20) {
    if (VtxCat == "Reco") {
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexEnergyRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip3dSigAboveCharm"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "flightDistance3dSig"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "jetNSecondaryVertices"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexFitProb"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "chargedHadronEnergyFraction"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "chargedHadronMultiplicity"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "hadronMultiplicity"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "hadronPhotonMultiplicity"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "massVertexEnergyFraction"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackPtRel_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackPtRatio_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackJetDist_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_3"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_4"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackEtaRel_all"));
    }
    else if (VtxCat == "Pseudo") {
      mlp.push_back(std::make_pair(procNorm+tagVtx, "vertexNTracks"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSip3dSigAboveCharm"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetEtRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "chargedHadronMultiplicity"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "hadronMultiplicity"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "hadronPhotonMultiplicity"));  
      mlp.push_back(std::make_pair(procNorm+tagVtx, "massVertexEnergyFraction"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackSip2dSig_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackPtRel_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackPtRatio_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_3"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_4"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackEtaRel_all"));
    }
    else if (VtxCat == "No") {
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetEtRatio"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "trackSumJetDeltaR"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "neutralHadronEnergyFraction"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "hadronPhotonMultiplicity"));  
      mlp.push_back(std::make_pair(procNorm+tagVtx, "totalMultiplicity"));
      mlp.push_back(std::make_pair(procNorm+tagVtx, "count_var1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackPtRel_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackDeltaR_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackJetDist_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "trackDecayLenVal_1"));      
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_1"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_2"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_3"));
      mlp.push_back(std::make_pair(procOpt+tagVtx, "LR_trackSip3dSig_4"));
    }
    else {std::cout << "Wrong VtxCat" << std::endl; exit(0);}
  }
  
  
}

#endif // #ifdef WriteFullProcMLP_cxx

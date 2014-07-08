#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <TString.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TROOT.h>
#include <fstream>

//USAGE
//root -q -b ~/Anal_Macros/CP3llbb/ComparePDFs.C'+("Reco")'
//root -q -b ~/Anal_Macros/CP3llbb/ComparePDFs.C'+("Pseudo")'
//root -q -b ~/Anal_Macros/CP3llbb/ComparePDFs.C'+("No")'
//
//Uses as inputs rootfiles obtained with command mvaExtractPDFs
// The output is a folder with plots comparing both set of inputs
// .html files are added to ease the comparison with a web browser
//To adjust:
//folderOutputRoot -> Folder where the plots will be stored. It will be created if it doesn't exist
//folderOld, folderNew->Folder where the "new" and "old" pdf's rootfiles to be compared are, the rootfiles are assumed to be called
//			pdf_CombinedSV{VtxCat}Vertex.root, e.g.: pdf_CombinedSVRecoVertex.root
//In ComparePDFs() The pdf's and input roofiles corresponding to different kind of processors are compared
//  it can be adapted to different kind of configurations. In this example:
//  @First the input variables are compared. Inputs for a first ProcNorm process
//   it is not assumed that the processes are in the same order in both considered rootfiles
//   mapNormInputDUSG first and mapNormInputC are used to match the proc numbers in the old and new rootfile
//   and to assign the name of the variable they represent (to be used in the plotting) 
//  @Second the pdfs for a likelihood done in different (pt/eta) categories (first for BvsDUSG, then for BvsC) are compared
//   std::vector<TString> Vars; is filled with the variables in the correct order
//   then e.g.: PlotPDFs("proc4", "proc5", Vars, "LkhDUSG"); -> gives the correspondance between the "old" and "new" process number
//   again they are not assumed to be necessarily the same, and name of the subfolder where the corresponding plots will go
//   inside PlotPDFs you need to push the name you want for the different categories of the LR in std::vector<TString> category;
//   you should add as many names as (pt/eta) bins in the LR
//  @Third: Similarly to @First, the plots corresponding to another ProcNorm (corresponding to the normalization of the previous LR) are
//    compared
//    usgin e.g.: PlotNormVars("proc5_norm1", "proc6_norm1", "Lkh_BvsDUSG", outPreNormLkh, html_PreNormLkh);
//  @Fourth: Using a similar logic to @Second, the "Bias likelihood" plots are compared
//copytoweb -> You can make a .sh command at the end of producing all the plots. In this case to copy the folder with all the plots to a
//  web server

#define NCat 15
std::map<TString, std::map<TString, std::pair<TString, TString> > > mapNormInputDUSG;
std::map<TString, std::map<TString, std::pair<TString, TString> > >::const_iterator itrNormInputDUSG;

std::map<TString, std::map<TString, std::pair<TString, TString> > > mapNormInputC;
std::map<TString, std::map<TString, std::pair<TString, TString> > >::const_iterator itrNormInputC;

TString folderOutputRoot = "./extractPDFstestxxx/";

TString folderOutput;
TString folderOld = "/home/fynu/vizangarciaj/storage/CMSSW53btag121031/CMSSW_5_3_5/src/RecoBTau/JetTagMVALearning/test/extractPDF_oldCSV/";
TString folderNew = "/home/fynu/vizangarciaj/storage/CMSSW53btag121031/CMSSW_5_3_5/src/RecoBTau/JetTagMVALearning/test/extractPDF_NoOffsetFixNoTrackFilterNofitReweight/";

TString outInputVarsDUSG = "InputVarsDUSG";
TString outInputVarsC = "InputVarsC";

TString outPreNormLkh = "PreNormLkh";


TFile* fold = 0;
TFile* fnew = 0;

bool PlotNormVars(TString, TString, TString, TString, ofstream&);
void PlotPDFs(TString procNew, TString procOld, std::vector<TString> vars, TString foldername);
void PlotBiasPDFs(TString procNew, TString procOld, TString foldername);

void NormalizeIntegralPlusUandO(TH1F* h);
void ComparePDFs(TString VtxCat) {

  folderOutput = folderOutputRoot+"/"+VtxCat+"/";
  
  fold = TFile::Open(folderOld+"pdf_CombinedSV"+VtxCat+"Vertex.root");
  fnew = TFile::Open(folderNew+"pdf_CombinedSV"+VtxCat+"Vertex.root");
  
  TString makedir = ".! mkdir " + folderOutputRoot;
  gROOT->ProcessLine(makedir);
  
  
  makedir = ".! mkdir " + folderOutput;
  gROOT->ProcessLine(makedir);
  
  

  //*********************************************************************
  //norm (input variables for B vs DUSG)
  //*********************************************************************
  if (VtxCat == "Reco") {
    mapNormInputDUSG["proc2"]["norm1"] = std::make_pair("proc3_norm1", "jetPt");
    mapNormInputDUSG["proc2"]["norm2"] = std::make_pair("proc3_norm2", "jetEta");
    mapNormInputDUSG["proc2"]["norm8"] = std::make_pair("proc3_norm8", "trackEtaRel");
    mapNormInputDUSG["proc2"]["norm9"] = std::make_pair("proc3_norm11", "vertexMass");
    mapNormInputDUSG["proc2"]["norm10"] = std::make_pair("proc3_norm12", "vertexNTracks");
    mapNormInputDUSG["proc2"]["norm11"] = std::make_pair("proc3_norm13", "vertexEnergyRatio");
    mapNormInputDUSG["proc2"]["norm12"] = std::make_pair("proc3_norm14", "trackSip2dSigAboveCharm");
    mapNormInputDUSG["proc2"]["norm14"] = std::make_pair("proc3_norm16", "flightDistance2dSig");
    mapNormInputDUSG["proc2"]["norm4"] = std::make_pair("proc3_norm4", "trackSip3dSig");//before splitting  
  }
  else if (VtxCat == "Pseudo") {
    mapNormInputDUSG["proc2"]["norm1"] = std::make_pair("proc3_norm1", "jetPt");
    mapNormInputDUSG["proc2"]["norm2"] = std::make_pair("proc3_norm2", "jetEta");
    mapNormInputDUSG["proc2"]["norm8"] = std::make_pair("proc3_norm8", "trackEtaRel");
    mapNormInputDUSG["proc2"]["norm9"] = std::make_pair("proc3_norm11", "vertexMass");
    mapNormInputDUSG["proc2"]["norm10"] = std::make_pair("proc3_norm12", "vertexNTracks");
    mapNormInputDUSG["proc2"]["norm11"] = std::make_pair("proc3_norm13", "vertexEnergyRatio");
    mapNormInputDUSG["proc2"]["norm12"] = std::make_pair("proc3_norm14", "trackSip2dSigAboveCharm");
    mapNormInputDUSG["proc2"]["norm4"] = std::make_pair("proc3_norm4", "trackSip3dSig");//before splitting  
  }
  else if (VtxCat == "No") {
    mapNormInputDUSG["proc2"]["norm1"] = std::make_pair("proc3_norm1", "jetPt");
    mapNormInputDUSG["proc2"]["norm2"] = std::make_pair("proc3_norm2", "jetEta");
    mapNormInputDUSG["proc2"]["norm4"] = std::make_pair("proc3_norm4", "trackSip3dSig");//before splitting  
  }
  
  
  makedir = ".! mkdir " + folderOutput + "/" + outInputVarsDUSG;
  gROOT->ProcessLine(makedir);
  ofstream html_inputvarsDUSG;
  html_inputvarsDUSG.open(folderOutput + "/" + outInputVarsDUSG + "/index.html");
  //header for Input vars html
  html_inputvarsDUSG << "<h3> Input Variables </h3><br><br>" << endl;
  html_inputvarsDUSG << "<table border=\"1\" cellpadding=\"5\" cellspacing=\"5\" width=\"100%\">" << endl;
  html_inputvarsDUSG << "<tr>" << endl;
  html_inputvarsDUSG << "<th>CSV input variables</th>" << endl;
  html_inputvarsDUSG << "</tr>" << endl;

  
  
  for(itrNormInputDUSG = mapNormInputDUSG.begin(); itrNormInputDUSG != mapNormInputDUSG.end(); ++itrNormInputDUSG){
    std::map<TString, std::pair<TString, TString> > mapHisto = (*itrNormInputDUSG).second;
    std::map<TString, std::pair<TString, TString> >::const_iterator itrHisto;
    std::cout << (*itrNormInputDUSG).first << std::endl;
     
    for (itrHisto = mapHisto.begin(); itrHisto != mapHisto.end(); ++itrHisto) {
      std::cout << "  " << (*itrHisto).first << " = " << (*itrHisto).second.first << std::endl;
       
      bool success = PlotNormVars((*itrNormInputDUSG).first + "_" + (*itrHisto).first, (*itrHisto).second.first, (*itrHisto).second.second, outInputVarsDUSG, html_inputvarsDUSG);      
       
    }
  }
   
  //tail for Input vars html
  html_inputvarsDUSG<< "</table><br><br><br>" << endl;


  //*********************************************************************
  //norm (input variables for B vs C)
  //*********************************************************************
  if (VtxCat == "Reco") {
    mapNormInputC["proc8"]["norm1"] = std::make_pair("proc7_norm1", "jetPt");
    mapNormInputC["proc8"]["norm2"] = std::make_pair("proc7_norm2", "jetEta");
    mapNormInputC["proc8"]["norm8"] = std::make_pair("proc7_norm8", "trackEtaRel");
    mapNormInputC["proc8"]["norm9"] = std::make_pair("proc7_norm11", "vertexMass");
    mapNormInputC["proc8"]["norm10"] = std::make_pair("proc7_norm12", "vertexNTracks");
    mapNormInputC["proc8"]["norm11"] = std::make_pair("proc7_norm13", "vertexEnergyRatio");
    mapNormInputC["proc8"]["norm12"] = std::make_pair("proc7_norm14", "trackSip2dSigAboveCharm");
    mapNormInputC["proc8"]["norm14"] = std::make_pair("proc7_norm16", "flightDistance2dSig");
    mapNormInputC["proc8"]["norm4"] = std::make_pair("proc7_norm4", "trackSip3dSig");//before splitting  
  }
  else if (VtxCat == "Pseudo") {
    mapNormInputC["proc8"]["norm1"] = std::make_pair("proc7_norm1", "jetPt");
    mapNormInputC["proc8"]["norm2"] = std::make_pair("proc7_norm2", "jetEta");
    mapNormInputC["proc8"]["norm8"] = std::make_pair("proc7_norm8", "trackEtaRel");
    mapNormInputC["proc8"]["norm9"] = std::make_pair("proc7_norm11", "vertexMass");
    mapNormInputC["proc8"]["norm10"] = std::make_pair("proc7_norm12", "vertexNTracks");
    mapNormInputC["proc8"]["norm11"] = std::make_pair("proc7_norm13", "vertexEnergyRatio");
    mapNormInputC["proc8"]["norm12"] = std::make_pair("proc7_norm14", "trackSip2dSigAboveCharm");
    mapNormInputC["proc8"]["norm4"] = std::make_pair("proc7_norm4", "trackSip3dSig");//before splitting  
  }
  else if (VtxCat == "No") {
    mapNormInputC["proc8"]["norm1"] = std::make_pair("proc7_norm1", "jetPt");
    mapNormInputC["proc8"]["norm2"] = std::make_pair("proc7_norm2", "jetEta");
    mapNormInputC["proc8"]["norm4"] = std::make_pair("proc7_norm4", "trackSip3dSig");//before splitting  
  }
  
  
  makedir = ".! mkdir " + folderOutput + "/" + outInputVarsC;
  gROOT->ProcessLine(makedir);
  ofstream html_inputvarsC;
  html_inputvarsC.open(folderOutput + "/" + outInputVarsC + "/index.html");
  //header for Input vars html
  html_inputvarsC << "<h3> Input Variables </h3><br><br>" << endl;
  html_inputvarsC << "<table border=\"1\" cellpadding=\"5\" cellspacing=\"5\" width=\"100%\">" << endl;
  html_inputvarsC << "<tr>" << endl;
  html_inputvarsC << "<th>CSV input variables</th>" << endl;
  html_inputvarsC << "</tr>" << endl;

  
  
  for(itrNormInputC = mapNormInputC.begin(); itrNormInputC != mapNormInputC.end(); ++itrNormInputC){
    std::map<TString, std::pair<TString, TString> > mapHisto = (*itrNormInputC).second;
    std::map<TString, std::pair<TString, TString> >::const_iterator itrHisto;
    std::cout << (*itrNormInputC).first << std::endl;
     
    for (itrHisto = mapHisto.begin(); itrHisto != mapHisto.end(); ++itrHisto) {
      std::cout << "  " << (*itrHisto).first << " = " << (*itrHisto).second.first << std::endl;
       
      bool success = PlotNormVars((*itrNormInputC).first + "_" + (*itrHisto).first, (*itrHisto).second.first, (*itrHisto).second.second, outInputVarsC, html_inputvarsC);      
       
    }
  }
   
  //tail for Input vars html
  html_inputvarsC<< "</table><br><br><br>" << endl;
   
  
  //*********************************************************************
  //Likelihood B vs DUSG and B vs C
  //*********************************************************************
  {
    std::vector<TString> Vars;
    if (VtxCat == "Reco") {
      Vars.push_back("trackEtaRel");
      Vars.push_back("vertexMass");
      Vars.push_back("vertexNTracks");
      Vars.push_back("vertexEnergyRatio");
      Vars.push_back("trackSip2dSigAboveCharm");
      Vars.push_back("flightDistance2dSig");
      Vars.push_back("trackSip3dSig_1");
      Vars.push_back("trackSip3dSig_2");
      Vars.push_back("trackSip3dSig_3");
      Vars.push_back("trackSip3dSig_rest");
    }
    else if (VtxCat == "Pseudo") {
      Vars.push_back("trackEtaRel");
      Vars.push_back("vertexMass");
      Vars.push_back("vertexNTracks");
      Vars.push_back("vertexEnergyRatio");
      Vars.push_back("trackSip2dSigAboveCharm");
      Vars.push_back("trackSip3dSig_1");
      Vars.push_back("trackSip3dSig_2");
      Vars.push_back("trackSip3dSig_3");
      Vars.push_back("trackSip3dSig_rest");
    }
    else if (VtxCat == "No") {
      Vars.push_back("trackSip3dSig_1");
      Vars.push_back("trackSip3dSig_2");
      Vars.push_back("trackSip3dSig_3");
      Vars.push_back("trackSip3dSig_rest");
    }

    PlotPDFs("proc4", "proc5", Vars, "LkhDUSG");
    PlotPDFs("proc10", "proc9", Vars, "LkhC");
  }
  
  
  

  //*********************************************************************
  //Normalize Likelihoods B vs DUSG and B vs C
  //*********************************************************************
  makedir = ".! mkdir " + folderOutput + "/" + outPreNormLkh;
  gROOT->ProcessLine(makedir);
  ofstream html_PreNormLkh;
  html_PreNormLkh.open(folderOutput + "/" + outPreNormLkh + "/index.html");
  //header for Input vars html
  html_PreNormLkh << "<h3> Input Variables </h3><br><br>" << endl;
  html_PreNormLkh << "<table border=\"1\" cellpadding=\"5\" cellspacing=\"5\" width=\"100%\">" << endl;
  html_PreNormLkh << "<tr>" << endl;
  html_PreNormLkh << "<th>Output of the likelihood (before normalization and bias)</th>" << endl;
  html_PreNormLkh << "</tr>" << endl;

  
  
  PlotNormVars("proc5_norm1", "proc6_norm1", "Lkh_BvsDUSG", outPreNormLkh, html_PreNormLkh);
  PlotNormVars("proc11_norm1", "proc10_norm1", "Lkh_BvsC", outPreNormLkh, html_PreNormLkh);
  html_PreNormLkh << "</table><br><br><br>" << endl;
    
  
  
  //*********************************************************************
  //Bias Likelihood B vs DUSG and B vs C
  //*********************************************************************
  PlotBiasPDFs("proc6", "proc11", "BiasDUSG");
  PlotBiasPDFs("proc12", "proc12", "BiasC");


  
  //Finally copy everything to my web area
  TString copytoweb = ".! scp  -r " +  folderOutputRoot + " server02.fynu.ucl.ac.be:~/public_html/private/CSV/";
  std::cout << copytoweb << std::endl;
  gROOT->ProcessLine(copytoweb);
}


void PlotPDFs(TString procNew, TString procOld, std::vector<TString> vars, TString folderName) {

  std::vector<TString> category;
  category.push_back("PTlt40_Barrel");
  category.push_back("PTlt40_Forward");
  category.push_back("PTlt40_VeryForward");
  category.push_back("PTlt60_Barrel");
  category.push_back("PTlt60_Forward");
  category.push_back("PTlt60_VeryForward");
  category.push_back("PTlt90_Barrel");
  category.push_back("PTlt90_Forward");
  category.push_back("PTlt90_VeryForward");
  category.push_back("PTlt150_Barrel");
  category.push_back("PTlt150_Forward");
  category.push_back("PTlt150_VeryForward");
  category.push_back("PTgt150_Barrel");
  category.push_back("PTgt150_Forward");
  category.push_back("PTgt150_VeryForward");
  
  ofstream html[NCat];
  
  TString makedir = ".! mkdir " + folderOutput+"/"+ folderName;
  gROOT->ProcessLine(makedir);
  for (unsigned int i = 0; i < category.size(); i++) {
    TString mksubdir = ".! mkdir " + folderOutput+"/"+ folderName+"/"+category[i];
    gROOT->ProcessLine(mksubdir);
    html[i].open(folderOutput+"/"+ folderName+"/"+category[i]+"/index.html");
    
    html[i] << "<h3> PDFs. </h3><br><br>" << endl;
    html[i] << "<table border=\"1\" cellpadding=\"5\" cellspacing=\"5\" width=\"100%\">" << endl;
    html[i] << "<tr>" << endl;
    html[i] << "<th>Signal and Background PDFs</th>" << endl;
    html[i] << "</tr>" << endl;

  }


  
  for (unsigned int i = 0; i < vars.size()*NCat; i++) {
  
    TString namevar = vars[i%vars.size()];
    int idxplot = i + 1;
    int idxcategory = int(i/vars.size());
    std::ostringstream os_number;
    os_number << idxplot;
    
    
    TString plotnumber= os_number.str();
    
    std::cout << i << " vars["<<i%vars.size()<<"]="<<namevar << " idxcategory="<< idxcategory << " cat="
              <<category[idxcategory] << ":" << plotnumber << std::endl;

    TH1F* hnew = (TH1F*)fnew->Get(procNew+"_sig"+plotnumber+"_histo");
    //std::cout << "gethisto1" << std::endl;
    TH1F* hold = (TH1F*)fold->Get(procOld+"_sig"+plotnumber+"_histo");
    TH1F* snew = (TH1F*)fnew->Get(procNew+"_sig"+plotnumber+"_spline");
    TH1F* sold = (TH1F*)fold->Get(procOld+"_sig"+plotnumber+"_spline");
  
    TH1F* hnew_bkg = (TH1F*)fnew->Get(procNew+"_bkg"+plotnumber+"_histo");
    //std::cout << "gethisto1" << std::endl;
    TH1F* hold_bkg = (TH1F*)fold->Get(procOld+"_bkg"+plotnumber+"_histo");
    TH1F* snew_bkg = (TH1F*)fnew->Get(procNew+"_bkg"+plotnumber+"_spline");
    TH1F* sold_bkg = (TH1F*)fold->Get(procOld+"_bkg"+plotnumber+"_spline");
  
    //std::cout << "before getxaxis" << std::endl;
  
    TAxis* xnew = (TAxis*)hnew->GetXaxis();
    TAxis* xold = (TAxis*)hold->GetXaxis();
  
    //std::cout << "before warnings" << std::endl;
    if (hnew->GetNbinsX() != hold->GetNbinsX()) {
      std::cout << "WARNING: hnew->GetNbinsX()="<<hnew->GetNbinsX()<<" is different from hold->GetNbinsX()="<<hold->GetNbinsX()<<std::endl;
    }
    if (xnew->GetXmin() != xold->GetXmin()) {
      std::cout << "WARNING: xnew->GetXmin()="<<xnew->GetXmin()<<" is different from xold->GetXmin()="<<xold->GetXmin()<<std::endl;
    }
    if (xnew->GetXmax() != xold->GetXmax()) {
      std::cout << "WARNING: xnew->GetXmax()="<<xnew->GetXmax()<<" is different from xold->GetXmax()="<<xold->GetXmax()<<std::endl;
    }
  
    //std::cout << "after warnings" << std::endl;
    NormalizeIntegralPlusUandO(hnew);
    NormalizeIntegralPlusUandO(hold);
    NormalizeIntegralPlusUandO(snew);
    NormalizeIntegralPlusUandO(sold);
  
    NormalizeIntegralPlusUandO(hnew_bkg);
    NormalizeIntegralPlusUandO(hold_bkg);
    NormalizeIntegralPlusUandO(snew_bkg);
    NormalizeIntegralPlusUandO(sold_bkg);
  
    hnew->SetStats(0);
    hold->SetStats(0);
    snew->SetStats(0);
    sold->SetStats(0);

    hnew_bkg->SetStats(0);
    hold_bkg->SetStats(0);
    snew_bkg->SetStats(0);
    sold_bkg->SetStats(0);


    hnew->SetLineWidth(3);
    hold->SetLineWidth(3);
    snew->SetLineWidth(3);
    sold->SetLineWidth(3);

    hnew_bkg->SetLineWidth(3);
    hold_bkg->SetLineWidth(3);
    snew_bkg->SetLineWidth(3);
    sold_bkg->SetLineWidth(3);


    hnew->SetLineColor(kRed);
    hold->SetLineColor(kGreen);
    snew->SetLineColor(kRed);
    sold->SetLineColor(kGreen);
  
    hnew_bkg->SetLineColor(kRed);
    hold_bkg->SetLineColor(kGreen);
    snew_bkg->SetLineColor(kRed);
    sold_bkg->SetLineColor(kGreen);

    hnew_bkg->SetLineStyle(2);
    hold_bkg->SetLineStyle(2);
    snew_bkg->SetLineStyle(2);
    sold_bkg->SetLineStyle(2);
    snew_bkg->SetLineWidth(1);
    sold_bkg->SetLineWidth(1);
    snew_bkg->SetLineColor(kRed+1);
    sold_bkg->SetLineColor(kGreen+1);

  
    TH1F* histo[4];
    histo[0] = (TH1F*)hnew->Clone();
    histo[1] = (TH1F*)hold->Clone();
    histo[2] = (TH1F*)hnew_bkg->Clone();
    histo[3] = (TH1F*)hold_bkg->Clone();
    
    TH1F* spline[4];
    spline[0] = (TH1F*)snew->Clone();
    spline[1] = (TH1F*)sold->Clone();
    spline[2] = (TH1F*)snew_bkg->Clone();
    spline[3] = (TH1F*)sold_bkg->Clone();

    int histo_max_idx = 0;
    double histo_max = -99.;
    for (unsigned int ihisto = 0; ihisto < 4; ihisto++) {
      if (histo[ihisto]->GetMaximum() > histo_max) {
        histo_max = histo[ihisto]->GetMaximum();
	histo_max_idx = ihisto;
      }
    
    }
    
    int spline_max_idx = 0;
    double spline_max = -99.;
    for (unsigned int ispline = 0; ispline < 4; ispline++) {
      if (spline[ispline]->GetMaximum() > spline_max) {
        spline_max = spline[ispline]->GetMaximum();
	spline_max_idx = ispline;
      }
    
    }

    
    TCanvas c;
    histo[histo_max_idx]->Draw();
    for (int ihisto = 0; ihisto < 4; ihisto++) {
      if (ihisto != histo_max_idx) histo[ihisto]->Draw("sames");
    }
  
    TString outdir = folderOutput+"/"+folderName+"/"+category[idxcategory]+"/";
    TString histo_lin = "pdf_"+namevar+"_"+"_histo";
    TString histo_log = "pdf_"+namevar+"_"+"_histo_log";
    
    gPad->SetLogy(0);
    c.SaveAs(outdir+histo_lin+".gif");
    gPad->SetLogy(1);
    c.SaveAs(outdir+histo_log+".gif");

    spline[spline_max_idx]->Draw();
    for (int ispline = 0; ispline < 4; ispline++) {
      if (ispline != spline_max_idx) spline[ispline]->Draw("sames");
    }

    TString spline_lin = "pdf_"+namevar+"_"+"_spline";
    TString spline_log = "pdf_"+namevar+"_"+"_spline_log";
  
    gPad->SetLogy(0);
    c.SaveAs(outdir+spline_lin+".gif");
    gPad->SetLogy(1);
    c.SaveAs(outdir+spline_log+".gif");
  

    html[idxcategory] << "<tr>" << endl;

    html[idxcategory] << "<td width=\"25%\">" << endl;
    html[idxcategory] << "<br><IMG SRC=\""+histo_lin+".gif"+"\" ><br>" << endl;
    html[idxcategory] << "<center>" << endl;
    html[idxcategory] << " <a href=\""+histo_lin+".gif"+"\">gif</a> "+namevar << endl; 

    html[idxcategory] << "<td width=\"25%\">" << endl;
    html[idxcategory] << "<br><IMG SRC=\""+histo_log+".gif"+"\" ><br>" << endl;
    html[idxcategory] << "<center>" << endl;
    html[idxcategory] << " <a href=\""+histo_log+".gif"+"\">gif</a>" << endl; 



    html[idxcategory] << "<td width=\"25%\">" << endl;
    html[idxcategory] << "<br><IMG SRC=\""+spline_lin+".gif"+"\" ><br>" << endl;
    html[idxcategory] << "<center>" << endl;
    html[idxcategory] << " <a href=\""+spline_lin+".gif"+"\">gif</a>" << endl; 

    html[idxcategory] << "<td width=\"25%\">" << endl;
    html[idxcategory] << "<br><IMG SRC=\""+spline_log+".gif"+"\" ><br>" << endl;
    html[idxcategory] << "<center>" << endl;
    html[idxcategory] << " <a href=\""+spline_log+".gif"+"\">gif</a>" << endl; 


    html[idxcategory] << "</tr>" << endl;
    html[idxcategory] << "</center>" << endl;

  }
  
  //tail of html files
  for (unsigned int i = 0; i < category.size(); i++) {
    
    html[i]<< "</table><br><br><br>" << endl;

  }

   

}



void PlotBiasPDFs(TString procNew, TString procOld, TString folderName) {



  std::vector<TString> category;
  category.push_back("PTlt40_Barrel");
  category.push_back("PTlt40_Forward");
  category.push_back("PTlt40_VeryForward");
  category.push_back("PTlt60_Barrel");
  category.push_back("PTlt60_Forward");
  category.push_back("PTlt60_VeryForward");
  category.push_back("PTlt90_Barrel");
  category.push_back("PTlt90_Forward");
  category.push_back("PTlt90_VeryForward");
  category.push_back("PTlt150_Barrel");
  category.push_back("PTlt150_Forward");
  category.push_back("PTlt150_VeryForward");
  category.push_back("PTgt150_Barrel");
  category.push_back("PTgt150_Forward");
  category.push_back("PTgt150_VeryForward");
  
  ofstream html;
  
  TString makedir = ".! mkdir " + folderOutput+"/"+ folderName;
  gROOT->ProcessLine(makedir);
  html.open(folderOutput+"/"+ folderName+"/index.html");
    
  html << "<h3> PDFs. </h3><br><br>" << endl;
  html << "<table border=\"1\" cellpadding=\"5\" cellspacing=\"5\" width=\"100%\">" << endl;
  html << "<tr>" << endl;
  html << "<th>Signal and Background PDFs</th>" << endl;
  html << "</tr>" << endl;

  


  
  for (unsigned int i = 0; i < NCat; i++) {
  
    int idxplot = i + 1;
    TString namevar = category[i];

    std::ostringstream os_number;
    os_number << idxplot;
    
    
    TString plotnumber= os_number.str();
    
    std::cout << i << " " << namevar << ":" << plotnumber << std::endl;

    TH1F* hnew = (TH1F*)fnew->Get(procNew+"_sig"+plotnumber+"_histo");
    //std::cout << "gethisto1" << std::endl;
    TH1F* hold = (TH1F*)fold->Get(procOld+"_sig"+plotnumber+"_histo");
    TH1F* snew = (TH1F*)fnew->Get(procNew+"_sig"+plotnumber+"_spline");
    TH1F* sold = (TH1F*)fold->Get(procOld+"_sig"+plotnumber+"_spline");
  
    TH1F* hnew_bkg = (TH1F*)fnew->Get(procNew+"_bkg"+plotnumber+"_histo");
    //std::cout << "gethisto1" << std::endl;
    TH1F* hold_bkg = (TH1F*)fold->Get(procOld+"_bkg"+plotnumber+"_histo");
    TH1F* snew_bkg = (TH1F*)fnew->Get(procNew+"_bkg"+plotnumber+"_spline");
    TH1F* sold_bkg = (TH1F*)fold->Get(procOld+"_bkg"+plotnumber+"_spline");
  
    //std::cout << "before getxaxis" << std::endl;
  
    TAxis* xnew = (TAxis*)hnew->GetXaxis();
    TAxis* xold = (TAxis*)hold->GetXaxis();
  
    //std::cout << "before warnings" << std::endl;
    if (hnew->GetNbinsX() != hold->GetNbinsX()) {
      std::cout << "WARNING: hnew->GetNbinsX()="<<hnew->GetNbinsX()<<" is different from hold->GetNbinsX()="<<hold->GetNbinsX()<<std::endl;
    }
    if (xnew->GetXmin() != xold->GetXmin()) {
      std::cout << "WARNING: xnew->GetXmin()="<<xnew->GetXmin()<<" is different from xold->GetXmin()="<<xold->GetXmin()<<std::endl;
    }
    if (xnew->GetXmax() != xold->GetXmax()) {
      std::cout << "WARNING: xnew->GetXmax()="<<xnew->GetXmax()<<" is different from xold->GetXmax()="<<xold->GetXmax()<<std::endl;
    }
  
    //std::cout << "after warnings" << std::endl;
    //NormalizeIntegralPlusUandO(hnew);
    //NormalizeIntegralPlusUandO(hold);
    //NormalizeIntegralPlusUandO(snew);
    //NormalizeIntegralPlusUandO(sold);
  
    //NormalizeIntegralPlusUandO(hnew_bkg);
    //NormalizeIntegralPlusUandO(hold_bkg);
    //NormalizeIntegralPlusUandO(snew_bkg);
    //NormalizeIntegralPlusUandO(sold_bkg);
  
    hnew->SetStats(0);
    hold->SetStats(0);
    snew->SetStats(0);
    sold->SetStats(0);

    hnew_bkg->SetStats(0);
    hold_bkg->SetStats(0);
    snew_bkg->SetStats(0);
    sold_bkg->SetStats(0);


    hnew->SetLineWidth(3);
    hold->SetLineWidth(3);
    snew->SetLineWidth(3);
    sold->SetLineWidth(3);

    hnew_bkg->SetLineWidth(3);
    hold_bkg->SetLineWidth(3);
    snew_bkg->SetLineWidth(3);
    sold_bkg->SetLineWidth(3);


    hnew->SetLineColor(kRed);
    hold->SetLineColor(kGreen);
    snew->SetLineColor(kRed);
    sold->SetLineColor(kGreen);
  
    hnew_bkg->SetLineColor(kRed);
    hold_bkg->SetLineColor(kGreen);
    snew_bkg->SetLineColor(kRed);
    sold_bkg->SetLineColor(kGreen);

    hnew_bkg->SetLineStyle(2);
    hold_bkg->SetLineStyle(2);
    snew_bkg->SetLineStyle(2);
    sold_bkg->SetLineStyle(2);
    snew_bkg->SetLineWidth(1);
    sold_bkg->SetLineWidth(1);
    snew_bkg->SetLineColor(kRed+1);
    sold_bkg->SetLineColor(kGreen+1);

  
    TH1F* histo[4];
    histo[0] = (TH1F*)hnew->Clone();
    histo[1] = (TH1F*)hold->Clone();
    histo[2] = (TH1F*)hnew_bkg->Clone();
    histo[3] = (TH1F*)hold_bkg->Clone();
    
    TH1F* spline[4];
    spline[0] = (TH1F*)snew->Clone();
    spline[1] = (TH1F*)sold->Clone();
    spline[2] = (TH1F*)snew_bkg->Clone();
    spline[3] = (TH1F*)sold_bkg->Clone();

    int histo_max_idx = 0;
    double histo_max = -99.;
    for (unsigned int ihisto = 0; ihisto < 4; ihisto++) {
      if (histo[ihisto]->GetMaximum() > histo_max) {
        histo_max = histo[ihisto]->GetMaximum();
	histo_max_idx = ihisto;
      }
    
    }
    
    int spline_max_idx = 0;
    double spline_max = -99.;
    for (unsigned int ispline = 0; ispline < 4; ispline++) {
      if (spline[ispline]->GetMaximum() > spline_max) {
        spline_max = spline[ispline]->GetMaximum();
	spline_max_idx = ispline;
      }
    
    }

    
    TCanvas c;
    histo[histo_max_idx]->Draw();
    for (int ihisto = 0; ihisto < 4; ihisto++) {
      if (ihisto != histo_max_idx) histo[ihisto]->Draw("sames");
    }
  
    TString outdir = folderOutput+"/"+folderName+"/";
    TString histo_lin = "pdf_"+namevar+"_"+"_histo";
    TString histo_log = "pdf_"+namevar+"_"+"_histo_log";
    
    gPad->SetLogy(0);
    c.SaveAs(outdir+histo_lin+".gif");
    gPad->SetLogy(1);
    c.SaveAs(outdir+histo_log+".gif");

    spline[spline_max_idx]->Draw();
    for (int ispline = 0; ispline < 4; ispline++) {
      if (ispline != spline_max_idx) spline[ispline]->Draw("sames");
    }

    TString spline_lin = "pdf_"+namevar+"_"+"_spline";
    TString spline_log = "pdf_"+namevar+"_"+"_spline_log";
  
    gPad->SetLogy(0);
    c.SaveAs(outdir+spline_lin+".gif");
    gPad->SetLogy(1);
    c.SaveAs(outdir+spline_log+".gif");
  

    html << "<tr>" << endl;

    html << "<td width=\"25%\">" << endl;
    html << "<br><IMG SRC=\""+histo_lin+".gif"+"\" ><br>" << endl;
    html << "<center>" << endl;
    html << " <a href=\""+histo_lin+".gif"+"\">gif</a> "+namevar << endl; 

    html << "<td width=\"25%\">" << endl;
    html << "<br><IMG SRC=\""+histo_log+".gif"+"\" ><br>" << endl;
    html << "<center>" << endl;
    html << " <a href=\""+histo_log+".gif"+"\">gif</a>" << endl; 



    html << "<td width=\"25%\">" << endl;
    html << "<br><IMG SRC=\""+spline_lin+".gif"+"\" ><br>" << endl;
    html << "<center>" << endl;
    html << " <a href=\""+spline_lin+".gif"+"\">gif</a>" << endl; 

    html << "<td width=\"25%\">" << endl;
    html << "<br><IMG SRC=\""+spline_log+".gif"+"\" ><br>" << endl;
    html << "<center>" << endl;
    html << " <a href=\""+spline_log+".gif"+"\">gif</a>" << endl; 


    html << "</tr>" << endl;
    html << "</center>" << endl;

  }
  
  //tail of html file
   html<< "</table><br><br><br>" << endl;


   

}





bool PlotNormVars(TString stringnew, TString stringold, TString namevar, TString outFolder, ofstream& html) {
  bool samehisto = true;
  std::cout << "  stringnew=" << stringnew << " stringold=" << stringold <<std::endl;
  
  TH1F* hnew = (TH1F*)fnew->Get(stringnew+"_histo");
  //std::cout << "gethisto1" << std::endl;
  TH1F* hold = (TH1F*)fold->Get(stringold+"_histo");
  TH1F* snew = (TH1F*)fnew->Get(stringnew+"_spline");
  TH1F* sold = (TH1F*)fold->Get(stringold+"_spline");
  
  //std::cout << "before getxaxis" << std::endl;
  
  TAxis* xnew = (TAxis*)hnew->GetXaxis();
  TAxis* xold = (TAxis*)hold->GetXaxis();
  
  //std::cout << "before warnings" << std::endl;
  if (hnew->GetNbinsX() != hold->GetNbinsX()) {
    samehisto = false;
    std::cout << "WARNING: hnew->GetNbinsX()="<<hnew->GetNbinsX()<<" is different from hold->GetNbinsX()="<<hold->GetNbinsX()<<std::endl;
  }
  if (xnew->GetXmin() != xold->GetXmin()) {
    samehisto = false;
    std::cout << "WARNING: xnew->GetXmin()="<<xnew->GetXmin()<<" is different from xold->GetXmin()="<<xold->GetXmin()<<std::endl;
  }
  if (xnew->GetXmax() != xold->GetXmax()) {
    samehisto = false;
    std::cout << "WARNING: xnew->GetXmax()="<<xnew->GetXmax()<<" is different from xold->GetXmax()="<<xold->GetXmax()<<std::endl;
  }
  
  //std::cout << "after warnings" << std::endl;
  NormalizeIntegralPlusUandO(hnew);
  NormalizeIntegralPlusUandO(hold);
  NormalizeIntegralPlusUandO(snew);
  NormalizeIntegralPlusUandO(sold);
  
  hnew->SetLineWidth(3);
  hold->SetLineWidth(3);
  snew->SetLineWidth(3);
  sold->SetLineWidth(3);

  hnew->SetStats(0);
  hold->SetStats(0);
  snew->SetStats(0);
  sold->SetStats(0);
    
  hnew->SetLineColor(kRed);
  hold->SetLineColor(kGreen);
  snew->SetLineColor(kRed);
  sold->SetLineColor(kGreen);
  
  TCanvas c;
  if (hnew->GetMaximum() > hold->GetMaximum()) {
    hnew->Draw();
    hold->Draw("sames");
  }
  else {
    hold->Draw();
    hnew->Draw("sames");
  }
  
  TString outdir =folderOutput+"/"+outFolder+"/";
  TString histo_lin = "normvar_"+namevar+"_"+stringnew+"_histo";
  TString histo_log = "normvar_"+namevar+"_"+stringnew+"_histo_log";
  TString spline_lin = "normvar_"+namevar+"_"+stringnew+"_spline";
  TString spline_log = "normvar_"+namevar+"_"+stringnew+"_spline_log";
  gPad->SetLogy(0);
  c.SaveAs(outdir+histo_lin+".gif");
  gPad->SetLogy(1);
  c.SaveAs(outdir+histo_log+".gif");

  if (snew->GetMaximum() > sold->GetMaximum()) {
    snew->Draw();
    sold->Draw("sames");
  }
  else {
    sold->Draw();
    snew->Draw("sames");
  }
  gPad->SetLogy(0);
  c.SaveAs(outdir+spline_lin+".gif");
  gPad->SetLogy(1);
  c.SaveAs(outdir+spline_log+".gif");
  
  
  html << "<td width=\"25%\">" << endl;
  html << "<br><IMG SRC=\""+histo_lin+".gif"+"\" ><br>" << endl;
  html << "<center>" << endl;
  html << " <a href=\""+histo_lin+".gif"+"\">gif</a> "+namevar << endl; 

  html << "<td width=\"25%\">" << endl;
  html << "<br><IMG SRC=\""+histo_log+".gif"+"\" ><br>" << endl;
  html << "<center>" << endl;
  html << " <a href=\""+histo_log+".gif"+"\">gif</a>" << endl; 



  html << "<td width=\"25%\">" << endl;
  html << "<br><IMG SRC=\""+spline_lin+".gif"+"\" ><br>" << endl;
  html << "<center>" << endl;
  html << " <a href=\""+spline_lin+".gif"+"\">gif</a>" << endl; 

  html << "<td width=\"25%\">" << endl;
  html << "<br><IMG SRC=\""+spline_log+".gif"+"\" ><br>" << endl;
  html << "<center>" << endl;
  html << " <a href=\""+spline_log+".gif"+"\">gif</a>" << endl; 

  html << "</tr>" << endl;
  html << "</center>" << endl;
 
  return samehisto;

}



void NormalizeIntegralPlusUandO(TH1F* h) {
  int nbins = h->GetNbinsX() + 1;
  double integ = h->Integral(0, nbins);
  if (integ != 0) {
    Double_t sc = 1.0/integ;
    h->Scale(sc);
    
  }
}

// proc10_norm1_histo
// proc10_norm1_spline
// proc11_bkg1_histo
// proc11_bkg1_spline
// proc11_bkg10_histo
// proc11_bkg10_spline
// proc11_bkg11_histo
// proc11_bkg11_spline
// proc11_bkg12_histo
// proc11_bkg12_spline
// proc11_bkg13_histo
// proc11_bkg13_spline
// proc11_bkg14_histo
// proc11_bkg14_spline
// proc11_bkg15_histo
// proc11_bkg15_spline
// proc11_bkg2_histo
// proc11_bkg2_spline
// proc11_bkg3_histo
// proc11_bkg3_spline
// proc11_bkg4_histo
// proc11_bkg4_spline
// proc11_bkg5_histo
// proc11_bkg5_spline
// proc11_bkg6_histo
// proc11_bkg6_spline
// proc11_bkg7_histo
// proc11_bkg7_spline
// proc11_bkg8_histo
// proc11_bkg8_spline
// proc11_bkg9_histo
// proc11_bkg9_spline
// proc11_sig1_histo
// proc11_sig1_spline
// proc11_sig10_histo
// proc11_sig10_spline
// proc11_sig11_histo
// proc11_sig11_spline
// proc11_sig12_histo
// proc11_sig12_spline
// proc11_sig13_histo
// proc11_sig13_spline
// proc11_sig14_histo
// proc11_sig14_spline
// proc11_sig15_histo
// proc11_sig15_spline
// proc11_sig2_histo
// proc11_sig2_spline
// proc11_sig3_histo
// proc11_sig3_spline
// proc11_sig4_histo
// proc11_sig4_spline
// proc11_sig5_histo
// proc11_sig5_spline
// proc11_sig6_histo
// proc11_sig6_spline
// proc11_sig7_histo
// proc11_sig7_spline
// proc11_sig8_histo
// proc11_sig8_spline
// proc11_sig9_histo
// proc11_sig9_spline
// proc12_bkg1_histo
// proc12_bkg1_spline
// proc12_bkg10_histo
// proc12_bkg10_spline
// proc12_bkg11_histo
// proc12_bkg11_spline
// proc12_bkg12_histo
// proc12_bkg12_spline
// proc12_bkg13_histo
// proc12_bkg13_spline
// proc12_bkg14_histo
// proc12_bkg14_spline
// proc12_bkg15_histo
// proc12_bkg15_spline
// proc12_bkg2_histo
// proc12_bkg2_spline
// proc12_bkg3_histo
// proc12_bkg3_spline
// proc12_bkg4_histo
// proc12_bkg4_spline
// proc12_bkg5_histo
// proc12_bkg5_spline
// proc12_bkg6_histo
// proc12_bkg6_spline
// proc12_bkg7_histo
// proc12_bkg7_spline
// proc12_bkg8_histo
// proc12_bkg8_spline
// proc12_bkg9_histo
// proc12_bkg9_spline
// proc12_sig1_histo
// proc12_sig1_spline
// proc12_sig10_histo
// proc12_sig10_spline
// proc12_sig11_histo
// proc12_sig11_spline
// proc12_sig12_histo
// proc12_sig12_spline
// proc12_sig13_histo
// proc12_sig13_spline
// proc12_sig14_histo
// proc12_sig14_spline
// proc12_sig15_histo
// proc12_sig15_spline
// proc12_sig2_histo
// proc12_sig2_spline
// proc12_sig3_histo
// proc12_sig3_spline
// proc12_sig4_histo
// proc12_sig4_spline
// proc12_sig5_histo
// proc12_sig5_spline
// proc12_sig6_histo
// proc12_sig6_spline
// proc12_sig7_histo
// proc12_sig7_spline
// proc12_sig8_histo
// proc12_sig8_spline
// proc12_sig9_histo
// proc12_sig9_spline
// proc3_norm1_histo
// proc3_norm1_spline
// proc3_norm10_histo
// proc3_norm10_spline
// proc3_norm11_histo
// proc3_norm11_spline
// proc3_norm12_histo
// proc3_norm12_spline
// proc3_norm13_histo
// proc3_norm13_spline
// proc3_norm14_histo
// proc3_norm14_spline
// proc3_norm15_histo
// proc3_norm15_spline
// proc3_norm16_histo
// proc3_norm16_spline
// proc3_norm17_histo
// proc3_norm17_spline
// proc3_norm18_histo
// proc3_norm18_spline
// proc3_norm19_histo
// proc3_norm19_spline
// proc3_norm2_histo
// proc3_norm2_spline
// proc3_norm20_histo
// proc3_norm20_spline
// proc3_norm21_histo
// proc3_norm21_spline
// proc3_norm3_histo
// proc3_norm3_spline
// proc3_norm4_histo
// proc3_norm4_spline
// proc3_norm5_histo
// proc3_norm5_spline
// proc3_norm6_histo
// proc3_norm6_spline
// proc3_norm7_histo
// proc3_norm7_spline
// proc3_norm8_histo
// proc3_norm8_spline
// proc3_norm9_histo
// proc3_norm9_spline
// proc5_bkg1_histo
// proc5_bkg1_spline
// proc5_bkg10_histo
// proc5_bkg10_spline
// proc5_bkg100_histo
// proc5_bkg100_spline
// proc5_bkg101_histo
// proc5_bkg101_spline
// proc5_bkg102_histo
// proc5_bkg102_spline
// proc5_bkg103_histo
// proc5_bkg103_spline
// proc5_bkg104_histo
// proc5_bkg104_spline
// proc5_bkg105_histo
// proc5_bkg105_spline
// proc5_bkg106_histo
// proc5_bkg106_spline
// proc5_bkg107_histo
// proc5_bkg107_spline
// proc5_bkg108_histo
// proc5_bkg108_spline
// proc5_bkg109_histo
// proc5_bkg109_spline
// proc5_bkg11_histo
// proc5_bkg11_spline
// proc5_bkg110_histo
// proc5_bkg110_spline
// proc5_bkg111_histo
// proc5_bkg111_spline
// proc5_bkg112_histo
// proc5_bkg112_spline
// proc5_bkg113_histo
// proc5_bkg113_spline
// proc5_bkg114_histo
// proc5_bkg114_spline
// proc5_bkg115_histo
// proc5_bkg115_spline
// proc5_bkg116_histo
// proc5_bkg116_spline
// proc5_bkg117_histo
// proc5_bkg117_spline
// proc5_bkg118_histo
// proc5_bkg118_spline
// proc5_bkg119_histo
// proc5_bkg119_spline
// proc5_bkg12_histo
// proc5_bkg12_spline
// proc5_bkg120_histo
// proc5_bkg120_spline
// proc5_bkg121_histo
// proc5_bkg121_spline
// proc5_bkg122_histo
// proc5_bkg122_spline
// proc5_bkg123_histo
// proc5_bkg123_spline
// proc5_bkg124_histo
// proc5_bkg124_spline
// proc5_bkg125_histo
// proc5_bkg125_spline
// proc5_bkg126_histo
// proc5_bkg126_spline
// proc5_bkg127_histo
// proc5_bkg127_spline
// proc5_bkg128_histo
// proc5_bkg128_spline
// proc5_bkg129_histo
// proc5_bkg129_spline
// proc5_bkg13_histo
// proc5_bkg13_spline
// proc5_bkg130_histo
// proc5_bkg130_spline
// proc5_bkg131_histo
// proc5_bkg131_spline
// proc5_bkg132_histo
// proc5_bkg132_spline
// proc5_bkg133_histo
// proc5_bkg133_spline
// proc5_bkg134_histo
// proc5_bkg134_spline
// proc5_bkg135_histo
// proc5_bkg135_spline
// proc5_bkg136_histo
// proc5_bkg136_spline
// proc5_bkg137_histo
// proc5_bkg137_spline
// proc5_bkg138_histo
// proc5_bkg138_spline
// proc5_bkg139_histo
// proc5_bkg139_spline
// proc5_bkg14_histo
// proc5_bkg14_spline
// proc5_bkg140_histo
// proc5_bkg140_spline
// proc5_bkg141_histo
// proc5_bkg141_spline
// proc5_bkg142_histo
// proc5_bkg142_spline
// proc5_bkg143_histo
// proc5_bkg143_spline
// proc5_bkg144_histo
// proc5_bkg144_spline
// proc5_bkg145_histo
// proc5_bkg145_spline
// proc5_bkg146_histo
// proc5_bkg146_spline
// proc5_bkg147_histo
// proc5_bkg147_spline
// proc5_bkg148_histo
// proc5_bkg148_spline
// proc5_bkg149_histo
// proc5_bkg149_spline
// proc5_bkg15_histo
// proc5_bkg15_spline
// proc5_bkg150_histo
// proc5_bkg150_spline
// proc5_bkg16_histo
// proc5_bkg16_spline
// proc5_bkg17_histo
// proc5_bkg17_spline
// proc5_bkg18_histo
// proc5_bkg18_spline
// proc5_bkg19_histo
// proc5_bkg19_spline
// proc5_bkg2_histo
// proc5_bkg2_spline
// proc5_bkg20_histo
// proc5_bkg20_spline
// proc5_bkg21_histo
// proc5_bkg21_spline
// proc5_bkg22_histo
// proc5_bkg22_spline
// proc5_bkg23_histo
// proc5_bkg23_spline
// proc5_bkg24_histo
// proc5_bkg24_spline
// proc5_bkg25_histo
// proc5_bkg25_spline
// proc5_bkg26_histo
// proc5_bkg26_spline
// proc5_bkg27_histo
// proc5_bkg27_spline
// proc5_bkg28_histo
// proc5_bkg28_spline
// proc5_bkg29_histo
// proc5_bkg29_spline
// proc5_bkg3_histo
// proc5_bkg3_spline
// proc5_bkg30_histo
// proc5_bkg30_spline
// proc5_bkg31_histo
// proc5_bkg31_spline
// proc5_bkg32_histo
// proc5_bkg32_spline
// proc5_bkg33_histo
// proc5_bkg33_spline
// proc5_bkg34_histo
// proc5_bkg34_spline
// proc5_bkg35_histo
// proc5_bkg35_spline
// proc5_bkg36_histo
// proc5_bkg36_spline
// proc5_bkg37_histo
// proc5_bkg37_spline
// proc5_bkg38_histo
// proc5_bkg38_spline
// proc5_bkg39_histo
// proc5_bkg39_spline
// proc5_bkg4_histo
// proc5_bkg4_spline
// proc5_bkg40_histo
// proc5_bkg40_spline
// proc5_bkg41_histo
// proc5_bkg41_spline
// proc5_bkg42_histo
// proc5_bkg42_spline
// proc5_bkg43_histo
// proc5_bkg43_spline
// proc5_bkg44_histo
// proc5_bkg44_spline
// proc5_bkg45_histo
// proc5_bkg45_spline
// proc5_bkg46_histo
// proc5_bkg46_spline
// proc5_bkg47_histo
// proc5_bkg47_spline
// proc5_bkg48_histo
// proc5_bkg48_spline
// proc5_bkg49_histo
// proc5_bkg49_spline
// proc5_bkg5_histo
// proc5_bkg5_spline
// proc5_bkg50_histo
// proc5_bkg50_spline
// proc5_bkg51_histo
// proc5_bkg51_spline
// proc5_bkg52_histo
// proc5_bkg52_spline
// proc5_bkg53_histo
// proc5_bkg53_spline
// proc5_bkg54_histo
// proc5_bkg54_spline
// proc5_bkg55_histo
// proc5_bkg55_spline
// proc5_bkg56_histo
// proc5_bkg56_spline
// proc5_bkg57_histo
// proc5_bkg57_spline
// proc5_bkg58_histo
// proc5_bkg58_spline
// proc5_bkg59_histo
// proc5_bkg59_spline
// proc5_bkg6_histo
// proc5_bkg6_spline
// proc5_bkg60_histo
// proc5_bkg60_spline
// proc5_bkg61_histo
// proc5_bkg61_spline
// proc5_bkg62_histo
// proc5_bkg62_spline
// proc5_bkg63_histo
// proc5_bkg63_spline
// proc5_bkg64_histo
// proc5_bkg64_spline
// proc5_bkg65_histo
// proc5_bkg65_spline
// proc5_bkg66_histo
// proc5_bkg66_spline
// proc5_bkg67_histo
// proc5_bkg67_spline
// proc5_bkg68_histo
// proc5_bkg68_spline
// proc5_bkg69_histo
// proc5_bkg69_spline
// proc5_bkg7_histo
// proc5_bkg7_spline
// proc5_bkg70_histo
// proc5_bkg70_spline
// proc5_bkg71_histo
// proc5_bkg71_spline
// proc5_bkg72_histo
// proc5_bkg72_spline
// proc5_bkg73_histo
// proc5_bkg73_spline
// proc5_bkg74_histo
// proc5_bkg74_spline
// proc5_bkg75_histo
// proc5_bkg75_spline
// proc5_bkg76_histo
// proc5_bkg76_spline
// proc5_bkg77_histo
// proc5_bkg77_spline
// proc5_bkg78_histo
// proc5_bkg78_spline
// proc5_bkg79_histo
// proc5_bkg79_spline
// proc5_bkg8_histo
// proc5_bkg8_spline
// proc5_bkg80_histo
// proc5_bkg80_spline
// proc5_bkg81_histo
// proc5_bkg81_spline
// proc5_bkg82_histo
// proc5_bkg82_spline
// proc5_bkg83_histo
// proc5_bkg83_spline
// proc5_bkg84_histo
// proc5_bkg84_spline
// proc5_bkg85_histo
// proc5_bkg85_spline
// proc5_bkg86_histo
// proc5_bkg86_spline
// proc5_bkg87_histo
// proc5_bkg87_spline
// proc5_bkg88_histo
// proc5_bkg88_spline
// proc5_bkg89_histo
// proc5_bkg89_spline
// proc5_bkg9_histo
// proc5_bkg9_spline
// proc5_bkg90_histo
// proc5_bkg90_spline
// proc5_bkg91_histo
// proc5_bkg91_spline
// proc5_bkg92_histo
// proc5_bkg92_spline
// proc5_bkg93_histo
// proc5_bkg93_spline
// proc5_bkg94_histo
// proc5_bkg94_spline
// proc5_bkg95_histo
// proc5_bkg95_spline
// proc5_bkg96_histo
// proc5_bkg96_spline
// proc5_bkg97_histo
// proc5_bkg97_spline
// proc5_bkg98_histo
// proc5_bkg98_spline
// proc5_bkg99_histo
// proc5_bkg99_spline
// proc5_sig1_histo
// proc5_sig1_spline
// proc5_sig10_histo
// proc5_sig10_spline
// proc5_sig100_histo
// proc5_sig100_spline
// proc5_sig101_histo
// proc5_sig101_spline
// proc5_sig102_histo
// proc5_sig102_spline
// proc5_sig103_histo
// proc5_sig103_spline
// proc5_sig104_histo
// proc5_sig104_spline
// proc5_sig105_histo
// proc5_sig105_spline
// proc5_sig106_histo
// proc5_sig106_spline
// proc5_sig107_histo
// proc5_sig107_spline
// proc5_sig108_histo
// proc5_sig108_spline
// proc5_sig109_histo
// proc5_sig109_spline
// proc5_sig11_histo
// proc5_sig11_spline
// proc5_sig110_histo
// proc5_sig110_spline
// proc5_sig111_histo
// proc5_sig111_spline
// proc5_sig112_histo
// proc5_sig112_spline
// proc5_sig113_histo
// proc5_sig113_spline
// proc5_sig114_histo
// proc5_sig114_spline
// proc5_sig115_histo
// proc5_sig115_spline
// proc5_sig116_histo
// proc5_sig116_spline
// proc5_sig117_histo
// proc5_sig117_spline
// proc5_sig118_histo
// proc5_sig118_spline
// proc5_sig119_histo
// proc5_sig119_spline
// proc5_sig12_histo
// proc5_sig12_spline
// proc5_sig120_histo
// proc5_sig120_spline
// proc5_sig121_histo
// proc5_sig121_spline
// proc5_sig122_histo
// proc5_sig122_spline
// proc5_sig123_histo
// proc5_sig123_spline
// proc5_sig124_histo
// proc5_sig124_spline
// proc5_sig125_histo
// proc5_sig125_spline
// proc5_sig126_histo
// proc5_sig126_spline
// proc5_sig127_histo
// proc5_sig127_spline
// proc5_sig128_histo
// proc5_sig128_spline
// proc5_sig129_histo
// proc5_sig129_spline
// proc5_sig13_histo
// proc5_sig13_spline
// proc5_sig130_histo
// proc5_sig130_spline
// proc5_sig131_histo
// proc5_sig131_spline
// proc5_sig132_histo
// proc5_sig132_spline
// proc5_sig133_histo
// proc5_sig133_spline
// proc5_sig134_histo
// proc5_sig134_spline
// proc5_sig135_histo
// proc5_sig135_spline
// proc5_sig136_histo
// proc5_sig136_spline
// proc5_sig137_histo
// proc5_sig137_spline
// proc5_sig138_histo
// proc5_sig138_spline
// proc5_sig139_histo
// proc5_sig139_spline
// proc5_sig14_histo
// proc5_sig14_spline
// proc5_sig140_histo
// proc5_sig140_spline
// proc5_sig141_histo
// proc5_sig141_spline
// proc5_sig142_histo
// proc5_sig142_spline
// proc5_sig143_histo
// proc5_sig143_spline
// proc5_sig144_histo
// proc5_sig144_spline
// proc5_sig145_histo
// proc5_sig145_spline
// proc5_sig146_histo
// proc5_sig146_spline
// proc5_sig147_histo
// proc5_sig147_spline
// proc5_sig148_histo
// proc5_sig148_spline
// proc5_sig149_histo
// proc5_sig149_spline
// proc5_sig15_histo
// proc5_sig15_spline
// proc5_sig150_histo
// proc5_sig150_spline
// proc5_sig16_histo
// proc5_sig16_spline
// proc5_sig17_histo
// proc5_sig17_spline
// proc5_sig18_histo
// proc5_sig18_spline
// proc5_sig19_histo
// proc5_sig19_spline
// proc5_sig2_histo
// proc5_sig2_spline
// proc5_sig20_histo
// proc5_sig20_spline
// proc5_sig21_histo
// proc5_sig21_spline
// proc5_sig22_histo
// proc5_sig22_spline
// proc5_sig23_histo
// proc5_sig23_spline
// proc5_sig24_histo
// proc5_sig24_spline
// proc5_sig25_histo
// proc5_sig25_spline
// proc5_sig26_histo
// proc5_sig26_spline
// proc5_sig27_histo
// proc5_sig27_spline
// proc5_sig28_histo
// proc5_sig28_spline
// proc5_sig29_histo
// proc5_sig29_spline
// proc5_sig3_histo
// proc5_sig3_spline
// proc5_sig30_histo
// proc5_sig30_spline
// proc5_sig31_histo
// proc5_sig31_spline
// proc5_sig32_histo
// proc5_sig32_spline
// proc5_sig33_histo
// proc5_sig33_spline
// proc5_sig34_histo
// proc5_sig34_spline
// proc5_sig35_histo
// proc5_sig35_spline
// proc5_sig36_histo
// proc5_sig36_spline
// proc5_sig37_histo
// proc5_sig37_spline
// proc5_sig38_histo
// proc5_sig38_spline
// proc5_sig39_histo
// proc5_sig39_spline
// proc5_sig4_histo
// proc5_sig4_spline
// proc5_sig40_histo
// proc5_sig40_spline
// proc5_sig41_histo
// proc5_sig41_spline
// proc5_sig42_histo
// proc5_sig42_spline
// proc5_sig43_histo
// proc5_sig43_spline
// proc5_sig44_histo
// proc5_sig44_spline
// proc5_sig45_histo
// proc5_sig45_spline
// proc5_sig46_histo
// proc5_sig46_spline
// proc5_sig47_histo
// proc5_sig47_spline
// proc5_sig48_histo
// proc5_sig48_spline
// proc5_sig49_histo
// proc5_sig49_spline
// proc5_sig5_histo
// proc5_sig5_spline
// proc5_sig50_histo
// proc5_sig50_spline
// proc5_sig51_histo
// proc5_sig51_spline
// proc5_sig52_histo
// proc5_sig52_spline
// proc5_sig53_histo
// proc5_sig53_spline
// proc5_sig54_histo
// proc5_sig54_spline
// proc5_sig55_histo
// proc5_sig55_spline
// proc5_sig56_histo
// proc5_sig56_spline
// proc5_sig57_histo
// proc5_sig57_spline
// proc5_sig58_histo
// proc5_sig58_spline
// proc5_sig59_histo
// proc5_sig59_spline
// proc5_sig6_histo
// proc5_sig6_spline
// proc5_sig60_histo
// proc5_sig60_spline
// proc5_sig61_histo
// proc5_sig61_spline
// proc5_sig62_histo
// proc5_sig62_spline
// proc5_sig63_histo
// proc5_sig63_spline
// proc5_sig64_histo
// proc5_sig64_spline
// proc5_sig65_histo
// proc5_sig65_spline
// proc5_sig66_histo
// proc5_sig66_spline
// proc5_sig67_histo
// proc5_sig67_spline
// proc5_sig68_histo
// proc5_sig68_spline
// proc5_sig69_histo
// proc5_sig69_spline
// proc5_sig7_histo
// proc5_sig7_spline
// proc5_sig70_histo
// proc5_sig70_spline
// proc5_sig71_histo
// proc5_sig71_spline
// proc5_sig72_histo
// proc5_sig72_spline
// proc5_sig73_histo
// proc5_sig73_spline
// proc5_sig74_histo
// proc5_sig74_spline
// proc5_sig75_histo
// proc5_sig75_spline
// proc5_sig76_histo
// proc5_sig76_spline
// proc5_sig77_histo
// proc5_sig77_spline
// proc5_sig78_histo
// proc5_sig78_spline
// proc5_sig79_histo
// proc5_sig79_spline
// proc5_sig8_histo
// proc5_sig8_spline
// proc5_sig80_histo
// proc5_sig80_spline
// proc5_sig81_histo
// proc5_sig81_spline
// proc5_sig82_histo
// proc5_sig82_spline
// proc5_sig83_histo
// proc5_sig83_spline
// proc5_sig84_histo
// proc5_sig84_spline
// proc5_sig85_histo
// proc5_sig85_spline
// proc5_sig86_histo
// proc5_sig86_spline
// proc5_sig87_histo
// proc5_sig87_spline
// proc5_sig88_histo
// proc5_sig88_spline
// proc5_sig89_histo
// proc5_sig89_spline
// proc5_sig9_histo
// proc5_sig9_spline
// proc5_sig90_histo
// proc5_sig90_spline
// proc5_sig91_histo
// proc5_sig91_spline
// proc5_sig92_histo
// proc5_sig92_spline
// proc5_sig93_histo
// proc5_sig93_spline
// proc5_sig94_histo
// proc5_sig94_spline
// proc5_sig95_histo
// proc5_sig95_spline
// proc5_sig96_histo
// proc5_sig96_spline
// proc5_sig97_histo
// proc5_sig97_spline
// proc5_sig98_histo
// proc5_sig98_spline
// proc5_sig99_histo
// proc5_sig99_spline
// proc6_norm1_histo
// proc6_norm1_spline
// proc7_norm1_histo
// proc7_norm1_spline
// proc7_norm10_histo
// proc7_norm10_spline
// proc7_norm11_histo
// proc7_norm11_spline
// proc7_norm12_histo
// proc7_norm12_spline
// proc7_norm13_histo
// proc7_norm13_spline
// proc7_norm14_histo
// proc7_norm14_spline
// proc7_norm15_histo
// proc7_norm15_spline
// proc7_norm16_histo
// proc7_norm16_spline
// proc7_norm17_histo
// proc7_norm17_spline
// proc7_norm18_histo
// proc7_norm18_spline
// proc7_norm19_histo
// proc7_norm19_spline
// proc7_norm2_histo
// proc7_norm2_spline
// proc7_norm20_histo
// proc7_norm20_spline
// proc7_norm21_histo
// proc7_norm21_spline
// proc7_norm3_histo
// proc7_norm3_spline
// proc7_norm4_histo
// proc7_norm4_spline
// proc7_norm5_histo
// proc7_norm5_spline
// proc7_norm6_histo
// proc7_norm6_spline
// proc7_norm7_histo
// proc7_norm7_spline
// proc7_norm8_histo
// proc7_norm8_spline
// proc7_norm9_histo
// proc7_norm9_spline
// proc9_bkg1_histo
// proc9_bkg1_spline
// proc9_bkg10_histo
// proc9_bkg10_spline
// proc9_bkg100_histo
// proc9_bkg100_spline
// proc9_bkg101_histo
// proc9_bkg101_spline
// proc9_bkg102_histo
// proc9_bkg102_spline
// proc9_bkg103_histo
// proc9_bkg103_spline
// proc9_bkg104_histo
// proc9_bkg104_spline
// proc9_bkg105_histo
// proc9_bkg105_spline
// proc9_bkg106_histo
// proc9_bkg106_spline
// proc9_bkg107_histo
// proc9_bkg107_spline
// proc9_bkg108_histo
// proc9_bkg108_spline
// proc9_bkg109_histo
// proc9_bkg109_spline
// proc9_bkg11_histo
// proc9_bkg11_spline
// proc9_bkg110_histo
// proc9_bkg110_spline
// proc9_bkg111_histo
// proc9_bkg111_spline
// proc9_bkg112_histo
// proc9_bkg112_spline
// proc9_bkg113_histo
// proc9_bkg113_spline
// proc9_bkg114_histo
// proc9_bkg114_spline
// proc9_bkg115_histo
// proc9_bkg115_spline
// proc9_bkg116_histo
// proc9_bkg116_spline
// proc9_bkg117_histo
// proc9_bkg117_spline
// proc9_bkg118_histo
// proc9_bkg118_spline
// proc9_bkg119_histo
// proc9_bkg119_spline
// proc9_bkg12_histo
// proc9_bkg12_spline
// proc9_bkg120_histo
// proc9_bkg120_spline
// proc9_bkg121_histo
// proc9_bkg121_spline
// proc9_bkg122_histo
// proc9_bkg122_spline
// proc9_bkg123_histo
// proc9_bkg123_spline
// proc9_bkg124_histo
// proc9_bkg124_spline
// proc9_bkg125_histo
// proc9_bkg125_spline
// proc9_bkg126_histo
// proc9_bkg126_spline
// proc9_bkg127_histo
// proc9_bkg127_spline
// proc9_bkg128_histo
// proc9_bkg128_spline
// proc9_bkg129_histo
// proc9_bkg129_spline
// proc9_bkg13_histo
// proc9_bkg13_spline
// proc9_bkg130_histo
// proc9_bkg130_spline
// proc9_bkg131_histo
// proc9_bkg131_spline
// proc9_bkg132_histo
// proc9_bkg132_spline
// proc9_bkg133_histo
// proc9_bkg133_spline
// proc9_bkg134_histo
// proc9_bkg134_spline
// proc9_bkg135_histo
// proc9_bkg135_spline
// proc9_bkg136_histo
// proc9_bkg136_spline
// proc9_bkg137_histo
// proc9_bkg137_spline
// proc9_bkg138_histo
// proc9_bkg138_spline
// proc9_bkg139_histo
// proc9_bkg139_spline
// proc9_bkg14_histo
// proc9_bkg14_spline
// proc9_bkg140_histo
// proc9_bkg140_spline
// proc9_bkg141_histo
// proc9_bkg141_spline
// proc9_bkg142_histo
// proc9_bkg142_spline
// proc9_bkg143_histo
// proc9_bkg143_spline
// proc9_bkg144_histo
// proc9_bkg144_spline
// proc9_bkg145_histo
// proc9_bkg145_spline
// proc9_bkg146_histo
// proc9_bkg146_spline
// proc9_bkg147_histo
// proc9_bkg147_spline
// proc9_bkg148_histo
// proc9_bkg148_spline
// proc9_bkg149_histo
// proc9_bkg149_spline
// proc9_bkg15_histo
// proc9_bkg15_spline
// proc9_bkg150_histo
// proc9_bkg150_spline
// proc9_bkg16_histo
// proc9_bkg16_spline
// proc9_bkg17_histo
// proc9_bkg17_spline
// proc9_bkg18_histo
// proc9_bkg18_spline
// proc9_bkg19_histo
// proc9_bkg19_spline
// proc9_bkg2_histo
// proc9_bkg2_spline
// proc9_bkg20_histo
// proc9_bkg20_spline
// proc9_bkg21_histo
// proc9_bkg21_spline
// proc9_bkg22_histo
// proc9_bkg22_spline
// proc9_bkg23_histo
// proc9_bkg23_spline
// proc9_bkg24_histo
// proc9_bkg24_spline
// proc9_bkg25_histo
// proc9_bkg25_spline
// proc9_bkg26_histo
// proc9_bkg26_spline
// proc9_bkg27_histo
// proc9_bkg27_spline
// proc9_bkg28_histo
// proc9_bkg28_spline
// proc9_bkg29_histo
// proc9_bkg29_spline
// proc9_bkg3_histo
// proc9_bkg3_spline
// proc9_bkg30_histo
// proc9_bkg30_spline
// proc9_bkg31_histo
// proc9_bkg31_spline
// proc9_bkg32_histo
// proc9_bkg32_spline
// proc9_bkg33_histo
// proc9_bkg33_spline
// proc9_bkg34_histo
// proc9_bkg34_spline
// proc9_bkg35_histo
// proc9_bkg35_spline
// proc9_bkg36_histo
// proc9_bkg36_spline
// proc9_bkg37_histo
// proc9_bkg37_spline
// proc9_bkg38_histo
// proc9_bkg38_spline
// proc9_bkg39_histo
// proc9_bkg39_spline
// proc9_bkg4_histo
// proc9_bkg4_spline
// proc9_bkg40_histo
// proc9_bkg40_spline
// proc9_bkg41_histo
// proc9_bkg41_spline
// proc9_bkg42_histo
// proc9_bkg42_spline
// proc9_bkg43_histo
// proc9_bkg43_spline
// proc9_bkg44_histo
// proc9_bkg44_spline
// proc9_bkg45_histo
// proc9_bkg45_spline
// proc9_bkg46_histo
// proc9_bkg46_spline
// proc9_bkg47_histo
// proc9_bkg47_spline
// proc9_bkg48_histo
// proc9_bkg48_spline
// proc9_bkg49_histo
// proc9_bkg49_spline
// proc9_bkg5_histo
// proc9_bkg5_spline
// proc9_bkg50_histo
// proc9_bkg50_spline
// proc9_bkg51_histo
// proc9_bkg51_spline
// proc9_bkg52_histo
// proc9_bkg52_spline
// proc9_bkg53_histo
// proc9_bkg53_spline
// proc9_bkg54_histo
// proc9_bkg54_spline
// proc9_bkg55_histo
// proc9_bkg55_spline
// proc9_bkg56_histo
// proc9_bkg56_spline
// proc9_bkg57_histo
// proc9_bkg57_spline
// proc9_bkg58_histo
// proc9_bkg58_spline
// proc9_bkg59_histo
// proc9_bkg59_spline
// proc9_bkg6_histo
// proc9_bkg6_spline
// proc9_bkg60_histo
// proc9_bkg60_spline
// proc9_bkg61_histo
// proc9_bkg61_spline
// proc9_bkg62_histo
// proc9_bkg62_spline
// proc9_bkg63_histo
// proc9_bkg63_spline
// proc9_bkg64_histo
// proc9_bkg64_spline
// proc9_bkg65_histo
// proc9_bkg65_spline
// proc9_bkg66_histo
// proc9_bkg66_spline
// proc9_bkg67_histo
// proc9_bkg67_spline
// proc9_bkg68_histo
// proc9_bkg68_spline
// proc9_bkg69_histo
// proc9_bkg69_spline
// proc9_bkg7_histo
// proc9_bkg7_spline
// proc9_bkg70_histo
// proc9_bkg70_spline
// proc9_bkg71_histo
// proc9_bkg71_spline
// proc9_bkg72_histo
// proc9_bkg72_spline
// proc9_bkg73_histo
// proc9_bkg73_spline
// proc9_bkg74_histo
// proc9_bkg74_spline
// proc9_bkg75_histo
// proc9_bkg75_spline
// proc9_bkg76_histo
// proc9_bkg76_spline
// proc9_bkg77_histo
// proc9_bkg77_spline
// proc9_bkg78_histo
// proc9_bkg78_spline
// proc9_bkg79_histo
// proc9_bkg79_spline
// proc9_bkg8_histo
// proc9_bkg8_spline
// proc9_bkg80_histo
// proc9_bkg80_spline
// proc9_bkg81_histo
// proc9_bkg81_spline
// proc9_bkg82_histo
// proc9_bkg82_spline
// proc9_bkg83_histo
// proc9_bkg83_spline
// proc9_bkg84_histo
// proc9_bkg84_spline
// proc9_bkg85_histo
// proc9_bkg85_spline
// proc9_bkg86_histo
// proc9_bkg86_spline
// proc9_bkg87_histo
// proc9_bkg87_spline
// proc9_bkg88_histo
// proc9_bkg88_spline
// proc9_bkg89_histo
// proc9_bkg89_spline
// proc9_bkg9_histo
// proc9_bkg9_spline
// proc9_bkg90_histo
// proc9_bkg90_spline
// proc9_bkg91_histo
// proc9_bkg91_spline
// proc9_bkg92_histo
// proc9_bkg92_spline
// proc9_bkg93_histo
// proc9_bkg93_spline
// proc9_bkg94_histo
// proc9_bkg94_spline
// proc9_bkg95_histo
// proc9_bkg95_spline
// proc9_bkg96_histo
// proc9_bkg96_spline
// proc9_bkg97_histo
// proc9_bkg97_spline
// proc9_bkg98_histo
// proc9_bkg98_spline
// proc9_bkg99_histo
// proc9_bkg99_spline
// proc9_sig1_histo
// proc9_sig1_spline
// proc9_sig10_histo
// proc9_sig10_spline
// proc9_sig100_histo
// proc9_sig100_spline
// proc9_sig101_histo
// proc9_sig101_spline
// proc9_sig102_histo
// proc9_sig102_spline
// proc9_sig103_histo
// proc9_sig103_spline
// proc9_sig104_histo
// proc9_sig104_spline
// proc9_sig105_histo
// proc9_sig105_spline
// proc9_sig106_histo
// proc9_sig106_spline
// proc9_sig107_histo
// proc9_sig107_spline
// proc9_sig108_histo
// proc9_sig108_spline
// proc9_sig109_histo
// proc9_sig109_spline
// proc9_sig11_histo
// proc9_sig11_spline
// proc9_sig110_histo
// proc9_sig110_spline
// proc9_sig111_histo
// proc9_sig111_spline
// proc9_sig112_histo
// proc9_sig112_spline
// proc9_sig113_histo
// proc9_sig113_spline
// proc9_sig114_histo
// proc9_sig114_spline
// proc9_sig115_histo
// proc9_sig115_spline
// proc9_sig116_histo
// proc9_sig116_spline
// proc9_sig117_histo
// proc9_sig117_spline
// proc9_sig118_histo
// proc9_sig118_spline
// proc9_sig119_histo
// proc9_sig119_spline
// proc9_sig12_histo
// proc9_sig12_spline
// proc9_sig120_histo
// proc9_sig120_spline
// proc9_sig121_histo
// proc9_sig121_spline
// proc9_sig122_histo
// proc9_sig122_spline
// proc9_sig123_histo
// proc9_sig123_spline
// proc9_sig124_histo
// proc9_sig124_spline
// proc9_sig125_histo
// proc9_sig125_spline
// proc9_sig126_histo
// proc9_sig126_spline
// proc9_sig127_histo
// proc9_sig127_spline
// proc9_sig128_histo
// proc9_sig128_spline
// proc9_sig129_histo
// proc9_sig129_spline
// proc9_sig13_histo
// proc9_sig13_spline
// proc9_sig130_histo
// proc9_sig130_spline
// proc9_sig131_histo
// proc9_sig131_spline
// proc9_sig132_histo
// proc9_sig132_spline
// proc9_sig133_histo
// proc9_sig133_spline
// proc9_sig134_histo
// proc9_sig134_spline
// proc9_sig135_histo
// proc9_sig135_spline
// proc9_sig136_histo
// proc9_sig136_spline
// proc9_sig137_histo
// proc9_sig137_spline
// proc9_sig138_histo
// proc9_sig138_spline
// proc9_sig139_histo
// proc9_sig139_spline
// proc9_sig14_histo
// proc9_sig14_spline
// proc9_sig140_histo
// proc9_sig140_spline
// proc9_sig141_histo
// proc9_sig141_spline
// proc9_sig142_histo
// proc9_sig142_spline
// proc9_sig143_histo
// proc9_sig143_spline
// proc9_sig144_histo
// proc9_sig144_spline
// proc9_sig145_histo
// proc9_sig145_spline
// proc9_sig146_histo
// proc9_sig146_spline
// proc9_sig147_histo
// proc9_sig147_spline
// proc9_sig148_histo
// proc9_sig148_spline
// proc9_sig149_histo
// proc9_sig149_spline
// proc9_sig15_histo
// proc9_sig15_spline
// proc9_sig150_histo
// proc9_sig150_spline
// proc9_sig16_histo
// proc9_sig16_spline
// proc9_sig17_histo
// proc9_sig17_spline
// proc9_sig18_histo
// proc9_sig18_spline
// proc9_sig19_histo
// proc9_sig19_spline
// proc9_sig2_histo
// proc9_sig2_spline
// proc9_sig20_histo
// proc9_sig20_spline
// proc9_sig21_histo
// proc9_sig21_spline
// proc9_sig22_histo
// proc9_sig22_spline
// proc9_sig23_histo
// proc9_sig23_spline
// proc9_sig24_histo
// proc9_sig24_spline
// proc9_sig25_histo
// proc9_sig25_spline
// proc9_sig26_histo
// proc9_sig26_spline
// proc9_sig27_histo
// proc9_sig27_spline
// proc9_sig28_histo
// proc9_sig28_spline
// proc9_sig29_histo
// proc9_sig29_spline
// proc9_sig3_histo
// proc9_sig3_spline
// proc9_sig30_histo
// proc9_sig30_spline
// proc9_sig31_histo
// proc9_sig31_spline
// proc9_sig32_histo
// proc9_sig32_spline
// proc9_sig33_histo
// proc9_sig33_spline
// proc9_sig34_histo
// proc9_sig34_spline
// proc9_sig35_histo
// proc9_sig35_spline
// proc9_sig36_histo
// proc9_sig36_spline
// proc9_sig37_histo
// proc9_sig37_spline
// proc9_sig38_histo
// proc9_sig38_spline
// proc9_sig39_histo
// proc9_sig39_spline
// proc9_sig4_histo
// proc9_sig4_spline
// proc9_sig40_histo
// proc9_sig40_spline
// proc9_sig41_histo
// proc9_sig41_spline
// proc9_sig42_histo
// proc9_sig42_spline
// proc9_sig43_histo
// proc9_sig43_spline
// proc9_sig44_histo
// proc9_sig44_spline
// proc9_sig45_histo
// proc9_sig45_spline
// proc9_sig46_histo
// proc9_sig46_spline
// proc9_sig47_histo
// proc9_sig47_spline
// proc9_sig48_histo
// proc9_sig48_spline
// proc9_sig49_histo
// proc9_sig49_spline
// proc9_sig5_histo
// proc9_sig5_spline
// proc9_sig50_histo
// proc9_sig50_spline
// proc9_sig51_histo
// proc9_sig51_spline
// proc9_sig52_histo
// proc9_sig52_spline
// proc9_sig53_histo
// proc9_sig53_spline
// proc9_sig54_histo
// proc9_sig54_spline
// proc9_sig55_histo
// proc9_sig55_spline
// proc9_sig56_histo
// proc9_sig56_spline
// proc9_sig57_histo
// proc9_sig57_spline
// proc9_sig58_histo
// proc9_sig58_spline
// proc9_sig59_histo
// proc9_sig59_spline
// proc9_sig6_histo
// proc9_sig6_spline
// proc9_sig60_histo
// proc9_sig60_spline
// proc9_sig61_histo
// proc9_sig61_spline
// proc9_sig62_histo
// proc9_sig62_spline
// proc9_sig63_histo
// proc9_sig63_spline
// proc9_sig64_histo
// proc9_sig64_spline
// proc9_sig65_histo
// proc9_sig65_spline
// proc9_sig66_histo
// proc9_sig66_spline
// proc9_sig67_histo
// proc9_sig67_spline
// proc9_sig68_histo
// proc9_sig68_spline
// proc9_sig69_histo
// proc9_sig69_spline
// proc9_sig7_histo
// proc9_sig7_spline
// proc9_sig70_histo
// proc9_sig70_spline
// proc9_sig71_histo
// proc9_sig71_spline
// proc9_sig72_histo
// proc9_sig72_spline
// proc9_sig73_histo
// proc9_sig73_spline
// proc9_sig74_histo
// proc9_sig74_spline
// proc9_sig75_histo
// proc9_sig75_spline
// proc9_sig76_histo
// proc9_sig76_spline
// proc9_sig77_histo
// proc9_sig77_spline
// proc9_sig78_histo
// proc9_sig78_spline
// proc9_sig79_histo
// proc9_sig79_spline
// proc9_sig8_histo
// proc9_sig8_spline
// proc9_sig80_histo
// proc9_sig80_spline
// proc9_sig81_histo
// proc9_sig81_spline
// proc9_sig82_histo
// proc9_sig82_spline
// proc9_sig83_histo
// proc9_sig83_spline
// proc9_sig84_histo
// proc9_sig84_spline
// proc9_sig85_histo
// proc9_sig85_spline
// proc9_sig86_histo
// proc9_sig86_spline
// proc9_sig87_histo
// proc9_sig87_spline
// proc9_sig88_histo
// proc9_sig88_spline
// proc9_sig89_histo
// proc9_sig89_spline
// proc9_sig9_histo
// proc9_sig9_spline
// proc9_sig90_histo
// proc9_sig90_spline
// proc9_sig91_histo
// proc9_sig91_spline
// proc9_sig92_histo
// proc9_sig92_spline
// proc9_sig93_histo
// proc9_sig93_spline
// proc9_sig94_histo
// proc9_sig94_spline
// proc9_sig95_histo
// proc9_sig95_spline
// proc9_sig96_histo
// proc9_sig96_spline
// proc9_sig97_histo
// proc9_sig97_spline
// proc9_sig98_histo
// proc9_sig98_spline
// proc9_sig99_histo
// proc9_sig99_spline



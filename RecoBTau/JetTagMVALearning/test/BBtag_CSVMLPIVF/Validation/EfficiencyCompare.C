int EfficiencyCompare(){

  gStyle->SetOptStat(0);
//  gStyle->SetOptTitle(0);
  
TFile* inFile = TFile::Open("/afs/cern.ch/user/t/thaarres/CSVTraining/CMSSW_5_3_14/src/RecoBTau/JetTagMVALearning/test/BBtag_CSVMLPIVF/Validation/DQM_V0001_R000000001__POG__BTAG__CSVV2.root");


TH1F* CSVV2_BvsDUSG = (TH1F*)inFile->Get("DQMData/Run 1/Btag/Run summary/CSVV2_GLOBAL/FlavEffVsBEff_DUSG_discr_CSVV2_GLOBAL");
TH1F* CSVV2_BvsC = (TH1F*)inFile->Get("DQMData/Run 1/Btag/Run summary/CSVV2_GLOBAL/FlavEffVsBEff_C_discr_CSVV2_GLOBAL");

TH1F* CSVIVFV2_BvsDUSG = (TH1F*)inFile->Get("DQMData/Run 1/Btag/Run summary/CSVIVFV2_GLOBAL/FlavEffVsBEff_DUSG_discr_CSVIVFV2_GLOBAL");
TH1F* CSVIVFV2_BvsC = (TH1F*)inFile->Get("DQMData/Run 1/Btag/Run summary/CSVIVFV2_GLOBAL/FlavEffVsBEff_C_discr_CSVIVFV2_GLOBAL");

TH1F* CSVIVFV2NEW_BvsDUSG = (TH1F*)inFile->Get("DQMData/Run 1/Btag/Run summary/CSVIVFV2NEW_GLOBAL/FlavEffVsBEff_DUSG_discr_CSVIVFV2NEW_GLOBAL");
TH1F* CSVIVFV2NEW_BvsC = (TH1F*)inFile->Get("DQMData/Run 1/Btag/Run summary/CSVIVFV2NEW_GLOBAL/FlavEffVsBEff_C_discr_CSVIVFV2NEW_GLOBAL");

TH1F* oldCSV_BvsDUSG = (TH1F*)inFile->Get("DQMData/Run 1/Btag/Run summary/oldCSV_GLOBAL/FlavEffVsBEff_DUSG_discr_oldCSV_GLOBAL");
TH1F* oldCSV_BvsC = (TH1F*)inFile->Get("DQMData/Run 1/Btag/Run summary/oldCSV_GLOBAL/FlavEffVsBEff_C_discr_oldCSV_GLOBAL");

TH1F* oldCSVIVF_BvsDUSG = (TH1F*)inFile->Get("DQMData/Run 1/Btag/Run summary/oldCSVIVF_GLOBAL/FlavEffVsBEff_DUSG_discr_oldCSVIVF_GLOBAL");
TH1F* oldCSVIVF_BvsC = (TH1F*)inFile->Get("DQMData/Run 1/Btag/Run summary/oldCSVIVF_GLOBAL/FlavEffVsBEff_C_discr_oldCSVIVF_GLOBAL");

CSVV2_BvsDUSG->SetMarkerColor(1);
CSVIVFV2_BvsDUSG->SetMarkerColor(2);
CSVIVFV2NEW_BvsDUSG->SetMarkerColor(3);
oldCSV_BvsDUSG->SetMarkerColor(4);
oldCSVIVF_BvsDUSG->SetMarkerColor(5);

CSVV2_BvsC->SetMarkerColor(1);
CSVIVFV2_BvsC->SetMarkerColor(2);
CSVIVFV2NEW_BvsC->SetMarkerColor(3);
oldCSV_BvsC->SetMarkerColor(4);
oldCSVIVF_BvsC->SetMarkerColor(5);

 CSVIVFV2_BvsDUSG->GetXaxis()->SetTitle("B efficiency");   
 CSVIVFV2_BvsDUSG->GetYaxis()->SetTitle("BB efficiency");
 CSVIVFV2_BvsDUSG->SetTitle("B vs BB");

 CSVIVFV2_BvsC->GetXaxis()->SetTitle("B efficiency");   
 CSVIVFV2_BvsC->GetYaxis()->SetTitle("C efficiency");
 CSVIVFV2_BvsC->SetTitle("B vs C");
 
 
TCanvas* c1 = new TCanvas("c1","CSV: B vs. BB",900,700);
c1->cd(1);
//c1->SetLogy();
gPad->SetGridx();
gPad->SetGridy();
//gPad->SetLogy();
CSVIVFV2_BvsDUSG->Draw();
CSVIVFV2NEW_BvsDUSG->Draw("same");
// CSVV2_BvsDUSG->Draw("same");
// oldCSV_BvsDUSG->Draw("same");
// oldCSVIVF_BvsDUSG->Draw("same");

leg = new TLegend(0.1,0.65,0.37,0.9);
leg->AddEntry(CSVIVFV2NEW_BvsDUSG, "CSVIVFV2NEW" , 	 "p");
leg->AddEntry(CSVIVFV2_BvsDUSG, "CSVIVFV2", 	 "p");
// leg->AddEntry(CSVV2_BvsDUSG, "CSVV2" , 	 "p");
// leg->AddEntry(oldCSV_BvsDUSG, "oldCSV" , 	 "p");
// leg->AddEntry(oldCSVIVF_BvsDUSG, "oldCSVIVF" , 	 "p");

leg->SetTextSize(0.04);
leg->Draw();

TCanvas* c2 = new TCanvas("c2","CSV: B vs. C",900,700);
c2->cd(1);
c2->SetLogy();
gPad->SetGridx();
gPad->SetGridy();
CSVIVFV2_BvsC->Draw();
CSVIVFV2NEW_BvsC->Draw("same");
// CSVV2_BvsC->Draw("same");
// oldCSV_BvsC->Draw("same");
// oldCSVIVF_BvsC->Draw("same");

leg2 = new TLegend(0.1,0.65,0.37,0.9);
leg2->AddEntry(CSVIVFV2NEW_BvsC, "CSVIVFV2NEW" , 	 "p");
leg2->AddEntry(CSVIVFV2_BvsC, "CSVIVFV2", 	 "p");
// leg2->AddEntry(CSVV2_BvsC, "CSVV2" , 	 "p");
// leg2->AddEntry(oldCSV_BvsC, "oldCSV" , 	 "p");
// leg2->AddEntry(oldCSVIVF_BvsC, "oldCSVIVF" , 	 "p");

leg2->SetTextSize(0.04);
leg2->Draw();

  
// c1->Print("Efficiency_plots/BvsDUSG_linear.root");
// c2->Print("Efficiency_plots/BvsC_linear.root");
}

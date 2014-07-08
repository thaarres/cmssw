int TagAlgo_Compare(){

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  
TFile* inFile = TFile::Open("/afs/cern.ch/user/t/thaarres/removeMe/CMSSW_5_3_14/src/RecoBTau/JetTagMVALearning/test/CSV_MLP/Validation/DQM_V0001_R000000001__POG__BTAG__CSVV2.root");


TH1F* JBP_BvsDUSG = (TH1F*)inFile->Get("DQMData/Run 1/Btag/Run summary/JBP_GLOBAL/FlavEffVsBEff_DUSG_discr_JBP_GLOBAL");
TH1F* JBP_BvsC = (TH1F*)inFile->Get("DQMData/Run 1/Btag/Run summary/JBP_GLOBAL/FlavEffVsBEff_C_discr_JBP_GLOBAL");

TH1F* JP_BvsDUSG = (TH1F*)inFile->Get("DQMData/Run 1/Btag/Run summary/JP_GLOBAL/FlavEffVsBEff_DUSG_discr_JP_GLOBAL");
TH1F* JP_BvsC = (TH1F*)inFile->Get("DQMData/Run 1/Btag/Run summary/JP_GLOBAL/FlavEffVsBEff_C_discr_JP_GLOBAL");

TH1F* CSVIVFV2NEW_BvsDUSG = (TH1F*)inFile->Get("DQMData/Run 1/Btag/Run summary/CSVIVFV2NEW_GLOBAL/FlavEffVsBEff_DUSG_discr_CSVIVFV2NEW_GLOBAL");
TH1F* CSVIVFV2NEW_BvsC = (TH1F*)inFile->Get("DQMData/Run 1/Btag/Run summary/CSVIVFV2NEW_GLOBAL/FlavEffVsBEff_C_discr_CSVIVFV2NEW_GLOBAL");

TH1F* SSVHP_BvsDUSG = (TH1F*)inFile->Get("DQMData/Run 1/Btag/Run summary/SSVHP_GLOBAL/FlavEffVsBEff_DUSG_discr_SSVHP_GLOBAL");
TH1F* SSVHP_BvsC = (TH1F*)inFile->Get("DQMData/Run 1/Btag/Run summary/SSVHP_GLOBAL/FlavEffVsBEff_C_discr_SSVHP_GLOBAL");

TH1F* SSVHE_BvsDUSG = (TH1F*)inFile->Get("DQMData/Run 1/Btag/Run summary/SSVHE_GLOBAL/FlavEffVsBEff_DUSG_discr_SSVHE_GLOBAL");
TH1F* SSVHE_BvsC = (TH1F*)inFile->Get("DQMData/Run 1/Btag/Run summary/SSVHE_GLOBAL/FlavEffVsBEff_C_discr_SSVHE_GLOBAL");

TH1F* TCHE_BvsDUSG = (TH1F*)inFile->Get("DQMData/Run 1/Btag/Run summary/TCHE_GLOBAL/FlavEffVsBEff_DUSG_discr_TCHE_GLOBAL");
TH1F* TCHE_BvsC = (TH1F*)inFile->Get("DQMData/Run 1/Btag/Run summary/TCHE_GLOBAL/FlavEffVsBEff_C_discr_TCHE_GLOBAL");

TH1F* TCHP_BvsDUSG = (TH1F*)inFile->Get("DQMData/Run 1/Btag/Run summary/TCHP_GLOBAL/FlavEffVsBEff_DUSG_discr_TCHP_GLOBAL");
TH1F* TCHP_BvsC = (TH1F*)inFile->Get("DQMData/Run 1/Btag/Run summary/TCHP_GLOBAL/FlavEffVsBEff_C_discr_TCHP_GLOBAL");

JBP_BvsDUSG->SetMarkerColor(1);
JP_BvsDUSG->SetMarkerColor(2);
CSVIVFV2NEW_BvsDUSG->SetMarkerColor(3);
SSVHP_BvsDUSG->SetMarkerColor(4);
SSVHE_BvsDUSG->SetMarkerColor(5);
TCHP_BvsDUSG->SetMarkerColor(6);
TCHE_BvsDUSG->SetMarkerColor(7);

JBP_BvsC->SetMarkerColor(1);
JP_BvsC->SetMarkerColor(2);
CSVIVFV2NEW_BvsC->SetMarkerColor(3);
SSVHP_BvsC->SetMarkerColor(4);
SSVHE_BvsC->SetMarkerColor(5);
TCHP_BvsC->SetMarkerColor(6);
TCHE_BvsC->SetMarkerColor(7);

 JBP_BvsDUSG->GetXaxis()->SetTitle("B efficiency");   
 JBP_BvsDUSG->GetYaxis()->SetTitle("DUSG efficiency");
 JBP_BvsDUSG->SetTitle("CSV");

 JBP_BvsC->GetXaxis()->SetTitle("B efficiency");   
 JBP_BvsC->GetYaxis()->SetTitle("C efficiency");
 JBP_BvsC->SetTitle("CSV");
 
 
TCanvas* c1 = new TCanvas("c1","CSV: B vs. UDSG",900,700);
c1->cd(1);
c1->SetLogy();
gPad->SetGridx();
gPad->SetGridy();
//gPad->SetLogy();
JBP_BvsDUSG->Draw();
JP_BvsDUSG->Draw("same");
CSVIVFV2NEW_BvsDUSG->Draw("same");
SSVHP_BvsDUSG->Draw("same");
SSVHE_BvsDUSG->Draw("same");
TCHP_BvsDUSG->Draw("same");
TCHE_BvsDUSG->Draw("same");

leg = new TLegend(0.1,0.65,0.37,0.9);
leg->AddEntry(CSVIVFV2NEW_BvsDUSG, "CSVIVFV2NEW" , 	 "p");
leg->AddEntry(JP_BvsDUSG, "JP", 	 "p");
leg->AddEntry(JBP_BvsDUSG, "JBP" , 	 "p");
leg->AddEntry(SSVHP_BvsDUSG, "SSVHP" , 	 "p");
leg->AddEntry(SSVHE_BvsDUSG, "SSVHE" , 	 "p");
leg->AddEntry(TCHP_BvsDUSG, "TCHP" , 	 "p");
leg->AddEntry(TCHE_BvsDUSG, "TCHE" , 	 "p");

leg->SetTextSize(0.04);
leg->Draw();

TCanvas* c2 = new TCanvas("c2","CSV: B vs. C",900,700);
c2->cd(1);
c2->SetLogy();
gPad->SetGridx();
gPad->SetGridy();
JBP_BvsC->Draw();
JP_BvsC->Draw("same");
CSVIVFV2NEW_BvsC->Draw("same");
SSVHP_BvsC->Draw("same");
SSVHE_BvsC->Draw("same");
TCHP_BvsC->Draw("same");
TCHE_BvsC->Draw("same");

leg2 = new TLegend(0.1,0.65,0.37,0.9);
leg2->AddEntry(CSVIVFV2NEW_BvsC, "CSVIVFV2NEW" , 	 "p");
leg2->AddEntry(JP_BvsC, "JP", 	 "p");
leg2->AddEntry(JBP_BvsC, "JBP" , 	 "p");
leg2->AddEntry(SSVHP_BvsC, "SSVHP" , 	 "p");
leg2->AddEntry(SSVHE_BvsC, "SSVHE" , 	 "p");
leg2->AddEntry(TCHP_BvsC, "TCHP" , 	 "p");
leg2->AddEntry(TCHE_BvsC, "TCHE" , 	 "p");

leg2->SetTextSize(0.04);
leg2->Draw();

  
c1->Print("Efficiency_plots/BvsDUSG_CompareTagAlgos_020714.root");
c2->Print("Efficiency_plots/BvsC_CompareTagAlgos_020714.root");
c1->Print("Efficiency_plots/BvsDUSG_CompareTagAlgos_020714.pdf");
c2->Print("Efficiency_plots/BvsC_CompareTagAlgos_020714.pdf");
}

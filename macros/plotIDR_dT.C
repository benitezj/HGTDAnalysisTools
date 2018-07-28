#include "HGTDAnalysisTools/macros/globals.h"

/////////////////////////////////////////////////////////////////////////
void plotIDR_dT(){

  SetAtlasStyle();
  gStyle->SetPalette(1,0);

  TString sample = "mc15_13TeV.301399.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_ZZ4nu.evgen.EVNT.e4397";


  TString PROD1="LArHitDec13IDR/LArHit_July26_Preshower_mu0";
  loadChain(INDIR+"/"+PROD1+"/"+sample);
  if(TREE->GetEntries()<=0) return;
  TTree* TREE1=TREE;
  
  TString PROD2="LArHitDec13IDR_CrabKissing/LArHit_July26_Preshower_mu0";
  loadChain(INDIR+"/"+PROD2+"/"+sample);
  if(TREE->GetEntries()<=0) return;
  TTree* TREE2=TREE;


  ///////////////////////////////////////////
  /// dT distributions
  //////////////////////////////////////////
  int nbins=200;
  float min=-1000;
  float max=1000;
  
  TH1F HNom("HNom","",nbins,min,max);
  TREE1->Draw("cell_dt*1000>>HNom");
  HNom.Scale(1./TREE1->GetEntries());
  /////HNom.Scale(1./HNom.Integral());
  
  // TH1F HNomTrue("HNomTrue","",nbins,min,max);
  // TREE1->Draw("cell_dttrue*1000>>HNomTrue");
  // HNomTrue.Scale(1./HNomTrue.Integral());
  /////HNomTrue.Scale(1./TREE1->GetEntries());
  // HNomTrue.SetLineStyle(2);
  
  TH1F HCK("HCK","",nbins,min,max);
  TREE2->Draw("cell_dt*1000>>HCK");
  HCK.Scale(1./TREE2->GetEntries());
  /////HCK.Scale(1./HCK.Integral());
  HCK.SetLineColor(4);
  HCK.SetMarkerColor(4);
  HCK.SetMarkerSize(0.5);

  // TH1F HCKTrue("HCKTrue","",nbins,min,max);
  // TREE2->Draw("cell_dttrue*1000>>HCKTrue");
  //////HCKTrue.Scale(1./HCKTrue.Integral());
  // HCKTrue.Scale(1./TREE2->GetEntries());
  // HCKTrue.SetLineStyle(2);
  // HCKTrue.SetLineColor(4);
  // HCKTrue.SetMarkerColor(4);
  // HCKTrue.SetMarkerSize(0.5);


  TLegend Legend(0.68,0.7,0.92,0.90);
  Legend.SetFillColor(0);
  Legend.SetFillStyle(0);
  Legend.SetBorderSize(0);
  Legend.SetTextSize(.033);
  //Legend.AddEntry(&HCKTrue,"#splitline{CrabKissing}{true vtx.}","p");
  Legend.AddEntry(&HCK,"#splitline{CrabKissing}{#sigma_{T}=29ps,#sigma_{z}=52mm}","l");
  //Legend.AddEntry(&HNomTrue,"#splitline{Nominal}{#sigma_{T}=175ps,true vtx.}","p");
  Legend.AddEntry(&HNom,"#splitline{Nominal}{#sigma_{T}=175ps,#sigma_{z}=45mm}","l");


  TCanvas C;
  C.Clear();
  //HNom.GetYaxis()->SetRangeUser(0,0.025);
  //HNom.GetYaxis()->SetRangeUser(0,2500);
  HNom.GetYaxis()->SetRangeUser(0,1.3*(HNom.GetMaximum()>HCK.GetMaximum()?HNom.GetMaximum():HCK.GetMaximum()));  
  HNom.GetXaxis()->SetTitle("t - t_{exp}    [ps]");
  HNom.GetYaxis()->SetTitle("# of cells per event / 10 ps");

  HNom.Draw("hist");
  HCK.Draw("histsame");

  // HNomTrue.Draw("histpsame");
  // HCKTrue.Draw("histpsame");


  Legend.Draw();
  DrawATLAS();
  DrawHGTD(PROD1,0,0,0.21,0.8);
  DrawECMSMu(PROD1);
  C.Print(OUTDIR+"/CellDeltaT_Nominal_CK.png");

  

  ///////////////////////////////////////////
  /// T distributions
  //////////////////////////////////////////
  int nbinsT=200;
  float minT=11;
  float maxT=13;

  TH1F HNomT("HNomT","",nbinsT,minT,maxT);
  TREE1->Draw("cell_t>>HNomT");
  ////HNomT.Scale(1./HNomT.Integral());
  HNomT.Scale(1./TREE1->GetEntries());
  
  // TH1F HNomTTrue("HNomTTrue","",nbinsT,minT,maxT);
  // TREE1->Draw("cell_t>>HNomTTrue");
  /////HNomTTrue.Scale(1./HNomTTrue.Integral());
  // HNomTTrue.Scale(1./TREE1->GetEntries());
  // HNomTTrue.SetLineStyle(2);
  
  TH1F HCKT("HCKT","",nbinsT,minT,maxT);
  TREE2->Draw("cell_t>>HCKT");
  ////HCKT.Scale(1./HCKT.Integral());
  HCKT.Scale(1./TREE2->GetEntries());
  HCKT.SetLineColor(4);
  HCKT.SetMarkerColor(4);
  HCKT.SetMarkerSize(0.5);

  // TH1F HCKTTrue("HCKTTrue","",nbinsT,minT,maxT);
  // TREE2->Draw("cell_t>>HCKTTrue");
  //// HCKTTrue.Scale(1./HCKTTrue.Integral());
  //  HCKTTrue.Scale(1./TREE2->GetEntries());
  // HCKTTrue.SetLineStyle(2);
  // HCKTTrue.SetLineColor(4);
  // HCKTTrue.SetMarkerColor(4);
  // HCKTTrue.SetMarkerSize(0.5);


  // TLegend LegendT(0.68,0.7,0.92,0.90);
  // LegendT.SetFillColor(0);
  // LegendT.SetFillStyle(0);
  // LegendT.SetBorderSize(0);
  // LegendT.SetTextSize(.033);
  // //LegendT.AddEntry(&HCKTTrue,"#splitline{CrabKissing}{true vtx.}","p");
  // LegendT.AddEntry(&HCKT,"#splitline{CrabKissing}{#sigma_{T}=29ps,#sigma_{z}=52mm}","l");
  // //LegendT.AddEntry(&HNomTTrue,"#splitline{Nominal}{#sigma_{T}=175ps,true vtx.}","p");
  // LegendT.AddEntry(&HNomT,"#splitline{Nominal}{#sigma_{T}=175ps,#sigma_{z}=45mm}","l");

  C.Clear();
  //HNomT.GetYaxis()->SetRangeUser(0,0.025);
  //HNomT.GetYaxis()->SetRangeUser(0,2500);
  HNomT.GetYaxis()->SetRangeUser(0,1.3*(HNomT.GetMaximum()>HCKT.GetMaximum()?HNomT.GetMaximum():HCKT.GetMaximum()));  
  HNomT.GetXaxis()->SetTitle("t  [ns]");
  HNomT.GetYaxis()->SetTitle("# of cells per event / 10 ps");

  HNomT.Draw("hist");
  HCKT.Draw("histsame");

  // HNomTTrue.Draw("histpsame");
  // HCKTTrue.Draw("histpsame");

  Legend.Draw();
  DrawATLAS();
  DrawHGTD(PROD1,0,0,0.21,0.8);
  DrawECMSMu(PROD1);
  C.Print(OUTDIR+"/CellT_Nominal_CK.png");

  
  gROOT->ProcessLine(".q");
}


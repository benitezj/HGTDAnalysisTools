#include "HGTDAnalysisTools/macros/globals.h"
#include "HGTDAnalysisTools/macros/plottingTools.C"

void compareCellEnergy(){

  SetAtlasStyle();
  gStyle->SetPalette(1,0);

  TString sample = "mc15_13TeV.301399.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_ZZ4nu.evgen.EVNT.e4397";
  
  TString DirMu0 = "LArHitDec13IDR/LArHit_July26_mu0";
  TString DirMu200 = "LArHitDec13IDR/LArHit_July26_mu200";

  TTree* tree0=NULL;
  loadChain(INDIR+"/"+DirMu0+"/"+sample);
  if(TREE->GetEntries()<=0) return;
  tree0=TREE;

  TTree* tree200=NULL;
  loadChain(INDIR+"/"+DirMu200+"/"+sample);
  if(TREE->GetEntries()<=0) return;
  tree200=TREE;


  TH1F HEMu0("HEMu0","",100,0,1);
  HEMu0.Sumw2();
  HEMu0.GetYaxis()->SetTitle("# of cells (unit norm)");
  HEMu0.GetXaxis()->SetTitle("cell energy [MeV]");

  TH1F* HEMu200=(TH1F*)HEMu0.Clone("HEMu200");  

  tree0->Draw("cell_E>>HEMu0","cell_jet>=0");//signal hits
  tree200->Draw("cell_E>>HEMu200","cell_jet>=0&&jet_truthJ[cell_jet]>=0");//pile-up hits

  HEMu0.Scale(1./HEMu0.Integral());
  HEMu200->Scale(1./HEMu200->Integral());

  // TCanvas C;
  // C.Clear(); 
  // HEMu0.Draw("histp");
  // HEMu200->Draw("histlsame");
  // C.Print("compareCellEnergy.png");

  HEMu0.SetTitle("mu=0 h.s. jet");
  HEMu200->SetTitle("mu=200 h.s. jet");
  compareWithRatio(HEMu200,&HEMu0,"~/www/HGTDStudies/compareCellEnergy",0.5,0,0,0.0001,"p");


  gROOT->ProcessLine(".q");

} 

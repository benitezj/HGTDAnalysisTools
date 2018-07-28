#include "HGTDAnalysisTools/macros/globals.h"

void plotHGTDTimeComparison(){

  //Compares the time distributions before and after the collision time smearing.

  gROOT->ProcessLine(".x ./HGTDAnalysisTools/macros/rootlogon.C");


  // TFile FSmeared("/data/det-hgtd/tuple/June17/SiHit_April12_June14_mu200_3CELLSIZE_60CELLENERGY_10TIMESMEAR/mc15_13TeV.341080.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_WWlvlv_EF_15_5.evgen.EVNT.e4397/tuple_0.root");
  // TFile FNoSmeared("/data/det-hgtd/tuple/June9/SiHit_April12_mu200_3CELLSIZE_60CELLENERGY_10TIMESMEAR/mc15_13TeV.341080.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_WWlvlv_EF_15_5.evgen.EVNT.e4397/tuple_0.root");
  // TTree* TSmeared = (TTree*)FSmeared.Get("tuple");
  // TTree* TNoSmeared = (TTree*)FNoSmeared.Get("tuple");
  // if(!TSmeared || !TNoSmeared) return;


  TChain* TSmeared = new TChain("tuple");
  loadChain("/data/det-hgtd/tuple/June17/SiHit_April12_June14_mu200_3CELLSIZE_60CELLENERGY_10TIMESMEAR/mc15_13TeV.341080.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_WWlvlv_EF_15_5.evgen.EVNT.e4397",TSmeared);
  TChain* TNoSmeared = new TChain("tuple");
  loadChain("/data/det-hgtd/tuple/June9/SiHit_April12_mu200_3CELLSIZE_60CELLENERGY_10TIMESMEAR/mc15_13TeV.341080.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_WWlvlv_EF_15_5.evgen.EVNT.e4397",TNoSmeared);

  if(TSmeared->GetEntries() == 0 || TNoSmeared->GetEntries() == 0 ) return;
  

  TCanvas C;  

  TH1F HDTSmeared("HDTSmeared","",100,-1000,1000);
  TH1F HDT("HDT","",100,-1000,1000);
  TSmeared->Draw("cell_dt*1000>>HDTSmeared");
  TNoSmeared->Draw("cell_dt*1000>>HDT","","lsame");
  C.Clear();
  HDT.GetXaxis()->SetTitle(" Cell #DeltaT    [ps] ");
  HDT.Draw();
  HDTSmeared.Draw("pesame");
  C.Print("/afs/cern.ch/user/b/benitezj/www/HGTDStudies/HGTDTimeComparison_DT.gif");


  TH1F HRawTSmeared("HRawTSmeared","",100,10,15);
  TH1F HRawT("HRawT","",100,10,15);
  TSmeared->Draw("cell_t>>HRawTSmeared");
  TNoSmeared->Draw("cell_t>>HRawT","","lsame");
  C.Clear();
  HRawT.GetXaxis()->SetTitle(" Cell Time   [ns] ");
  HRawT.Draw();
  HRawTSmeared.Draw("pesame");
  C.Print("/afs/cern.ch/user/b/benitezj/www/HGTDStudies/HGTDTimeComparison_RawT.gif");


  gROOT->ProcessLine(".q");
}

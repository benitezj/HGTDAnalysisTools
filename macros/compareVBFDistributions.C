#include "HGTDAnalysisTools/macros/globals.h"

/////////////////////////////////////////////////////////////////////////
void compareVBFDistributions(TString PROD="June29/SiHit_June15_mu200_3CELLSIZE_60CELLENERGY_10TIMESMEAR"){
  gROOT->ProcessLine(".x ./HGTDAnalysisTools/macros/rootlogon.C");  

  TString sample1 = "mc15_valid.361034.Pythia8EvtGen_A2MSTW2008LO_minbias_inelastic_low.evgen.EVNT.e4800_Last20Files";
  TString sample2 = "mc15_13TeV.341080.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_WWlvlv_EF_15_5.evgen.EVNT.e3871";
  TString sample3 = "mc15_13TeV.305174.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH500_ZZ4nu.evgen.EVNT.e5019";


  /////some global tools
  line.SetLineColor(2);
  line.SetLineWidth(1);
  text.SetTextSize(0.03);
  text.SetTextColor(4);

  TLegend Leg;
  Leg.SetFillStyle (0);
  Leg.SetFillColor (0);
  Leg.SetBorderSize(0);
  Leg.SetTextSize(.04);
  Leg.SetX1NDC(0.6);
  Leg.SetY1NDC(0.6);
  Leg.SetX2NDC(0.85);
  Leg.SetY2NDC(0.87);

  ////
  TTree*TMinB;
  loadChain(INDIR+"/"+PROD+"/"+sample1);
  if(TREE->GetEntries()<=0) return;
  TMinB=TREE;

  TTree*TVBF125;
  loadChain(INDIR+"/"+PROD+"/"+sample2);
  if(TREE->GetEntries()<=0) return;
  TVBF125=TREE;

  TTree*TVBF500;
  loadChain(INDIR+"/"+PROD+"/"+sample3);
  if(TREE->GetEntries()<=0) return;
  TVBF500=TREE;



  //clean the output path
  TString outpath=OUTDIR+"/"+PROD+"_VBFDistributions";
  system((TString("rm -rf ")+outpath).Data());
  system((TString("mkdir -p ")+outpath).Data());


  TCanvas C;



  /////pT of leading jet
  C.Clear();
  TH1F HpTVBF125("HpTVBF125","",30,0,150);
  TVBF125->Draw("jet_pt[vbf_j1]/1000>>HpTVBF125","vbf_mjj>0");
  HpTVBF125.Scale(1./HpTVBF125.Integral());
  TH1F HpTVBF500("HpTVBF500","",30,0,150);
  TVBF500->Draw("jet_pt[vbf_j1]/1000>>HpTVBF500","vbf_mjj>0");
  HpTVBF500.Scale(1./HpTVBF500.Integral());
  TH1F HpTMinB("HpTMinB","",30,0,150);
  TMinB->Draw("jet_pt[vbf_j1]/1000>>HpTMinB","vbf_mjj>0");
  HpTMinB.Scale(1./HpTMinB.Integral());

  C.Clear();  
  HpTVBF125.GetXaxis()->SetTitle("pT(leading jet) [GeV]");
  HpTVBF125.GetYaxis()->SetRangeUser(0,HpTMinB.GetMaximum()*1.2);
  HpTVBF125.Draw("hist");
  HpTVBF500.SetLineColor(4);
  HpTVBF500.Draw("histsame");
  HpTMinB.Draw("histsamep");
  Leg.AddEntry(&HpTMinB,"Min-bias","p");
  Leg.AddEntry(&HpTVBF125,"VBF125","l");
  Leg.AddEntry(&HpTVBF500,"VBF500","l");
  Leg.Draw();
  C.Print(outpath+"/pTleadjet.png");


  /////pT of sub-leading jet
  C.Clear();
  TH1F HSubLeadpTVBF125("HSubLeadpTVBF125","",30,0,150);
  TVBF125->Draw("jet_pt[vbf_j2]/1000>>HSubLeadpTVBF125","vbf_mjj>0");
  HSubLeadpTVBF125.Scale(1./HSubLeadpTVBF125.Integral());
  TH1F HSubLeadpTVBF500("HSubLeadpTVBF500","",30,0,150);
  TVBF500->Draw("jet_pt[vbf_j2]/1000>>HSubLeadpTVBF500","vbf_mjj>0");
  HSubLeadpTVBF500.Scale(1./HSubLeadpTVBF500.Integral());
  TH1F HSubLeadpTMinB("HSubLeadpTMinB","",30,0,150);
  TMinB->Draw("jet_pt[vbf_j2]/1000>>HSubLeadpTMinB","vbf_mjj>0");
  HSubLeadpTMinB.Scale(1./HSubLeadpTMinB.Integral());

  C.Clear();  
  HSubLeadpTVBF125.GetXaxis()->SetTitle("pT(subleading jet) [GeV]");
  HSubLeadpTVBF125.GetYaxis()->SetRangeUser(0,HSubLeadpTMinB.GetMaximum()*1.2);
  HSubLeadpTVBF125.Draw("hist");
  HSubLeadpTVBF500.SetLineColor(4);
  HSubLeadpTVBF500.Draw("histsame");
  HSubLeadpTMinB.Draw("histsamep");
  Leg.Draw();
  C.Print(outpath+"/pTsubleadjet.png");



  ////Mjj
  C.Clear();
  TH1F HMjjVBF125("HMjjVBF125","",50,0,5000);
  TVBF125->Draw("vbf_mjj/1000>>HMjjVBF125","vbf_mjj>0");
  HMjjVBF125.Scale(1./HMjjVBF125.Integral());
  TH1F HMjjVBF500("HMjjVBF500","",50,0,5000);
  TVBF500->Draw("vbf_mjj/1000>>HMjjVBF500","vbf_mjj>0");
  HMjjVBF500.Scale(1./HMjjVBF500.Integral());
  TH1F HMjjMinB("HMjjMinB","",50,0,5000);
  TMinB->Draw("vbf_mjj/1000>>HMjjMinB","vbf_mjj>0");
  HMjjMinB.Scale(1./HMjjMinB.Integral());

  C.Clear();  
  HMjjVBF125.GetXaxis()->SetTitle("m(jj)   [GeV]");
  HMjjVBF125.GetYaxis()->SetRangeUser(0,HMjjMinB.GetMaximum()*1.2);
  HMjjVBF125.Draw("hist");
  HMjjVBF500.SetLineColor(4);
  HMjjVBF500.Draw("histsame");
  HMjjMinB.Draw("histsamep");
  Leg.Draw();
  C.Print(outpath+"/mjj.png");


  
  ////deltaEta
  C.Clear();
  TH1F HdEtaVBF125("HdEtaVBF125","",30,0,12);
  TVBF125->Draw("abs(jet_eta[vbf_j1]-jet_eta[vbf_j2])>>HdEtaVBF125","vbf_mjj>0");
  HdEtaVBF125.Scale(1./HdEtaVBF125.Integral());
  TH1F HdEtaVBF500("HdEtaVBF500","",30,0,12);
  TVBF500->Draw("abs(jet_eta[vbf_j1]-jet_eta[vbf_j2])>>HdEtaVBF500","vbf_mjj>0");
  HdEtaVBF500.Scale(1./HdEtaVBF500.Integral());
  TH1F HdEtaMinB("HdEtaMinB","",30,0,12);
  TMinB->Draw("abs(jet_eta[vbf_j1]-jet_eta[vbf_j2])>>HdEtaMinB","vbf_mjj>0");
  HdEtaMinB.Scale(1./HdEtaMinB.Integral());

  C.Clear();  
  HdEtaVBF125.GetXaxis()->SetTitle("|eta(j1) - eta(j2)|");
  HdEtaVBF125.GetYaxis()->SetRangeUser(0,HdEtaMinB.GetMaximum()*1.2);
  HdEtaVBF125.Draw("hist");
  HdEtaVBF500.SetLineColor(4);
  HdEtaVBF500.Draw("histsame");
  HdEtaMinB.Draw("histsamep");
  Leg.Draw();
  C.Print(outpath+"/deltaEta.png");


  ////////////
  //pT distribution before and after the HGTD wp requirement
  ///////////
  C.Clear();
  TH1F HSubLeadpT("HSubLeadpT","",30,0,150);
  TVBF125->Draw("jet_pt[vbf_j1]/1000>>HSubLeadpT","vbf_mjj>0");
  TH1F HSubLeadpTCut("HSubLeadpTCut","",30,0,150);
  TVBF125->Draw("jet_pt[vbfdt2_j1]/1000>>HSubLeadpTCut","vbfdt2_mjj>0");

  C.Clear();  
  HSubLeadpT.GetXaxis()->SetTitle("pT(leading jet) [GeV]");
  HSubLeadpT.Draw("hist");
  HSubLeadpTCut.Draw("histpesame");
  Leg.Clear();
  Leg.AddEntry(&HSubLeadpT,"Baseline","l");
  Leg.AddEntry(&HSubLeadpTCut,"70% w.p.","p");
  Leg.Draw();
  C.Print(outpath+"/pTleadjet_wpcut.png");


  C.Clear();
  TH1F HSubLeadEta("HSubLeadEta","",20,0,8);
  TVBF125->Draw("fabs(jet_eta[vbf_j1])>>HSubLeadEta","vbf_mjj>0");
  TH1F HSubLeadEtaCut("HSubLeadEtaCut","",20,0,8);
  TVBF125->Draw("fabs(jet_eta[vbfdt2_j1])>>HSubLeadEtaCut","vbfdt2_mjj>0");

  C.Clear();  
  HSubLeadEta.GetXaxis()->SetTitle("|eta(leading jet)|");
  HSubLeadEta.Draw("hist");
  HSubLeadEtaCut.Draw("histpesame");
  Leg.Clear();
  Leg.AddEntry(&HSubLeadEta,"Baseline","l");
  Leg.AddEntry(&HSubLeadEtaCut,"70% w.p.","p");
  Leg.Draw();
  C.Print(outpath+"/etaleadjet_wpcut.png");


  gROOT->ProcessLine(".q");
}


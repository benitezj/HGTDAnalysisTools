#include "HGTDAnalysisTools/macros/globals.h"

//void plotROCs(TString Prod="VBFHinv_June29",TString Preshower="", TString ROCFile="ROC_JetRejection", TString TAG=""){
void plotROCs(TString PROD="LArHitAug17", 
	      TString SAMPLE="LArHit_July26_mu200_3CELLSIZE_40CELLENERGY_10TIMESMEAR/mc15_13TeV.301399.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_ZZ4nu.evgen.EVNT.e4397", 
	      TString PT="pT3040", 
	      TString ROCFile="Discriminator_ClusCellDTFitFractionEcw"){
  SetAtlasStyle();

  //gROOT->ProcessLine(".x ./HGTDAnalysisTools/macros/rootlogon.C");
  
  TString inpath=OUTDIR+"/ROC/"+PROD;
  if(SAMPLE.Contains("Preshower"))
    inpath=OUTDIR+"/ROC_Preshower/"+PROD;


  TFile F10(inpath+"/"+SAMPLE+"_"+PT+"/"+ROCFile+".root","read");

  SAMPLE("10TIMESMEAR")="30TIMESMEAR";
  TFile F30(inpath+"/"+SAMPLE+"_"+PT+"/"+ROCFile+".root","read");

  SAMPLE("30TIMESMEAR")="50TIMESMEAR";
  TFile F50(inpath+"/"+SAMPLE+"_"+PT+"/"+ROCFile+".root","read");

  //SAMPLE("50TIMESMEAR")="100TIMESMEAR";
  //TFile F100(inpath+"/"+SAMPLE+"_"+PT+"/"+ROCFile+".root","read");

  
  TTree * t10=(TTree*)F10.Get("tuple"); if(!t10) cout<<"No 10TimeRes"<<endl;
  TTree * t30=(TTree*)F30.Get("tuple"); if(!t30) cout<<"No 30TimeRes"<<endl;
  TTree * t50=(TTree*)F50.Get("tuple"); if(!t50) cout<<"No 50TimeRes"<<endl;
  TTree * t100=NULL;//(TTree*)F100.Get("tuple"); if(!t100) cout<<"No 100TimeRes"<<endl;


  ///////////////////////////////////////////////////
  //// ROC vs time resolution
  ///////////////////////////////////////////////////
  TString JetEtaCut=etamin+"<eta&&eta<"+etamax;
  cout<<"ROC vs Time res. for: "<< JetEtaCut<<endl;

  TH1F HFrame("HFrame","",1,0.4,1);
  HFrame.GetYaxis()->SetRangeUser(0,1);
  HFrame.GetYaxis()->SetTitle("pileup jet efficiency");
  HFrame.GetXaxis()->SetTitle("signal jet efficiency");

  TCanvas C;
  C.SetGrid();

  TGraph * G10=NULL;  if(t10) G10=getROCGraph(t10,JetEtaCut,1);
  TGraph * G30=NULL;  if(t30) G30=getROCGraph(t30,JetEtaCut,3);
  TGraph * G50=NULL;  if(t50) G50=getROCGraph(t50,JetEtaCut,4);
  TGraph * G100=NULL; if(t100) G100=getROCGraph(t100,JetEtaCut,2);

  C.Clear();
  HFrame.Draw();
  if(G10) G10->Draw("lsame");
  if(G30) G30->Draw("lsame");
  if(G50) G50->Draw("lsame");
  if(G100) G100->Draw("lsame");


  ///Print the resolutions
  TLatex Text; Text.SetTextSize(0.04);
  if(G10) {Text.SetTextColor(G10->GetLineColor()); Text.DrawLatexNDC(0.23,.85,"#sigma_{T} = 10ps");}
  if(G30) {Text.SetTextColor(G30->GetLineColor()); Text.DrawLatexNDC(0.23,.80,"#sigma_{T} = 30ps");}
  if(G50) {Text.SetTextColor(G50->GetLineColor()); Text.DrawLatexNDC(0.23,.75,"#sigma_{T} = 50ps");}
  if(G100) {Text.SetTextColor(G100->GetLineColor()); Text.DrawLatexNDC(0.23,.70,"#sigma_{T} = 100ps");}

  //Draw the boundary
  TGraph GBoundary;
  GBoundary.SetPoint(0,0,0);
  GBoundary.SetPoint(1,1,1);
  GBoundary.SetLineStyle(2);
  GBoundary.Draw("lsame");

  text.DrawLatexNDC(0.4,0.85,PT);
  text.DrawLatexNDC(0.4,0.80,etamin+"<#eta<"+etamax);


  C.Print(inpath+"/ROCvsTimeRes_"+ROCFile+"_"+PT+".png");



  ////////////////////////////////////////////////////////////////////
  ///plot the pu jet rejection for a signal jet efficiency wp as a function of eta
  ////////////////////////////////////////////////////////////////////
  TString EffWP="0.70";
  TString EffWPSel=TString("(abs(eff_sig-")+EffWP+")<0.03)";
  cout<<"Signal Eff. WP: "<<EffWP<<endl;

  TH1F HFrameRejVsEta("HFrameRejVsEta","",1,2.4,4.0);
  HFrameRejVsEta.GetYaxis()->SetRangeUser(0,1);
  HFrameRejVsEta.GetYaxis()->SetTitle("pileup jet efficiency");
  HFrameRejVsEta.GetXaxis()->SetTitle("jet eta ");

  TH1F HPURejVsEta_10("HPURejVsEta_10","",ETABINS,ETAMIN,ETAMAX);
  TH1F HPURejVsEtaNorm_10("HPURejVsEtaNorm_10","",ETABINS,ETAMIN,ETAMAX);
  if(t10){
    t10->Draw("eta>>HPURejVsEta_10",EffWPSel+"*(eff_pu)");
    t10->Draw("eta>>HPURejVsEtaNorm_10",EffWPSel);
    HPURejVsEta_10.Divide(&HPURejVsEtaNorm_10);
    HPURejVsEta_10.SetLineColor(G10->GetLineColor());
  }

  TH1F HPURejVsEta_30("HPURejVsEta_30","",ETABINS,ETAMIN,ETAMAX);
  TH1F HPURejVsEtaNorm_30("HPURejVsEtaNorm_30","",ETABINS,ETAMIN,ETAMAX);
  if(t30){
    t30->Draw("eta>>HPURejVsEta_30",EffWPSel+"*(eff_pu)");
    t30->Draw("eta>>HPURejVsEtaNorm_30",EffWPSel);
    HPURejVsEta_30.Divide(&HPURejVsEtaNorm_30);
    HPURejVsEta_30.SetLineColor(G30->GetLineColor());
  }

  TH1F HPURejVsEta_50("HPURejVsEta_50","",ETABINS,ETAMIN,ETAMAX);
  TH1F HPURejVsEtaNorm_50("HPURejVsEtaNorm_50","",ETABINS,ETAMIN,ETAMAX);
  if(t50){
    t50->Draw("eta>>HPURejVsEta_50",EffWPSel+"*(eff_pu)");
    t50->Draw("eta>>HPURejVsEtaNorm_50",EffWPSel);
    HPURejVsEta_50.Divide(&HPURejVsEtaNorm_50);
    HPURejVsEta_50.SetLineColor(G50->GetLineColor());
  }

  TH1F HPURejVsEta_100("HPURejVsEta_100","",ETABINS,ETAMIN,ETAMAX);
  TH1F HPURejVsEtaNorm_100("HPURejVsEtaNorm_100","",ETABINS,ETAMIN,ETAMAX);
  if(t100){
    t100->Draw("eta>>HPURejVsEta_100",EffWPSel+"*(eff_pu)");
    t100->Draw("eta>>HPURejVsEtaNorm_100",EffWPSel);
    HPURejVsEta_100.Divide(&HPURejVsEtaNorm_100);
    HPURejVsEta_100.SetLineColor(G100->GetLineColor());
  }
  

  C.Clear();
  HFrameRejVsEta.Draw("hist");
  if(t10) HPURejVsEta_10.Draw("histsame");
  if(t30) HPURejVsEta_30.Draw("histsame");
  if(t50) HPURejVsEta_50.Draw("histsame");
  if(t100) HPURejVsEta_100.Draw("histsame");

  if(G10) {Text.SetTextColor(G10->GetLineColor()); Text.DrawLatexNDC(0.23,.85,"#sigma_{T} = 10ps");}
  if(G30) {Text.SetTextColor(G30->GetLineColor()); Text.DrawLatexNDC(0.23,.80,"#sigma_{T} = 30ps");}
  if(G50) {Text.SetTextColor(G50->GetLineColor()); Text.DrawLatexNDC(0.23,.75,"#sigma_{T} = 50ps");}
  if(G100) {Text.SetTextColor(G100->GetLineColor()); Text.DrawLatexNDC(0.23,.70,"#sigma_{T} = 100ps");}

  text.DrawLatexNDC(0.4,0.85,TString("Signal Eff.=")+EffWP);
  text.DrawLatexNDC(0.4,0.80,PT);
  C.Print(inpath+"/RejectionvsEta_"+ROCFile+"_"+PT+".png");


  gROOT->ProcessLine(".q");
}

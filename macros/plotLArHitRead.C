#include <TString.h>
#include <TFile.h>
#include <TH1F.h>
#include <TDirectory.h>
#include <TString.h>
#include <TLegend.h>
#include <TProfile.h>
#include <TLatex.h>

TString OUTDIR="/afs/cern.ch/user/b/benitezj/www/HGTDStudies/HitPlots";
TLatex text;
TLine line;


void plotTruth(TTree*TREE,TString outpath){
  TCanvas C;

  ///Truth distribution
  TH1F HTruthEta("HTruthEta","",120,0,600);
  HTruthEta.GetXaxis()->SetTitle("Hit radius");
  TREE->Draw("truth_eta>>HTruthEta","truth_status==1");
  HTruthEta.Draw("hist");
  C.Print(outpath+"/TruthEta.png");

}

void plotRawHitSpatial(TTree*TREE,TString outpath){
  TCanvas C;

  C.Clear();
  TH2F HHitXY("HHitXY","",240,-600,600,240,-600,600);
  HHitXY.GetXaxis()->SetTitle("Hit X [mm]");
  HHitXY.GetYaxis()->SetTitle("Hit Y [mm]");
  TREE->Draw("hit_y:hit_x>>HHitXY");
  HHitXY.Draw("colz");
  C.Print(outpath+"/HitXY.png");

  C.Clear();
  TH1F HHitS("HHitS","",11,-5.5,5.5);
  HHitS.GetXaxis()->SetTitle("layer #");
  HHitS.GetYaxis()->SetTitle("# of hits");
  TREE->Draw("hit_S>>HHitS");
  HHitS.Draw("colz");
  C.Print(outpath+"/HitS.png");

  TH1F HHitR("HHitR","",60,0,600);
  HHitR.GetXaxis()->SetTitle("Hit radius");
  HHitR.GetYaxis()->SetTitle("Hits / 5mm");
  TREE->Draw("sqrt(hit_y*hit_y+hit_x*hit_x)>>HHitR");
  HHitR.Draw("hist");
  C.Print(outpath+"/HitR.png");

  TH2F HHitRVsL("HHitRVsL","",11,-5.5,5.5,120,0,600);
  HHitRVsL.GetYaxis()->SetTitle("Hit radius (5mm)");
  HHitRVsL.GetXaxis()->SetTitle("layer #");
  TREE->Draw("sqrt(hit_y*hit_y+hit_x*hit_x):hit_S>>HHitRVsL");
  HHitRVsL.Draw("colz");
  C.Print(outpath+"/HitRVsL.png");

}

void plotRawHitTime(TTree*TREE,TString outpath){
  TCanvas C;

  C.Clear();
  TH1F HHitT("HHitT","",50,10,20);
  HHitT.GetXaxis()->SetTitle("Hit time [ns]");
  TREE->Draw("hit_t>>HHitT");
  HHitT.Draw("hist");
  C.Print(outpath+"/HitT.png");
  C.SetLogy(1);
  C.Print(outpath+"/HitT_log.png");
  C.SetLogy(0);

  C.Clear();
  TH2F HHitTvsL("HHitTvsL","",11,-5.5,5.5,50,10,15);
  HHitTvsL.GetYaxis()->SetTitle("Hit time [ns]");
  HHitTvsL.GetXaxis()->SetTitle("layer number");
  TREE->Draw("hit_t:hit_S>>HHitTvsL");
  HHitTvsL.Draw("colz");
  C.Print(outpath+"/HHitTvsL.png");

  C.Clear();
  TH2F HHitTVsE("HHitTVsE","",100,0,2,100,10,15);
  HHitTVsE.GetYaxis()->SetTitle("Hit time [ns]");
  HHitTVsE.GetXaxis()->SetTitle("Hit energy [MeV]");
  TREE->Draw("(hit_t):hit_E>>HHitTVsE");
  HHitTVsE.Draw("colz");
  C.Print(outpath+"/HitTVsE.png");


}

void plotRawHitEnergy(TTree*TREE,TString outpath){
  TCanvas C;

  //////////////////////////////
  ///Hit energy 
  C.Clear();
  TH1F HHitE("HHitE","",60,0,3);
  HHitE.GetXaxis()->SetTitle("Hit energy [MeV]");
  TREE->Draw("hit_E>>HHitE");
  HHitE.Draw("hist");
  C.Print(outpath+"/HitE.png");
  C.SetLogy(1);
  C.Print(outpath+"/HitE_log.png");
  C.SetLogy(0);

  C.Clear();
  TH1F HHitELow("HHitELow","",100,0,1);
  HHitELow.GetXaxis()->SetTitle("Hit energy [MeV]");
  TREE->Draw("hit_E>>HHitELow");
  HHitELow.GetYaxis()->SetRangeUser(0.1,HHitELow.GetMaximum()*1.4);
  HHitELow.Draw("hist");
  line.DrawLine(0.05,0.1,0.05,HHitELow.GetMaximum());
  C.Print(outpath+"/HitELow.png");
  C.SetLogy(1);
  C.Print(outpath+"/HitELow_log.png");
  C.SetLogy(0);
}


void plotLArHitRead(TString infile,TString tag){
  
  gROOT->ProcessLine(".x ./HGTDAnalysisTools/macros/rootlogon.C");

  line.SetLineColor(2);
  line.SetLineWidth(1);

  TFile INPUT((infile).Data(),"READ");
  if(INPUT.IsZombie()){cout<<"Could not open file"<<endl; return;}
  cout<<"Opened file : "<<infile<<endl;

  TTree* TREE = (TTree*) INPUT.Get("tuple");
  if(!TREE){cout<<"no tuple"<<endl; return;}
  cout<<"Number of events: "<<TREE->GetEntries()<<endl;
  

  TCanvas C("C");
  TString outpath=OUTDIR+"/LArHitRead_"+tag;
  system((TString("rm -rf ")+outpath).Data());
  system((TString("mkdir ")+outpath).Data());


  text.SetTextSize(0.03);
  text.SetTextColor(4);
 
  // plotTruth(TREE,outpath);
  plotRawHitSpatial(TREE,outpath);
  plotRawHitTime(TREE,outpath);
  plotRawHitEnergy(TREE,outpath);

  gROOT->ProcessLine(".q");
}

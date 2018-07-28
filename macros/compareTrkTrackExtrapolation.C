#include "HGTDAnalysisTools/macros/globals.h"
#include "HGTDAnalysisTools/macros/plottingTools.C"

void compareTrkTrackExtrapolation(){
  
  SetAtlasStyle();
  gStyle->SetPalette(1,0);


  TCanvas *C=new TCanvas("C");

  TFile F1("tuple_xAODTrack.root","read");
  TFile F2("tuple_TrkTrack.root","read");
  
  TTree*t1=(TTree*)F1.Get("tuple");
  TTree*t2=(TTree*)F2.Get("tuple");
  if(!t1||!t2) return;

  gROOT->cd();
  
  TH1F*H1=new TH1F("H1","",40,-20,20); 
  H1->Sumw2();
  TH1F*H2=new TH1F("H2","",40,-20,20);
  H2->Sumw2();

  t1->Draw("cell_x-trk_hgtd0_x[cell_trk]>>H1","cell_trk>=0");
  t2->Draw("cell_x-trk_hgtd0_x[cell_trk]>>H2","cell_trk>=0");
  //cout<<H1->Integral()<<" "<<H1->GetMean()<<endl;

  // C->Clear();
  // C->Divide(2,1);
  // C->cd(1);
  // H1->Draw("hist");
  // H2->Draw("histpesame");
  // C->cd(2);
  // TH1F*hr=(TH1F*)H1->Clone("ratio");
  // hr->Divide(H2);
  // hr->Draw("pe");

  
  compareWithRatio(H1,H2,"/afs/cern.ch/user/b/benitezj/www/HGTDStudies/compareTrkTrackExtrapolation.gif");

  gROOT->ProcessLine(".q");
}

#include "HGTDAnalysisTools/macros/globals.h"

void compareTrkLayerPosCorrection(){

  SetAtlasStyle();
  gStyle->SetPalette(1,0);

  TFile FNoCor("tuple_noLayerCorr.root","read");
  TFile FWithCor("tuple_wLayerCorr.root","read");

  TTree*TNoCor=(TTree*)FNoCor.Get("tuple");
  TTree*TWithCor=(TTree*)FWithCor.Get("tuple");

  TString trackSel="(cell_trk>-1&&trk_truth_pt[cell_trk]>1000&&2.5<abs(trk_eta[cell_trk])&&abs(trk_eta[cell_trk])<4.0&&cell_type==2)";


  TCanvas C("C","",800,800);
  C.Divide(2,2);
  
  for(long l=1;l<=4;l++){
    C.cd(l);

    TH1F* HNoCor=new TH1F(TString("HNoCor")+l,"",15,0,15);
    TNoCor->Draw(TString("cell_trkdR>>HNoCor")+l,trackSel+"&&abs(cell_S)=="+l);
    TH1F* HWithCor=new TH1F(TString("HWithCor")+l,"",15,0,15);
    TWithCor->Draw(TString("cell_trkdR>>HWithCor")+l,trackSel+"&&abs(cell_S)=="+l);
    HNoCor->GetXaxis()->SetTitle("cell x-y distance to track   [mm]");    


    // TH1F* HNoCor=new TH1F(TString("HNoCor")+l,"",30,-15,15);
    // TNoCor->Draw(TString("cell_x-trk_ext_x[cell_trk]>>HNoCor")+l,trackSel+"&&abs(cell_S)=="+l);
    // TH1F* HWithCor=new TH1F(TString("HWithCor")+l,"",30,-15,15);
    // TWithCor->Draw(TString("cell_x-trk_hgtd")+(l-1)+"_x[cell_trk]>>HWithCor"+l,trackSel+"&&abs(cell_S)=="+l);
    // HWithCor->GetXaxis()->SetTitle("cell x - track x   [mm]");    

    // TH1F* HNoCor=new TH1F(TString("HNoCor")+l,"",30,-15,15);
    // TNoCor->Draw(TString("cell_y-trk_ext_y[cell_trk]>>HNoCor")+l,trackSel+"&&abs(cell_S)=="+l);
    // TH1F* HWithCor=new TH1F(TString("HWithCor")+l,"",30,-15,15);
    // TWithCor->Draw(TString("cell_y-trk_hgtd")+(l-1)+"_y[cell_trk]>>HWithCor"+l,trackSel+"&&abs(cell_S)=="+l);
    // HWithCor->GetXaxis()->SetTitle("cell y - track y   [mm]");    

    HWithCor->Draw("histpe");
    HNoCor->Draw("histsame");


    TLatex latex;
    latex.DrawLatexNDC(0.7,0.7,TString("Layer ")+(l-1));
  }
  C.Print("/afs/cern.ch/user/b/benitezj/www/HGTDStudies/compareTrkLayerPosCorrection.gif");

  
  gROOT->ProcessLine(".q");

}


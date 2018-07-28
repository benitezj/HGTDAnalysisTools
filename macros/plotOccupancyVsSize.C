
void plotOccupancyVsSize(TString TAG="SiHit_April12_Preshower_mu200",TString ECUT="20CELLENERGY"){
  gROOT->ProcessLine(".x ./HGTDAnalysisTools/macros/rootlogon.C");

  TString inpath="/afs/cern.ch/user/b/benitezj/www/HGTDStudies/HitPlots";  
  TString outpath=TString("/afs/cern.ch/user/b/benitezj/www/HGTDStudies/HitPlots/OccupancyVsSize_") + TAG + "_" + ECUT;
  TString sample="mc15_13TeV.341080.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_WWlvlv_EF_15_5.evgen.EVNT.e4397";
  
  system(TString("rm -f ")+outpath+"/*");
  system(TString("mkdir ")+outpath);

  //////////////////////////////////////////////////////////////////
  ///////////////////Ocupancys vs mu
  //////////////////////////////////////////////////////////////////
  TFile F1(inpath+"/"+TAG+"_1CELLSIZE_"+ECUT+"_"+sample+"/NCellVsR.root");
  TH1F * H1_L4=(TH1F*)F1.Get("HNCellVsR4"); // if(!H1_L4) return;

  TFile F3(inpath+"/"+TAG+"_3CELLSIZE_"+ECUT+"_"+sample+"/NCellVsR.root");
  TH1F * H3_L4=(TH1F*)F3.Get("HNCellVsR4"); // if(!H3_L4) return;

  TFile F5(inpath+"/"+TAG+"_5CELLSIZE_"+ECUT+"_"+sample+"/NCellVsR.root");
  TH1F * H5_L4=(TH1F*)F5.Get("HNCellVsR4"); if(!H5_L4) return;

  TFile F10(inpath+"/"+TAG+"_10CELLSIZE_"+ECUT+"_"+sample+"/NCellVsR.root");
  TH1F * H10_L4=(TH1F*)F10.Get("HNCellVsR4"); // if(!H10_L4) return;

  TFile F20(inpath+"/"+TAG+"_20CELLSIZE_"+ECUT+"_"+sample+"/NCellVsR.root");
  TH1F * H20_L4=(TH1F*)F20.Get("HNCellVsR4"); //if(!H20_L4) return;
  

  TCanvas C;
  C.Clear();
  H5_L4->GetYaxis()->SetRangeUser(0,1);
  H5_L4->GetYaxis()->SetTitle("% of cells");
  H5_L4->GetXaxis()->SetTitle("cell radius [mm]");
  H5_L4->Draw("p");
  if(H1_L4)H1_L4->Draw("psame");
  if(H3_L4) H3_L4->Draw("psame");
  if(H10_L4) H10_L4->Draw("psame");
  if(H20_L4) H20_L4->Draw("psame");
  C.Print(outpath+"/OccupancyVsSize.png");


  gROOT->ProcessLine(".q");

}

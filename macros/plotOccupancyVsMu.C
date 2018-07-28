
void plotOccupancyVsMu(TString TAG="SiHit_April12_Preshower",TString CellSize="10CELLSIZE_20CELLENERGY"){
  gROOT->ProcessLine(".x ./HGTDAnalysisTools/macros/rootlogon.C");

  TString inpath="/afs/cern.ch/user/b/benitezj/www/HGTDStudies/HitPlots";  
  TString outpath=TString("/afs/cern.ch/user/b/benitezj/www/HGTDStudies/HitPlots/Occupancy_") + TAG +"_"+ CellSize;
  TString sample="mc15_13TeV.341080.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_WWlvlv_EF_15_5.evgen.EVNT.e4397";
  
  system(TString("rm -f ")+outpath+"/*");
  system(TString("mkdir ")+outpath);

 
  //////////////////////////////////////////////////////////////////
  //          Ocupancys vs mu
  //////////////////////////////////////////////////////////////////
  TFile Fmu0(inpath+"/"+TAG+"_mu0_"+CellSize+"_"+sample+"/NCellVsR.root");
  TH1F * Hmu0_L4=(TH1F*)Fmu0.Get("HNCellVsR4"); 
  TH1F * Hmu0_L1=(TH1F*)Fmu0.Get("HNCellVsR1"); 
  TFile Fmu80(inpath+"/"+TAG+"_mu80_"+CellSize+"_"+sample+"/NCellVsR.root");
  TH1F * Hmu80_L4=(TH1F*)Fmu80.Get("HNCellVsR4"); 
  TH1F * Hmu80_L1=(TH1F*)Fmu80.Get("HNCellVsR1"); 
  TFile Fmu140(inpath+"/"+TAG+"_mu140_"+CellSize+"_"+sample+"/NCellVsR.root");
  TH1F * Hmu140_L4=(TH1F*)Fmu140.Get("HNCellVsR4"); 
  TH1F * Hmu140_L1=(TH1F*)Fmu140.Get("HNCellVsR1"); 
  TFile Fmu200(inpath+"/"+TAG+"_mu200_"+CellSize+"_"+sample+"/NCellVsR.root");
  TH1F * Hmu200_L4=(TH1F*)Fmu200.Get("HNCellVsR4"); if(!Hmu200_L4) return;
  TH1F * Hmu200_L1=(TH1F*)Fmu200.Get("HNCellVsR1"); 


  TCanvas C;
  C.Clear();
  Hmu200_L4->GetYaxis()->SetTitle("% of cells");
  Hmu200_L4->GetXaxis()->SetTitle("cell radius [mm]");
  Hmu200_L4->Draw("p");
  if(Hmu140_L4){  Hmu140_L4->Draw("psame");}
  if(Hmu80_L4){  Hmu80_L4->Draw("psame");}
  if(Hmu0_L4){Hmu0_L4->Draw("psame");}

  if(Hmu0_L1){Hmu0_L1->SetMarkerColor(2);    Hmu0_L1->Draw("psame");}
  if(Hmu80_L1){Hmu80_L1->SetMarkerColor(2);    Hmu80_L1->Draw("psame");}
  if(Hmu140_L1){Hmu140_L1->SetMarkerColor(2);    Hmu140_L1->Draw("psame");}
  if(Hmu200_L1){Hmu200_L1->SetMarkerColor(2); Hmu200_L1->Draw("psame");}





  C.Print(outpath+"/OccupancyVsMu.png");






  gROOT->ProcessLine(".q");

}

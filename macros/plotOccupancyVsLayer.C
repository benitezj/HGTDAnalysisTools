#include "HGTDAnalysisTools/macros/globals.h"

void plotOccupancyVsLayer(TString inpath){
  gROOT->ProcessLine(".x ./HGTDAnalysisTools/macros/rootlogon.C");

  TFile F1(inpath+"/NCellVsR.root");
  TH1F * HL[4];
  HL[0]=(TH1F*)F1.Get("HNCellVsR1"); if(!HL[0]) return;
  HL[1]=(TH1F*)F1.Get("HNCellVsR2"); if(!HL[1]) return;
  HL[2]=(TH1F*)F1.Get("HNCellVsR3"); if(!HL[2]) return;
  HL[3]=(TH1F*)F1.Get("HNCellVsR4"); if(!HL[3]) return;

  HL[0]->SetLineColor(1);
  HL[1]->SetLineColor(2);
  HL[2]->SetLineColor(3);
  HL[3]->SetLineColor(4);

  HL[0]->SetMarkerColor(1);
  HL[1]->SetMarkerColor(2);
  HL[2]->SetMarkerColor(3);
  HL[3]->SetMarkerColor(4);
  

  TLegend leg;
  leg.AddEntry(HL[3],"Layer 4","l");
  if(HL[2])leg.AddEntry(HL[2],"Layer 3","l");
  if(HL[1])leg.AddEntry(HL[1],"Layer 2","l");
  if(HL[0])leg.AddEntry(HL[0],"Layer 1","l");
  leg.SetX1NDC(0.65);
  leg.SetX2NDC(0.80);
  leg.SetY1NDC(0.70);
  leg.SetY2NDC(0.85);


  TCanvas C;
  C.Clear();
  HL[3]->GetYaxis()->SetRangeUser(0,1.0);
  HL[3]->GetYaxis()->SetTitle("% of total cells");
  HL[3]->GetXaxis()->SetTitle("radius [mm]");
  HL[3]->Draw("phist");
  DrawATLAS(200);
  if(HL[2])HL[2]->Draw("phistsame");
  if(HL[1])HL[1]->Draw("phistsame");
  if(HL[0])HL[0]->Draw("phistsame");
  leg.Draw();
  C.Print(inpath+"/OccupancyVsLayer.png");



  ///////////////////////////////////
  /////Save the values into .txt files:
  //////////////////////////////////
  for (long i=0;i<4;i++){
    FILE * fp = fopen((inpath+"/Occupancy_Layer"+(i+1)+".txt").Data(),"w");
    if (fp == NULL) {
      cout<<"Can't open file."<<endl;
      exit(1);
    }
    
  
    for(int b=1;b<=HL[i]->GetNbinsX();b++){
      fprintf(fp,"%.1f %.4f\n",HL[i]->GetBinCenter(b),HL[i]->GetBinContent(b));
    }
    
    fclose(fp);
  }
  
  gROOT->ProcessLine(".q");

}

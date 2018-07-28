#include "HGTDAnalysisTools/macros/globals.h"

void drawDRTemplates(TString path){
  SetAtlasStyle();
  setCuts(path);

  TString file="dRTemplates";
  TFile FILE(path+"/"+file+".root");
  if(FILE.IsZombie()) return;
  gROOT->cd();

  ////Draw distributions
  TLegend legend(0.6,0.2,0.8,0.45);
  TCanvas C;
  C.SetRightMargin(0.15);
  int color;

  int etastep=4;
  int minetaidx=24;
  int maxetaidx=36;

  int ptstep=20;
  int minptidx=30;
  int maxptidx=70;

  TH1F * HdREta[50][100];
  TH1F * HdREtaPU[50][100];

  ////Draw the dR distributions for Signal
  C.Clear();
  for(long pt=minptidx;pt<=maxptidx;pt+=ptstep){
    color=0;
    for(long eta=minetaidx;eta<=maxetaidx;eta+=etastep){
      HdREta[eta][pt]=(TH1F*)FILE.Get(TString("HdR_eta")+eta+"_pt"+pt);
      if(!HdREta[eta][pt]) return;
      HdREta[eta][pt]->SetLineColor(++color);
      HdREta[eta][pt]->GetYaxis()->SetTitle("# of hits (unit norm)");
      HdREta[eta][pt]->GetXaxis()->SetTitle("#DeltaR(cell,jet)");
      HdREta[eta][pt]->Scale(1./HdREta[eta][pt]->Integral());
      HdREta[eta][pt]->SetAxisRange(0.,0.35,"X");
      if(eta==minetaidx){
	HdREta[eta][pt]->GetYaxis()->SetRangeUser(0.,0.2);
	HdREta[eta][pt]->Draw("hist");
      }else HdREta[eta][pt]->Draw("histsame");

      if(pt==minptidx)
	legend.AddEntry(HdREta[eta][pt],TString("#eta=")+(eta)/10+"."+(eta)%10+"-"+(eta+4)/10+"."+(eta+4)%10,"l");
    }
    legend.Draw();
    C.Print(path+"/"+file+"_dR_Sig_pt"+pt+".png");
  }

  ////Draw the dR distributions for PU
  C.Clear();
  for(long pt=minptidx;pt<=maxptidx;pt+=ptstep){
    color=0;
    for(long eta=minetaidx;eta<=maxetaidx;eta+=etastep){
      HdREtaPU[eta][pt]=(TH1F*)FILE.Get(TString("HdRPU_eta")+eta+"_pt"+pt);
      if(!HdREtaPU[eta][pt]) return;
      HdREtaPU[eta][pt]->SetLineColor(++color);
      HdREtaPU[eta][pt]->GetYaxis()->SetTitle("# of hits (unit norm)");
      HdREtaPU[eta][pt]->GetXaxis()->SetTitle("#DeltaR(cell,jet)");
      HdREtaPU[eta][pt]->Scale(1./HdREtaPU[eta][pt]->Integral());
      HdREtaPU[eta][pt]->SetAxisRange(0.,0.35,"X");
      if(eta==minetaidx){
	HdREtaPU[eta][pt]->GetYaxis()->SetRangeUser(0.,0.2);
	HdREtaPU[eta][pt]->Draw("hist");
      }else HdREtaPU[eta][pt]->Draw("histsame");
    }
    legend.Draw();
    C.Print(path+"/"+file+"_dR_PU_pt"+pt+".png");
  }



  gROOT->ProcessLine(".q");
}

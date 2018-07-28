#include "HGTDAnalysisTools/macros/globals.h"

void testJetShapeKnee(float angle=180){
  char Angle[100];
  sprintf(Angle,"%.5f",angle*TMath::Pi()/180);

  TCanvas*C=new TCanvas("C");

  gStyle->SetOptStat(0);
  TH1F* H=new TH1F("H","",1,-1.5,1.5);
  H->GetYaxis()->SetRangeUser(-1,1);
  H->Draw();
  
  TLegend *leg =new TLegend(0.8,0.1,0.9,0.9);
  TLatex text;
  text.SetTextSize(0.03);

  long np=20;
  for(long n=0;n<np;n++){
    float subangle=1.0*TMath::Pi()+(n*1*TMath::Pi())/np;
    char Subangle[100];
    sprintf(Subangle,"%.5f",subangle);

    //TF1* F=new TF1(TString("F")+n,TString("x*((x<0)*tan(") +Angle + ") + (x>0)*tan(" + Angle + "+" + Subangle +") )",-1,1);
    TF1* F=new TF1(TString("F")+n,TString("x*((x<0)*tan(3.14159/2+") +Angle + ") + (x>0)*tan(3.14159/2+" + Subangle +") )",-1,1);
    F->SetLineColor(n+1);
    F->Draw("same");

    long ANGLE=angle;
    long SUBANGLE=180*subangle/TMath::Pi();
    leg->AddEntry(F,TString("")+ANGLE+","+SUBANGLE,"l");
    
    text.DrawLatex(0.9,F->Eval(0.9),TString()+SUBANGLE);    
  }
  leg->Draw("same");

  C->Print(OUTDIR+"/testJetShapeKnee.png");
}

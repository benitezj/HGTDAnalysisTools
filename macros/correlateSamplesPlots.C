#include "HGTDAnalysisTools/macros/globals.h"


void printFraction(TH1F*h){
  
  float fraction=0.;
  for(int b=1;b<h->GetNbinsX();b++){
    if(h->GetBinCenter(b)<0.1)
      fraction+=h->GetBinContent(b);
  }
  
  char fracText[100];
  sprintf(fracText,"fraction < 0.1  :  %.2f",fraction/h->Integral());
  text.DrawLatexNDC(0.6,0.8,fracText);

  line.DrawLine(0.1,0,0.1,h->GetMaximum());
}


/////////////////////////////////////////
/// Macro makes plots from tuple created by correlateSamples.C
/// shows the correlation of the mu=200 jet clusters with the mu=0 jet clusters
/////////////////////////////////////////
void correlateSamplesPlots(TString jetsel="",TString TAG=""){
  cout<<"Selection: "<<jetsel<<endl;


  TString outpath=OUTDIR+"/correlateSamplesPlots";
  if(TAG.CompareTo("")!=0) outpath+= TAG;
  system(TString("rm -rf ")+outpath);
  system(TString("mkdir ")+outpath);


  SetAtlasStyle();
  gStyle->SetPalette(1,0);
  setCuts(outpath);

  

  TFile F("tuple_correlateClusters_VBFmu200Vsmu0.root","read");

  TTree* t=(TTree*)F.Get("tuple");

  C1D.Clear();
  TH1F hpt("hpt","",50,0,500);
  hpt.GetXaxis()->SetTitle(" jet truth pT  [GeV]");
  t->Draw("pt/1000>>hpt",jetsel);
  C1D.Print(outpath+"/jetpt.png");

  C1D.Clear();
  TH1F heta("heta","",30,2,5);
  heta.GetXaxis()->SetTitle(" jet truth eta ");
  t->Draw("abs(eta)>>heta",jetsel);
  C1D.Print(outpath+"/jeteta.png");


  ///j1 is the mu=200
  TH1F hdrj1("hdrj1","",40,0,0.8);
  hdrj1.GetXaxis()->SetTitle(" #DeltaR(J_{#mu=200}, J_{true})");
  t->Draw("drj1>>hdrj1",jetsel);
  printFraction(&hdrj1);
  C1D.Print(outpath+"/jetdr_mu200vstrue.png");
  
  ////j2 is mu=0
  TH1F hdrj2("hdrj2","",40,0,0.8);
  hdrj2.GetXaxis()->SetTitle(" #DeltaR(J_{#mu=0}, J_{true})");
  t->Draw("drj2>>hdrj2",jetsel);
  printFraction(&hdrj2);
  C1D.Print(outpath+"/jetdr_mu0vstrue.png");
  
  //j1-j2 
  TH1F hdrj12("hdrj12","",40,0,0.8);
  hdrj12.GetXaxis()->SetTitle(" #DeltaR(J_{#mu=0}, J_{#mu=200})");
  t->Draw("drj12>>hdrj12",jetsel);
  printFraction(&hdrj12);
  C1D.Print(outpath+"/jetdr_mu0vsmu200.png");
  

  //drcl : difference between leading clusters
  TH1F hdrcl("hdrcl","",40,0,0.8);
  hdrcl.GetXaxis()->SetTitle("lead cluster  #DeltaR(clus_{#mu=0}, clus_{#mu=200})");
  t->Draw("drcl>>hdrcl",jetsel);
  printFraction(&hdrcl);
  C1D.Print(outpath+"/leadclusdr_mu0vsmu200.png");

  TH1F hptcl("hptcl","",25,0,5);
  hptcl.GetXaxis()->SetTitle(" lead cluster E_{#mu=200} / E_{#mu=0} ");
  t->Draw("ptcl>>hptcl",jetsel);
  C1D.Print(outpath+"/leadclusE_mu0vsmu200.png");


  //drcsl : difference between subleading clusters
  TH1F hdrcsl("hdrcsl","",40,0,0.8);
  hdrcsl.GetXaxis()->SetTitle(" sub-lead cluster #DeltaR(clus_{#mu=0}, clus_{#mu=200})");
  t->Draw("drcsl>>hdrcsl",jetsel);
  printFraction(&hdrcsl);
  C1D.Print(outpath+"/subleadclusdr_mu0vsmu200.png");

  TH1F hptcsl("hptcsl","",25,0,5);
  hptcsl.GetXaxis()->SetTitle(" sub-lead cluster E_{#mu=200} / E_{#mu=0} ");
  t->Draw("ptcsl>>hptcsl",jetsel);
  C1D.Print(outpath+"/subleadclusE_mu0vsmu200.png");


  //drcsln : difference between subleading cluster and nearest mu=200 cluster
  TH1F hdrcsln("hdrcsln","",40,0,0.8);
  hdrcsln.GetXaxis()->SetTitle(" #DeltaR(nearest clus_{#mu=200}, sub-lead clus_{#mu=0})");
  t->Draw("drcsln>>hdrcsln",jetsel);
  printFraction(&hdrcsln);
  C1D.Print(outpath+"/subleadclusNeardr_mu0vsmu200.png");

  TH1F hptcsln("hptcsln","",25,0,5);
  hptcsln.GetXaxis()->SetTitle(" nearest cluster E_{#mu=200}  / sub-lead cluster E_{#mu=0} ");
  t->Draw("ptcsln>>hptcsln",jetsel);
  C1D.Print(outpath+"/subleadclusNearE_mu0vsmu200.png");

}

#include "HGTDAnalysisTools/macros/globals.h"

void findWP(TString PROD="VBFHinv_June29",TString Preshower="",long TIMERES=10, long pTL=30, long pTH=40,TString Discriminator="ClusCellDTFitFraction",float unit=1.0){
				     
  float WP=0.70;
  float WPerr=0.01;
  
  TFile F(OUTDIR+"/"+PROD+"/SiHit_June15"+Preshower+"_mu200_3CELLSIZE_60CELLENERGY_"+TIMERES+"TIMESMEAR_ROC_pT"+pTL+pTH+"/Discriminator_"+Discriminator+".root","read");
  if(F.IsZombie()){
    cout<<F.GetName()<<" is not accessible"<<endl;
    gROOT->ProcessLine(".q");
  }

  TTree*T=(TTree*)F.Get("tuple");
  if(!T){
    cout<<"tuple is not accessible"<<endl;
    gROOT->ProcessLine(".q");
  }
  
  float eta; 
  T->SetBranchAddress("eta",&eta);
  float eff_sig; 
  T->SetBranchAddress("eff_sig",&eff_sig);
  int nhit; 
  T->SetBranchAddress("nhit",&nhit);
  

  TH1F H("H","",ETABINS,ETAMIN,ETAMAX); 
  TH1F HN("HN","",ETABINS,ETAMIN,ETAMAX);

  for(int e=0;e<=T->GetEntries();e++){
    T->GetEntry(e);
    
    for(int b=1;b<=H.GetNbinsX();b++){
      if(fabs(H.GetBinCenter(b)-eta)<0.1 && fabs(eff_sig-WP)<WPerr){
	H.AddBinContent(b,nhit);
	HN.AddBinContent(b,1);
      }
    }

    //cout<<eta<<" "<<eff_sig<<" "<<nhit<<endl;
  }

  H.Divide(&HN);


  TString OUTFILENAME=TString("WP")+(long(WP*100))+"_"+PROD+Preshower+"_"+TIMERES+"_pT"+pTL+pTH+"_"+Discriminator+".txt";
  FILE * fp = fopen(OUTFILENAME.Data(),"w");
  if (fp == NULL) {
    fprintf(stderr, "Can't open file.\n");
    exit(1);
  }else
    cout<<"Opened : "<<OUTFILENAME<<endl;


  for(int b=1;b<=H.GetNbinsX();b++){
    fprintf(fp,"%d %d %.2f %.2f %.4f\n",int(pTL),int(pTH),H.GetBinLowEdge(b),H.GetBinLowEdge(b)+H.GetBinWidth(b),H.GetBinContent(b)*unit);
  }

  fclose(fp);
  
  
  gROOT->ProcessLine(".q");
}

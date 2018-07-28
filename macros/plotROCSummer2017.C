#include "HGTDAnalysisTools/macros/globals.h"

void plotROCSummer2017(TString SAMPLE="LArHit_July26_mu200_3CELLSIZE_40CELLENERGY_10TIMESMEAR/mc15_13TeV.301399.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_ZZ4nu.evgen.EVNT.e4397", 
		       TString TAG="ClusCellDTFitFractionEcw"){
  SetAtlasStyle();

  
  TString inpath=OUTDIR+"/ROC/"+SAMPLE;
 
  TFile FInput(inpath+"/Discriminator_"+TAG+".root","read");
  if(FInput.IsZombie()) return;

  TH1F HFrame("HFrame","",1,0.,1);
  HFrame.GetYaxis()->SetRangeUser(0,1);
  HFrame.GetYaxis()->SetTitle("p.u. efficiency");
  HFrame.GetXaxis()->SetTitle("h.s. efficiency");

  
  TTree * TREE=(TTree*)FInput.Get("tuple"); 
  if(!TREE){
    cout<<"No tuple"<<endl;
    return;
  }


  char legtxt[100];
  TCanvas C;
  for(long e=0;e<ETABINS;e++){ 
    //C.SetGrid();
    C.Clear();      
    
    HFrame.Draw();

    TLegend legend(0.19,0.53,0.45,0.65);  
    legend.SetTextSize(0.04);
    legend.SetFillColor(0);
    legend.SetFillStyle(0);
    legend.SetBorderSize(0);
    legend.SetLineColor(0);
    legend.SetLineStyle(0);

    for(long p=0;p<PTBINS;p++){ 

      char cuttxt[100];
      sprintf(cuttxt,"%.1f<eta&&eta<%.1f&&%.0f<pt&&pt<%.0f",
	      ETAMIN+(e)*(ETAMAX-ETAMIN)/ETABINS,
	      ETAMIN+(e+1)*(ETAMAX-ETAMIN)/ETABINS,
	      (float)(PTMIN+(p)*(PTMAX-PTMIN)/PTBINS)/1000.,
	      (float)(PTMIN+(p+1)*(PTMAX-PTMIN)/PTBINS)/1000.);


    
      cout<<e<<" "<<p<<" "<<cuttxt<<endl;

      TGraph * G=getROCGraph(TREE,cuttxt,1);
      if(!G){
	cout<<"getROCGraph failed"<<endl;
	return;
      }
      G->SetLineColor(JETTYPECOLOR[1]);
      G->SetLineWidth(2);
      G->SetLineStyle(p+1);
      G->Draw("lsame");

      sprintf(legtxt,"p_{T}: %.0f-%.0f GeV",
       	      (float)(PTMIN+(p)*(PTMAX-PTMIN)/PTBINS)/1000.,
       	      (float)(PTMIN+(p+1)*(PTMAX-PTMIN)/PTBINS)/1000.);
      //text.DrawLatexNDC(0.21,0.73,legtxt);

 
      legend.AddEntry(G,legtxt,"l");
      

    }
 
    text.SetTextSize(0.040);
    text.DrawLatexNDC(0.21,0.74,"Anti-k_{t} EM+JES (R=0.4) jets");  
    sprintf(legtxt,"%.1f<|#eta|<%.1f",
	    ETAMIN+(e)*(ETAMAX-ETAMIN)/ETABINS,
	    ETAMIN+(e+1)*(ETAMAX-ETAMIN)/ETABINS);
    text.DrawLatexNDC(0.21,0.68,legtxt);


    legend.Draw();
    drawROCBoundary();      
    DrawVBFH(inpath);  
    DrawECMSMu(inpath);
    DrawATLAS(0.21,0.88);
    DrawHGTD(inpath,0,-1,0.21,0.82); 
    C.Print(inpath+"/ROCvsPtEta_"+TAG+"_eta"+e+".png");
    C.Print(inpath+"/ROCvsPtEta_"+TAG+"_eta"+e+".pdf");
    C.Print(inpath+"/ROCvsPtEta_"+TAG+"_eta"+e+".eps");
  }



  gROOT->ProcessLine(".q");
}

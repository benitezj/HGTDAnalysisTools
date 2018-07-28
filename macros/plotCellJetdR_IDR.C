#include "HGTDAnalysisTools/macros/globals.h"

void plotCellJetdR_IDR(){
  TString inpath="/afs/cern.ch/user/b/benitezj/www/HGTDStudies/HitPlots";

  TString prod="VBFH125inv_LArHitDec13IDR";

  /////choose Geometry and mu
  //TString outpath=inpath+"/"+prod+"/LArHit_July26_mu0";
  //TString outpath=inpath+"/"+prod+"/LArHit_July26_Preshower_mu0";
  TString outpath=inpath+"/"+prod+"/LArHit_July26_mu200";
  //TString outpath=inpath+"/"+prod+"/LArHit_July26_Preshower_mu200";

  TString HistName="HdR";//"HdREw"


  SetAtlasStyle();
  setCuts(outpath);//some plotting things get set here

  ////Draw distributions
  TCanvas C;

  TString file="dRTemplates";
  TFile FILE(outpath+"/"+file+".root");
  if(FILE.IsZombie()) return;
  gROOT->cd();

  //grab the histo containing the number of jets
  TH2F * HNJet=(TH2F*)FILE.Get("HFillCelldRNJet");
  TH2F * HNJetPU=(TH2F*)FILE.Get("HFillCelldRNJetPU");
  if(!HNJet){cout<<"No HNJet"<<endl;return;}
  if(!HNJetPU){cout<<"No HNJetPU"<<endl;return;}



  // C.Clear();
  // HNJet->Draw("colz");
  // C.Print(outpath+"/CellJetdRvsEta_Njet"+plottag+".png");


  TString ytitle="#eta-#phi cell density";
  TString xtitle="R_{cell}";

  TLegend legend(0.73,0.70,0.92,0.90);
  legend.SetBorderSize(0);
  char legtext[100];

  int color;
  TH1F * HdREtaIncl[50];
  TF1 * FitFun[50];



  /////low eta
  int etastep=4;
  int minetaidx=26;
  int maxetaidx=34;
  for(long eta=minetaidx;eta<=maxetaidx;eta+=etastep){
    
    HdREtaIncl[eta]=(TH1F*)(FILE.Get(HistName+"_eta"+eta+"_pt30")->Clone());
    HdREtaIncl[eta]->Add((TH1F*)(FILE.Get(HistName+"_eta"+eta+"_pt50")));//add high pT

    ///merge two eta slices (dEta are made with 0.2)
    HdREtaIncl[eta]->Add((TH1F*)(FILE.Get(HistName+"_eta"+(eta+etastep/2)+"_pt30")));//add next eta slice
    HdREtaIncl[eta]->Add((TH1F*)(FILE.Get(HistName+"_eta"+(eta+etastep/2)+"_pt50")));//add next eta slice high pT
    
    //HdREtaIncl[eta]->Scale(1./HdREtaIncl[eta]->Integral());

    ///divide by area for each ring and by number of jets
    int etabinlow=1+(eta-24)/2;
    int etabinhigh=1+(eta+etastep-24)/2-1;// subtract 1 to include bin only before next slice
    cout<<"Norm range: "<<HNJet->GetXaxis()->GetBinLowEdge(etabinlow)
	<<","<<HNJet->GetXaxis()->GetBinLowEdge(etabinhigh)+HNJet->GetXaxis()->GetBinWidth(etabinhigh)
	<<"    "<<HNJet->GetYaxis()->GetBinLowEdge(1)
	<<","<<HNJet->GetYaxis()->GetBinLowEdge(2)+HNJet->GetYaxis()->GetBinWidth(2)
	<<endl;


    HdREtaIncl[eta]->Scale(1./HNJet->Integral(etabinlow,etabinhigh,1,2));
    //HdREtaIncl[eta]->Rebin(2);

    //HdREtaIncl[eta]->Divide(&HArea);
    dividedRDensityHisto(HdREtaIncl[eta]);


    
    /////////Fit 
    //FitFun[eta]=new TF1(TString("Fit")+eta,"[0]*exp(-[1]*x^0.5)",0,0.4);
    //HdREtaIncl[eta]->Fit(FitFun[eta]);

  }


  C.Clear();
  color=0;
  for(long eta=minetaidx;eta<=maxetaidx;eta+=etastep){
    HdREtaIncl[eta]->SetMarkerColor(ETACOLOR[color]);
    HdREtaIncl[eta]->SetLineColor(ETACOLOR[color]);

    if(eta==minetaidx){
      //HdREtaIncl[eta]->GetYaxis()->SetRangeUser(0,0.25);
      if(outpath.Contains("_mu0"))
	HdREtaIncl[eta]->GetYaxis()->SetRangeUser(0,outpath.Contains("Preshower") ? 9000 : 4000);
      else
	HdREtaIncl[eta]->GetYaxis()->SetRangeUser(0,outpath.Contains("Preshower") ? 18000 : 9000);
      HdREtaIncl[eta]->GetYaxis()->SetTitle(ytitle);
      HdREtaIncl[eta]->GetXaxis()->SetTitle(xtitle);      
      HdREtaIncl[eta]->Draw("histpe1");
    }  else  HdREtaIncl[eta]->Draw("histpe1same");
    
    sprintf(legtext,"%.1f<|#eta^{J}|<%.1f",eta/10.,(eta+etastep)/10.);
    legend.AddEntry(HdREtaIncl[eta],legtext,"lp");
    
    //FitFun[eta]->SetLineColor(6);
    //FitFun[eta]->Draw("lsame");

    color++;
  }


  legend.Draw();
  DrawATLAS(0.23,0.87);
  DrawHGTD(outpath,0,0,0.23,0.82);
  DrawECMSMu(outpath);

  text.SetTextSize(0.06);
  text.DrawLatexNDC(0.75,0.6,"h.s. jet");
  
  C.Print(outpath+"/CellJetdRvsEta_Sig"+plottag+".png");
  C.Print(outpath+"/CellJetdRvsEta_Sig"+plottag+".eps");
  C.Print(outpath+"/CellJetdRvsEta_Sig"+plottag+".pdf");



  /////////////////////////////////////////////
  /////Pileup
  /////////////////////////////////////////////
  TH1F * HdREtaInclPU[50];
  color=0;
  C.Clear();
  for(long eta=minetaidx;eta<=maxetaidx;eta+=etastep){
    HdREtaInclPU[eta]=(TH1F*)(FILE.Get(HistName+"PU_eta"+eta+"_pt30")->Clone());
    HdREtaInclPU[eta]->Add((TH1F*)(FILE.Get(HistName+"PU_eta"+eta+"_pt50")));
    
    ///merge two eta slices (dEta are made with 0.2)
    HdREtaInclPU[eta]->Add((TH1F*)(FILE.Get(HistName+"PU_eta"+(eta+etastep/2)+"_pt30")));//add next eta slice
    HdREtaInclPU[eta]->Add((TH1F*)(FILE.Get(HistName+"PU_eta"+(eta+etastep/2)+"_pt50")));//add next eta slice high pT

    ///
    HdREtaInclPU[eta]->SetMarkerColor(ETACOLOR[color]);
    HdREtaInclPU[eta]->SetLineColor(ETACOLOR[color]);

    ///divide by area for each ring and by number of jets
    int etabinlow=1+(eta-24)/2;
    int etabinhigh=1+(eta+etastep-24)/2-1;// subtract 1 to include bin only before next slice
    cout<<"Norm range: "<<HNJetPU->GetXaxis()->GetBinLowEdge(etabinlow)
	<<","<<HNJetPU->GetXaxis()->GetBinLowEdge(etabinhigh)+HNJetPU->GetXaxis()->GetBinWidth(etabinhigh)
	<<": "<<HNJetPU->GetYaxis()->GetBinLowEdge(1)
	<<","<<HNJetPU->GetYaxis()->GetBinLowEdge(2)+HNJetPU->GetYaxis()->GetBinWidth(2)
	<<endl;
    HdREtaInclPU[eta]->Scale(1./HNJetPU->Integral(etabinlow,etabinhigh,1,2));

    //HdREtaInclPU[eta]->Divide(&HArea);
    dividedRDensityHisto(HdREtaInclPU[eta]);

    if(eta==minetaidx){
      //HdREtaInclPU[eta]->GetYaxis()->SetRangeUser(0,0.25);

      if(outpath.Contains("_mu0"))
	HdREtaInclPU[eta]->GetYaxis()->SetRangeUser(0,outpath.Contains("Preshower") ? 1 : 1);
      else
	HdREtaInclPU[eta]->GetYaxis()->SetRangeUser(0,outpath.Contains("Preshower") ? 18000 : 9000);

      HdREtaInclPU[eta]->GetYaxis()->SetTitle(ytitle);
      HdREtaInclPU[eta]->GetXaxis()->SetTitle(xtitle);      
      HdREtaInclPU[eta]->Draw("histpe1");
    }  else  HdREtaInclPU[eta]->Draw("histpe1same");


    color++;
  }


  legend.Draw();
  DrawATLAS(0.23,0.87);
  DrawHGTD(outpath,0,0,0.23,0.82);
  DrawECMSMu(outpath);

  text.SetTextSize(0.06);
  text.DrawLatexNDC(0.75,0.6,"p.u. jet");
  C.Print(outpath+"/CellJetdRvsEta_PU"+plottag+".png");
  C.Print(outpath+"/CellJetdRvsEta_PU"+plottag+".eps");
  C.Print(outpath+"/CellJetdRvsEta_PU"+plottag+".pdf");




  ////////////////////////////////////////////////
  /////compare the signal and pu shapes
  //////////////////////////////////////////////
  C1D.Clear();

  for(long eta=minetaidx;eta<=maxetaidx;eta+=etastep){
    //HdREtaInclPU[eta]->Divide(HdREtaIncl[eta]);
    TH1F*hratio=makeHistRatio(HdREtaInclPU[eta],HdREtaIncl[eta]);
    if(eta==minetaidx){

      if(outpath.Contains("_mu0"))
	hratio->GetYaxis()->SetRangeUser(0,1);
      else
	hratio->GetYaxis()->SetRangeUser(0,2);

      hratio->GetXaxis()->SetTitle(xtitle);
      hratio->GetYaxis()->SetTitle("ratio");      
      hratio->Draw("histpe1");
    }  else  hratio->Draw("histpe1same");

  }


  legend.Draw();
  DrawATLAS(0.20,0.85);
  DrawECMSMu(outpath);

  //text.SetTextSize(0.045);
  //sprintf(legtext,"%.0f GeV<p_{T}^{J}<%.0f GeV",(float)minptidx,(float)(minptidx+npt*ptstep));
  //text.DrawLatexNDC(0.67,0.7,legtext);
  C1D.Print(outpath+"/CellJetdRvsEta_SigPURatio.png");




  gROOT->ProcessLine(".q");
}

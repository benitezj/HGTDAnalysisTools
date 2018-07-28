



void plotJetNHGTrksIDR(TChain*TREE,TString outpath){ 

  TString plottag="_Timing";
  if(outpath.Contains("Preshower"))
    plottag="_Preshower";

  if(outpath.Contains("_mu0"))
    plottag+="_mu0";
  else plottag+="_mu200";


  int nhitmin=0;
  int nhitmax=120;
  int nhitbins=12;

  TH1F HFrame("HFrame","",nhitbins,nhitmin,nhitmax);
  HFrame.GetXaxis()->SetTitle("number of tracks per jet");
  HFrame.GetYaxis()->SetTitle("Probability");
  HFrame.GetYaxis()->SetRangeUser(0,0.6);

  TLegend legend(0.7,0.7,0.92,0.9);
  legend.SetFillColor(0);
  legend.SetBorderSize(0);
  char legtext[100];

  TCanvas C;
  C.Clear();
  HFrame.Draw();
  int color=0;

  //low eta
  int etamin=10*ETAMIN;
  int etamax=10*ETAMAX;
  int etastep=0.5+10*(ETAMAX-ETAMIN)/ETABINS;
  for(long e=etamin;e<etamax;e+=etastep){

    TString selection=TString("(")+e+"<abs(jet_eta*10)&&abs(jet_eta*10)<"+(e+etastep)+")";

    ///test removing square corners (for Dirk)
    //TString selection=TString("(")+e+"<abs(jet_eta*10)&&abs(jet_eta*10)<"+(e+etastep)+"&&!(fabs(jet_phi+3*3.14159/4)<3.14159/8||fabs(jet_phi+3.14159/4)<3.14159/8||fabs(jet_phi-3.14159/4)<3.14159/8||fabs(jet_phi-3*3.14159/4)<3.14159/8))";
    
    cout<<e<<" "<<selection<<endl;

    TH1F * HEta=new TH1F(TString("HEta")+e,"",nhitbins,nhitmin,nhitmax);
    TREE->Draw(TString("jet_nhgtrk>>HEta")+e,signaljetCUT+"*"+JetPtCut+"*"+selection,"same");
    HEta->Scale(1./HEta->Integral());

    HEta->SetLineColor(ETACOLOR[color]);
    HEta->SetLineWidth(3);

    sprintf(legtext,"%.1f<|#eta^{J}|<%.1f",float(e)/10,float(e+etastep)/10.);
    legend.AddEntry(HEta,legtext,"l");

    color++;
  }

  legend.Draw();
  DrawATLAS(0.20,0.85);
  DrawHGTD(outpath,0,0,0.20,0.80);
  DrawECMSMu(outpath);

  text.SetTextSize(0.045);
  sprintf(legtext,"%.0f GeV<p_{T}^{J}<%.0f GeV",PTMIN/1000.,PTMAX/1000.);
  text.DrawLatexNDC(0.65,0.6,legtext);

  text.SetTextSize(0.045);
  text.DrawLatexNDC(0.80,0.50,"#DeltaR<0.2");

  C.Print(outpath+"/JetNHGTrkvsEta_IDR"+plottag+".png");

}


void drawJetHGTrkdEtadPhiLabels(TString outpath,TString label){
  DrawATLAS(0.2,0.835);
  DrawHGTD(outpath,0,0,0.185,0.76);
  DrawECMSMu(outpath,0.54,0.765);
  text.DrawLatexNDC(0.69,0.25,label);
  text.DrawLatexNDC(0.63,0.2,TString("")+etamin+"<|#eta^{J}|<"+etamax);
}

void printJetHGTrkdEtadPhi(TChain*TREE,TString outpath){

 TCanvas C("C","",700,700);
  C.SetRightMargin(0.18);
  C.SetTopMargin(0.18);
  C.SetLeftMargin(0.17);
  C.SetBottomMargin(0.17);

  TString xtitle="#phi^{trk} - #phi^{J}";
  TString ytitle="|#eta^{trk}| - |#eta^{J}|";
  TString ztitle="avg. # of tracks per jet / (0.02x0.02)";
  float titlesize=0.05;
  
  int nbins=50;
  float min=-0.5;
  float max=0.5;

  TH2F HHGTrkdEtadPhiEta("HHGTrkdEtadPhiEta","",nbins,min,max,nbins,min,max);
  HHGTrkdEtadPhiEta.GetXaxis()->SetTitle(xtitle);
  HHGTrkdEtadPhiEta.GetYaxis()->SetTitle(ytitle);
  HHGTrkdEtadPhiEta.GetZaxis()->SetTitle(ztitle);
  HHGTrkdEtadPhiEta.GetXaxis()->SetTitleSize(titlesize);
  HHGTrkdEtadPhiEta.GetYaxis()->SetTitleSize(titlesize);
  HHGTrkdEtadPhiEta.GetZaxis()->SetTitleSize(0.8*titlesize);
  HHGTrkdEtadPhiEta.GetXaxis()->SetTitleOffset(1.1);
  HHGTrkdEtadPhiEta.GetYaxis()->SetTitleOffset(1.1);
  HHGTrkdEtadPhiEta.GetZaxis()->SetTitleOffset(1.2);
  HHGTrkdEtadPhiEta.GetXaxis()->SetLabelSize(0.7*titlesize);
  HHGTrkdEtadPhiEta.GetYaxis()->SetLabelSize(0.7*titlesize);
  HHGTrkdEtadPhiEta.GetZaxis()->SetLabelSize(0.6*titlesize);


  TH1F HJetsNorm("HJetsNorm","",100,-10,10);

 
  ///Signal jet
  HHGTrkdEtadPhiEta.Scale(0.);
  HJetsNorm.Scale(0.);
  TREE->Draw(dEtaHGTrk+":"+dPhiHGTrk+">>HHGTrkdEtadPhiEta",CUTSignalJetHGTrk+"*"+CUTPtJetHGTrk+"*"+CUTEtaJetHGTrk);
  TREE->Draw("jet_eta>>HJetsNorm",signaljetCUT+"*"+JetPtCut+"*"+JetEtaCut);
  HHGTrkdEtadPhiEta.Scale(1./HJetsNorm.Integral());
  C.Clear();
  HHGTrkdEtadPhiEta.Draw("colz");
  drawJetHGTrkdEtadPhiLabels(outpath,"h.s. jet");
  C.Print(outpath+"_Sig.png");


  //PU jet
  HHGTrkdEtadPhiEta.Scale(0.);
  HJetsNorm.Scale(0.);
  TREE->Draw(dEtaHGTrk+":"+dPhiHGTrk+">>HHGTrkdEtadPhiEta",CUTPUJetHGTrk+"*"+CUTPtJetHGTrk+"*"+CUTEtaJetHGTrk);
  HJetsNorm.Scale(0.);
  TREE->Draw("jet_eta>>HJetsNorm",pujetCUT+"*"+JetPtCut+"*"+JetEtaCut);
  HHGTrkdEtadPhiEta.Scale(1./HJetsNorm.Integral());
  C.Clear();
  HHGTrkdEtadPhiEta.Draw("colz");
  drawJetHGTrkdEtadPhiLabels(outpath,"p.u. jet");
  C.Print(outpath+"_PU.png");


}



void plotJetHGTrkdEtadPhiIDR(TChain*TREE,TString outpath){//2D hit distribution w.r.t jet axis

  TString plottag="_Timing";
  if(outpath.Contains("Preshower"))
    plottag="_Preshower";

  if(outpath.Contains("_mu0"))
    plottag+="_mu0";
  else plottag+="_mu200";


  ////Low eta
  etamin="2.8";
  etamax="3.0";
  timecut="0.180";
  setCuts(outpath);
  printJetHGTrkdEtadPhi(TREE,outpath+"/JetHGTrkdEtadPhi_LowEta_IDR"+plottag);

  ////med eta
  etamin="3.2";
  etamax="3.4";
  timecut="0.120";
  setCuts(outpath);
  printJetHGTrkdEtadPhi(TREE,outpath+"/JetHGTrkdEtadPhi_MedEta_IDR"+plottag);

  ////High eta
  etamin="3.6";
  etamax="3.8";
  timecut="0.090";
  setCuts(outpath);
  printJetHGTrkdEtadPhi(TREE,outpath+"/JetHGTrkdEtadPhi_HighEta_IDR"+plottag);


}



///////////////////////////////////////////
//// Density plots
/////////////////////////////////////////
void plotJetHGTrkdR(TChain*TREE,TString outpath){

  TString plottag="_Timing";
  if(outpath.Contains("Preshower"))
    plottag="_Preshower";

  if(outpath.Contains("_mu0"))
    plottag+="_mu0";
  else plottag+="_mu200";


  TString Sel="hgtrk_jet>=0&&jet_truth_pt[hgtrk_jet]>0";
  TString SelPU="hgtrk_jet>=0&&jet_truth_pt[hgtrk_jet]<=0";

  TString SeldEtadPhi="(1)";
  //TString SeldEtadPhi=TString("((abs(")+dEtadHGTrk+")<0.5 && abs("+dPhidHGTrk+")<0.5)||hgtrk_deta==0.)";
  //TString SeldEtadPhi="(hgtrk_deta==0.)";
  
  
  long minetaidx=26;
  long maxetaidx=38;
  int neta=3;
  long etastep=(maxetaidx-minetaidx)/neta;
  long minptidx=30;
  long maxptidx=70;
  int npt=1;
  long ptstep=(maxptidx-minptidx)/npt;

  TH2F HNJet("HNJet","",neta,minetaidx/10.,maxetaidx/10.,
	     npt,minptidx,maxptidx);
  
  TH2F HNJetPU("HNJetPU","",neta,minetaidx/10.,maxetaidx/10.,
	       npt,minptidx,maxptidx);

  
  if(outpath.Contains("_mu0")){//for mu=0 jet calibration does not apply
    TREE->Draw("jet_truth_pt/1000.:abs(jet_eta)>>HNJet","jet_truth_pt>0");
  }else{
    TREE->Draw("jet_pt/1000.:abs(jet_eta)>>HNJet","jet_truth_pt>0");
    TREE->Draw("jet_pt/1000.:abs(jet_eta)>>HNJetPU","jet_truth_pt<=0");
  }


  ////Fill distributions
  TH1F * HdREta[20][20];
  TH1F * HdREtaPU[20][20];
  for(long eta=0;eta<neta;eta++){
    for(long pt=0;pt<npt;pt++){

      TString ptetaselection=TString("((")+(minetaidx+eta*etastep)+"/10.)<abs(jet_eta[hgtrk_jet])&&abs(jet_eta[hgtrk_jet])<("+(minetaidx+(eta+1)*etastep)+"/10.))"+"&&(("+(minptidx+pt*ptstep)+"*1000.)<jet_pt[hgtrk_jet]&&jet_pt[hgtrk_jet]<("+(minptidx+(pt+1)*ptstep)+"*1000.))";

      //for mu=0 jet calibration does not apply
      if(outpath.Contains("_mu0"))
	ptetaselection=TString("((")+(minetaidx+eta*etastep)+"/10.)<abs(jet_eta[hgtrk_jet])&&abs(jet_eta[hgtrk_jet])<("+(minetaidx+(eta+1)*etastep)+"/10.))"+"&&(("+(minptidx+pt*ptstep)+"*1000.)<jet_truth_pt[hgtrk_jet]&&jet_truth_pt[hgtrk_jet]<("+(minptidx+(pt+1)*ptstep)+"*1000.))";
      
      
      HdREta[eta][pt]=new TH1F(TString("HdR_eta")+eta+"_pt"+pt,"",10,.0,.5);
      TREE->Draw(TString("hgtrk_jetdR>>HdR_eta")+eta+"_pt"+pt,Sel+"&&"+ptetaselection+"&&"+SeldEtadPhi);
      
      HdREtaPU[eta][pt]=new TH1F(TString("HdRPU_eta")+eta+"_pt"+pt,"",10,.0,.5);
      TREE->Draw(TString("hgtrk_jetdR>>HdRPU_eta")+eta+"_pt"+pt,SelPU+"&&"+ptetaselection+"&&"+SeldEtadPhi);
      
    }
  }


  ////Plot
  TCanvas C;
  TString ytitle="#eta-#phi track density";
  TString xtitle="R_{trk}";

  TLegend legend(0.73,0.70,0.92,0.90);
  legend.SetBorderSize(0);
  char legtext[100];

  int color;
  TH1F * HdREtaIncl[50];
  TF1 * FitFun[50];

  TH1F HArea("HArea","",10,0,0.5);
  HArea.SetBinContent(1,TMath::Pi()*pow(0.05,2));
  HArea.SetBinContent(2,TMath::Pi()*(pow(0.10,2)-pow(0.05,2)));
  HArea.SetBinContent(3,TMath::Pi()*(pow(0.15,2)-pow(0.10,2)));
  HArea.SetBinContent(4,TMath::Pi()*(pow(0.20,2)-pow(0.15,2)));
  HArea.SetBinContent(5,TMath::Pi()*(pow(0.25,2)-pow(0.20,2)));
  HArea.SetBinContent(6,TMath::Pi()*(pow(0.30,2)-pow(0.25,2)));
  HArea.SetBinContent(7,TMath::Pi()*(pow(0.35,2)-pow(0.30,2)));
  HArea.SetBinContent(8,TMath::Pi()*(pow(0.40,2)-pow(0.35,2)));
  HArea.SetBinContent(9,TMath::Pi()*(pow(0.45,2)-pow(0.40,2)));
  HArea.SetBinContent(10,TMath::Pi()*(pow(0.50,2)-pow(0.45,2)));


  ////Signal jets
  C.Clear();
  color=0;
  for(long eta=0;eta<neta;eta++){
    for(long pt=0;pt<npt;pt++){
      
      //per jet
      if(HNJet.GetBinContent(eta+1,pt+1)>0)
	HdREta[eta][pt]->Scale(1./HNJet.GetBinContent(eta+1,pt+1));
      else 	HdREta[eta][pt]->Scale(0);

      //density
      HdREta[eta][pt]->Divide(&HArea);

      /////color
      HdREta[eta][pt]->SetMarkerColor(ETACOLOR[color]);
      HdREta[eta][pt]->SetLineColor(ETACOLOR[color]);


      ///////////////////////////////
      /////// PU Jets
      //per jet
      if(HNJetPU.GetBinContent(eta+1,pt+1)>0)
	HdREtaPU[eta][pt]->Scale(1./HNJetPU.GetBinContent(eta+1,pt+1));
      else   HdREtaPU[eta][pt]->Scale(0);

      //density
      HdREtaPU[eta][pt]->Divide(&HArea);

      /////color
      HdREtaPU[eta][pt]->SetMarkerColor(ETACOLOR[color]);
      HdREtaPU[eta][pt]->SetLineColor(ETACOLOR[color]);
      //HdREtaPU[eta][pt]->SetMarkerSize(0);


      ///////////////////////////////
      if(eta==0){
	if(outpath.Contains("_mu0"))
	  HdREta[eta][pt]->GetYaxis()->SetRangeUser(0,outpath.Contains("Preshower") ? 0 : 800);
	else
	  HdREta[eta][pt]->GetYaxis()->SetRangeUser(0,outpath.Contains("Preshower") ? 0 : 1400);
	HdREta[eta][pt]->GetYaxis()->SetTitle(ytitle);
	HdREta[eta][pt]->GetXaxis()->SetTitle(xtitle);      
	HdREta[eta][pt]->Draw("histpe1");
	//HdREtaPU[eta][pt]->Draw("histpe1same");
      }  else {
	HdREta[eta][pt]->Draw("histpe1same");
	//HdREtaPU[eta][pt]->Draw("histpe1same");
      }
    
      sprintf(legtext,"%.1f<|#eta^{J}|<%.1f",(minetaidx+eta*etastep)/10.,(minetaidx+(eta+1)*etastep)/10.);
      legend.AddEntry(HdREta[eta][pt],legtext,"lp");
    
      //FitFun[eta]->SetLineColor(6);
      //FitFun[eta]->Draw("lsame");

      color++;
    }
  }

  legend.Draw();
  DrawATLAS(0.23,0.87);
  DrawHGTD(outpath,0,0,0.23,0.82);
  DrawECMSMu(outpath);

  text.SetTextSize(0.06);
  text.DrawLatexNDC(0.75,0.6,"h.s. jet");
  
  C.Print(outpath+"/HGTrkJetdRvsEta_Sig"+plottag+".png");
  //C.Print(outpath+"/HGTrkJetdRvsEta_Sig"+plottag+".eps");
  //C.Print(outpath+"/HGTrkJetdRvsEta_Sig"+plottag+".pdf");



  ////PU jets
  C.Clear();
  color=0;
  for(long eta=0;eta<neta;eta++){
    for(long pt=0;pt<npt;pt++){
      
      if(eta==0){
	if(outpath.Contains("_mu0"))
	  HdREtaPU[eta][pt]->GetYaxis()->SetRangeUser(0,outpath.Contains("Preshower") ? 0 : 0);
	else
	  HdREtaPU[eta][pt]->GetYaxis()->SetRangeUser(0,outpath.Contains("Preshower") ? 0 : 1400);
	HdREtaPU[eta][pt]->GetYaxis()->SetTitle(ytitle);
	HdREtaPU[eta][pt]->GetXaxis()->SetTitle(xtitle);      
	HdREtaPU[eta][pt]->Draw("histpe1");
      }  else  HdREtaPU[eta][pt]->Draw("histpe1same");
    
      //sprintf(legtext,"%.1f<|#eta^{J}|<%.1f",(minetaidx+eta*etastep)/10.,(minetaidx+(eta+1)*etastep)/10.);
      //legend.AddEntry(HdREtaPU[eta][pt],legtext,"lp");
    
      //FitFun[eta]->SetLineColor(6);
      //FitFun[eta]->Draw("lsame");

      color++;
    }
  }

  legend.Draw();
  DrawATLAS(0.23,0.87);
  DrawHGTD(outpath,0,0,0.23,0.82);
  DrawECMSMu(outpath);

  text.SetTextSize(0.06);
  text.DrawLatexNDC(0.75,0.6,"p.u. jet");
  
  C.Print(outpath+"/HGTrkJetdRvsEta_PU"+plottag+".png");


}

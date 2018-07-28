
/////////////////////////////////////////////////////////////////
///////////Trigger
/////////////////////////////////////////////////////////////////
void plotTrigger(TChain*TREE,TString outpath){
  TCanvas C;
  C.SetRightMargin(0.15);

  ///Tile sizes
  TH1F HTileSize("HTileSize","",200,0,200);  
  HTileSize.GetXaxis()->SetTitle(" tile size   [mm] ");
  TREE->Draw("trig_size>>HTileSize");
  C.Clear();
  HTileSize.Draw();
  C.Print(outpath+"/Trig_tilesize.png");


  ///////////////////////////////////////
  /// # of triggered tiles
  ///////////////////////////////////
  TH1F HNTrig5("HNTrig5","",90,0,450);    TREE->Draw("ntrig_5>>HNTrig5");   HNTrig5.Scale(1./HNTrig5.Integral());  HNTrig5.SetLineColor(1);
  TH1F HNTrig10("HNTrig10","",90,0,450);  TREE->Draw("ntrig_10>>HNTrig10"); HNTrig10.Scale(1./HNTrig10.Integral()); HNTrig10.SetLineColor(2);
  TH1F HNTrig15("HNTrig15","",90,0,450);  TREE->Draw("ntrig_15>>HNTrig15"); HNTrig15.Scale(1./HNTrig15.Integral()); HNTrig15.SetLineColor(3);
  TH1F HNTrig20("HNTrig20","",90,0,450);  TREE->Draw("ntrig_20>>HNTrig20"); HNTrig20.Scale(1./HNTrig20.Integral()); HNTrig20.SetLineColor(4);
  TH1F HNTrig25("HNTrig25","",90,0,450);  TREE->Draw("ntrig_25>>HNTrig25"); HNTrig25.Scale(1./HNTrig25.Integral()); HNTrig25.SetLineColor(5);
  //TH1F HNTrig30("HNTrig30","",90,0,450);  TREE->Draw("ntrig_30>>HNTrig30"); HNTrig30.Scale(1./HNTrig30.Integral()); HNTrig30.SetLineColor(6);
  TLegend legend(0.83,0.60,0.97,0.92);
  legend.SetFillColor(0);
  legend.AddEntry(&HNTrig5,"# hits>5","l");
  legend.AddEntry(&HNTrig10,"# hits>10","l");
  legend.AddEntry(&HNTrig15,"# hits>15","l");
  legend.AddEntry(&HNTrig20,"# hits>20","l");
  legend.AddEntry(&HNTrig25,"# hits>25","l");
  //legend.AddEntry(&HNTrig30,"# hits>30","l");

  C.Clear();
  HNTrig5.GetXaxis()->SetTitle("# of triggered tiles");
  HNTrig5.GetYaxis()->SetTitle("unit norm");
  HNTrig5.GetYaxis()->SetRangeUser(0,1.1);
  HNTrig5.Draw("hist");
  HNTrig10.Draw("histsame");
  HNTrig15.Draw("histsame");
  HNTrig20.Draw("histsame");
  HNTrig25.Draw("histsame");
  //HNTrig30.Draw("histsame");
  legend.Draw();
  C.Print(outpath+"/Trig_ntrig_vs_nhit.png");
  
}



void plotTriggerCut(TChain*TREE,TString outpath,TString trigsel,float eff,TString plottag){
  TCanvas C;
  C.SetRightMargin(0.15);

  int nr=62; int rmin=0, rmax=620;
  TH2F HNHitVsR("HNHitVsR","",nr,rmin,rmax,300,-0.5,299.5);
  HNHitVsR.GetXaxis()->SetTitle("tile radius   [mm]");
  HNHitVsR.GetYaxis()->SetTitle("# of hits on peak");
  HNHitVsR.GetZaxis()->SetTitle("# of jets");
  TREE->Draw("trig_nhit_peak:trig_r>>HNHitVsR",TriggerTruthSelection+"*"+trigsel);//+"*(trig_nhit_peak>"+TriggerCut70+")");

  //search for the cuts to be applied vs radius
  TH1F HTrigR("HTrigR","",nr,rmin,rmax);
  TREE->Draw("trig_r>>HTrigR",TriggerTruthSelection+"*"+trigsel);
  TH1F HCut("HCut","",nr,rmin,rmax);
  for(int r=1;r<=HNHitVsR.GetNbinsX();r++){
    if(HTrigR.GetBinContent(r)==0) continue;

    for(int n=1;n<=HNHitVsR.GetNbinsY();n++){
      HCut.AddBinContent(r,HNHitVsR.GetBinContent(r,n)/(float)(HTrigR.GetBinContent(r)));
      if(HCut.GetBinContent(r)>(1-eff)){
	HCut.SetBinContent(r,n-1);
	break;
      }
    }

  }


  C.Clear();
  C.SetLogy(1);
  C.SetLogz(1);
  HNHitVsR.Draw("colz");
  HCut.Draw("histsame");
  C.Print(outpath+"/Trig_nhit_vs_r_"+plottag+".png");

  char CutTxt[100];

  TString CutString=trigsel+"*(";
  for(int r=1;r<=HCut.GetNbinsX();r++){
    if(HCut.GetBinContent(r)<1) continue;
    sprintf(CutTxt,"%.0f*(%.0f<trig_r&&trig_r<=%.0f)",HCut.GetBinContent(r),HCut.GetBinLowEdge(r),HCut.GetBinLowEdge(r)+HCut.GetBinWidth(r));
    if(CutString.CompareTo(trigsel+"*(")==0) CutString=CutString+CutTxt;
    else CutString=CutString+"+"+CutTxt;
  }
  CutString+=")";
  cout<<"TriggerCut=\""<<CutString<<"\";"<<endl;

  

}


TH2F* getTrigger2D(TString name, TString ztitle,int Q=1){

  int nxybins=40;//full , fine binning use 130
  int xymin=-650;
  int xymax=650;
  if(Q==1){///Q1 only
    nxybins=20;//fine binning use 65
    xymin=0;
    xymax=650;
  }

  TH2F * HTrigger2D = new TH2F(name,"",nxybins,xymin,xymax,nxybins,xymin,xymax); 
  HTrigger2D->GetXaxis()->SetTitle("x [mm]"); 
  HTrigger2D->GetYaxis()->SetTitle("y [mm]");
  HTrigger2D->GetZaxis()->SetTitle(ztitle);

  HTrigger2D->GetXaxis()->SetNdivisions(7);
  HTrigger2D->GetYaxis()->SetNdivisions(9);
  HTrigger2D->GetZaxis()->SetLabelSize(0.035);
  HTrigger2D->GetZaxis()->SetLabelSize(0.035);
  HTrigger2D->GetXaxis()->SetLabelSize(0.04);
  HTrigger2D->GetYaxis()->SetLabelSize(0.04);

  return HTrigger2D;
}

void plotTrigger2D(TH2F* H2, TString outpath,int Q=1,TString option="colz"){

  TCanvas C("C","",800,800);
  C.SetRightMargin(0.20);
  C.SetLeftMargin(0.15);
  C.SetTopMargin(0.20);
  C.SetBottomMargin(0.15);

  C.Clear();    
  
  if(option.Contains("text"))
    gStyle->SetPaintTextFormat("2.0f");

  H2->Draw(option.Data());


  if(Q==1){
    drawTriggerTiles(1,0);
  }else{
    drawTriggerTiles(1,1);
    drawTriggerTiles(-1,1);
  }
  DrawATLAS(0.18,0.82);
  DrawHGTD(outpath,0,0,0.17,0.755);
  //DrawVBFH(outpath);
  C.Print(outpath);

}


void plotTriggerDiagnostics(TChain*TREE,TString outpath){

  TString tilesel="(trig_r<=600)";
  TString trigxy="abs(trig_y):abs(trig_x)";
  //TString trigxy="trig_y:trig_x";


  ////Geometric quantities
  TH2F * HTrigger2DNormGeo = getTrigger2D("HTrigger2DNormGeo","");
  TREE->Draw(trigxy+">>HTrigger2DNormGeo",tilesel); 

  TH2F * HTrigger2DR = getTrigger2D("HTrigger2DR","radius");
  TREE->Draw(trigxy+">>HTrigger2DR",tilesel+"*(trig_r)"); 
  HTrigger2DR->Divide(HTrigger2DNormGeo);
  plotTrigger2D(HTrigger2DR,outpath+"/Trig_r.png");

  TH2F * HTrigger2DX = getTrigger2D("HTrigger2DX","X");
  TREE->Draw(trigxy+">>HTrigger2DX",tilesel+"*(abs(trig_x))"); 
  HTrigger2DX->Divide(HTrigger2DNormGeo);
  plotTrigger2D(HTrigger2DX,outpath+"/Trig_x.png");

  TH2F * HTrigger2DY = getTrigger2D("HTrigger2DY","Y");
  TREE->Draw(trigxy+">>HTrigger2DY",tilesel+"*(abs(trig_y))"); 
  HTrigger2DY->Divide(HTrigger2DNormGeo);
  plotTrigger2D(HTrigger2DY,outpath+"/Trig_y.png");

  TH2F * HTrigger2DdEta = getTrigger2D("HTrigger2DdEta","tile size");
  TREE->Draw(trigxy+">>HTrigger2DdEta",tilesel+"*(trig_sizeeta/2)");
  HTrigger2DdEta->Divide(HTrigger2DNormGeo);
  plotTrigger2D(HTrigger2DdEta,outpath+"/Trig_dEta.png");

  ////
  TH2F * HTrigger2DNormTruth = getTrigger2D("HTrigger2DNormTruth","");
  TREE->Draw(trigxy+">>HTrigger2DNormTruth",TriggerTruthSelection); 

  TH2F * HTrigger2DNhit = getTrigger2D("HTrigger2DNhit","# of hits");
  TREE->Draw(trigxy+">>HTrigger2DNhit",TriggerTruthSelection+"*(trig_nhit)"); 
  HTrigger2DNhit->Divide(HTrigger2DNormTruth);
  plotTrigger2D(HTrigger2DNhit,outpath+"/Trig_nhit.png");

  TH2F * HTrigger2DNhit0 = getTrigger2D("HTrigger2DNhit0","# of 0 hits");
  TREE->Draw(trigxy+">>HTrigger2DNhit0",TriggerTruthSelection+"*(trig_nhit==0)"); 
  HTrigger2DNhit0->Divide(HTrigger2DNormTruth);
  plotTrigger2D(HTrigger2DNhit0,outpath+"/Trig_nhit0.png");

  TH2F * HTrigger2DNhitPeak = getTrigger2D("HTrigger2DNhitPeak","# of peak hits");
  TREE->Draw(trigxy+">>HTrigger2DNhitPeak",TriggerTruthSelection+"*(trig_nhit_peak)"); 
  HTrigger2DNhitPeak->Divide(HTrigger2DNormTruth);
  plotTrigger2D(HTrigger2DNhitPeak,outpath+"/Trig_nhitpeak.png");

}

void plotTriggerProbabilities2D(TChain*TREE,TString outpath){

  TString trigxy="abs(trig_y):abs(trig_x)";
  //TString trigxy="trig_y:trig_x";
  cout<<TriggerCut<<endl;

  ///show the cut being applied
  TH2F * HTrigger2DNorm = getTrigger2D("HTrigger2DNorm","");
  TREE->Draw(trigxy+">>HTrigger2DNorm","(trig_r<=600)"); 

  TH2F * HTrigger2DCut= getTrigger2D("HTrigger2DCut","cut");
  TREE->Draw(trigxy+">>HTrigger2DCut",TriggerCut+"*(trig_r<=600)"); 
  HTrigger2DCut->Divide(HTrigger2DNorm);
  HTrigger2DCut->GetZaxis()->SetRangeUser(0,outpath.Contains("Preshower")?200:100);
  plotTrigger2D(HTrigger2DCut,outpath+"/Trig_cut.png",1,"text");


  /////////////////////////////////////////////////
  ///show the signal trigger probability
  ////////////////////////////////////////////////
  TH2F * HTrigger2DGen = getTrigger2D("HTrigger2DGen","# of gen. jets");
  TREE->Draw(trigxy+">>HTrigger2DGen",TriggerTruthSelection); 
  plotTrigger2D(HTrigger2DGen,outpath+"/Trig_njet.png");

  TH2F * HTrigger2D = getTrigger2D("HTrigger2D","trigger prob. per signal jet (%)");
  TREE->Draw(trigxy+">>HTrigger2D",TriggerTruthSelection+"*(trig_nhit_peak>"+TriggerCut+")"); 
  HTrigger2D->Divide(HTrigger2DGen);
  HTrigger2D->Scale(100);
  plotTrigger2D(HTrigger2D,outpath+"/Trig_probability.png",1,"text");


  ///////////////////////////////////////////////
  //show the pu tile trigger probability
  //////////////////////////////////////////////
  TString PUTileSelection="(trig_r<=600&&(trig_jet==-1||jet_truthJ[trig_jet]==-1))";
  TH2F * HTrigger2DPUTrigProbNorm = getTrigger2D("HTrigger2DPUTrigProbNorm","");
  TREE->Draw(trigxy+">>HTrigger2DPUTrigProbNorm",PUTileSelection); 

  TH2F * HTrigger2DPUTrigProb= getTrigger2D("HTrigger2DPUTrigProb","trigger prob. per event (%)");
  TREE->Draw(trigxy+">>HTrigger2DPUTrigProb",PUTileSelection+"*(trig_nhit_peak>"+TriggerCut+")"); 
  HTrigger2DPUTrigProb->Divide(HTrigger2DPUTrigProbNorm);
  HTrigger2DPUTrigProb->Scale(100);
  plotTrigger2D(HTrigger2DPUTrigProb,outpath+"/Trig_pu_probability.png",1,"text");



  // ///////////////////////////////////////////////
  // //show the pu tile trigger probability, combined with calo jets > 30GeV
  // //////////////////////////////////////////////
  // TH2F * HTrigger2DPUTrigCaloJetProb= getTrigger2D("HTrigger2DPUTrigCaloJetProb","trigger prob. per event (%)");
  // TREE->Draw(trigxy+">>HTrigger2DPUTrigCaloJetProb",TString("(trig_r<=600&&trig_jet>=0)*(trig_nhit_peak>")+TriggerCut+")"); 
  // HTrigger2DPUTrigCaloJetProb->Divide(HTrigger2DPUTrigProbNorm);
  // HTrigger2DPUTrigCaloJetProb->Scale(100);
  // plotTrigger2D(HTrigger2DPUTrigCaloJetProb,outpath+"/Trig_pu_calojet30_probability.png");


  // ///////////////////////////////////////////////
  // //show the pu tile trigger probability, combined with calo jets > 50GeV
  // //////////////////////////////////////////////
  // TH2F * HTrigger2DPUTrigCaloJet50Prob= getTrigger2D("HTrigger2DPUTrigCaloJet50Prob","trigger prob. per event (%)");
  // TREE->Draw(trigxy+">>HTrigger2DPUTrigCaloJet50Prob",TString("(trig_r<=600&&trig_jet>=0&&jet_pt[trig_jet]>50000)*(trig_nhit_peak>")+TriggerCut+")"); 
  // HTrigger2DPUTrigCaloJet50Prob->Divide(HTrigger2DPUTrigProbNorm);
  // HTrigger2DPUTrigCaloJet50Prob->Scale(100);
  // plotTrigger2D(HTrigger2DPUTrigCaloJet50Prob,outpath+"/Trig_pu_calojet50_probability.png");



}


void plotTriggerJets(TChain*TREE,TString outpath){
  TCanvas C;

  /////////////////////////////
  ///# of calo jets 
  ////////////////////////////
  int nbins=15;
  float ptlow=0;
  float pthigh=150;
  TString xtitle=" Jet p_{T}   [GeV] ";
  TString ytitle="number of jets per event / 10 GeV";

  TH1F HFrame("HFrame","",nbins,ptlow,pthigh);  
  HFrame.GetXaxis()->SetTitle(xtitle);
  HFrame.GetYaxis()->SetTitle(ytitle);

  TLegend leg(0.6,0.7,0.9,0.9);
  leg.SetFillColor(0);
  leg.SetFillStyle(0);
  leg.SetBorderSize(0);
  leg.SetTextSize(0.05);


  /////////////////////////
  ///This is for low eta
  TH1F HJetPtLowEta("HJetPtLowEta","",nbins,ptlow,pthigh);  
  HJetPtLowEta.Sumw2();
  HJetPtLowEta.GetXaxis()->SetTitle(xtitle);
  HJetPtLowEta.GetYaxis()->SetTitle(ytitle);
  TREE->Draw("jet_pt/1000>>HJetPtLowEta","jet_truthJ>=0&&abs(jet_eta)>2.6&&abs(jet_eta)<3.2");
  HJetPtLowEta.Scale(1./TREE->GetEntries());
  TH1F HJetPtLowEtaPU("HJetPtLowEtaPU","",nbins,ptlow,pthigh);  
  HJetPtLowEtaPU.Sumw2();
  HJetPtLowEtaPU.GetXaxis()->SetTitle(xtitle);
  HJetPtLowEtaPU.GetYaxis()->SetTitle(ytitle);
  TREE->Draw("jet_pt/1000>>HJetPtLowEtaPU","jet_truthJ<0&&abs(jet_eta)>2.6&&abs(jet_eta)<3.2");
  HJetPtLowEtaPU.Scale(1./TREE->GetEntries());

  // leg.AddEntry(&HJetPtLowEta,"h.s. jet","p");
  // leg.AddEntry(&HJetPtLowEtaPU,"p.u. jet","l");
  // C.Clear();
  // HFrame.GetYaxis()->SetRangeUser(0,HJetPtLowEtaPU.GetMaximum()*1.2);
  // HFrame.Draw("hist");
  // HJetPtLowEta.Draw("histpsame");
  // HJetPtLowEtaPU.Draw("histsame");
  // leg.Draw();
  // C.Print(outpath+"/Trig_JetPt_LowEta_PUandSig.png");

  HJetPtLowEta.SetTitle("h.s. jet");
  HJetPtLowEtaPU.SetTitle("p.u. jet");
  compareWithRatio(&HJetPtLowEta,&HJetPtLowEtaPU, outpath+"/Trig_JetPt_LowEta_PUandSig",0.,ptlow,pthigh,0);
  

  /////////////////////////
  ///This is for high eta
  TH1F HJetPtHighEta("HJetPtHighEta","",nbins,ptlow,pthigh);  
  HJetPtHighEta.Sumw2();
  HJetPtHighEta.GetXaxis()->SetTitle(xtitle);
  HJetPtHighEta.GetYaxis()->SetTitle(ytitle);  TREE->Draw("jet_pt/1000>>HJetPtHighEta","jet_truthJ>=0&&abs(jet_eta)>3.2&&abs(jet_eta)<4.0");
  HJetPtHighEta.Scale(1./TREE->GetEntries());


  TH1F HJetPtHighEtaPU("HJetPtHighEtaPU","",nbins,ptlow,pthigh);
  HJetPtHighEtaPU.Sumw2();
  HJetPtHighEtaPU.GetXaxis()->SetTitle(xtitle);
  HJetPtHighEtaPU.GetYaxis()->SetTitle(ytitle);
  TREE->Draw("jet_pt/1000>>HJetPtHighEtaPU","jet_truthJ<0&&abs(jet_eta)>3.2&&abs(jet_eta)<4.0");
  HJetPtHighEtaPU.Scale(1./TREE->GetEntries());

  // C.Clear();
  // HFrame.GetYaxis()->SetRangeUser(0,HJetPtHighEtaPU.GetMaximum()*1.2);
  // HFrame.Draw("hist");
  // HJetPtHighEta.Draw("histpsame");
  // HJetPtHighEtaPU.Draw("histsame");
  // leg.Draw();
  // C.Print(outpath+"/Trig_JetPt_HighEta_PUandSig.png");

  HJetPtHighEta.SetTitle("h.s. jet");
  HJetPtHighEtaPU.SetTitle("p.u. jet");
  compareWithRatio(&HJetPtHighEta,&HJetPtHighEtaPU, outpath+"/Trig_JetPt_HighEta_PUandSig",0.,ptlow,pthigh,0);



  /////////////////////////
  ///This is for low eta with trigger selection
  TH1F HJetPtLowEtaTrig("HJetPtLowEtaTrig","",nbins,ptlow,pthigh);  
  HJetPtLowEtaTrig.Sumw2();
  HJetPtLowEtaTrig.GetXaxis()->SetTitle(xtitle);
  HJetPtLowEtaTrig.GetYaxis()->SetTitle(ytitle);
  TREE->Draw("jet_pt/1000>>HJetPtLowEtaTrig",TString("jet_truthJ>=0&&abs(jet_eta)>2.6&&abs(jet_eta)<3.2")+"&&(jet_pt>90000||(jet_trig>=0&&trig_nhit_peak[jet_trig]>"+TriggerCutJet+"))");
  HJetPtLowEtaTrig.Scale(1./TREE->GetEntries());

  TH1F HJetPtLowEtaTrigPU("HJetPtLowEtaTrigPU","",nbins,ptlow,pthigh);  
  HJetPtLowEtaTrigPU.Sumw2();
  HJetPtLowEtaTrigPU.GetXaxis()->SetTitle(xtitle);
  HJetPtLowEtaTrigPU.GetYaxis()->SetTitle(ytitle);
  TREE->Draw("jet_pt/1000>>HJetPtLowEtaTrigPU",TString("jet_truthJ<0&&abs(jet_eta)>2.6&&abs(jet_eta)<3.2")+"&&(jet_pt>90000||(jet_trig>=0&&trig_nhit_peak[jet_trig]>"+TriggerCutJet+"))");
  HJetPtLowEtaTrigPU.Scale(1./TREE->GetEntries());

  // C.Clear();
  // HFrame.GetYaxis()->SetRangeUser(0,HJetPtLowEtaTrigPU.GetMaximum()*1.2);
  // HFrame.Draw("hist");
  // HJetPtLowEtaTrig.Draw("histpsame");
  // HJetPtLowEtaTrigPU.Draw("histsame");
  // leg.Draw();
  // C.Print(outpath+"/Trig_JetPt_LowEta_PUandSig_TrigCut.png");

  HJetPtLowEtaTrig.SetTitle("h.s. jet");
  HJetPtLowEtaTrigPU.SetTitle("p.u. jet");
  compareWithRatio(&HJetPtLowEtaTrig,&HJetPtLowEtaTrigPU, outpath+"/Trig_JetPt_LowEta_PUandSig_TrigCut",0.,ptlow,pthigh,0);
  



  /////////////////////////
  ///This is for high eta with trigger selection
  TH1F HJetPtHighEtaTrig("HJetPtHighEtaTrig","",nbins,ptlow,pthigh);  
  HJetPtHighEtaTrig.Sumw2();
  HJetPtHighEtaTrig.GetXaxis()->SetTitle(xtitle);
  HJetPtHighEtaTrig.GetYaxis()->SetTitle(ytitle);
  TREE->Draw("jet_pt/1000>>HJetPtHighEtaTrig",TString("jet_truthJ>=0&&abs(jet_eta)>3.2&&abs(jet_eta)<4.0")+"&&(jet_pt>90000||(jet_trig>=0&&trig_nhit_peak[jet_trig]>"+TriggerCutJet+"))");
  HJetPtHighEtaTrig.Scale(1./TREE->GetEntries());

  TH1F HJetPtHighEtaTrigPU("HJetPtHighEtaTrigPU","",nbins,ptlow,pthigh);  
  HJetPtHighEtaTrigPU.Sumw2();
  HJetPtHighEtaTrigPU.GetXaxis()->SetTitle(xtitle);
  HJetPtHighEtaTrigPU.GetYaxis()->SetTitle(ytitle);
  TREE->Draw("jet_pt/1000>>HJetPtHighEtaTrigPU",TString("jet_truthJ<0&&abs(jet_eta)>3.2&&abs(jet_eta)<4.0")+"&&(jet_pt>90000||(jet_trig>=0&&trig_nhit_peak[jet_trig]>"+TriggerCutJet+"))");
  HJetPtHighEtaTrigPU.Scale(1./TREE->GetEntries());

  // C.Clear();
  // HFrame.GetYaxis()->SetRangeUser(0,HJetPtHighEtaTrigPU.GetMaximum()*1.2);
  // HFrame.Draw("hist");
  // HJetPtHighEtaTrig.Draw("histpsame");
  // HJetPtHighEtaTrigPU.Draw("histsame");
  // leg.Draw();
  // C.Print(outpath+"/Trig_JetPt_HighEta_PUandSig_TrigCut.png");

  HJetPtHighEtaTrig.SetTitle("h.s. jet");
  HJetPtHighEtaTrigPU.SetTitle("p.u. jet");
  compareWithRatio(&HJetPtHighEtaTrig,&HJetPtHighEtaTrigPU, outpath+"/Trig_JetPt_HighEta_PUandSig_TrigCut",0.,ptlow,pthigh,0);


}

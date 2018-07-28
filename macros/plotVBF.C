
void plotVBF(TTree*TREE,TString outpath){

  TCanvas C;

  ////////////
  /// check the WP is working fine on the individual jets:
  //////////////
  //signal jets
  TH1F HPT("HPT","",30,0,150);
  HPT.GetXaxis()->SetTitle(" jet pT   [GeV]");
  TREE->Draw("jet_pt/1000>>HPT",signaljetCUT+"&&jet_eta>2.4");
  TH1F HPTWP("HPTWP","",30,0,150);
  TREE->Draw("jet_pt/1000>>HPTWP",signaljetCUT+"&&jet_eta>2.4&&jet_dTFitEcw_wp70==1");
  C.Clear();  
  HPT.Draw("hist");
  HPTWP.Draw("histsame");
  C.Print(outpath+"/JetPt_CheckWP.png");

  TH1F* HPTRatio = makeHistRatio(&HPTWP,&HPT);
  HPTRatio->GetYaxis()->SetTitle(HPT.GetXaxis()->GetTitle());
  C.Clear();  
  HPTRatio->GetYaxis()->SetRangeUser(0,1.2);
  HPTRatio->Draw("histpe");
  line.DrawLine(HPT.GetXaxis()->GetXmin(),1,HPT.GetXaxis()->GetXmax(),1);
  line.DrawLine(HPT.GetXaxis()->GetXmin(),0.7,HPT.GetXaxis()->GetXmax(),0.7);
  C.Print(outpath+"/JetPt_CheckWPRatio.png");


  //pu jets
  TH1F HPTPU("HPTPU","",30,0,150);
  HPTPU.GetXaxis()->SetTitle(" jet pT   [GeV]");
  TREE->Draw("jet_pt/1000>>HPTPU",pujetCUT+"&&jet_eta>2.4");
  TH1F HPTPUWP("HPTPUWP","",30,0,150);
  TREE->Draw("jet_pt/1000>>HPTPUWP",pujetCUT+"&&jet_eta>2.4&&jet_dTFitEcw_wp70==1");
  C.Clear();  
  HPTPU.Draw("hist");
  HPTPUWP.Draw("histsame");
  C.Print(outpath+"/JetPtPU_CheckWP.png");

  TH1F* HPTPURatio = makeHistRatio(&HPTPUWP,&HPTPU);
  HPTPURatio->GetYaxis()->SetTitle(HPTPU.GetXaxis()->GetTitle());
  C.Clear();  
  HPTPURatio->GetYaxis()->SetRangeUser(0,1.2);
  HPTPURatio->Draw("histpe");
  line.DrawLine(HPTPU.GetXaxis()->GetXmin(),1,HPTPU.GetXaxis()->GetXmax(),1);
  C.Print(outpath+"/JetPtPU_CheckWPRatio.png");
  

  //////////////////////////////////////////////////////////////
  //////////
  ///////////////////////////////////////////////////////////
  TH1F HMjj("HMjj","",50,0,5000);
  HMjj.GetXaxis()->SetTitle("m(jj)   [GeV]");
  TREE->Draw("vbf_mjj/1000>>HMjj","vbf_mjj>0");
  TH1F HMjjDT2("HMjjDT2","",50,0,5000);
  TREE->Draw("vbfdt2_mjj/1000>>HMjjDT2","vbfdt2_mjj>0");
  C.Clear();  
  HMjj.GetYaxis()->SetRangeUser(0,HMjj.GetMaximum()*1.2);
  HMjj.Draw("hist");
  C.Print(outpath+"/VBF_mjj.png");


  TH1F HdEta("HdEta","",20,0,10);
  HdEta.GetXaxis()->SetTitle("delta eta");
  TREE->Draw("abs(jet_eta[vbf_j1]-jet_eta[vbf_j2])>>HdEta","vbf_mjj>0");
  TH1F HdEtaDT2("HdEtaDT2","",20,0,10);
  TREE->Draw("abs(jet_eta[vbfdt2_j1]-jet_eta[vbfdt2_j2])>>HdEtaDT2","vbfdt2_mjj>0");
  C.Clear();  
  HdEta.Draw("hist");
  HdEtaDT2.Draw("histsame");
  C.Print(outpath+"/VBF_deta.png");


  TH1F HPTj1("HPTj1","",40,0,200);
  HPTj1.GetXaxis()->SetTitle("leading jet pT   [GeV]");
  TREE->Draw("jet_pt[vbf_j1]/1000>>HPTj1","vbf_mjj>0");
  TH1F HPTj1DT2("HPTj1DT2","",40,0,200);
  TREE->Draw("jet_pt[vbfdt2_j1]/1000>>HPTj1DT2","vbfdt2_mjj>0");
  C.Clear();  
  HPTj1.Draw("hist");
  HPTj1DT2.Draw("histsame");
  C.Print(outpath+"/VBF_J1PT.png");


  TH1F HPTj2("HPTj2","",40,0,200);
  HPTj2.GetXaxis()->SetTitle("sub-leading jet pT   [GeV]");
  TREE->Draw("jet_pt[vbf_j2]/1000>>HPTj2","vbf_mjj>0");
  TH1F HPTj2DT2("HPTj2DT2","",40,0,200);
  TREE->Draw("jet_pt[vbfdt2_j2]/1000>>HPTj2DT2","vbfdt2_mjj>0");
  C.Clear();  
  HPTj2.Draw("hist");
  HPTj2DT2.Draw("histsame");
  C.Print(outpath+"/VBF_J2PT.png");



  ///////////////////////////////////
  /// Coincidence 
  /////////////////////////////////
  TH1F HMjjDTcoinNoWP("HMjjDTcoinNoWP","",50,0,5000);
  TREE->Draw("vbfcoinnowp_mjj/1000>>HMjjDTcoinNoWP","vbfcoinnowp_mjj>0");

  TH1F HMjjDTcoinNoWPJ1("HMjjDTcoinNoWPJ1","",50,0,5000);
  TREE->Draw("vbfcoinnowpj1_mjj/1000>>HMjjDTcoinNoWPJ1","vbfcoinnowpj1_mjj>0");

  TH1F HMjjDTcoin("HMjjDTcoin","",50,0,5000);
  TREE->Draw("vbfcoin_mjj/1000>>HMjjDTcoin","vbfcoin_mjj>0");



  ////////////////////////////////
  // show the difference in the signal means for the coincidence selection
  ///////////////////////////////
  TString coinselection="vbfdt2_j1>=0&&vbfdt2_j2>=0&&jet_dTFit_signalmean[vbfdt2_j1]!=0&&jet_dTFit_signalmean[vbfdt2_j2]!=0";
  TH1F HDTMeanDiff("HDTMeanDiff","",100,-1000,1000);
  HDTMeanDiff.GetXaxis()->SetTitle("mean_dt(j1) - mean_dt(j2)    [ps]");
  TREE->Draw("1000*(jet_dTFit_signalmean[vbfdt2_j1]-jet_dTFit_signalmean[vbfdt2_j2])>>HDTMeanDiff",coinselection);
  C.Clear();  
  HDTMeanDiff.Draw("hist");
  C.Print(outpath+"/VBF_DTMeanDiff.png");

  TH1F HDTMeanDiffZoom("HDTMeanDiffZoom","",100,-100,100);
  HDTMeanDiffZoom.GetXaxis()->SetTitle("mean_dt(j1) - mean_dt(j2)    [ps]");
  TREE->Draw("1000*(jet_dTFit_signalmean[vbfdt2_j1]-jet_dTFit_signalmean[vbfdt2_j2])>>HDTMeanDiffZoom",coinselection);
  TF1 FGaus("FGaus","[0]*exp(-0.5*(x-[1])**2/[2]**2)",-50,50);
  FGaus.SetParameter(0,1); 
  FGaus.SetParameter(1,0); 
  FGaus.SetParameter(2,10);
  FGaus.SetLineColor(4);
  HDTMeanDiffZoom.Fit("FGaus");
  C.Clear();  
  HDTMeanDiffZoom.Draw("hist");
  FGaus.Draw("lsame");
  C.Print(outpath+"/VBF_DTMeanDiffZoom.png");



  ////////////////////////////////////////////////
  /// With CJV: Note: the baseline changes
  ///////////////////////////////////////////////
  TH1F HMjjCJV("HMjjCJV","",50,0,5000);
  HMjjCJV.GetXaxis()->SetTitle("m(jj)   [GeV]");
  TREE->Draw("vbfcjv_mjj/1000>>HMjjCJV","vbfcjv_mjj>0");
  TH1F HMjjDT2CJV("HMjjDT2CJV","",50,0,5000);
  TREE->Draw("vbfdt2cjv_mjj/1000>>HMjjDT2CJV","vbfdt2cjv_mjj>0");
  C.Clear();  
  HMjjCJV.Draw("hist");
  HMjjDT2CJV.Draw("histsame");
  C.Print(outpath+"/VBFCJV_mjj.png");


  ///////////////////////////////////////////////////
  ///Print the rates:
  ///////////////////////////////////////////////////
  
  ofstream f;
  f.open(outpath+"/rates.txt");

  f<<setw(15)<<"Base:"<<setw(10)<<setprecision(3)<<HMjj.Integral()
      <<setprecision(3)<<","<<setw(10)<<100*HMjj.Integral()/TREE->GetEntries()<<setprecision(2)<<" +/- "<<100*sqrt(HMjj.Integral())/TREE->GetEntries()
      <<setprecision(3)<<","<<setw(10)<<100*HMjj.Integral()/HMjj.Integral()<<setprecision(2)<<" +/- "<<100*sqrt(HMjj.Integral())/HMjj.Integral()
      <<endl;

  f<<setw(15)<<"HGTDWP:"<<setw(10)<<setprecision(3)<<HMjjDT2.Integral()
      <<setprecision(3)<<","<<setw(10)<<100*HMjjDT2.Integral()/TREE->GetEntries()<<setprecision(2)<<" +/- "<<100*sqrt(HMjjDT2.Integral())/TREE->GetEntries()
      <<setprecision(3)<<","<<setw(10)<<100*HMjjDT2.Integral()/HMjj.Integral()<<setprecision(2)<<" +/- "<<100*sqrt(HMjjDT2.Integral())/HMjj.Integral()
      <<endl;


  f<<setw(15)<<"CoincidenceNoWP:"<<setw(10)<<setprecision(3)<<HMjjDTcoinNoWP.Integral()
      <<setprecision(3)<<","<<setw(10)<<100*HMjjDTcoinNoWP.Integral()/TREE->GetEntries()<<setprecision(2)<<" +/- "<<100*sqrt(HMjjDTcoinNoWP.Integral())/TREE->GetEntries()
      <<setprecision(3)<<","<<setw(10)<<100*HMjjDTcoinNoWP.Integral()/HMjj.Integral()<<setprecision(2)<<" +/- "<<100*sqrt(HMjjDTcoinNoWP.Integral())/HMjj.Integral()
      <<endl;

  f<<setw(15)<<"CoincidenceNoWPJ1:"<<setw(10)<<setprecision(3)<<HMjjDTcoinNoWPJ1.Integral()
      <<setprecision(3)<<","<<setw(10)<<100*HMjjDTcoinNoWPJ1.Integral()/TREE->GetEntries()<<setprecision(2)<<" +/- "<<100*sqrt(HMjjDTcoinNoWPJ1.Integral())/TREE->GetEntries()
      <<setprecision(3)<<","<<setw(10)<<100*HMjjDTcoinNoWPJ1.Integral()/HMjj.Integral()<<setprecision(2)<<" +/- "<<100*sqrt(HMjjDTcoinNoWPJ1.Integral())/HMjj.Integral()
      <<endl;

  f<<setw(15)<<"Coincidence:"<<setw(10)<<setprecision(3)<<HMjjDTcoin.Integral()
      <<setprecision(3)<<","<<setw(10)<<100*HMjjDTcoin.Integral()/TREE->GetEntries()<<setprecision(2)<<" +/- "<<100*sqrt(HMjjDTcoin.Integral())/TREE->GetEntries()
      <<setprecision(3)<<","<<setw(10)<<100*HMjjDTcoin.Integral()/HMjj.Integral()<<setprecision(2)<<" +/- "<<100*sqrt(HMjjDTcoin.Integral())/HMjj.Integral()
      <<endl;

  f<<setw(15)<<"BaseCJV:"<<setw(10)<<setprecision(3)<<HMjjCJV.Integral()
      <<setprecision(3)<<","<<setw(10)<<100*HMjjCJV.Integral()/TREE->GetEntries()<<setprecision(2)<<" +/- "<<100*sqrt(HMjjCJV.Integral())/TREE->GetEntries()
      <<setprecision(3)<<","<<setw(10)<<100*HMjjCJV.Integral()/HMjjCJV.Integral()<<setprecision(2)<<" +/- "<<100*sqrt(HMjjCJV.Integral())/HMjjCJV.Integral()
      <<endl;

  f<<setw(15)<<"HGTDCJV:"<<setw(10)<<setprecision(3)<<HMjjDT2CJV.Integral()
      <<setprecision(3)<<","<<setw(10)<<100*HMjjDT2CJV.Integral()/TREE->GetEntries()<<setprecision(2)<<" +/- "<<100*sqrt(HMjjDT2CJV.Integral())/TREE->GetEntries()
      <<setprecision(3)<<","<<setw(10)<<100*HMjjDT2CJV.Integral()/HMjjCJV.Integral()<<setprecision(2)<<" +/- "<<100*sqrt(HMjjDT2CJV.Integral())/HMjjCJV.Integral()
      <<endl;

  f.close();
  cout<<"Rates : "<<outpath+"/rates.txt"<<endl;

}

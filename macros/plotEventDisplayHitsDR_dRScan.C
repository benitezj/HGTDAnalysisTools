void plotEventDisplayHitsDR_dRScan(TTree *Tuple, TString outpath,TString tag, TString Selection){
  
  TCanvas C;
  
  //////////////////
  /////
  /////////////////
  int nbins=10;
  float min=0;
  float max=0.5;
  TString dTYTitle="hit density";

  ///Choose time resolution
  long dtwindow=120;
  if(fabs(jeteta[1])>3.2){
    dtwindow=60;
  }

  ///choose fit function
  TF1 * F=NULL;
  if( fabs(jeteta[1])<3.0 )
    F=&dRFitL;
  if( 3.0<fabs(jeteta[1]) && fabs(jeteta[1])<3.4 )
    F=&dRFitM;
  if( 3.4<fabs(jeteta[1]) )
    F=&dRFitH;

  
  /////////////////////////
  /// inclusive
  TH1F* HCellDR = getdRDensityHisto("HCellDR_Jet");
  Tuple->Draw(TString("cell_jetdR>>HCellDR_Jet"),TString("(")+Selection+")"); 
  dividedRDensityHisto(HCellDR);

  /////////////////////////
  /// signal time bin
  TH1F* HCellDRSig = getdRDensityHisto("HCellDRSig_Jet");
  Tuple->Draw(TString("cell_jetdR>>HCellDRSig_Jet"),TString("(")+Selection+"&&(abs(cell_dt-jet_truedt[cell_jet])*1000<"+dtwindow+")"+")"); 
  dividedRDensityHisto(HCellDRSig);

  /////////////////////////
  /// 
  TFile OutFile(outpath+"/EventDisplaydR_CellsdR_"+tag+".root","recreate"); 
  OutFile.cd();
  HCellDR->Write();
  HCellDRSig->Write();


  ////////////////////////////////////////////////////
  ///  get the signal distribution
  ////////////////////////////////////////////////////
  TFile * InFile=NULL;
  TH1F * HCellDRMu0=NULL;
  TH1F * HCellDRSigMu0=NULL;
  if(outpath.Contains("mu200")){
    TString outpathmu0=outpath;
    outpathmu0("mu200")="mu0";
    InFile = new TFile(outpathmu0+"/EventDisplaydR_CellsdR_"+tag+".root","read"); 
    gROOT->cd();
    if(!InFile->IsZombie()){
      HCellDRMu0 = (TH1F*)InFile->Get(TString("HCellDR_Jet"));
      HCellDRSigMu0 = (TH1F*)InFile->Get(TString("HCellDRSig_Jet"));
    }
  }


  ///////////////////////////
  //// inclusive
  ///////////////////////////
  C.Clear();  
  HCellDR->GetXaxis()->SetTitle("R_{cell}"); 
  HCellDR->GetYaxis()->SetTitle(dTYTitle); 
  //HCellDR->GetYaxis()->SetRangeUser(0,100);
  HCellDR->GetYaxis()->SetRangeUser(0,HCellDR->GetMaximum()*1.3);
  HCellDR->Draw("hist");

  if(HCellDRMu0){
    HCellDRMu0->SetLineColor(2);
    HCellDRMu0->Draw("histsame");
    text.SetTextColor(2);
    text.DrawLatexNDC(0.7,0.82,"#topbar signal jet, #mu=0");
  }

  C.RedrawAxis();
  C.Print(outpath+"/EventDisplaydR_CellsdR_"+tag+".png");
  //C.Print(outpath+"/EventDisplaydR_CellsdR_"+tag+".eps");
  //C.Print(outpath+"/EventDisplaydR_CellsdR_"+tag+".pdf");
  


  ////////////////////////////
  ///// signal time bin
  ////////////////////////////
  C.Clear();  
  HCellDRSig->GetXaxis()->SetTitle("R_{cell}"); 
  HCellDRSig->GetYaxis()->SetTitle(dTYTitle); 
  //HCellDRSig->GetYaxis()->SetRangeUser(0,100);
  HCellDRSig->GetYaxis()->SetRangeUser(0,HCellDRSig->GetMaximum()*1.3);
  HCellDRSig->Draw("hist");

  if(HCellDRSigMu0){
    HCellDRSigMu0->SetLineColor(2);
    HCellDRSigMu0->Draw("histsame");
    text.SetTextColor(2);
    text.DrawLatexNDC(0.7,0.82,"#topbar signal jet, #mu=0");
  }
  C.RedrawAxis();
  C.Print(outpath+"/EventDisplaydR_CellsdRSig_"+tag+".png");
  //C.Print(outpath+"/EventDisplaydR_CellsdR_"+tag+".eps");
  //C.Print(outpath+"/EventDisplaydR_CellsdR_"+tag+".pdf");
  

  ////////////////////////
  /// scan time bins
  ////////////////////////
  C.Clear();
  C.Divide(2,5);
  for(long t=0;t<10;t++){
    C.cd(t+1);

    long tmin=-600+t*2*dtwindow;
    long tmax=-600+(t+1)*2*dtwindow;
    
    ///signal time bin
    TH1F* HCellDRvsT = getdRDensityHisto(TString("HCellDRvsT_Jet")+t);
    Tuple->Draw(TString(TString("cell_jetdR>>HCellDRvsT_Jet")+t),TString("(")+Selection+"&&("+tmin+"<cell_dt*1000&&cell_dt*1000<"+tmax+")"+")"); 
    dividedRDensityHisto(HCellDRvsT);

    OutFile.cd();
    HCellDRvsT->Write();


    HCellDRvsT->SetTitle(TString("dT:[")+tmin+" , "+tmax+"]");
    HCellDRvsT->GetYaxis()->SetNdivisions(3);
    HCellDRvsT->GetYaxis()->SetLabelSize(0.16);
    HCellDRvsT->GetYaxis()->SetTitleSize(0.16);
    HCellDRvsT->GetYaxis()->SetTitleOffset(0.4);
    HCellDRvsT->GetYaxis()->SetTitle("");
    HCellDRvsT->GetXaxis()->SetNdivisions(3);
    HCellDRvsT->GetXaxis()->SetLabelSize(0.16);
    HCellDRvsT->GetXaxis()->SetTitleSize(0.16);
    HCellDRvsT->GetXaxis()->SetTitleOffset(0.5);
    HCellDRvsT->GetYaxis()->SetRangeUser(0,1.5*HCellDRvsT->GetMaximum());

    HCellDRvsT->Draw("histpe");


    TF1* FitFunc=(TF1*)(F->Clone(TString("FitFunc")+t));
    FitFunc->SetLineColor(4);
    HCellDRvsT->Fit(FitFunc,"Q","LL",0,0.4);
    FitFunc->Draw("lsame");


    ////////////////////////////////
    ////draw the mu=0 distribution
    //////////////////////////////
    TH1F*HCellDRvsTMu0=NULL;
    if(InFile)
      HCellDRvsTMu0 = (TH1F*)InFile->Get(TString("HCellDRvsT_Jet")+t);
    if(HCellDRvsTMu0){
      HCellDRvsTMu0->SetLineColor(2);
      HCellDRvsTMu0->Draw("histsame");
      //text.SetTextColor(2);
      //text.DrawLatexNDC(0.7,0.82,"#topbar signal jet, #mu=0");
    }

    text.SetTextColor(1);
    text.SetTextSize(0.16);
    text.DrawLatexNDC(0.84,0.80,TString("")+tmin);
    text.DrawLatexNDC(0.84,0.60,TString("")+tmax);


    if(FitFunc){
      text.SetTextColor(4);
      char txt[100];
      sprintf(txt,"%.1f",FitFunc->GetParameter(1)/FitFunc->GetParError(1));
      text.DrawLatexNDC(0.84,0.30,txt);//TString("")+long(10*FitFunc->GetParameter(1)/FitFunc->GetParError(1)));
    }
  }
  C.Print(outpath+"/EventDisplaydR_CellsdRvsT_"+tag+".png");
  


  //////////////////////////////////////
  ///close files
  /////////////////////////////////////////
  if(InFile) delete InFile;
  OutFile.Close();
}
  


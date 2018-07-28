
void plotEventDisplayHitsDT_dRScan(TTree *Tuple, TString outpath,TString tag, TString Selection){
  
  TCanvas C;
  
  //////////////////
  /////Plot the deltaT distributions
  /////////////////
  int ntbins=17;
  int tmin=-1000;
  int tmax=1000;
  TString dTYTitle="number of hits / 120 ps";

  long dtwindow=120;
  if(fabs(jeteta[1])>3.2){
    ntbins=33;
    dTYTitle="number of hits / 60 ps";

    dtwindow=60;
  }


  TFile OutFile(outpath+"/EventDisplaydR_CellsdT_"+tag+".root","recreate"); //save the dT distributions for plotting mu=0 in mu=200

  TH1F HCellDT(TString("HCellDT_Jet"),"",ntbins,tmin,tmax);
  Tuple->Draw(TString("cell_dt*1000>>HCellDT_Jet"),TString("(")+Selection+")"); 

  OutFile.cd();//save histo at this point, below there is a background subtraction
  HCellDT.Write();

  
  ////////////////////////////////////////////////////
  //show the signal distribution
  ////////////////////////////////////////////////////
  TFile * InFile=NULL;
  TH1F * HSig=NULL;
  if(outpath.Contains("mu200")){// && tag.CompareTo("jet")==0){
    TString outpathmu0=outpath;
    outpathmu0("mu200")="mu0";
    InFile = new TFile(outpathmu0+"/EventDisplaydR_CellsdT_"+tag+".root","read"); 
    gROOT->cd();
    if(!InFile->IsZombie()){
      HSig = (TH1F*)InFile->Get(TString("HCellDT_Jet"));
    }
  }



  ////////////////////////////
  C.Clear();  
  HCellDT.GetXaxis()->SetTitle(" t - t_{exp}   [ps]"); 
  HCellDT.GetYaxis()->SetTitle(dTYTitle); 
  //HCellDT.GetYaxis()->SetRangeUser(0,100);
  HCellDT.GetYaxis()->SetRangeUser(0,HCellDT.GetMaximum()*1.3);
  HCellDT.Draw("hist");
  
  DrawATLAS(0.20,0.89);
  DrawHGTD(outpath,0,0,0.20,0.835);
  DrawVBFH(outpath);
  DrawECMSMu(outpath);
    
  float xpostxt=0.68;
  text.SetTextColor(1);

    
  if(HSig){
    HSig->SetLineColor(2);
    HSig->Draw("histsame");
    text.SetTextColor(2);
    text.DrawLatexNDC(xpostxt,0.82,"#topbar signal jet, #mu=0");
  }

  ///draw the time bins
  for(long t=0;t<=10;t++){
    long tmin=-600+t*2*dtwindow;
    line.SetLineColor(4);
    line.DrawLine(tmin,0,tmin,HCellDT.GetMaximum());
  }
  

  C.RedrawAxis();
  C.Print(outpath+"/EventDisplaydR_CellsdT_"+tag+".png");
  //C.Print(outpath+"/EventDisplaydR_CellsdT_"+tag+".eps");
  //C.Print(outpath+"/EventDisplaydR_CellsdT_"+tag+".pdf");
  

  //////////////////////////////////////////
  ///Save histograms in root file
  /////////////////////////////////////////
  if(InFile) delete InFile;
  OutFile.Close();
}







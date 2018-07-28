
void plotEventDisplayHitsDT(TTree *Tuple, TString outpath,TString tag, TString Selection, TString SelectionBkg=""){
  
  TCanvas C;
  
  //////////////////
  /////Plot the deltaT distributions
  ////////////
  int tmin=-1000;
  int tmax=1000;


  // int ntbins=17;
  // TString dTYTitle="number of hits / 120 ps";
  // if(fabs(jeteta[1])>3.2){
  //   ntbins=33;
  //   dTYTitle="number of hits / 60 ps";
  // }

  int ntbins=66;
  TString dTYTitle="number of hits / 30 ps";


  TFile OutFile(outpath+"/EventDisplay_CellsdT_"+tag+".root","recreate"); //save the dT distributions for plotting mu=0 in mu=200

  TH1F HCellDT(TString("HCellDT_Jet"),"",ntbins,tmin,tmax);
  Tuple->Draw(TString("cell_dt*1000>>HCellDT_Jet"),Selection); 

  OutFile.cd();//save histo at this point, below there is a background subtraction
  HCellDT.Write();

  
  ////////////////////////////////////////////////////
  //show the signal distribution
  ////////////////////////////////////////////////////
  TFile * InFile=NULL;
  TH1F * HSig=NULL;
  if(outpath.Contains("mu200")){
    TString outpathmu0=outpath;
    outpathmu0("mu200")="mu0";
    InFile = new TFile(outpathmu0+"/EventDisplay_CellsdT_"+tag+".root","read"); 
    gROOT->cd();
    if(!InFile->IsZombie()){
      HSig = (TH1F*)InFile->Get(TString("HCellDT_Jet"));
    }
  }


  //////////////////////////////////////////
  ////Show the background estimate 
  ///////////////////////////////////////////
  TH1F* HCellDTBkg=NULL;
  if(SelectionBkg.CompareTo("")!=0){
    HCellDTBkg=new TH1F(TString("HCellDTBkg_Jet"),"",ntbins,tmin,tmax);
    Tuple->Draw(TString("cell_dt*1000>>HCellDTBkg_Jet"),SelectionBkg);      
  }


  ////////////////////////////
  C.Clear();  
  HCellDT.GetXaxis()->SetTitle(" t - t_{exp}   [ps]"); 
  HCellDT.GetYaxis()->SetTitle(dTYTitle); 
  //HCellDT.GetYaxis()->SetRangeUser(0,100);
  HCellDT.GetYaxis()->SetRangeUser(0,HCellDT.GetMaximum()*1.3);
  HCellDT.Draw("hist");
  
  DrawATLAS(0.20,0.89);
  DrawHGTD(outpath,0,-1,0.20,0.835,"1 mm cells");
  DrawVBFH(outpath);
  DrawECMSMu(outpath);
    
  float xpostxt=0.70;
  text.SetTextColor(1);
  text.SetTextSize(0.042);

  if(tag.Contains("core10")) text.DrawLatexNDC(xpostxt,0.75,"#Delta R < 0.1");
  if(tag.Contains("core20")) text.DrawLatexNDC(xpostxt,0.75,"#Delta R < 0.2");
  if(tag.Contains("core25")) text.DrawLatexNDC(xpostxt,0.75,"#Delta R < 0.25");
  if(tag.Contains("core30")) text.DrawLatexNDC(xpostxt,0.75,"#Delta R < 0.3");
  if(tag.Contains("core40")) text.DrawLatexNDC(xpostxt,0.75,"#Delta R < 0.4");

    
  text.DrawLatexNDC(xpostxt,0.85,"#topbar h.s. jet, #mu=200");
  //text.DrawLatexNDC(xpostxt,0.75,"#Delta R < 0.2");
 
  if(HSig){
    HSig->SetLineColor(2);
    HSig->Draw("histsame");
    text.SetTextColor(2);
    text.DrawLatexNDC(xpostxt,0.80,"#topbar h.s. jet, #mu=0");
  }



  C.RedrawAxis();
  C.Print(outpath+"/EventDisplay_CellsdT_"+tag+".png");
  C.Print(outpath+"/EventDisplay_CellsdT_"+tag+".eps");
  C.Print(outpath+"/EventDisplay_CellsdT_"+tag+".pdf");
  

  if(HCellDTBkg){
    HCellDTBkg->SetLineColor(4);
    HCellDTBkg->Draw("histsame");
    text.SetTextColor(4);
    text.DrawLatexNDC(xpostxt,0.85,"#topbar side-band");
    C.Print(outpath+"/EventDisplay_CellsdT_"+tag+"_wBkg.png");

    ///////////////////////////////////////////////////////////
    /////rescale the background to the bin at the mean of the background
    // float bkgmean=HCellDTBkg->GetMean();    
    // int bkgmeanbin=0;
    // for(int b=1;b<=HCellDTBkg->GetNbinsX();b++){
    //   if(HCellDTBkg->GetBinLowEdge(b)<bkgmean&&bkgmean<HCellDTBkg->GetBinLowEdge(b)+HCellDTBkg->GetBinWidth(b))
    // 	bkgmeanbin=b;
    // }
    // float bkgscale=HCellDT.Integral(bkgmeanbin-1,bkgmeanbin+1)/HCellDTBkg->Integral(bkgmeanbin-1,bkgmeanbin+1);

    /////////////////////////////////////////////////
    ////make fit until X% of bins become negative
    int ntotbins=0;
    int bincountmin=5;
    for(int b=1;b<=HCellDT.GetNbinsX();b++){
      if(HCellDT.GetBinContent(b)>bincountmin)//dont count small bins
	ntotbins++;
    }
    int scaleF=0;
    int lastnneg=0;
    for(int s=50;s<100;s++){
      HCellDTBkg->Scale((s/100.)*HCellDT.Integral()/HCellDTBkg->Integral());
      int nneg=0;
      for(int b=1;b<=HCellDT.GetNbinsX();b++){
	if(HCellDT.GetBinContent(b)<=bincountmin) continue;
	if(HCellDT.GetBinContent(b)-HCellDTBkg->GetBinContent(b)  < 0)
	  nneg++;	 
      }
      if(nneg>0.4*ntotbins){
	scaleF=s;
	break;
      }
    }
    float bkgscale=(scaleF/100.)*HCellDT.Integral()/HCellDTBkg->Integral();
    
    
    HCellDTBkg->Scale(bkgscale);
    HCellDTBkg->Draw("histsame");
    C.Update();
    C.Print(outpath+"/EventDisplay_CellsdT_"+tag+"_wBkgNorm.png");



    ////background subtracted plot
    TH1F * HCellDTBkgSub=(TH1F*)(HCellDT.Clone("HCellDTBkgSub"));
    HCellDTBkgSub->Add(HCellDTBkg,-1);
    C.Clear();
    HCellDTBkgSub->Draw("hist");
    C.Print(outpath+"/EventDisplay_CellsdT_"+tag+"_BkgSub.png");

  }



  //////////////////////////////////////////
  ///Save histograms in root file
  /////////////////////////////////////////
  if(InFile) delete InFile;
  OutFile.Close();
}
  

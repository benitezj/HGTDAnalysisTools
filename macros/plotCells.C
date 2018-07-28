//#include "HGTDAnalysisTools/macros/drawCellGrid.C"

/////////////////////////////////////////////////////////////////
///////////Cell plots
/////////////////////////////////////////////////////////////////
void plotCellSpatial(TChain*TREE,TString outpath){
  TCanvas C;

  TH1F HNcell("HNcell","",100,0,200);
  HNcell.GetYaxis()->SetTitle(" events ");
  HNcell.GetXaxis()->SetTitle("# of cells per event ");
  TREE->Draw("ncell>>HNcell");
  C1D.Clear();
  HNcell.Draw("hist");
  C1D.SetLogy(1);
  C1D.Print(outpath+"/CELL_n.png");
  C1D.SetLogy(0);

  TH2F HXY("HXY","",120,-600,600,120,-600,600);
  HXY.GetXaxis()->SetTitle(" X [mm]");
  HXY.GetYaxis()->SetTitle(" Y [mm]");
  HXY.GetZaxis()->SetTitle("# of cells per event / 10mmx10mm");
  TREE->Draw("cell_y:cell_x>>HXY");
  HXY.Scale(1./TREE->GetEntries());//normalize by events
  C2D.cd();
  C2D.Clear();
  HXY.Draw("histcolz");
  C2D.Print(outpath+"/CELL_XY.png");

  TH1F HZ("HZ","",100,3500,3550);
  HZ.GetXaxis()->SetTitle(" Z [mm]");
  HZ.GetYaxis()->SetTitle("# of cells per event ");
  TREE->Draw("abs(cell_z)>>HZ");
  HZ.Scale(1./TREE->GetEntries());//normalize by events
  C2D.cd();
  C2D.Clear();
  HZ.Draw("hist");
  C2D.Print(outpath+"/CELL_Z.png");
 
  TH1F HS("HS","",24,-5.5,5.5);
  HS.GetXaxis()->SetTitle("layer #");
  HS.GetYaxis()->SetTitle("# of cells per event ");
  TREE->Draw("cell_S>>HS");
  HS.Scale(1./TREE->GetEntries());//normalize by events
  C.Clear();
  HS.Draw("hist");
  C.Print(outpath+"/CELL_S.png");

  TH1F HEta("HEta","",60,2,5);
  HEta.GetXaxis()->SetTitle("cell |#eta|");
  HEta.GetYaxis()->SetTitle("# of cells per event ");
  TREE->Draw("cell_eta>>HEta"); 
  HEta.Scale(1./TREE->GetEntries());//normalize by events 
  C.Clear();
  HEta.Draw("hist");
  C.Print(outpath+"/CELL_Eta.png");

  TH1F HR("HR","",60,0,600);
  HR.GetXaxis()->SetTitle("cell radius  [mm]");
  HR.GetYaxis()->SetTitle("# of cells per event ");
  TREE->Draw("cell_r>>HR");
  HR.Scale(1./TREE->GetEntries());//normalize by events
  C.Clear();
  HR.Draw("hist");
  C.Print(outpath+"/CELL_R.png");

  
  // C.Clear();
  // TH1F HRNHitG1("HRNHitG1","",HR.GetXaxis()->GetNbins(),HR.GetXaxis()->GetXmin(),HR.GetXaxis()->GetXmax());
  // HRNHitG1.GetXaxis()->SetTitle(" radius");
  // HRNHitG1.GetYaxis()->SetTitle("fraction of cells with > 1 hit");
  // TREE->Draw("cell_r>>HRNHitG1","cell_nhit>1");
  // HRNHitG1.Divide(&HR);
  // HRNHitG1.Draw("p");
  // C.Print(outpath+"/CELL_NhitG1FractionVsR.png");



  ///fraction of cells with nhit
  TH1F * HRL[4];
  for(long s=1;s<=4;s++){
    HRL[s-1]=new TH1F(TString("HRL")+s,"",HR.GetXaxis()->GetNbins(),HR.GetXaxis()->GetXmin(),HR.GetXaxis()->GetXmax());
    TREE->Draw(TString("cell_r>>HRL")+s,(TString("cell_r>50&&abs(cell_S)==")+s).Data());
  }

  TH1F * HRLNhit[4][3];
  for(long Nhit=1;Nhit<=3;Nhit++){
    for(long s=1;s<=4;s++){
      HRLNhit[s-1][Nhit-1]=new TH1F(TString("HRNhit")+s+"_"+Nhit,"",HR.GetXaxis()->GetNbins(),HR.GetXaxis()->GetXmin(),HR.GetXaxis()->GetXmax());
      if(Nhit==3)
	TREE->Draw(TString("cell_r>>HRNhit")+s+"_"+Nhit,(TString("cell_r>50&&abs(cell_S)==")+s+"&&cell_nhit>="+Nhit).Data());
      else 
	TREE->Draw(TString("cell_r>>HRNhit")+s+"_"+Nhit,(TString("cell_r>50&&abs(cell_S)==")+s+"&&cell_nhit=="+Nhit).Data());
      HRLNhit[s-1][Nhit-1]->Divide(HRL[s-1]);
      HRLNhit[s-1][Nhit-1]->SetLineColor(s);
      HRLNhit[s-1][Nhit-1]->SetMarkerColor(s);
      //HRLNhit[s-1][Nhit-1]->SetMarkerSize(0.5);
      HRLNhit[s-1][Nhit-1]->SetMarkerStyle(Nhit+1);
    }
  }
  
  TLegend leg;
  leg.AddEntry(HRLNhit[0][0],"Layer 1","l");
  leg.AddEntry(HRLNhit[1][0],"Layer 2","l");
  leg.AddEntry(HRLNhit[2][0],"Layer 3","l");
  leg.AddEntry(HRLNhit[3][0],"Layer 4","l");
  leg.SetX1NDC(0.25);
  leg.SetX2NDC(0.45);
  leg.SetY1NDC(0.45);
  leg.SetY2NDC(0.60);

  C.Clear();   
  for(long Nhit=1;Nhit<=3;Nhit++){
    for(long s=1;s<=4;s++){
      if(s==1&&Nhit==1){
	HRLNhit[s-1][Nhit-1]->GetYaxis()->SetTitle("fraction of total hit cells");
	HRLNhit[s-1][Nhit-1]->GetXaxis()->SetTitle("radius  [mm]");
	HRLNhit[s-1][Nhit-1]->GetYaxis()->SetRangeUser(0,1);
	HRLNhit[s-1][Nhit-1]->Draw("histp");
	leg.Draw();
      }else HRLNhit[s-1][Nhit-1]->Draw("histpsame");
    }
  }
  C.Print(outpath+"/CELL_NhitFractionVsR.png");

  for(long Nhit=1;Nhit<=3;Nhit++){
    for(long s=1;s<=4;s++){
      delete HRLNhit[s-1][Nhit-1];
    }
  }
  for(long s=1;s<=4;s++){
    delete HRL[s-1];
  }


  ///CELL distance to primary vertex
  TH1F HCELLTrueD("HCELLTrueD","",100,3000,4000);
  HCELLTrueD.GetXaxis()->SetTitle("distance to vertex [mm]");
  TREE->Draw("cell_trueD>>HCELLTrueD","cell_S>0");
  TH1F HCELLD("HCELLD","",100,3000,4000);
  TREE->Draw("cell_D>>HCELLD","cell_S>0");
  TH1F HCELLDZ0("HCELLDZ0","",100,3000,4000);
  TREE->Draw("cell_DZ0>>HCELLDZ0","cell_S>0");

  C.Clear();
  HCELLDZ0.SetLineColor(4);
  HCELLDZ0.Draw("hist");
  // HCELLTrueD.SetLineColor(1);
  // HCELLTrueD.Draw("histsame");
  // HCELLD.SetLineColor(2);
  // HCELLD.Draw("histsame");
  C.Print(outpath+"/CELLD.png");


  TH1F HCELLdeltaD("HCELLdeltaD","",100,-200,200);
  HCELLdeltaD.GetXaxis()->SetTitle("reco. dist. - true dist. [mm]");
  TREE->Draw("(cell_D-cell_trueD)>>HCELLdeltaD");
  TH1F HCELLdeltaDZ0("HCELLdeltaDZ0","",100,-200,200);
  TREE->Draw("(cell_DZ0-cell_trueD)>>HCELLdeltaDZ0");

  C.Clear();
  HCELLdeltaD.SetLineColor(2);
  HCELLdeltaD.Draw("hist");
  HCELLdeltaDZ0.SetLineColor(4);
  HCELLdeltaDZ0.Draw("histsame");
  text.SetTextColor(2);
  text.DrawLatexNDC(0.24,.85,TString("rms=")+(long)HCELLdeltaD.GetRMS());  
  text.SetTextColor(4);
  text.DrawLatexNDC(0.24,.80,TString("rms=")+(long)HCELLdeltaDZ0.GetRMS());  
  C.Print(outpath+"/CELLdeltaD.png");


  TH2F HCELLdeltaDZ0VsR("HCELLdeltaDZ0VsR","",60,0,600,100,-200,200);
  HCELLdeltaDZ0VsR.GetYaxis()->SetTitle("reco. dist. - true dist. [mm]");
  HCELLdeltaDZ0VsR.GetXaxis()->SetTitle(" radius [mm]");
  TREE->Draw("(cell_DZ0-cell_trueD):cell_r>>HCELLdeltaDZ0VsR");
  C2D.cd();
  C2D.Clear();
  HCELLdeltaDZ0VsR.Draw("histcolz");
  C2D.Print(outpath+"/CELLdeltaDVsR.png");



  TH2F HRVsLayer("HRVsLayer","",11,-5.5,5.5,120,0,600);
  HRVsLayer.GetYaxis()->SetTitle(" radius  [mm]");
  HRVsLayer.GetXaxis()->SetTitle("layer ");
  TREE->Draw("cell_r:cell_S>>HRVsLayer");
  HRVsLayer.GetZaxis()->SetTitle("# of cells per event ");
  C2D.cd();
  C2D.Clear();
  HRVsLayer.Draw("colz");
  C2D.Print(outpath+"/CELL_RVsL.png");

  //Here we need to save histogram to root file for later comparisions in scans
  C.cd();
  if(HNCELLvsR){
    TFile FNCellvsRRatio(outpath+"/NCellVsR.root","recreate");
    for(long s=1;s<=4;s++){
      TH1F HR(TString("HR")+s,"",HNCELLvsR->GetXaxis()->GetNbins(),HNCELLvsR->GetXaxis()->GetXmin(),HNCELLvsR->GetXaxis()->GetXmax());
      TREE->Draw(TString("cell_r>>HR")+s,(TString("abs(cell_S)==")+s).Data());
      for(int b=1;b<=HNCELLvsR->GetXaxis()->GetNbins();b++){
  	if(HNCELLvsR->GetBinContent(b)>0){
  	  HR.SetBinContent(b,HR.GetBinContent(b)/(2*HNCELLvsR->GetBinContent(b)*TREE->GetEntries()));
  	  HR.SetBinError(b,0);
  	}else {
  	  HR.SetBinContent(b,0);
  	  HR.SetBinError(b,0);
  	}
      }
      FNCellvsRRatio.cd();
      HR.SetName(TString("HNCellVsR")+s);
      HR.Write();      
    }
    FNCellvsRRatio.ls();

    C.Clear();
    HNCELLvsR->GetXaxis()->SetTitle("radius (mm)");
    HNCELLvsR->GetYaxis()->SetTitle("number of cells");
    HNCELLvsR->Draw();
    C.Print(outpath+"/NCellsVsR.png");
  }


}

void plotCellTime(TChain*TREE,TString outpath){
  TCanvas C;

  /////////////////////////////////////////
  ///CELL raw time distribution
  /////////////////////////////////////////
  char paramstext[100];



  TH1F HCELLT("HCELLT","",100,10,15);
  HCELLT.GetXaxis()->SetTitle("CELL time [ns]");
  TREE->Draw("cell_t>>HCELLT");

  TF1 FTGaus("FTGaus","[0]*exp(-(x-[1])**2/[2]**2)",10,14);
  FTGaus.SetLineColor(4);

  C.Clear();
  HCELLT.Draw("hist");
  //HCELLT.Fit("FTGaus");
  //FTGaus.Draw("lsame");
  //sprintf(paramstext,"Mean=%.1f ns",FTGaus.GetParameter(1));
  //text.DrawTextNDC(0.65,0.8,paramstext);
  //sprintf(paramstext,"Sigma=%.2f ns",FTGaus.GetParameter(2));
  //text.DrawTextNDC(0.65,0.7,paramstext);
  C.Print(outpath+"/CELLT.png");
  //C.SetLogy(1);
  //C.Print(outpath+"/CELLT_log.png");
  //C.SetLogy(0);
  cout<<"HCELLT Mean = "<<HCELLT.GetMean()<<" RMS="<<HCELLT.GetRMS()<<endl;

  /////////////////////////////////////////
  ///CELL delta T distribution
  /////////////////////////////////////////
  TH1F HCELLDT("HCELLDT","",200,-1000,1000);
  HCELLDT.GetXaxis()->SetTitle("cell deltaT [ps]");
  HCELLDT.SetMarkerSize(0.5);
  TREE->Draw("cell_dtreco*1000>>HCELLDT");

  // TH1F HCELLtrueDT("HCELLtrueDT","",200,-1000,1000);
  // TREE->Draw("cell_dttrue*1000>>HCELLtrueDT");

  TH1F HCELLDTZ0("HCELLDTZ0","",200,-1000,1000);
  TREE->Draw("cell_dtZ0*1000>>HCELLDTZ0");

  TF1 FDTGaus("FDTGaus","[0]*exp(-0.5*(x-[1])**2/[2]**2)",-400,400);
  FDTGaus.SetLineColor(4);
  FDTGaus.SetParLimits(1,-200,200);
  FDTGaus.SetParLimits(2,10,500);

  C.Clear();
  HCELLDTZ0.Draw("hist");
  fitHistGaus(&HCELLDTZ0,-400,400,0.75,0.8);
  C.Update();
  C.Print(outpath+"/CELLDT.png");


  /////////////////////////////////////////
  ///CELL delta T distribution
  /////////////////////////////////////////
  TH1F HCELLDTwrtTrueDT("HCELLDTwrtTrueDT","",100,-500,500);
  HCELLDTwrtTrueDT.GetXaxis()->SetTitle("reco. deltaT - true deltaT [ps]");
  HCELLDTwrtTrueDT.SetMarkerSize(0.5);
  TREE->Draw("(cell_dtreco-cell_dttrue)*1000>>HCELLDTwrtTrueDT");

  TH1F HCELLDTZ0wrtTrueDT("HCELLDTZ0wrtTrueDT","",100,-500,500);
  TREE->Draw("(cell_dtZ0-cell_dttrue)*1000>>HCELLDTZ0wrtTrueDT");

  C.Clear();
  //HCELLDTwrtTrueDT.SetLineColor(2);
  //HCELLDTwrtTrueDT.Draw("hist");
  //text.SetTextColor(2);
  //text.DrawLatexNDC(0.24,.85,TString("rms=")+(long)HCELLDTwrtTrueDT.GetRMS());  
  HCELLDTZ0wrtTrueDT.SetLineColor(4);
  HCELLDTZ0wrtTrueDT.Draw("histsame");
  text.SetTextColor(4);
  text.DrawLatexNDC(0.24,.80,TString("rms=")+(long)HCELLDTZ0wrtTrueDT.GetRMS());  
  //C.SetLogy(1);
  C.Print(outpath+"/CELLDTwrtTrueDT.png");
  //C.SetLogy(0);

}

void plotCellEnergy(TChain*TREE,TString outpath){
  TCanvas C;
  

  //////////////////////////////
  ///Hit energy 
  C.Clear();
  TH1F HCELLE("HCELLE","",100,0,10);
  HCELLE.GetXaxis()->SetTitle("cell energy [MeV]");
  TREE->Draw("cell_E>>HCELLE");
  HCELLE.Draw("hist");
  C.Print(outpath+"/CELLE.png");
  C.SetLogy(1);
  C.Print(outpath+"/CELLE_log.png");
  C.SetLogy(0);

  C.Clear();
  TH1F HCELLELow("HCELLELow","",100,0,1);
  HCELLELow.GetXaxis()->SetTitle("cell energy [MeV]");
  TREE->Draw("cell_E>>HCELLELow");
  HCELLELow.GetYaxis()->SetRangeUser(0.1,HCELLELow.GetMaximum()*1.4);
  HCELLELow.Draw("hist");

  if(outpath.Contains("_20CELLENERGY")) line.DrawLine(0.020,0.1,0.020,HCELLELow.GetMaximum());  
  if(outpath.Contains("_40CELLENERGY")) line.DrawLine(0.040,0.1,0.040,HCELLELow.GetMaximum());
  if(outpath.Contains("_60CELLENERGY")) line.DrawLine(0.060,0.1,0.060,HCELLELow.GetMaximum());
  if(outpath.Contains("_80CELLENERGY")) line.DrawLine(0.080,0.1,0.080,HCELLELow.GetMaximum());
  if(outpath.Contains("_100CELLENERGY")) line.DrawLine(0.100,0.1,0.100,HCELLELow.GetMaximum());

  C.Print(outpath+"/CELLELow.png");
  C.SetLogy(1);
  C.Print(outpath+"/CELLELow_log.png");
  C.SetLogy(0);



  // TH2F HCELLEnergyVsR("HCELLEnergyVsR","",60,0,600,100,0,1);
  // HCELLEnergyVsR.GetYaxis()->SetTitle("energy  [MeV]");
  // HCELLEnergyVsR.GetXaxis()->SetTitle("radius [mm]");
  // TREE->Draw("cell_E:cell_r>>HCELLEnergyVsR");
  // C2D.cd();
  // C2D.Clear();
  // HCELLEnergyVsR.Draw("histcolz");
  // C2D.Print(outpath+"/CELLEVsR.png");



  ///CELL delta T vs energy correlation
  TH2F HCELLDTvsE("HCELLDTvsE","",60,-1,2,100,0,10);
  HCELLDTvsE.GetXaxis()->SetTitle("cell true dt [ns]");
  HCELLDTvsE.GetYaxis()->SetTitle("cell energy [MeV]");
  TREE->Draw("cell_E:cell_dttrue>>HCELLDTvsE");
  C2D.cd();
  C2D.Clear();
  HCELLDTvsE.Draw("colz");
  //C.SetLogz(1);
  C2D.Print(outpath+"/CELLDTvsE.png");
  //C.SetLogz(0);

}


void plotCellMoments(TChain*TREE,TString outpath){

    TCanvas C;

    int NBINS=6;
    float xmin=0.;
    float xmax=0.6;
  
    ////Number of cells vs deltaR 
    TH1F HN_signal("HN_signal","",NBINS,xmin,xmax);
    TREE->Draw("cell_jetdR>>HN_signal",CUTSignalJetCell);
    HN_signal.Scale(1./HN_signal.Integral());

    TH1F HN_pu("HN_pu","",NBINS,xmin,xmax);
    TREE->Draw("cell_jetdR>>HN_pu",CUTPUJetCell);
    HN_pu.Scale(1./HN_pu.Integral());  

    C.Clear();
    HN_signal.GetYaxis()->SetTitle(" number of cells ");
    HN_signal.GetXaxis()->SetTitle(" cell deltaR ");
    HN_signal.Draw("histp");
    HN_pu.Draw("histsame");
    C.Print(outpath+"/CELL_jetdR_SignalAndPU.png");


    ////Number of cells vs deltaR energy weighted
    TH1F HE_signal("HE_signal","",NBINS,xmin,xmax);
    TREE->Draw("cell_jetdR>>HE_signal",CUTSignalJetCell+"*(cell_E)");
    HE_signal.Scale(1./HE_signal.Integral());

    TH1F HE_pu("HE_pu","",NBINS,xmin,xmax);
    TREE->Draw("cell_jetdR>>HE_pu",CUTPUJetCell+"*(cell_E)");
    HE_pu.Scale(1./HE_pu.Integral());  

    C.Clear();
    HE_signal.GetYaxis()->SetTitle(" number of cells E weighted ");
    HE_signal.GetXaxis()->SetTitle(" cell deltaR ");
    HE_signal.Draw("histp");
    HE_pu.Draw("histsame");
    C.Print(outpath+"/CELL_jetdR_Eweight_SignalAndPU.png");

}

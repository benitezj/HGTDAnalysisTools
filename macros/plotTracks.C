
TString trackSelection="(trk_truth_vtxt!=0.&&abs(trk_truth_vtxt)<0.6&&trk_pt>1000)"; //nominal good track
TString trackEtaSelection="(2.5<abs(trk_eta)&&abs(trk_eta)<3.9)"; //nominal good track
TString trackHiPtSel="(trk_pt>5000)"; //high pT track
TString trackCellSel="(trk_ncell>0)"; //matching cells 
//TString trk1mmSel="(abs(trk_ext_x)<297&&abs(trk_ext_y)<297)";
//TString trk3mmSel="(abs(trk_ext_x)>297||abs(trk_ext_y)>297)";
TString trk1mmSel="(1)";
TString trk3mmSel="(0)";
TString trackPlotLabel="track p_{T} > 1 GeV";

TString trackHGTrkSel="(trk_hgtrk>=0)"; //matching tracklet


void plotTracks(TChain*TREE,TString outpath){
  TCanvas C;

  //Plot the number of tracks in the event
  TH1F HTrkN("HTrkN","",100,0,100);
  HTrkN.GetXaxis()->SetTitle("Number of tracks");
  TREE->Draw("ntrk>>HTrkN",trackSelection);
  C.Clear();
  HTrkN.Draw("hist");
  C.Print(outpath+"/Track_n.png");


  //Plot the track energy
  TH1F HTrkE("HTrkE","",100,0,200);
  HTrkE.GetXaxis()->SetTitle("Track p   [GeV]");
  TREE->Draw("trk_e/1000>>HTrkE",trackSelection);
  C.Clear();
  HTrkE.Draw("hist");
  C.Print(outpath+"/Track_E.png");

  //Plot the track pT
  TH1F HTrkPt("HTrkPt","",50,0,25);
  HTrkPt.GetXaxis()->SetTitle("Track p_{T}   [GeV]");
  TREE->Draw("trk_pt/1000>>HTrkPt",trackSelection);
  C.Clear();
  HTrkPt.Draw("hist");
  C.Print(outpath+"/Track_pt.png");


  //Plot the eta distribution           
  TH1F HTrkEta("HTrkEta","",18,2.4,4.2);
  HTrkEta.GetXaxis()->SetTitle("track |#eta|");
  TREE->Draw("trk_eta>>HTrkEta",trackSelection);
  C.Clear();
  HTrkEta.Draw("hist");
  C.Print(outpath+"/Track_eta.png");

  //Plot the phi distribution           
  TH1F HTrkPhi("HTrkPhi","",100,3.2,3.2);
  HTrkPhi.GetXaxis()->SetTitle("track phi");
  TREE->Draw("trk_phi>>HTrkPhi",trackSelection);
  C.Clear();
  HTrkPhi.Draw("hist");
  C.Print(outpath+"/Track_phi.png");


  //Plot the eta distribution           
  TH2F HTrkExtEtaVsEta("HTrkExtEtaVsEta","",90,2.4,4.2,90,2.4,4.2);
  HTrkExtEtaVsEta.GetXaxis()->SetTitle("track eta");
  HTrkExtEtaVsEta.GetYaxis()->SetTitle("track extrapolation eta");
  TREE->Draw("trk_ext_eta:trk_eta>>HTrkExtEtaVsEta",trackSelection);
  C2D.Clear();
  HTrkExtEtaVsEta.Draw("colz");
  C2D.Print(outpath+"/Track_ext_eta_vs_eta.png");



  //Plot the track energy
  TH1F HTrkdZ0("HTrkdZ0","",100,-20,20);
  HTrkdZ0.GetXaxis()->SetTitle("Track z - true vertex z   [mm]");
  TREE->Draw("trk_z0 - truevtx_z>>HTrkdZ0",trackSelection);
  C.Clear();
  HTrkdZ0.Draw("hist");
  C.Print(outpath+"/Track_truedz0.png");

  TH1F HTrkd0("HTrkd0","",100,0,1);
  HTrkd0.GetXaxis()->SetTitle("Track d0 [mm]");
  TREE->Draw("trk_d0>>HTrkd0",trackSelection);
  C.Clear();
  HTrkd0.Draw("hist");
  C.Print(outpath+"/Track_d0.png");


  ////////////////////////////////////
  /////Extrapolation distributions
  ////////////////////////////////////

  //difference in eta distribution           
  TH1F HTrkdEta("HTrkdEta","",100,-0.25,0.25);
  HTrkdEta.GetXaxis()->SetTitle("track extrapolation eta - track eta");
  TREE->Draw("trk_ext_eta-trk_eta>>HTrkdEta",trackSelection);
  C.Clear();
  HTrkdEta.Draw("hist");
  C.Print(outpath+"/Track_dEtaExtrap.png");

  //difference in phi distribution           
  TH1F HTrkdPhi("HTrkdPhi","",100,-0.25,0.25);
  HTrkdPhi.GetXaxis()->SetTitle("track extrapolation phi - track phi");
  TREE->Draw("trk_ext_phi-trk_phi>>HTrkdPhi",trackSelection);
  C.Clear();
  HTrkdPhi.Draw("hist");
  C.Print(outpath+"/Track_dPhiExtrap.png");


  //////////////////////////
  ////// position at HGTD
  //////////////////////////
  //Plot the eta distribution           
  TH1F HTrkExtEta("HTrkExtEta","",100,-5,5);
  HTrkExtEta.GetXaxis()->SetTitle("track extrapolation eta");
  TREE->Draw("trk_ext_eta>>HTrkExtEta",trackSelection);
  C.Clear();
  HTrkExtEta.Draw("hist");
  C.Print(outpath+"/Track_ext_eta.png");

  //Plot the phi distribution           
  TH1F HTrkExtPhi("HTrkExtPhi","",100,3.2,3.2);
  HTrkExtPhi.GetXaxis()->SetTitle("track extrapolation phi");
  TREE->Draw("trk_ext_phi>>HTrkExtPhi",trackSelection);
  C.Clear();
  HTrkExtPhi.Draw("hist");
  C.Print(outpath+"/Track_ext_phi.png");


  //Plot radial distribution           
  TH1F HTrkR("HTrkR","",100,0,600);
  HTrkR.GetXaxis()->SetTitle("track extrapolation r [mm]");
  TREE->Draw("sqrt(trk_ext_x*trk_ext_x+trk_ext_y*trk_ext_y)>>HTrkR",trackSelection);
  C.Clear();
  HTrkR.Draw("hist");
  C.Print(outpath+"/Track_r.png");

  //Plot x-y distribution  
  TCanvas C2("C2","",700,700);
  C2.SetRightMargin(0.15);
  C2.SetTopMargin(0.15);

  TH2F HTrkXY("HTrkXY","",120,-600,600,120,-600,600);
  HTrkXY.GetXaxis()->SetTitle("X  [mm]");
  HTrkXY.GetYaxis()->SetTitle("Y  [mm]");
  TREE->Draw("trk_ext_y:trk_ext_x>>HTrkXY",trackSelection);
  C2.Clear();
  HTrkXY.Draw("histcolz");
  drawSensors(1,1);
  drawSensors(-1,1);
  C2.Print(outpath+"/Track_XY.png");


  ////////////////////////////
  //// high pT tracks with no hits
  //////////////////////////////
  TH2F HTrkXYNoHit("HTrkXYNoHit","",120,-600,600,120,-600,600);
  HTrkXYNoHit.GetXaxis()->SetTitle("X  [mm]");
  HTrkXYNoHit.GetYaxis()->SetTitle("Y  [mm]");
  TREE->Draw("trk_ext_y:trk_ext_x>>HTrkXYNoHit",trackSelection+"&&(trk_pt>5000&&trk_ncell==0)");
  C2.Clear();
  HTrkXYNoHit.Draw("histcolz");
  drawSensors(1,1);
  drawSensors(-1,1);
  C2.Print(outpath+"/Track_XYNoHit.png");

  TH2F HTrkXYNoHitQ1("HTrkXYNoHitQ1","",120,0,600,120,0,600);
  HTrkXYNoHitQ1.GetXaxis()->SetTitle("X  [mm]");
  HTrkXYNoHitQ1.GetYaxis()->SetTitle("Y  [mm]");
  TREE->Draw("abs(trk_ext_y):abs(trk_ext_x)>>HTrkXYNoHitQ1",trackSelection+"&&(trk_pt>5000&&trk_ncell==0)");
  C2.Clear();
  HTrkXYNoHitQ1.Draw("histcolz");
  drawSensors(1,0);
  C2.Print(outpath+"/Track_XYNoHitQ1.png");

  TH2F HTrkXYNoHitQ1Zoom("HTrkXYNoHitQ1Zoom","",100,100,200,100,100,200);
  HTrkXYNoHitQ1Zoom.GetXaxis()->SetTitle("X  [mm]");
  HTrkXYNoHitQ1Zoom.GetYaxis()->SetTitle("Y  [mm]");
  TREE->Draw("abs(trk_ext_y):abs(trk_ext_x)>>HTrkXYNoHitQ1Zoom",trackSelection+"&&(trk_pt>5000&&trk_ncell==0)");
  C2.Clear();
  HTrkXYNoHitQ1Zoom.Draw("histcolz");
  drawSensors(1,0);
  C2.Print(outpath+"/Track_XYNoHitQ1Zoom.png");



  ////////////////////////////
  /// outliers
  ////////////////////////////
  TH1F HTrkOutliers("HTrkOutliers","",5,0,5);
  HTrkOutliers.GetXaxis()->SetTitle("number of time outliers per track");
  HTrkOutliers.GetYaxis()->SetTitle("number of tracks");
  TREE->Draw("trk_outlier>>HTrkOutliers",trackSelection);
  C.Clear();
  HTrkOutliers.GetYaxis()->SetRangeUser(0,HTrkOutliers.GetMaximum()*1.2);
  HTrkOutliers.Draw("hist");
  C.Print(outpath+"/Track_outlier.png");


}


void plotTrackCells(TChain*TREE,TString outpath){
  /// for now only high pT tracks > 5GeV
  /// track selection in the 2.5<eta<4.0
  
  TString trackPtCut="(trk_pt>5000)"; 

  TString cellSelection="(abs(cell_S)==1&&cell_trk>=0&&trk_ext_eta[cell_trk]!=0&&abs(trk_eta[cell_trk])>2.5&&abs(trk_eta[cell_trk])<4.0)";
  TString cellTrkHiPtSel="(trk_pt[cell_trk]>5000)";
  TString cellTrkLoPtSel="(trk_pt[cell_trk]<5000)";
  TString cell1mmSel="(cell_type==1)";
  TString cell3mmSel="(cell_type==2)";


  ////# of cells associated in Layer 0 vs radius
  TH2F HTrak_ncellvsr("HTrak_ncellvsr","",60,0,600,10,-0.5,9.5);
  HTrak_ncellvsr.GetXaxis()->SetTitle("radius  [mm]");
  HTrak_ncellvsr.GetYaxis()->SetTitle("number of matched cells");
  TREE->Draw("trk_ncell:sqrt(trk_ext_x*trk_ext_x+trk_ext_y*trk_ext_y)>>HTrak_ncellvsr",trackSelection+"&&"+trackPtCut);
  C2D.Clear();
  HTrak_ncellvsr.Draw("colz");
  C2D.Print(outpath+"/TrackCell_Trak_ncellvsr.png");


  ////# of cells associated in all layers
  TH1F HTrak_ncell("HTrak_ncell","",10,-0.5,9.5);
  HTrak_ncell.GetXaxis()->SetTitle("number of matched cells");
  TREE->Draw("trk_ncell>>HTrak_ncell",trackSelection+"&&"+trackPtCut);
  C1D.Clear();
  HTrak_ncell.Draw("hist");
  C1D.Print(outpath+"/TrackCell_Trak_ncellAll.png");


  ///// cell distance to track
  TH1F HdR_1mm("HdR_1mm","",100,0,10);
  HdR_1mm.GetXaxis()->SetTitle("cell - track distance    [mm]");
  TREE->Draw("cell_trkdR>>HdR_1mm",cellSelection+"&&"+cellTrkHiPtSel+"&&"+cell1mmSel);
  C1D.Clear();
  HdR_1mm.Draw("hist");
  C1D.Print(outpath+"/TrackCell_dRHiPt_1mm.png");

  TH1F HdR_3mm("HdR_3mm","",100,0,10);
  HdR_3mm.GetXaxis()->SetTitle("cell - track distance    [mm]");
  TREE->Draw("cell_trkdR>>HdR_3mm",cellSelection+"&&"+cellTrkHiPtSel+"&&"+cell3mmSel);
  C1D.Clear();
  HdR_3mm.Draw("hist");
  C1D.Print(outpath+"/TrackCell_dRHiPt_3mm.png");

  //// cell 2D map from track position in eta-phi
  TH2F HdEtadPhi("HdEtadPhi","",100,-0.1,0.1,100,-0.1,0.1);
  HdEtadPhi.GetXaxis()->SetTitle("cell phi - track phi");
  HdEtadPhi.GetYaxis()->SetTitle("cell eta - track eta");
  TREE->Draw(dEtaCellTrk+":"+dPhiCellTrk+">>HdEtadPhi",cellSelection+"&&"+cellTrkHiPtSel);
  C2D.Clear();
  HdEtadPhi.Draw("colz");
  C2D.Print(outpath+"/TrackCell_dEtadPhi.png");

  ////cell 2D map from tack position in x-y
  TH2F Hdxdy("Hdxdy","",50,-10,10,50,-10,10);
  Hdxdy.GetXaxis()->SetTitle("cell x - track x");
  Hdxdy.GetYaxis()->SetTitle("cell y - track y");
  TREE->Draw("cell_y-trk_ext_y[cell_trk]:cell_x-trk_ext_x[cell_trk]>>Hdxdy",cellSelection+"&&"+cellTrkHiPtSel);
  C2D.Clear();
  Hdxdy.Draw("colz");
  C2D.Print(outpath+"/TrackCell_dxdy.png");



  ///Check how many cells did not have matching track
  TH1F HTrak_cellr("HTrak_cellr","",60,0,600);
  HTrak_cellr.GetXaxis()->SetTitle("cell radius  [mm]");
  TREE->Draw("cell_r>>HTrak_cellr","abs(cell_S)==1");
  C1D.Clear();
  HTrak_cellr.Draw("hist");
  TREE->Draw("cell_r","abs(cell_S)==1&&cell_trk>=0","psame");
  C1D.Print(outpath+"/TrackCell_cellrvsNotrk.png");
  

}



void plotTrackCellResolutionVsPt(TChain*TREE,TString outpath, bool dXFits=0){
 
  ///Only layer 0 quantities
  TString cellSelection="(abs(cell_S)==1&&cell_trk>=0&&2.5<abs(trk_eta[cell_trk])&&abs(trk_eta[cell_trk])<4.0)";
  TString cell1mmSel="(cell_type==1)";
  TString cell3mmSel="(cell_type==2)";

  int maxpt=22;
  int maxradius=30;
  int maxradiusdR=100;
  int dRTailMin=5;

  TCanvas C1mm("C1mm","",1000,500);

  TH1F HIntegral1mm("HIntegral1mm","",maxpt,0,maxpt);
  HIntegral1mm.GetYaxis()->SetTitle("number of tracks");
  HIntegral1mm.GetXaxis()->SetTitle("track pT   [GeV] ");

  TH1F HSigma1mm("HSigma1mm","",maxpt,0,maxpt);
  HSigma1mm.GetYaxis()->SetTitle("#sigma   [mm]");
  HSigma1mm.GetXaxis()->SetTitle("track pT   [GeV] ");

  TH1F HRMS1mm("HRMS1mm","",maxpt,0,maxpt);
  HRMS1mm.GetYaxis()->SetTitle("#sigma   [mm]");
  HRMS1mm.GetXaxis()->SetTitle("track pT   [GeV] ");

  TH1F HGaus2Frac1mm("HGaus2Frac1mm","",maxpt,0,maxpt);
  HGaus2Frac1mm.GetYaxis()->SetTitle("fraction2");
  HGaus2Frac1mm.GetXaxis()->SetTitle("track pT   [GeV] ");

  TH1F HGaus2Sigma1mm("HGaus2Sigma1mm","",maxpt,0,maxpt);
  HGaus2Sigma1mm.GetYaxis()->SetTitle("#sigma2  [mm]");
  HGaus2Sigma1mm.GetXaxis()->SetTitle("track pT   [GeV] ");

  
  TH1F H99Cut1mm("H99Cut1mm","",maxpt,0,maxpt);
  H99Cut1mm.GetYaxis()->SetTitle("matching radius [mm]");
  H99Cut1mm.GetXaxis()->SetTitle("track pT   [GeV] ");

  TH1F H95Cut1mm("H95Cut1mm","",maxpt,0,maxpt);
  H95Cut1mm.GetYaxis()->SetTitle("matching radius  [mm]");
  H95Cut1mm.GetXaxis()->SetTitle("track pT   [GeV] ");

  TH1F H99dRCut1mm("H99dRCut1mm","",maxpt,0,maxpt);
  H99dRCut1mm.GetYaxis()->SetTitle("matching radius [mm]");
  H99dRCut1mm.GetXaxis()->SetTitle("track pT   [GeV] ");

  TH1F H95dRCut1mm("H95dRCut1mm","",maxpt,0,maxpt);
  H95dRCut1mm.GetYaxis()->SetTitle("matching radius  [mm]");
  H95dRCut1mm.GetXaxis()->SetTitle("track pT   [GeV] ");

  TH1F HdRTailFrac1mm("HdRTailFrac1mm","",maxpt,0,maxpt);
  HdRTailFrac1mm.GetYaxis()->SetTitle("tail fraction");
  HdRTailFrac1mm.GetXaxis()->SetTitle("track pT   [GeV] ");



  TCanvas C3mm("C3mm","",1000,500);

  TH1F HIntegral3mm("HIntegral3mm","",maxpt,0,maxpt);
  HIntegral3mm.GetYaxis()->SetTitle("number of tracks");
  HIntegral3mm.GetXaxis()->SetTitle("track pT   [GeV] ");

  TH1F HSigma3mm("HSigma3mm","",maxpt,0,maxpt);
  HSigma3mm.GetYaxis()->SetTitle("#sigma   [mm] ");
  HSigma3mm.GetXaxis()->SetTitle("track pT   [GeV] ");

  TH1F HRMS3mm("HRMS3mm","",maxpt,0,maxpt);
  HRMS3mm.GetYaxis()->SetTitle("#sigma   [mm]");
  HRMS3mm.GetXaxis()->SetTitle("track pT   [GeV] ");


  TH1F HGaus2Frac3mm("HGaus2Frac3mm","",maxpt,0,maxpt);
  HGaus2Frac3mm.GetYaxis()->SetTitle("fraction2");
  HGaus2Frac3mm.GetXaxis()->SetTitle("track pT   [GeV] ");

  TH1F HGaus2Sigma3mm("HGaus2Sigma3mm","",maxpt,0,maxpt);
  HGaus2Sigma3mm.GetYaxis()->SetTitle("#sigma2  [mm]");
  HGaus2Sigma3mm.GetXaxis()->SetTitle("track pT   [GeV] ");

  
  TH1F H99Cut3mm("H99Cut3mm","",maxpt,0,maxpt);
  H99Cut3mm.GetYaxis()->SetTitle("matching radius [mm]");
  H99Cut3mm.GetXaxis()->SetTitle("track pT   [GeV] ");

  TH1F H95Cut3mm("H95Cut3mm","",maxpt,0,maxpt);
  H95Cut3mm.GetYaxis()->SetTitle("matching radius  [mm]");
  H95Cut3mm.GetXaxis()->SetTitle("track pT   [GeV] ");


  TH1F H99dRCut3mm("H99dRCut3mm","",maxpt,0,maxpt);
  H99dRCut3mm.GetYaxis()->SetTitle("matching radius [mm]");
  H99dRCut3mm.GetXaxis()->SetTitle("track pT   [GeV] ");

  TH1F H95dRCut3mm("H95dRCut3mm","",maxpt,0,maxpt);
  H95dRCut3mm.GetYaxis()->SetTitle("matching radius  [mm]");
  H95dRCut3mm.GetXaxis()->SetTitle("track pT   [GeV] ");

  TH1F HdRTailFrac3mm("HdRTailFrac3mm","",maxpt,0,maxpt);
  HdRTailFrac3mm.GetYaxis()->SetTitle("tail fraction");
  HdRTailFrac3mm.GetXaxis()->SetTitle("track pT   [GeV] ");



  C1mm.Divide(2,1);
  C3mm.Divide(2,1);
  for(long p=1;p<maxpt;p+=1){

    C1D.cd();
    TString cellTrkPtSel=TString("(")+(p-1)+"<trk_pt[cell_trk]/1000&&trk_pt[cell_trk]/1000<"+(p)+")";

    /////////////////////////////////
    /// scan the dR distribution
    /////////////////////////////////
    TH1F*HdR_1mm=new TH1F(TString("HdR_1mm")+p,"",maxradiusdR,0,maxradiusdR);
    HdR_1mm->GetXaxis()->SetTitle("x-y distance to track  [mm]");
    TREE->Draw(TString("cell_trkdR>>HdR_1mm")+p,cellSelection+"&&"+cell1mmSel+"&&"+cellTrkPtSel);
    if( HdR_1mm->Integral()>3000 ){
      float frac=0;
      for(int b=1;b<HdR_1mm->GetNbinsX()/2;b++){
	frac+= HdR_1mm->GetBinContent(HdR_1mm->GetXaxis()->GetNbins()-b+1)/HdR_1mm->Integral();
	if(frac<0.01){
	  H99dRCut1mm.SetBinContent(p,maxradius-b*(HdR_1mm->GetXaxis()->GetBinWidth(1)));
	  H99dRCut1mm.SetBinError(p,HdR_1mm->GetXaxis()->GetBinWidth(1));
	}
	if(frac<0.05){
	  H95dRCut1mm.SetBinContent(p,maxradius-b*(HdR_1mm->GetXaxis()->GetBinWidth(1)));
	  H95dRCut1mm.SetBinError(p,HdR_1mm->GetXaxis()->GetBinWidth(1));
	}
      }

      HdRTailFrac1mm.SetBinContent(p,HdR_1mm->Integral(dRTailMin,HdR_1mm->GetNbinsX()+1)/(float)HdR_1mm->Integral(0,HdR_1mm->GetNbinsX()+1));
      HdRTailFrac1mm.SetBinError(p,sqrt(HdR_1mm->Integral(dRTailMin,HdR_1mm->GetNbinsX()+1))/(float)HdR_1mm->Integral(0,HdR_1mm->GetNbinsX()+1));

    }
    
    TH1F*HdR_3mm=new TH1F(TString("HdR_3mm")+p,"",maxradiusdR,0,maxradiusdR);
    HdR_3mm->GetXaxis()->SetTitle("x-y distance to track  [mm]");
    TREE->Draw(TString("cell_trkdR>>HdR_3mm")+p,cellSelection+"&&"+cell3mmSel+"&&"+cellTrkPtSel);
    if( HdR_3mm->Integral()>3000 ){
      float frac=0;
      for(int b=1;b<HdR_3mm->GetNbinsX()/2;b++){
	frac+= HdR_3mm->GetBinContent(HdR_3mm->GetXaxis()->GetNbins()-b+1)/HdR_3mm->Integral();
	if(frac<0.01){
	  H99dRCut3mm.SetBinContent(p,maxradius-b*(HdR_3mm->GetXaxis()->GetBinWidth(1)));
	  H99dRCut3mm.SetBinError(p,HdR_3mm->GetXaxis()->GetBinWidth(1));
	}
	if(frac<0.05){
	  H95dRCut3mm.SetBinContent(p,maxradius-b*(HdR_3mm->GetXaxis()->GetBinWidth(1)));
	  H95dRCut3mm.SetBinError(p,HdR_3mm->GetXaxis()->GetBinWidth(1));
	}
      }

      
      HdRTailFrac3mm.SetBinContent(p,HdR_3mm->Integral(dRTailMin,HdR_3mm->GetNbinsX()+1)
				   /(float)HdR_3mm->Integral(0,HdR_3mm->GetNbinsX()+1));
      HdRTailFrac3mm.SetBinError(p,sqrt(HdR_3mm->Integral(dRTailMin,HdR_3mm->GetNbinsX()+1))
				 /(float)HdR_3mm->Integral(0,HdR_3mm->GetNbinsX()+1));


    }



    ///////////////////////////////
    //// fit dX distributions
    ///////////////////////////////
    if(dXFits){
      TVirtualPad * pad1mm=NULL;

      if(p==2)      pad1mm = C1mm.cd(1);
      else if(p==6) pad1mm = C1mm.cd(2);
      else C1D.cd();
      
      TH1F*HdX_1mm=new TH1F(TString("HdX_1mm")+p,"",2*2*2*maxradius,-maxradius,maxradius);
      HdX_1mm->GetXaxis()->SetTitle("cell x   -  track x    [mm]");
      TREE->Draw(TString("cell_x - trk_hgtd0_x[cell_trk]>>HdX_1mm")+p,cellSelection+"&&"+cell1mmSel+"&&"+cellTrkPtSel);
      TF1*F1mm=0;
      if(HdX_1mm->Integral()>3000){
	
	HIntegral1mm.SetBinContent(p,HdX_1mm->Integral());

	F1mm=fitMatchResolutionNew(HdX_1mm,-maxradius,maxradius,0.64,0.85);

	float frac=0.;
	for(int b=0;b<=HdX_1mm->GetNbinsX()+1;b++)
	  if(HdX_1mm->GetXaxis()->GetBinCenter(b)<(F1mm->GetParameter(1)-4*F1mm->GetParameter(2))
	     || HdX_1mm->GetXaxis()->GetBinCenter(b)>(F1mm->GetParameter(1)+4*F1mm->GetParameter(2)))
	    frac+=HdX_1mm->GetBinContent(b);
	frac=frac/HdX_1mm->Integral();
	float fracerr=sqrt(frac)/HdX_1mm->Integral();

	if(F1mm
	   && F1mm->GetParError(2)/F1mm->GetParameter(2)<0.1
	   && frac<0.3
	   ){

	  HSigma1mm.SetBinContent(p,F1mm->GetParameter(2));
	  HSigma1mm.SetBinError(p,F1mm->GetParError(2));
	
	  HRMS1mm.SetBinContent(p,HdX_1mm->GetRMS());
	  HRMS1mm.SetBinError(p,0);

	  //if(F1mm->GetParError(3)/F1mm->GetParameter(3)<0.1){
	  // HGaus2Frac1mm.SetBinContent(p,F1mm->GetParameter(3));
	  // HGaus2Frac1mm.SetBinError(p,F1mm->GetParError(3));
	  // HGaus2Sigma1mm.SetBinContent(p,F1mm->GetParameter(4));
	  // HGaus2Sigma1mm.SetBinError(p,F1mm->GetParError(4));
	    
	  HGaus2Frac1mm.SetBinContent(p,frac);
	  HGaus2Frac1mm.SetBinError(p,fracerr);
	  //HGaus2Sigma1mm.SetBinContent(p,F1mm->GetParameter(4));
	  //HGaus2Sigma1mm.SetBinError(p,F1mm->GetParError(4));

	  //}
      

	  ///scan the 99 and 95% selectiont cut
	  float frac=0;
	  for(int b=1;b<HdX_1mm->GetNbinsX()/2;b++){
	    frac+=(HdX_1mm->GetBinContent(b)+HdX_1mm->GetBinContent(HdX_1mm->GetXaxis()->GetNbins()-b+1))/HdX_1mm->Integral();
	    //cout<<p<<" "<<b<<" "<<frac<<endl;
	    if(frac<0.01)
	      H99Cut1mm.SetBinContent(p,maxradius-b*(HdX_1mm->GetXaxis()->GetBinWidth(1)));
	    if(frac<0.05)
	      H95Cut1mm.SetBinContent(p,maxradius-b*(HdX_1mm->GetXaxis()->GetBinWidth(1)));
	  }

	  //HdX_1mm->Draw("histpe");
	  //F1mm->Draw("same");
	}      

      }


      TVirtualPad * pad3mm = NULL;
      if(p==2)      pad3mm = C3mm.cd(1);
      else if(p==6) pad3mm = C3mm.cd(2);
      else C1D.cd();

      TH1F*HdX_3mm=new TH1F(TString("HdX_3mm")+p,"",2*2*2*maxradius,-maxradius,maxradius);
      HdX_3mm->GetXaxis()->SetTitle("cell x   -  track x    [mm]");
      TREE->Draw(TString("cell_x - trk_hgtd0_x[cell_trk]>>HdX_3mm")+p,cellSelection+"&&"+cell3mmSel+"&&"+cellTrkPtSel);
      TF1*F3mm=0;
      if(HdX_3mm->Integral()>3000){

	HIntegral3mm.SetBinContent(p,HdX_3mm->Integral());

	F3mm=fitMatchResolutionNew(HdX_3mm,-maxradius,maxradius,0.64,0.85);

	float frac=0.;
	for(int b=0;b<=HdX_3mm->GetNbinsX()+1;b++)
	  if(HdX_3mm->GetXaxis()->GetBinCenter(b)<(F3mm->GetParameter(1)-4*F3mm->GetParameter(2))
	     ||HdX_3mm->GetXaxis()->GetBinCenter(b)>(F3mm->GetParameter(1)+4*F3mm->GetParameter(2)))
	    frac+=HdX_3mm->GetBinContent(b);
	frac=frac/HdX_1mm->Integral();
	float fracerr=sqrt(frac)/HdX_1mm->Integral();
	
	if(F3mm
	   && F3mm->GetParError(2)/F3mm->GetParameter(2)<0.1
	   && frac<0.3
	   ){

	  HSigma3mm.SetBinContent(p,F3mm->GetParameter(2));
	  HSigma3mm.SetBinError(p,F3mm->GetParError(2));

	  HRMS3mm.SetBinContent(p,HdX_3mm->GetRMS());
	  HRMS3mm.SetBinError(p,0);


	  //if(F3mm->GetParError(3)/F3mm->GetParameter(3)<0.2){
	  // HGaus2Frac3mm.SetBinContent(p,F3mm->GetParameter(3));
	  // HGaus2Frac3mm.SetBinError(p,F3mm->GetParError(3));
	  // HGaus2Sigma3mm.SetBinContent(p,F3mm->GetParameter(4));
	  // HGaus2Sigma3mm.SetBinError(p,F3mm->GetParError(4));
	    
	  HGaus2Frac3mm.SetBinContent(p,frac);
	  HGaus2Frac3mm.SetBinError(p,fracerr);
	  
	  //}


	  ///scan the 99 and 95% selectiont cut
	  float frac=0;
	  for(int b=1;b<HdX_3mm->GetNbinsX()/2;b++){
	    frac+=(HdX_3mm->GetBinContent(b)+HdX_3mm->GetBinContent(HdX_3mm->GetXaxis()->GetNbins()-b+1))/HdX_3mm->Integral();
	    //cout<<p<<" "<<b<<" "<<frac<<endl;
	    if(frac<0.01)
	      H99Cut3mm.SetBinContent(p,maxradius-b*(HdX_3mm->GetXaxis()->GetBinWidth(1)));
	    if(frac<0.05)
	      H95Cut3mm.SetBinContent(p,maxradius-b*(HdX_3mm->GetXaxis()->GetBinWidth(1)));
	  }

	  //HdX_3mm->Draw("histpe");
	  //F3mm->Draw("same");
	}

      }

      if(pad1mm) pad1mm->SetLogy(1);
      if(pad3mm) pad3mm->SetLogy(1);

    }
  }


  //////////////////
  // sample fit plots
  ///////////////////
  C1mm.Print(outpath+"/TrackCell_dXVsPt_1mm.png");
  C3mm.Print(outpath+"/TrackCell_dXVsPt_3mm.png");

  ////////////////////////
  /// for debugging
  ////////////////////////
  C1D.Clear();
  HIntegral1mm.Draw("histp");
  C1D.Print(outpath+"/TrackCell_NTrack_1mm.png");

  C1D.Clear();
  HIntegral3mm.Draw("histp");
  C1D.Print(outpath+"/TrackCell_NTrack_3mm.png");
  
  
  ///////////////////////
  /// Core resolutions vs pT
  ////////////////////////
  C1D.Clear();
  HSigma1mm.GetYaxis()->SetRangeUser(0,2);
  TF1*FRes1mm=fitResolutionVsPt(&HSigma1mm,1,25,0.7,0.8);
  HSigma1mm.Draw("histpe");
  FRes1mm->Draw();
  C1D.Print(outpath+"/TrackCell_dXVsPtRes_1mm.png");

  TF1*FRes3mm=fitResolutionVsPt(&HSigma3mm,1,25,0.7,0.8);
  C1D.Clear();
  HSigma3mm.GetYaxis()->SetRangeUser(0,2);
  HSigma3mm.Draw("histpe");
  FRes3mm->Draw("same");
  C1D.Print(outpath+"/TrackCell_dXVsPtRes_3mm.png");


  ///////////////////////
  /// Tail componet vs pT
  ////////////////////////
  C1D.Clear();
  HGaus2Frac1mm.GetYaxis()->SetRangeUser(0,0.3);
  HGaus2Frac1mm.Draw("histpe");
  C1D.Print(outpath+"/TrackCell_dXVsPtGaus2Frac_1mm.png");

  C1D.Clear();
  HGaus2Sigma1mm.GetYaxis()->SetRangeUser(0,10);
  HGaus2Sigma1mm.Draw("histpe");
  C1D.Print(outpath+"/TrackCell_dXVsPtGaus2Res_1mm.png");

  C1D.Clear();
  HGaus2Frac3mm.GetYaxis()->SetRangeUser(0,0.3);
  HGaus2Frac3mm.Draw("histpe");
  C1D.Print(outpath+"/TrackCell_dXVsPtGaus2Frac_3mm.png");

  C1D.Clear();
  HGaus2Sigma3mm.GetYaxis()->SetRangeUser(0,10);
  HGaus2Sigma3mm.Draw("histpe");
  C1D.Print(outpath+"/TrackCell_dXVsPtGaus2Res_3mm.png");

  ////////////////////////////
  /// Tail fraction based on dR 
  /////////////////////////////  
  C1D.Clear();
  HdRTailFrac1mm.GetYaxis()->SetRangeUser(0,0.3);
  HdRTailFrac1mm.Draw("histpe");
  C1D.Print(outpath+"/TrackCell_dRVsPtTailFrac_1mm.png");

  C1D.Clear();
  HdRTailFrac3mm.GetYaxis()->SetRangeUser(0,0.3);
  HdRTailFrac3mm.Draw("histpe");
  C1D.Print(outpath+"/TrackCell_dRVsPtTailFrac_3mm.png");



  //////////////////////////////////////////
  //compare matching radius selections
  ///////////////////////////////////////////
  TH1F H2SigmaCut1mm("H2SigmaCut1mm","",maxpt,0,maxpt);
  H2SigmaCut1mm.GetYaxis()->SetTitle("matching radius  [mm]");
  H2SigmaCut1mm.GetXaxis()->SetTitle("track pT   [GeV] ");

  TH1F H3SigmaCut1mm("H3SigmaCut1mm","",maxpt,0,maxpt);
  H3SigmaCut1mm.GetYaxis()->SetTitle("matching radius  [mm]");
  H3SigmaCut1mm.GetXaxis()->SetTitle("track pT   [GeV] ");
 
  for(int p=1;p<=maxpt;p++){
    H2SigmaCut1mm.SetBinContent(p,2*FRes1mm->Eval(p-0.5));
    H3SigmaCut1mm.SetBinContent(p,3*FRes1mm->Eval(p-0.5));
  }

  C1D.Clear();
  H99Cut1mm.GetYaxis()->SetRangeUser(0,10);
  H99Cut1mm.SetMarkerColor(1);
  H99Cut1mm.Draw("histp");
  fitResolutionVsPt(&H99Cut1mm,0,25,0.4,0.9);
  H95Cut1mm.SetMarkerColor(2);
  H95Cut1mm.Draw("histpsame");
  fitResolutionVsPt(&H95Cut1mm,0,25,0.7,0.9);
  H3SigmaCut1mm.SetLineColor(1);
  H3SigmaCut1mm.Draw("histlsame");
  H2SigmaCut1mm.SetLineColor(2);
  H2SigmaCut1mm.Draw("histlsame");
  C1D.Print(outpath+"/TrackCell_SelectionCutVsPt_1mm.png");


  TH1F H2SigmaCut3mm("H2SigmaCut3mm","",maxpt,0,maxpt);
  H2SigmaCut3mm.GetYaxis()->SetTitle("matching radius  [mm]");
  H2SigmaCut3mm.GetXaxis()->SetTitle("track pT   [GeV] ");

  TH1F H3SigmaCut3mm("H3SigmaCut3mm","",maxpt,0,maxpt);
  H3SigmaCut3mm.GetYaxis()->SetTitle("matching radius  [mm]");
  H3SigmaCut3mm.GetXaxis()->SetTitle("track pT   [GeV] ");
 
  for(int p=1;p<=maxpt;p++){
    H2SigmaCut3mm.SetBinContent(p,2*FRes3mm->Eval(p-0.5));
    H3SigmaCut3mm.SetBinContent(p,3*FRes3mm->Eval(p-0.5));
  }

  C1D.Clear();
  H99Cut3mm.GetYaxis()->SetRangeUser(0,10);
  H99Cut3mm.SetMarkerColor(1);
  H99Cut3mm.Draw("histp");
  fitResolutionVsPt(&H99Cut3mm,0,25,0.4,0.9);
  H95Cut3mm.SetMarkerColor(2);
  H95Cut3mm.Draw("histpsame");
  fitResolutionVsPt(&H95Cut3mm,0,25,0.7,0.9);
  H3SigmaCut3mm.SetLineColor(1);
  H3SigmaCut3mm.Draw("histlsame");
  H2SigmaCut3mm.SetLineColor(2);
  H2SigmaCut3mm.Draw("histlsame");
  C1D.Print(outpath+"/TrackCell_SelectionCutVsPt_3mm.png");


  /////////////////////////////////
  ///dR scan
  /////////////////////////////////
  C1D.Clear();
  H99dRCut1mm.GetYaxis()->SetRangeUser(0,10);
  H99dRCut1mm.SetMarkerColor(1);
  H99dRCut1mm.Draw("histp");
  fitResolutionVsPt(&H99dRCut1mm,0,25,0.4,0.45);
  H95dRCut1mm.SetMarkerColor(2);
  H95dRCut1mm.Draw("histpsame");
  fitResolutionVsPt(&H95dRCut1mm,0,25,0.7,0.45);
  C1D.Print(outpath+"/TrackCell_SelectiondRCutVsPt_1mm.png");


  C1D.Clear();
  H99dRCut3mm.GetYaxis()->SetRangeUser(0,10);
  H99dRCut3mm.SetMarkerColor(1);
  H99dRCut3mm.Draw("histp");
  fitResolutionVsPt(&H99dRCut3mm,1,25,0.4,0.45);
  H95dRCut3mm.SetMarkerColor(2);
  H95dRCut3mm.Draw("histpsame");
  fitResolutionVsPt(&H95dRCut3mm,1,25,0.7,0.45);
  C1D.Print(outpath+"/TrackCell_SelectiondRCutVsPt_3mm.png");




  ///////////////////////
  // compare track extrapolation uncertainty
  //////////////////////
  TH2F HTrkResVsPt_1mm("HTrkResVsPt_1mm","",100,0,25,100,0,10);
  HTrkResVsPt_1mm.GetXaxis()->SetTitle("track  pT [GeV]");
  HTrkResVsPt_1mm.GetYaxis()->SetTitle("extrapolation sigma_x [mm]");
  TREE->Draw("trk_ext_dx*1000:trk_pt/1000>>HTrkResVsPt_1mm",trackSelection+"&&"+trk1mmSel);
  C2D.Clear();
  HTrkResVsPt_1mm.Draw("histcol");
  HRMS1mm.Draw("psame");
  FRes1mm->SetLineColor(2);
  FRes1mm->Draw("lsame");
  C2D.Print(outpath+"/TrackCell_dxResVsPt_1mm.png");


  TH2F HTrkResVsPt_3mm("HTrkResVsPt_3mm","",100,0,25,100,0,10);
  HTrkResVsPt_3mm.GetXaxis()->SetTitle("track  pT [GeV]");
  HTrkResVsPt_3mm.GetYaxis()->SetTitle("extrapolation sigma_x [mm]");
  TREE->Draw("trk_ext_dx*1000:trk_pt/1000>>HTrkResVsPt_3mm",trackSelection+"&&"+trk3mmSel);
  C2D.Clear();
  HTrkResVsPt_3mm.Draw("histcol");
  HRMS3mm.Draw("psame");
  FRes3mm->SetLineColor(2);
  FRes3mm->Draw("lsame");
  C2D.Print(outpath+"/TrackCell_dxResVsPt_3mm.png");


}





void plotBadResTracks(TChain*TREE,TString outpath){

  TString cellSelection="(abs(cell_S)==1&&cell_trk>=0&&2.5<abs(trk_eta[cell_trk])&&abs(trk_eta[cell_trk])<4.0)";
  
  TH1F HdR("HdR","",100,0,100);
  TREE->Draw("cell_trkdR>>HdR",cellSelection);
  C1D.Clear();
  HdR.GetXaxis()->SetTitle("distance to closest cell [mm]");  
  HdR.GetYaxis()->SetTitle("# of tracks");  
  HdR.Draw("histpe");
  C1D.SetLogy(1);
  C1D.Print(outpath+"/BadResTracks_celldR.png");
  C1D.SetLogy(0);


  TH1F HNcellGoodTrack("HNcellGoodTrack","",100,0,100);
  TREE->Draw("ncell>>HNcellGoodTrack",cellSelection+"&&cell_trkdR<5");
  HNcellGoodTrack.Scale(1./HNcellGoodTrack.Integral());

  TH1F HNcellBadTrack("HNcellBadTrack","",100,0,100);
  TREE->Draw("ncell>>HNcellBadTrack",cellSelection+"&&cell_trkdR>5");
  HNcellBadTrack.Scale(1./HNcellBadTrack.Integral());

  

  C1D.Clear();
  HNcellGoodTrack.GetXaxis()->SetTitle("total # of HGTD hits");  
  HNcellGoodTrack.GetYaxis()->SetTitle("events");  
  HNcellGoodTrack.Draw("hist");
  HNcellBadTrack.Draw("histpsame");
  text.DrawLatexNDC(0.65,0.85,TString("<5mm Mean = ")+(long)HNcellGoodTrack.GetMean());
  text.DrawLatexNDC(0.65,0.80,TString(">5mm Mean = ")+(long)HNcellBadTrack.GetMean());
  C1D.SetLogy(1);
  C1D.Print(outpath+"/BadResTracks_ncell.png");
  C1D.SetLogy(0);
  

}




//////////////////////////////
void plotTrackMatchingEff(TChain*TREE,TString outpath){

  TString MatchSelection="(trk_ncell>0)";
  //TString MatchSelection="(trk_ncellL0>0)";
  //TString MatchSelection="(trk_hgtrk>=0)";


  ///////////////////////////////
  //  vs pT
  ///////////////////////////////
  TH1F HTrackPt("HTrackPt","",NTRKMATCHEFFPTBINS,ptbinstrkmatch);
  TH1F HTrackPtCut("HTrackPtCut","",NTRKMATCHEFFPTBINS,ptbinstrkmatch);
  HTrackPt.GetXaxis()->SetTitle("track p_{T}  [GeV]");
  TREE->Draw("trk_pt/1000>>HTrackPt",trackSelection+"*"+trackEtaSelection);//+"*"+trk3mmSel
  TREE->Draw("trk_pt/1000>>HTrackPtCut",trackSelection+"*"+trackEtaSelection+"*"+MatchSelection);//+"*"+trk3mmSel

  // TH1F HTrackPt("HTrackPt","",250,0,25);
  // TH1F HTrackPtCut("HTrackPtCut","",250,0,25);
  // HTrackPt.GetXaxis()->SetTitle(" track true p_{T}   [GeV]");
  // TREE->Draw("trk_truth_pt/1000>>HTrackPt",trackSelection);//+"*"+trk1mmSel
  // TREE->Draw("trk_truth_pt/1000>>HTrackPtCut",trackSelection+"*"+MatchSelection);//+"*"+trk1mmSel


  TH1F*HEff=makeHistRatio(&HTrackPt,&HTrackPtCut,0.05);
  TF1*FTrkFit=fitTrkMatchEffVsPt(HEff,1,25);
  if(!HEff) return;
  HEff->GetXaxis()->SetTitle(HTrackPt.GetXaxis()->GetTitle());
  HEff->GetYaxis()->SetTitle("Efficiency");
  HEff->GetYaxis()->SetRangeUser(0.6,1.2);
  C1D.Clear();
  gStyle->SetErrorX(0.00001);
  HEff->Draw("histpe");
  FTrkFit->Draw("lsame");
  HEff->Draw("histpesame");
  DrawSinglePi();
  //DrawVBFH(outpath);  DrawECMSMu(outpath);
  DrawATLAS(0.21,0.88);
  DrawITK(0.21,0.83);
  DrawHGTD(outpath,0,-1,0.21,0.78,"1 mm cells"); 
  //DrawHGTD(outpath,0,-1,0.3,0.83); 
  text.SetTextSize(0.04);
  text.DrawLatexNDC(0.7,0.85," 2.5 < |#eta | < 3.9");

  char fittxt[100];
  sprintf(fittxt,"#epsilon(p_{T}) = %.2f - %.2f e^{-%.2f*p_{T}}",FTrkFit->GetParameter(0),FTrkFit->GetParameter(1),FTrkFit->GetParameter(2));
  text.SetTextColor(4);
  text.SetTextSize(0.045);
  text.DrawLatexNDC(0.6,0.26,fittxt);

  C1D.Print(outpath+"/TrackMatchEff_pt.png");
  C1D.Print(outpath+"/TrackMatchEff_pt.eps");
  C1D.Print(outpath+"/TrackMatchEff_pt.pdf");



  ///////////////////////////////
  //  vs eta (Note: this plot must be made including only 1 pT sample eg. 5 GeV)
  ///////////////////////////////
  TString trackptTruthSel="(trk_truth_pt==5000)";
  int nbinseta=14;
  float mineta=2.5;
  float maxeta=3.9;
  
  TH1F HTrackEta("HTrackEta","",nbinseta,mineta,maxeta);
  HTrackEta.GetXaxis()->SetTitle("track |#eta| ");
  TREE->Draw("abs(trk_eta)>>HTrackEta",trackSelection+"*"+trackptTruthSel);
  TH1F HTrackEtaCut("HTrackEtaCut","",nbinseta,mineta,maxeta);
  TREE->Draw("abs(trk_eta)>>HTrackEtaCut",trackSelection+"*"+trackptTruthSel+"*"+MatchSelection);

  TH1F*HEffEta=makeHistRatio(&HTrackEta,&HTrackEtaCut);
  if(!HEffEta) return;
  HEffEta->GetXaxis()->SetTitle(HTrackEta.GetXaxis()->GetTitle());
  HEffEta->GetYaxis()->SetTitle("Efficiency");
  C1D.Clear();
  HEffEta->GetYaxis()->SetRangeUser(0.6,1.2);
  HEffEta->Draw("histpe");
  DrawSinglePi(5); 
  DrawATLAS(0.21,0.88);
  DrawITK(0.21,0.83);
  DrawHGTD(outpath,0,-1,0.21,0.78,"1 mm cells"); 
  //DrawHGTD(outpath,0,-1,0.3,0.83); 
  text.SetTextSize(0.04);
  //text.DrawLatexNDC(0.7,0.85," track p_{T} > 1 GeV");
  C1D.Print(outpath+"/TrackMatchEff_eta.png");
  C1D.Print(outpath+"/TrackMatchEff_eta.eps");
  C1D.Print(outpath+"/TrackMatchEff_eta.pdf");

}



void plotTrackTimeMatchingEff(TChain*TREE,TString outpath){

  float tlow=-500;
  float thigh=1500;
  int nbins=150;
  float fitmin=-200;
  float fitmax=100;
  
  TString yTitle="# of tracks";
  TString xTitle="track t - truth track t  [ps]";
  
  TString trkdtr="(trk_tr-trk_truetr)";
  //TString trkdtr="(hgtrk_tr[trk_hgtrk] - trk_truetr)";


  TH1F HBeamspot("HBeamspot","",200,-1000,3000);//beamspot determined from truth track 
  HBeamspot.GetYaxis()->SetTitle("# of tracks");
  HBeamspot.GetXaxis()->SetTitle("track truth vertex  t   [ps]");
  TREE->Draw("(trk_truth_vtxt)*1000>>HBeamspot",trackSelection);
  C1D.Clear();
  HBeamspot.Draw("hist");
  fitHistGaus(&HBeamspot,-500,300,0.7,0.85);
  C1D.Print(outpath+"/TrackTimeMatchEff_Beamspot.png");


  TH1F HTrackTFullRange("HTrackTFullRange","",200,-1000,3000);
  HTrackTFullRange.GetYaxis()->SetTitle(yTitle);
  HTrackTFullRange.GetXaxis()->SetTitle(xTitle);
  TREE->Draw(trkdtr+"*1000>>HTrackTFullRange",trackSelection);
  C1D.Clear();
  HTrackTFullRange.Draw("hist");
  C1D.SetLogy(0);
  C1D.Print(outpath+"/TrackTimeMatchEff_trueTdiff_full.png");
  C1D.SetLogy(1);
  C1D.Print(outpath+"/TrackTimeMatchEff_trueTdiff_full_log.png");
  C1D.SetLogy(0);


  TH1F HTrackT("HTrackT","",nbins,tlow,thigh);
  HTrackT.GetYaxis()->SetTitle(yTitle);
  HTrackT.GetXaxis()->SetTitle(xTitle);
  TREE->Draw(trkdtr+"*1000>>HTrackT",trackSelection);
  TF1*F=fitTrkTruthT(&HTrackT,fitmin,fitmax,0.7,0.85); 
  C1D.Clear();
  HTrackT.Draw("hist");
  F->Draw("same");
  C1D.SetLogy(0);
  C1D.Print(outpath+"/TrackTimeMatchEff_trueTdiff.png");
  C1D.SetLogy(1);
  C1D.Print(outpath+"/TrackTimeMatchEff_trueTdiff_log.png");
  C1D.SetLogy(0);


  ///////////////////////////////
  //// vs pt
  ///////////////////////////////
  TH1F HEffVsPt("HEffVsPt","",NTRKMATCHEFFPTBINS,ptbinstrkmatch);

  TCanvas C("C","",800,800);
  C.Divide(3,3);
  
  for(long p=0;p<NTRKMATCHEFFPTBINS;p++){
    char cuttxt[100];
    sprintf(cuttxt,"(%.2f<abs(trk_pt/1000)&&abs(trk_pt/1000)<%0.2f)",ptbinstrkmatch[p],ptbinstrkmatch[p+1]);

    TPad*pad=(TPad*)C.cd(p+1);
    pad->SetLogy(1);
    pad->Clear();

    TH1F* HTrackT=new TH1F(TString("HTrackT")+p,cuttxt,nbins,tlow,thigh);
    TREE->Draw(trkdtr+"*1000>>HTrackT"+p,trackSelection+"*"+trackEtaSelection+"&&"+cuttxt);
    TF1*F=fitTrkTruthT(HTrackT,fitmin,fitmax,0.7,0.85); 
    if(F){
      HTrackT->GetYaxis()->SetTitle(yTitle);
      HTrackT->GetXaxis()->SetTitle(xTitle);
      HTrackT->Draw("hist");
      F->Draw("same");
      
      float total=HTrackT->Integral(1,HTrackT->GetNbinsX());
      TH1F*HTrackTSub=(TH1F*)HTrackT->Clone("HTrackTSub");
      HTrackTSub->Add(F,-1);
      float bkgsub=HTrackTSub->Integral();
      HEffVsPt.SetBinContent(p+1,bkgsub/total);
      HEffVsPt.SetBinError(p+1,sqrt(total)/total);
    }
    
    
  }
  C.Print(outpath+"/TrackTimeMatchEff_FitsvsPt.png");


  TF1*FTrkFit=fitTrkMatchEffVsPt(&HEffVsPt,1,25);
  
  C1D.SetLogy(0);
  C1D.Clear();
  gStyle->SetErrorX(0.00001);
  HEffVsPt.GetXaxis()->SetTitle("track p_{T}   [GeV]");
  HEffVsPt.GetYaxis()->SetTitle("Efficiency");
  HEffVsPt.GetYaxis()->SetRangeUser(0.6,1.2);
  HEffVsPt.Draw("histpe");
  FTrkFit->Draw("lsame");
  HEffVsPt.Draw("histpesame");
  DrawVBFH(outpath);  DrawECMSMu(outpath);
  DrawATLAS(0.21,0.88);
  DrawITK(0.21,0.83);
  DrawHGTD(outpath,0,-1,0.21,0.78,"1 mm  cells"); 
  text.SetTextSize(0.04);
  text.DrawLatexNDC(0.7,0.85,"2.5 < |#eta| < 3.9");

  char fittxt[100];
  sprintf(fittxt,"#epsilon(p_{T}) = %.2f - %.2f e^{-%.2f*p_{T}}",FTrkFit->GetParameter(0),FTrkFit->GetParameter(1),FTrkFit->GetParameter(2));
  text.SetTextColor(4);
  text.SetTextSize(0.045);
  text.DrawLatexNDC(0.6,0.26,fittxt);

  C1D.Print(outpath+"/TrackTimeMatchEff_EffvsPt.png");
  C1D.Print(outpath+"/TrackTimeMatchEff_EffvsPt.eps");
  C1D.Print(outpath+"/TrackTimeMatchEff_EffvsPt.pdf");





  ///////////////////////////////
  //// vs eta
  ///////////////////////////////
  long ptcut=5;
  TString trkPtSelection=TString("(trk_pt/1000>")+ptcut+")";

  //  #define NETABINS 6
  //float etax[NETABINS+1]={2.5,2.75,3.0,3.25,3.5,3.75,4.0};

  //  #define NETABINS 8
  //float etax[NETABINS+1]={2.4,2.6,2.8,3.0,3.2,3.4,3.6,3.8,4.0};

  #define NETABINS 14
  float etax[NETABINS+1]={2.5,2.6,2.7,2.8,2.9,3.0,3.1,3.2,3.3,3.4,3.5,3.6,3.7,3.8,3.9};

  TH1F HEffVsEta("HEffVsEta","",NETABINS,etax);

  C.Clear();
  C.Divide(3,3);
  
  for(long p=0;p<NETABINS;p++){
    char cuttxt[100];
    sprintf(cuttxt,"(%.2f<abs(trk_eta)&&abs(trk_eta)<%0.2f)",etax[p],etax[p+1]);

    TPad*pad=(TPad*)C.cd(p+1);
    pad->SetLogy(1);
    pad->Clear();

    TH1F* HTrackT=new TH1F(TString("HTrackT")+p,cuttxt,nbins,tlow,thigh);
    TREE->Draw(trkdtr+"*1000>>HTrackT"+p,trackSelection+"&&"+trkPtSelection+"&&"+cuttxt);
    TF1*F=fitTrkTruthT(HTrackT,fitmin,fitmax,0.7,0.85); 
    if(F){
      HTrackT->GetYaxis()->SetTitle(yTitle);
      HTrackT->GetXaxis()->SetTitle(xTitle);
      HTrackT->Draw("hist");
      F->Draw("same");
      
      float total=HTrackT->Integral(1,HTrackT->GetNbinsX());
      TH1F*HTrackTSub=(TH1F*)HTrackT->Clone("HTrackTSub");
      HTrackTSub->Add(F,-1);
      float bkgsub=HTrackTSub->Integral();
      HEffVsEta.SetBinContent(p+1,bkgsub/total);
      HEffVsEta.SetBinError(p+1,sqrt(total)/total);
    }
    
    
  }
  C.Print(outpath+"/TrackTimeMatchEff_FitsvsEta.png");

  C1D.SetLogy(0);
  C1D.Clear();
  HEffVsEta.GetXaxis()->SetTitle("track |#eta| ");
  HEffVsEta.GetYaxis()->SetTitle("Efficiency");
  HEffVsEta.GetYaxis()->SetRangeUser(0.6,1.2);
  HEffVsEta.Draw("histpe");
  DrawVBFH(outpath);  
  DrawECMSMu(outpath);
  DrawATLAS(0.21,0.88);
  DrawITK(0.21,0.83);
  DrawHGTD(outpath,0,-1,0.21,0.78,"1 mm  cells"); 
  text.SetTextSize(0.04);
  text.DrawLatexNDC(0.7,0.85,TString(" track p_{T} > ")+ptcut+" GeV");
  C1D.Print(outpath+"/TrackTimeMatchEff_EffvsEta.png");
  C1D.Print(outpath+"/TrackTimeMatchEff_EffvsEta.eps");

}


void plotTrackdXPublicPlot(TChain*TREE,TString outpath){

  TString cellSel="(cell_trk>=0&&abs(cell_S)==1&&2.5<abs(trk_eta[cell_trk])&&abs(trk_eta[cell_trk])<3.9)";
  TString cell1mm="(cell_type==1)";
  TString cell3mm="(cell_type==2)";

  float tlow=-10;
  float thigh=10;
  int nbins=40;

  float fitmin=-10;
  float fitmax=10;

  TString yTitle="# of tracks / 0.5 mm";
  TString xTitle="cell x  -  track x    [mm]";
  TString trackdt="(cell_x-trk_ext_x[cell_trk])";
  
  TH1F HTrack1mm("HTrack1mm","",nbins,tlow,thigh);
  HTrack1mm.GetYaxis()->SetTitle(yTitle);
  HTrack1mm.GetXaxis()->SetTitle(xTitle);
  TREE->Draw(trackdt+">>HTrack1mm",cellSel+"&&"+cell1mm);
  C1D.Clear();
  TF1*F=fitHist2Gaus(&HTrack1mm,fitmin,fitmax);
  char txt[100];
  text.SetTextSize(0.05);
  sprintf(txt,"#sigma  = %.1f mm",F->GetParameter(2));
  text.SetTextColor(4);
  text.DrawLatexNDC(0.70,0.7,txt);
  text.SetTextSize(0.04);
  text.SetTextColor(1);
  text.DrawLatexNDC(0.70,0.85,"2.5<|#eta|<3.9");  
  //text.DrawLatexNDC(0.7,0.85,trackPlotLabel);
  DrawSinglePi(2);
  DrawATLAS(0.21,0.88);
  DrawITK(0.21,0.83);
  DrawHGTD(outpath,0,-1,0.21,0.78,"1 mm  cells"); 
  C1D.Print(outpath+"/TrackdXPublicPlot_1mm.png");
  C1D.Print(outpath+"/TrackdXPublicPlot_1mm.eps");
  C1D.Print(outpath+"/TrackdXPublicPlot_1mm.pdf");


  TH1F HTrack3mm("HTrack3mm","",nbins,tlow,thigh);
  HTrack3mm.GetYaxis()->SetTitle(yTitle);
  HTrack3mm.GetXaxis()->SetTitle(xTitle);
  TREE->Draw(trackdt+">>HTrack3mm",cellSel+"&&"+cell3mm);
  C1D.Clear();
  TF1*F3mm=fitHist2Gaus(&HTrack3mm,fitmin,fitmax);
  text.SetTextSize(0.05);
  sprintf(txt,"#sigma  = %.1f mm",F3mm->GetParameter(2));
  text.SetTextColor(4);
  text.DrawLatexNDC(0.70,0.7,txt);
  text.SetTextSize(0.04);
  text.SetTextColor(1);
  text.DrawLatexNDC(0.70,0.85,"2.5<|#eta|<3.9");
  //text.DrawLatexNDC(0.7,0.85,trackPlotLabel);
  DrawSinglePi(2);
  DrawATLAS(0.21,0.88);
  DrawITK(0.21,0.83);
  DrawHGTD(outpath,0,-1,0.21,0.78,"3 mm  cells"); 
  C1D.Print(outpath+"/TrackdXPublicPlot_3mm.png");
  C1D.Print(outpath+"/TrackdXPublicPlot_3mm.eps");
  C1D.Print(outpath+"/TrackdXPublicPlot_3mm.pdf");


}





void plotTrackdTPublicPlot(TChain*TREE,TString outpath){

  float tlow=-200;
  float thigh=200;
  int nbins=80;

  float fitmin=-200;
  float fitmax=200;
  
  TString yTitle="# of tracks / 5 ps";
  TString xTitle="track  t - truth t   [ps]";
  //TString trackdt="(trk_dt-trk_truedt)";
  TString trackdt="(trk_t-trk_truetr)";
  
  TH1F HTrackT1mm("HTrackT1mm","",nbins,tlow,thigh);
  HTrackT1mm.GetYaxis()->SetTitle(yTitle);
  HTrackT1mm.GetXaxis()->SetTitle(xTitle);
  TREE->Draw(trackdt+"*1000>>HTrackT1mm",trackSelection+"&&"+trackCellSel+"&&"+trk1mmSel);
  //TREE->Draw(trackdt+"*1000>>HTrackT1mm",trackSelection+"&&"+trackCellSel+"&&"+trackHiPtSel);
  C1D.Clear();
  TF1* F= fitHistTrackdT(&HTrackT1mm,fitmin,fitmax);
  char txt[100];
  text.SetTextSize(0.04);
  sprintf(txt,"#sigma_{t} = %.0f ps",F->GetParameter(2));
  text.SetTextColor(4);
  text.DrawLatexNDC(0.70,0.7,txt);
  text.SetTextColor(1);
  text.DrawLatexNDC(0.7,0.85,trackPlotLabel);
  DrawVBFH(outpath);  DrawECMSMu(outpath);
  //DrawSinglePi(5);
  DrawATLAS(0.21,0.88);
  DrawITK(0.21,0.83);
  DrawHGTD(outpath,0,-1,0.21,0.78,"1 mm  cells");
  text.SetTextSize(0.04);
  text.DrawLatexNDC(0.70,0.80,"2.5<|#eta|<3.9"); 
  C1D.Print(outpath+"/TrackdTPublicPlot_1mm.png");
  C1D.Print(outpath+"/TrackdTPublicPlot_1mm.eps");
  C1D.Print(outpath+"/TrackdTPublicPlot_1mm.pdf");


  TH1F HTrackT3mm("HTrackT3mm","",nbins,tlow,thigh);
  HTrackT3mm.GetYaxis()->SetTitle(yTitle);
  HTrackT3mm.GetXaxis()->SetTitle(xTitle);
  TREE->Draw(trackdt+"*1000>>HTrackT3mm",trackSelection+"&&"+trackCellSel+"&&"+trk3mmSel);
  //TREE->Draw(trackdt+"*1000>>HTrackT3mm",trackSelection+"&&"+trackCellSel+"&&"+trackHiPtSel);
  C1D.Clear();
  TF1* F3mm= fitHistTrackdT(&HTrackT3mm,fitmin,fitmax);
  text.SetTextSize(0.04);
  sprintf(txt,"#sigma_{t} = %.0f ps",F3mm->GetParameter(2));
  text.SetTextColor(4);
  text.DrawLatexNDC(0.70,0.7,txt);
  text.SetTextColor(1);
  text.DrawLatexNDC(0.7,0.85,trackPlotLabel);
  DrawVBFH(outpath);  DrawECMSMu(outpath);
  //DrawSinglePi(5);
  DrawATLAS(0.21,0.88);
  DrawITK(0.21,0.83);
  DrawHGTD(outpath,0,-1,0.21,0.78,"3 mm  cells");
  text.SetTextSize(0.04);
  text.DrawLatexNDC(0.70,0.80,"2.5<|#eta|<3.9"); 
  C1D.Print(outpath+"/TrackdTPublicPlot_3mm.png");
  C1D.Print(outpath+"/TrackdTPublicPlot_3mm.eps");
  C1D.Print(outpath+"/TrackdTPublicPlot_3mm.pdf");




  ///////////////////////////
  /// validation plots
  //////////////////////////
  TH1F HTrackTime1mm("HTrackTime1mm","",100,10,14);
  HTrackTime1mm.GetYaxis()->SetTitle("# of tracks");
  HTrackTime1mm.GetXaxis()->SetTitle("track time  [ns]");
  TREE->Draw("trk_t>>HTrackTime1mm",trackSelection+"&&"+trackCellSel+"&&"+trk1mmSel);

  TH1F HTrackTrueTime1mm("HTrackTrueTime1mm","",100,10,14);
  HTrackTrueTime1mm.GetYaxis()->SetTitle("# of tracks");
  HTrackTrueTime1mm.GetXaxis()->SetTitle("track true time  [ns]");
  TREE->Draw("trk_truetr>>HTrackTrueTime1mm",trackSelection+"&&"+trackCellSel+"&&"+trk1mmSel);

  TH2F HTrackTime2DCorr1mm("HTrackTime2DCorr1mm","",100,10,14,100,10,14);
  HTrackTime2DCorr1mm.GetYaxis()->SetTitle("track time [ns]");
  HTrackTime2DCorr1mm.GetXaxis()->SetTitle("track true time  [ns]");
  TREE->Draw("trk_t:trk_truetr>>HTrackTime2DCorr1mm",trackSelection+"&&"+trackCellSel+"&&"+trk1mmSel);

  C1D.Clear();
  HTrackTime1mm.Draw("histpe");
  C1D.Print(outpath+"/TrackTime_1mm.png");

  C1D.Clear();
  HTrackTrueTime1mm.Draw("hist");
  C1D.Print(outpath+"/TrackTrueTime_1mm.png");

  C2D.Clear();
  HTrackTime2DCorr1mm.Draw("histcolz");
  C2D.Print(outpath+"/TrackTime2DCorr_1mm.png");

}


void plotTrackTimePlots(TChain*TREE,TString outpath){


}



////////////////////////////////////////////////////////////////////////////////////
/////////////// Use matched HGTD track
/////////////////////////////////////////////////////////////////////////////////////

void plotTrackHGTrks(TChain*TREE,TString outpath){
  TString trkSelection="(trk_ext_eta!=0&&abs(trk_eta)>2.5&&abs(trk_eta)<4.0&&trk_hgtrk>=0)";
  TString hgtrk1mmSel="(hgtrk_type[trk_hgtrk]==1)";
  TString hgtrk3mmSel="(hgtrk_type[trk_hgtrk]==2)";

  TH1F HdR_1mm("HdR_1mm","",100,0,10);
  HdR_1mm.GetXaxis()->SetTitle("cell - track distance    [mm]");
  TREE->Draw("trk_hgtrkdR[trk_hgtrk]>>HdR_1mm",trkSelection+"&&"+trackHiPtSel+"&&"+hgtrk1mmSel);
  C1D.Clear();
  HdR_1mm.Draw("hist");
  C1D.Print(outpath+"/TrackHGTrk_dRHiPt_1mm.png");

  TH1F HdR_3mm("HdR_3mm","",100,0,10);
  HdR_3mm.GetXaxis()->SetTitle("cell - track distance    [mm]");
  TREE->Draw("trk_hgtrkdR[trk_hgtrk]>>HdR_3mm",trkSelection+"&&"+trackHiPtSel+"&&"+hgtrk3mmSel);
  C1D.Clear();
  HdR_3mm.Draw("hist");
  C1D.Print(outpath+"/TrackHGTrk_dRHiPt_3mm.png");

  TH2F Hdxdy("Hdxdy","",50,-10,10,50,-10,10);
  Hdxdy.GetXaxis()->SetTitle("cell x - track x");
  Hdxdy.GetYaxis()->SetTitle("cell y - track y");
  TREE->Draw("hgtrk_y[trk_hgtrk]-trk_ext_y:hgtrk_x[trk_hgtrk]-trk_ext_x>>Hdxdy",trkSelection+"&&"+trackHiPtSel);
  C2D.Clear();
  Hdxdy.Draw("colz");
  C2D.Print(outpath+"/TrackHGTrk_dxdy.png");

}


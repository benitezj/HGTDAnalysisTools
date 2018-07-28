void plotEventDisplayHitsdEtadPhi_HGTracks(TTree *Tuple, TString outpath,TString tag, TString Selection){
  
  //TString CellSelection="(cell_jet>=0&&jet_truthJ[cell_jet]=="+Selection+")";

  TCanvas C("CdEtadPhi","",800,800);
  C.SetRightMargin(0.15);
  C.SetTopMargin(0.15);

  //int nbins=100;//gives bin size of 0.01 about 3x3mm cell
  int nbins=150;//gives about ??mm cell
  //int nbins=200;//gives about 2x2mm cell
  //int nbins=300;//gives about 1x1mm cell
  float range=0.5;


  TH2F HCellsdEtadPhi("HCellsdEtadPhi_Jet","",nbins,-range,range,nbins,-range,range);
  Tuple->Draw(dEta+":"+dPhi+">>HCellsdEtadPhi_Jet",Selection); 

  C.Clear();  
  HCellsdEtadPhi.GetYaxis()->SetTitle("|#eta_{cell}| - |#eta_{jet}|");
  HCellsdEtadPhi.GetXaxis()->SetTitle("#phi_{cell} - #phi_{jet}");
  HCellsdEtadPhi.GetZaxis()->SetTitle("# of hits");
  HCellsdEtadPhi.Draw("colz");
  drawJetCircles();
  C.Print(outpath+"/EventDisplaydR_CellsdEtadPhi_"+tag+".png");

    
  ///////////////////////////////////
  ///clean out bins with less than 4 hits
  for(int bx=1;bx<=HCellsdEtadPhi.GetNbinsX();bx++){
    for(int by=1;by<=HCellsdEtadPhi.GetNbinsY();by++){
      if(HCellsdEtadPhi.GetBinContent(bx,by)<3)
	HCellsdEtadPhi.SetBinContent(bx,by,0);
      else HCellsdEtadPhi.SetBinContent(bx,by,1);
    }
  }
  
  C.Clear();  
  HCellsdEtadPhi.SetMarkerSize(0.3);
  HCellsdEtadPhi.SetMarkerColor(2);
  HCellsdEtadPhi.Draw("colz");
  drawJetCircles();
  C.Print(outpath+"/EventDisplaydR_CellsdEtadPhiTracks_"+tag+".png");




  // ////////////////////////////////
  // /// Show 3D tracks
  // /////////////////////////////////
  // TH3F HCellsdEtadPhiZ("HCellsdEtadPhiZ_Jet","",nbins,-range,range,nbins,-range,range,32,0.5,4.5);
  // Tuple->Draw(TString("cell_S:")+dEta+":"+dPhi+">>HCellsdEtadPhiZ_Jet",Selection); 
  // HCellsdEtadPhiZ.SetMarkerSize(0.05);

  // C.Clear();  
  // HCellsdEtadPhiZ.GetXaxis()->SetTitle("#phi_{cell} - #phi_{jet}");
  // HCellsdEtadPhiZ.GetYaxis()->SetTitle("|#eta_{cell}| - |#eta_{jet}|");
  // HCellsdEtadPhiZ.GetZaxis()->SetTitle("layer");
  // HCellsdEtadPhiZ.Draw("box");
  // C.Print(outpath+"/EventDisplaydR_CellsdEtadPhiZ_"+tag+".png");

  // gROOT->cd();
  // TH3F*H3=(TH3F*)HCellsdEtadPhiZ.Clone("Tracks3D");

}

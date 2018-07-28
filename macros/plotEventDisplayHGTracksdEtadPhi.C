
void plotEventDisplayHGTracksdEtadPhi(TTree *Tuple, TString outpath,TString tag, TString TrkSelection, TString ClusSelection=""){
 
  TCanvas C("CdEtadPhi","",800,800);
  C.SetRightMargin(0.15);
  C.SetTopMargin(0.15);


  //int nbins=100;//gives bin size of 0.01 about 3x3mm cell
  int nbins=150;//gives about ??mm cell
  //int nbins=200;//gives about 2x2mm cell
  //int nbins=300;//gives about 1x1mm cell
  float range=0.5;

  ///get the tracks
  TH2F HTrksdEtadPhi("HTrksdEtadPhi_Jet","",nbins,-range,range,nbins,-range,range);
  Tuple->Draw(dEtaHGTrk+":"+dPhiHGTrk+">>HTrksdEtadPhi_Jet",TrkSelection); 
  //Tuple->Draw("abs(hgtrk_eta)-jet_eta[hgtrk_jet]:hgtrk_phi-jet_phi[hgtrk_jet]>>HTrksdEtadPhi_Jet",TrkSelection); 


  ///get the clusters
  TH2F HClusdEtadPhi("HClusdEtadPhi_Jet","",10*nbins,-range,range,10*nbins,-range,range);
  if(ClusSelection.CompareTo("")!=0){
    Tuple->Draw(dEtaClus+":"+dPhiClus+">>HClusdEtadPhi_Jet",ClusSelection+"*(clus_E/1000)");//energy will be displayed as text 
    HClusdEtadPhi.SetMarkerColor(4);
    HClusdEtadPhi.SetMarkerStyle(24);
    HClusdEtadPhi.SetMarkerSize(4);
  }



  C.Clear();  
  HTrksdEtadPhi.GetYaxis()->SetTitle("|#eta_{trk}| - |#eta_{jet}|");
  HTrksdEtadPhi.GetXaxis()->SetTitle("#phi_{trk} - #phi_{jet}");
  HTrksdEtadPhi.GetZaxis()->SetTitle("# of hits");
  HTrksdEtadPhi.Draw("col");

  ////Overlay the calo clusters
  if(ClusSelection.CompareTo("")!=0){
    TH2F* HClusdEtadPhiClone=(TH2F*)HClusdEtadPhi.Clone("");
    HClusdEtadPhiClone->SetMarkerColor(1);
    HClusdEtadPhiClone->SetMarkerSize(1.0);
    gStyle->SetPaintTextFormat("3.0f");
    HClusdEtadPhi.Draw("same");
    HClusdEtadPhiClone->Draw("textsame");
    //HTrksdEtadPhi.Draw("colsame");
  }

  drawJetCircles();
  C.Print(outpath+"/EventDisplay_HGTrksdEtadPhi_"+tag+".png");


}








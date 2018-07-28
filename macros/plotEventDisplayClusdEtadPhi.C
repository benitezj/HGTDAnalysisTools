
void plotEventDisplayClusdEtadPhi(TTree *Tuple, TString outpath,TString tag, TString ClusSelection){
  //TCanvas C("CdEtadPhi","",800,800);
  //C.SetRightMargin(0.15);
  //C.SetTopMargin(0.15);

  //int nbins=100;//gives bin size of 0.01 about 3x3mm cell
  int nbins=150;//gives about ??mm cell
  //int nbins=200;//gives about 2x2mm cell
  //int nbins=300;//gives about 1x1mm cell
  float range=0.5;

  
  ///get the clusters
  TH2F HClusdEtadPhi("HClusdEtadPhi","",10*nbins,-range,range,10*nbins,-range,range);
  Tuple->Draw(dEtaClus+":"+dPhiClus+">>HClusdEtadPhi",ClusSelection+"*(clus_E/1000)");//energy will be displayed as text 
  HClusdEtadPhi.SetMarkerColor(4);
  HClusdEtadPhi.SetMarkerStyle(24);
  HClusdEtadPhi.SetMarkerSize(4);

  TH2F* HClusdEtadPhiClone=(TH2F*)HClusdEtadPhi.Clone("HClusdEtadPhiClone");
  HClusdEtadPhiClone->SetMarkerColor(1);
  HClusdEtadPhiClone->SetMarkerSize(1.0);
  gStyle->SetPaintTextFormat("3.0f");


  C2D.Clear();  
  HClusdEtadPhi.GetYaxis()->SetTitle("|#eta_{clus}| - |#eta_{jet}|");
  HClusdEtadPhi.GetXaxis()->SetTitle("#phi_{clus} - #phi_{jet}");
  HClusdEtadPhi.Draw("same");
  HClusdEtadPhiClone->Draw("textsame");
  drawJetCircles();
  C2D.Print(outpath+"/EventDisplay_ClusdEtadPhi_"+tag+".png");


  //////////////////////////////////////
  ///show full event 
  ///////////////////////////////////
  TCanvas C("C","",800,400);
  C.SetRightMargin(0.15);
  C.SetTopMargin(0.05);

  TH2F HClusdEtadPhiAll("HClusdEtadPhiAll","",60,-TMath::Pi(),TMath::Pi(),60,2,5);
  Tuple->Draw("clus_eta:clus_phi>>HClusdEtadPhiAll","(clus_E/1000)");
  HClusdEtadPhiAll.SetMarkerColor(4);
  HClusdEtadPhiAll.SetMarkerStyle(24);
  HClusdEtadPhiAll.SetMarkerSize(1);


  ////// show the jets as circles of dR=0.4
  //TH2F HRecoJets("HRecoJets","",1000,-TMath::Pi(),TMath::Pi(),1000,side==1?2:-5,side==1?5:-2); 
  TH2F HRecoJets("HRecoJets","",1000,-TMath::Pi(),TMath::Pi(),1000,2,5); 
  Tuple->Draw("jet_eta:jet_phi>>HRecoJets"); 
  HRecoJets.SetMarkerColor(1);
  HRecoJets.SetMarkerStyle(24);
  //HRecoJets.SetMarkerSize(5);///0.2 cone
  HRecoJets.SetMarkerSize(10);//0.4 cone
  //HRecoJets.SetLineWidth(8);


  C.Clear();  
  HClusdEtadPhiAll.GetYaxis()->SetTitle("#eta_{clus}");
  HClusdEtadPhiAll.GetXaxis()->SetTitle("#phi_{clus}");
  HClusdEtadPhiAll.GetZaxis()->SetTitle("cluster energy [GeV]");
  HClusdEtadPhiAll.Draw("colz");
  HRecoJets.Draw("same");
  C.Print(outpath+"/EventDisplay_ClusEtaPhi_"+tag+".png");

}








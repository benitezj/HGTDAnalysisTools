
void plotHGTracks(TChain*TREE,TString outpath){
  TCanvas C;

  //Plot the number of tracks in the event
  TH1F HTrkN("HTrkN","",100,0,1000);
  HTrkN.GetXaxis()->SetTitle("Number of tracks");
  TREE->Draw("nhgtrk>>HTrkN");
  C.Clear();
  HTrkN.Draw("hist");
  C.Print(outpath+"/HGTrack_n.png");

  //Plot the number of hits on the track
  TH1F HTrkNhit("HTrkNhit","",20,0,20);
  HTrkNhit.GetXaxis()->SetTitle("Number of hits per track");
  TREE->Draw("hgtrk_nhit>>HTrkNhit");
  C.Clear();
  HTrkNhit.Draw("hist");
  C.Print(outpath+"/HGTrack_nhit.png");

  //Plot the track energy
  TH1F HTrkE("HTrkE","",30,0,3);
  HTrkE.GetXaxis()->SetTitle("Track cell energy   [MeV]");
  TREE->Draw("hgtrk_E>>HTrkE");
  C.Clear();
  HTrkE.Draw("hist");
  C.Print(outpath+"/HGTrack_E.png");

  //Plot the eta distribution           
  TH1F HTrkEta("HTrkEta","",100,-5,5);
  HTrkEta.GetXaxis()->SetTitle("track eta");
  TREE->Draw("hgtrk_eta>>HTrkEta");
  C.Clear();
  HTrkEta.Draw("hist");
  C.Print(outpath+"/HGTrack_eta.png");

  //Plot the phi distribution           
  TH1F HTrkPhi("HTrkPhi","",100,3.2,3.2);
  HTrkPhi.GetXaxis()->SetTitle("track phi");
  TREE->Draw("hgtrk_phi>>HTrkPhi");
  C.Clear();
  HTrkPhi.Draw("hist");
  C.Print(outpath+"/HGTrack_phi.png");

  //Plot the eta distribution           
  TH1F HTrkR("HTrkR","",100,0,600);
  HTrkR.GetXaxis()->SetTitle("track r [mm]");
  TREE->Draw("hgtrk_r>>HTrkR");
  C.Clear();
  HTrkR.Draw("hist");
  C.Print(outpath+"/HGTrack_r.png");

  //Plot x-y distribution  
  TCanvas C2("C2","",700,700);
  C2.SetRightMargin(0.15);
  C2.SetTopMargin(0.15);
  TH2F HTrkXY("HTrkXY","",120,-600,600,120,-600,600);
  HTrkXY.GetXaxis()->SetTitle("X  [mm]");
  HTrkXY.GetYaxis()->SetTitle("Y  [mm]");
  TREE->Draw("hgtrk_y:hgtrk_x>>HTrkXY");
  C2.Clear();
  HTrkXY.Draw("histcolz");
  C2.Print(outpath+"/HGTrack_XY.png");

  TH2F HTrkEtaPhi("HTrkEtaPhi","",100,-3.2,3.2,100,-6,6);
  HTrkEtaPhi.GetXaxis()->SetTitle("#phi");
  HTrkEtaPhi.GetYaxis()->SetTitle("#eta");
  TREE->Draw("hgtrk_deta:hgtrk_dphi>>HTrkEtaPhi");
  C.Clear();
  HTrkEtaPhi.Draw("histcolz");
  C.Print(outpath+"/HGTrack_EtaPhi.png");


  ////dEta : dPhi w.r.t. Jet
  TH2F HTrkdEtadPhiJet("HTrkdEtadPhiJet","",100,-3,3,100,-2,2);
  HTrkdEtadPhiJet.GetXaxis()->SetTitle("#Delta#phi");
  HTrkdEtadPhiJet.GetYaxis()->SetTitle("#Delta#eta");
  TREE->Draw(dEtadHGTrk+":"+dPhidHGTrk+">>HTrkdEtadPhiJet","hgtrk_jet>=0&&(jet_truthJ[hgtrk_jet]==truth_VBFJ1||jet_truthJ[hgtrk_jet]==truth_VBFJ2)");
  C2.Clear();
  HTrkdEtadPhiJet.Draw("histcolz");
  C2.Print(outpath+"/HGTrack_dEtadPhiJet_Sig.png");

  TH2F HTrkdEtadPhiJetPU("HTrkdEtadPhiJetPU","",100,-3,3,100,-2,2);
  HTrkdEtadPhiJetPU.GetXaxis()->SetTitle("#Delta#phi");
  HTrkdEtadPhiJetPU.GetYaxis()->SetTitle("#Delta#eta");
  TREE->Draw(dEtadHGTrk+":"+dPhidHGTrk+">>HTrkdEtadPhiJetPU","hgtrk_jet>=0&&jet_truthJ[hgtrk_jet]<0");
  C2.Clear();
  HTrkdEtadPhiJetPU.Draw("histcolz");
  C2.Print(outpath+"/HGTrack_dEtadPhiJet_PU.png");


  TH1F HTrkdEtaJet("HTrkdEtaJet","",100,-2,2);
  HTrkdEtaJet.GetXaxis()->SetTitle("#Delta#eta");
  TREE->Draw(dEtadHGTrk+">>HTrkdEtaJet",CUTSignalJetHGTrk);
  C2.Clear();
  HTrkdEtaJet.Draw("hist");
  C2.Print(outpath+"/HGTrack_dEtaJet_Sig.png");

  TH1F HTrkdPhiJet("HTrkdPhiJet","",100,-3,3);
  HTrkdPhiJet.GetXaxis()->SetTitle("#Delta#phi");
  TREE->Draw(dPhidHGTrk+">>HTrkdPhiJet",CUTSignalJetHGTrk);
  C2.Clear();
  HTrkdPhiJet.Draw("hist");
  C2.Print(outpath+"/HGTrack_dPhiJet_Sig.png");


}


void plotHGTrackCells(TChain*TREE,TString outpath){

  TH2F HTrkNvsNCell("HTrkNvsNCell","",200,0,200,50,-0.5,49.5);
  HTrkNvsNCell.GetXaxis()->SetTitle("# of cells");
  HTrkNvsNCell.GetYaxis()->SetTitle("# of tracks");
  TREE->Draw("nhgtrk:ncell>>HTrkNvsNCell","ncell>0");
  C2D.Clear();
  HTrkNvsNCell.Draw("colz");
  TF1 F("F","x/4.",HTrkNvsNCell.GetXaxis()->GetXmin(),HTrkNvsNCell.GetXaxis()->GetXmax());
  F.Draw("lsame");
  C2D.SetLogz(1);
  C2D.Print(outpath+"/HGTrack_NTrkvsNcell.png");
  C2D.SetLogz(0);
}

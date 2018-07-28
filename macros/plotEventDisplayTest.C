

void plotEventDisplayTest(TChain*TREE, TString outpath,int eventid=1){

  //Select an event with only one true jet in the positive forward region

  TString PtSelection="(1)";
  TString EtaSelection="(1)";
  TString Isolation="(1)";

  //TString PtSelection="(30000<truth_pt[truth_VBFJ1]&&truth_pt[truth_VBFJ1]<70000&&njet>=1)";
  //TString EtaSelection="(2.8<truth_eta[truth_VBFJ1]&&truth_eta[truth_VBFJ1]<3.0)";//low eta
  //TString EtaSelection="(3.4<truth_eta[truth_VBFJ1]&&truth_eta[truth_VBFJ1]<3.8)";//high eta
  //TString EtaSelection="(2.8<truth_eta[truth_VBFJ1]&&truth_eta[truth_VBFJ1]<3.8)";//inclusive
  //TString Isolation="((truth_Gluon==-1||abs(truth_eta[truth_Gluon])<2.0||abs(truth_eta[truth_Gluon])>5.0)&&truth_eta[truth_VBFJ2]<0)";


  TString EventSelection=PtSelection+"&&"+EtaSelection+"&&"+Isolation;
  cout<<"Base selection: "<<EventSelection<<endl;


  ////////////////////////////////////////////
  // find a good event
  ///////////////////////////////////////////
  TH2F HEventId("HEventId","",20000,0.5,20000.5,1,2,5);//do not change this binning 
  TREE->Draw("truth_eta[truth_VBFJ1]:event>>HEventId",EventSelection+"&&truth_VBFJ1>=0","col");
  for(int b=1;b<=HEventId.GetXaxis()->GetNbins();b++)
    if(HEventId.GetBinContent(b,1)>0) 
      cout<<"Event "<<b<<endl; 


  //////////////////////
  /// select the event for faster plotting
  //////////////////////
  TTree* Tuple = TREE->CopyTree(TString("event==")+(long)eventid);


  /////Select the jet
  TH3F HRecoJetPtEta("HRecoJetPtEta","",200,0,200,100,-5.0,5.0,124,-3.14159,3.14159);
  //Tuple->Draw("jet_phi:jet_eta:jet_pt/1000>>HRecoJetPtEta","jet_truthJ==truth_VBFJ1");//signal jet
  Tuple->Draw("jet_phi:jet_eta:jet_pt/1000>>HRecoJetPtEta","jet_eta>0");//all jets
  long jetcntr=1;
  for(int bx=1;bx<HRecoJetPtEta.GetNbinsX();bx++)
    for(int by=1;by<HRecoJetPtEta.GetNbinsY();by++)
      for(int bz=1;bz<HRecoJetPtEta.GetNbinsZ();bz++)
	if(HRecoJetPtEta.GetBinContent(bx,by,bz)>0){
	  jetpt[jetcntr]=HRecoJetPtEta.GetXaxis()->GetBinCenter(bx); 
	  jeteta[jetcntr]=HRecoJetPtEta.GetYaxis()->GetBinCenter(by);
	  jetphi[jetcntr]=HRecoJetPtEta.GetZaxis()->GetBinCenter(bz);
	  cout<<jetcntr<<"  pT="<<jetpt[jetcntr]<<",  eta="<<jeteta[jetcntr]<<", phi="<<jetphi[jetcntr]<<endl;
	  jetcntr++;
	}
  if( jetpt.size()==0) return;
  






  ///////////////////////////////////////////////
  //////// plot Cells 2D and dT
  ///////////////////////////////////////////////
  TString JetSelection="(cell_jet>=0)"; 
  TString JetTruthSel="(jet_truthJ[cell_jet]==truth_VBFJ1)";

  /////Basic hit XY displays
  plotEventDisplayHitsXY(Tuple,outpath,"all","1",1); //all cells
  //plotEventDisplayHitsXY(Tuple,outpath,"alljet",JetSelection,1);//all jets

  /////Basic hit eta-phi displays
  //plotEventDisplayHitsEtaPhi(Tuple,outpath,"all","(1)",1); //all cells
  //plotEventDisplayHitsEtaPhi(Tuple,outpath,"allEw","(cell_E)",1); //all cells

  ///dT distributions for all jets
  plotEventDisplayHitsDTAllJet(Tuple,outpath,"alljet",JetSelection+"&&(cell_jetdR<0.2)"); //all jets

  /////dR fits in time slices
  //plotEventDisplayHitsDT_dRScan(Tuple,outpath,"jet",JetSelection+"&&"+JetTruthSel);
  //plotEventDisplayHitsDR_dRScan(Tuple,outpath,"jet",JetSelection+"&&"+JetTruthSel);
  //plotEventDisplayHitsdEtadPhi_dRScan(Tuple,outpath,"jet",JetSelection+"&&"+JetTruthSel);

  ///clusters (dEta:dPhi)
  //plotEventDisplayClusdEtadPhi(Tuple,outpath,"jet","(clus_jet>=0&&jet_truthJ[clus_jet]==truth_VBFJ1)");

  //////HGTD tracks
  //plotEventDisplayHGTrksXY(Tuple,outpath,"all","1"); //all cells
  //plotEventDisplayHitsdEtadPhi_HGTracks(Tuple,outpath,"jet","(cell_jet>=0&&jet_truthJ[cell_jet]==truth_VBFJ1)");
  //plotEventDisplayHGTracksdEtadPhi(Tuple,outpath,"jet","(hgtrk_jet>=0&&jet_truthJ[hgtrk_jet]==truth_VBFJ1)");
  
  ///fit to the Jet shape
  //plotEventDisplayJetShape(Tuple,outpath,"signal",JetSelection+"&&"+JetTruthSel);


  ////ITK Tracks
  //plotEventDisplayTrksXY(Tuple,outpath,"all","1"); //all cells
  //plotEventDisplayHitsdXdY_Trks(Tuple,outpath,"Trk","(abs(cell_S)==1)");

}












void plotEventDisplayTrks(TChain*TREE, TString outpath,int eventid=1){


  TString PtSelection="(trk_pt[0]>5000)";
  TString EtaSelection="(2.6<trk_eta[0]&&trk_eta[0]<2.8&&abs(trk_phi[0]-1.0)<0.1)";
  TString Isolation="(1)";


  TString EventSelection=PtSelection+"&&"+EtaSelection+"&&"+Isolation;
  cout<<"Base selection: "<<EventSelection<<endl;


  ////////////////////////////////////////////
  // find a good event
  ///////////////////////////////////////////
  TH2F HEventId("HEventId","",20000,0.5,20000.5,1,2,5);//do not change this binning 
  TREE->Draw("trk_eta[0]:event>>HEventId",TString("(ntrk>0)")+"&&"+EventSelection,"col");
  for(int b=1;b<=HEventId.GetXaxis()->GetNbins();b++)
    if(HEventId.GetBinContent(b,1)>0) 
      cout<<"Event "<<b<<endl; 


  //////////////////////
  /// select the event for faster plotting
  //////////////////////
  TTree* Tuple = TREE->CopyTree(TString("event==")+(long)eventid);


  // /////Select the jet
  // TH3F HRecoJetPtEta("HRecoJetPtEta","",200,0,200,100,-5.0,5.0,124,-3.14159,3.14159);
  // //Tuple->Draw("jet_phi:jet_eta:jet_pt/1000>>HRecoJetPtEta","jet_truthJ==truth_VBFJ1");//signal jet
  // Tuple->Draw("jet_phi:jet_eta:jet_pt/1000>>HRecoJetPtEta","jet_eta>0");//all jets
  // long jetcntr=1;
  // for(int bx=1;bx<HRecoJetPtEta.GetNbinsX();bx++)
  //   for(int by=1;by<HRecoJetPtEta.GetNbinsY();by++)
  //     for(int bz=1;bz<HRecoJetPtEta.GetNbinsZ();bz++)
  // 	if(HRecoJetPtEta.GetBinContent(bx,by,bz)>0){
  // 	  jetpt[jetcntr]=HRecoJetPtEta.GetXaxis()->GetBinCenter(bx); 
  // 	  jeteta[jetcntr]=HRecoJetPtEta.GetYaxis()->GetBinCenter(by);
  // 	  jetphi[jetcntr]=HRecoJetPtEta.GetZaxis()->GetBinCenter(bz);
  // 	  cout<<jetcntr<<"  pT="<<jetpt[jetcntr]<<",  eta="<<jeteta[jetcntr]<<", phi="<<jetphi[jetcntr]<<endl;
  // 	  jetcntr++;
  // 	}
  // if( jetpt.size()==0) return;
  






  ///////////////////////////////////////////////
  //////// plot Cells 2D and dT
  ///////////////////////////////////////////////
  // TString JetSelection="(cell_jet>=0)"; 
  // TString JetTruthSel="(jet_truthJ[cell_jet]==truth_VBFJ1)";

  /////Basic hit XY displays
  plotEventDisplayHitsXY(Tuple,outpath,"all","1",1); //all cells
  //plotEventDisplayHitsXY(Tuple,outpath,"alljet",JetSelection,1);//all jets

  //////HGTD tracks
  plotEventDisplayHGTrksXY(Tuple,outpath,"all","1"); //all cells
  //plotEventDisplayHitsdEtadPhi_HGTracks(Tuple,outpath,"jet","(cell_jet>=0&&jet_truthJ[cell_jet]==truth_VBFJ1)");
   
  ////ITK Tracks
  plotEventDisplayTrksXY(Tuple,outpath,"all","1"); //all cells
  //plotEventDisplayHitsdXdY_Trks(Tuple,outpath,"Trk","(abs(cell_S)==1)");

}










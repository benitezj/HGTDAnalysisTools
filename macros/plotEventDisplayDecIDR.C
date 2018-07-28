void plotEventDisplayDecIDR(TChain*TREE, TString outpath,int eventid=1){

  // TString BasePtSelection="(30000<truth_pt[truth_VBFJ2]&&truth_pt[truth_VBFJ2]<80000&&njet>=1)";
  // TString BaseEtaSelection="(-2.8>truth_eta[truth_VBFJ2]&&truth_eta[truth_VBFJ2]>-3.0)";
  // TString BaseIsolation="((truth_Gluon==-1||abs(truth_eta[truth_Gluon])<2.0||abs(truth_eta[truth_Gluon])>5.0))";
  // TString BaseSelection=BasePtSelection+"&&"+BaseEtaSelection+"&&"+BaseIsolation;


  TString BasePtSelection="(50000<truth_pt[truth_VBFJ1]&&truth_pt[truth_VBFJ1]<70000)";
  TString BaseEtaSelection="(2.8<truth_eta[truth_VBFJ1]&&truth_eta[truth_VBFJ1]<3.0)";
  TString BaseIsolation="((truth_Gluon==-1||abs(truth_eta[truth_Gluon])<2.0||abs(truth_eta[truth_Gluon])>5.0))";
  TString BaseSelection=BasePtSelection+"&&"+BaseEtaSelection+"&&"+BaseIsolation;

  //TString BaseSelection="1";
  cout<<"Base selection: "<<BaseSelection<<endl;


  /////////////////////
  //find a good event
  /////////////////////
  TH2F HEventId("HEventId","",20000,0.5,20000.5,1,2,5);//do not change this binning 
  TREE->Draw("abs(truth_eta[truth_VBFJ1]):event>>HEventId",BaseSelection,"col");
  for(int b=1;b<=HEventId.GetXaxis()->GetNbins();b++)
    if(HEventId.GetBinContent(b,1)>0) 
      cout<<"Event "<<b<<endl; 


  //////////////////////
  ///select the event for faster plotting
  //////////////////////
  TTree* Tuple = TREE->CopyTree(TString("event==")+(long)eventid);

  TH2F HRecoJetPtEta("HRecoJetPtEta","",100,-5.0,5.0,200,0,200);
  Tuple->Draw("jet_pt/1000:jet_eta>>HRecoJetPtEta","jet_truthJ==truth_VBFJ1");
  long jetcntr=1;
  for(int bx=1;bx<HRecoJetPtEta.GetNbinsX();bx++)
    for(int by=1;by<HRecoJetPtEta.GetNbinsY();by++)
      if(HRecoJetPtEta.GetBinContent(bx,by)>0){
 	cout<<"Jet: pT="<<by<<" GeV  ,  eta="<<(HRecoJetPtEta.GetXaxis()->GetXmin()+bx*0.1)<<endl;
 	jetpt[jetcntr]=by; 
 	jeteta[jetcntr]=-5.0+bx*0.1;
 	jetcntr++;
     }
  if( jetpt.size()==0) return;
  


  /////////////////////////////
  ////////plot Cells 2D and dT
  /////////////////////////////
  // plotEventDisplayHitsXY(Tuple,outpath,"all","1",1); //all cells
  // plotEventDisplayHitsXY(Tuple,outpath,"all","1",-1); //all cells
  
  // plotEventDisplayHitsEtaPhi(Tuple,outpath,"allEtaPhi","1",1); //all cells
  // plotEventDisplayHitsEtaPhi(Tuple,outpath,"allEtaPhi","1",-1); //all cells
  
  
  TString JetSelection="(cell_jet>=0)"; 
  plotEventDisplayHitsXY(Tuple,outpath,"jet",JetSelection,1);//all jets
  plotEventDisplayHitsXY(Tuple,outpath,"jet",JetSelection,-1);//all jets

  TString JetTruthSel="(jet_truthJ[cell_jet]==truth_VBFJ1)";
  plotEventDisplayHitsXY(Tuple,outpath,"HScore40",JetSelection+"&&"+JetTruthSel+"&&(cell_jetdR<0.4)",1);
  plotEventDisplayHitsDT(Tuple,outpath,"HScore40",JetSelection+"&&"+JetTruthSel+"&&(cell_jetdR<0.4)");
  plotEventDisplayHitsDT(Tuple,outpath,"HScore20",JetSelection+"&&"+JetTruthSel+"&&(cell_jetdR<0.2)");

}




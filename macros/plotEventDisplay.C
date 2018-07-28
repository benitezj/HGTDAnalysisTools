void plotEventDisplay2(TChain*TREE, TString outpath,int eventid=1){

  //Select an event with only one true jet in the positive forward region
  //TString BasePtSelection="(30000<truth_pt[truth_VBFJ1]&&truth_pt[truth_VBFJ1]<80000&&njet>=1)";
  //TString BaseEtaSelection="(2.8<truth_eta[truth_VBFJ1]&&truth_eta[truth_VBFJ1]<3.0)";//low eta
  //TString BaseEtaSelection="(3.4<truth_eta[truth_VBFJ1]&&truth_eta[truth_VBFJ1]<3.8)";//high eta
  //TString BaseIsolation="((truth_Gluon==-1||abs(truth_eta[truth_Gluon])<2.0||abs(truth_eta[truth_Gluon])>5.0)&&truth_eta[truth_VBFJ2]<0)";

  TString BasePtSelection="(30000<truth_pt[truth_VBFJ2]&&truth_pt[truth_VBFJ2]<80000&&njet>=1)";
  TString BaseEtaSelection="(-2.8>truth_eta[truth_VBFJ2]&&truth_eta[truth_VBFJ2]>-3.0)";
  TString BaseIsolation="((truth_Gluon==-1||abs(truth_eta[truth_Gluon])<2.0||abs(truth_eta[truth_Gluon])>5.0))";

  TString BaseSelection=BasePtSelection+"&&"+BaseEtaSelection+"&&"+BaseIsolation;
  cout<<"Base selection: "<<BaseSelection<<endl;


  ////////////////////////////////////////////
  //find a good event
  ///////////////////////////////////////////
  TH2F HEventId("HEventId","",20000,0.5,20000.5,1,2,5);//do not change this binning 
  TREE->Draw("abs(truth_eta[truth_VBFJ2]):event>>HEventId",BaseSelection,"col");
  for(int b=1;b<=HEventId.GetXaxis()->GetNbins();b++)
    if(HEventId.GetBinContent(b,1)>0) 
      cout<<"Event "<<b<<endl; 


  //////////////////////
  ///select the event for faster plotting
  //////////////////////
  TTree* Tuple = TREE->CopyTree(TString("event==")+(long)eventid);

  TH2F HRecoJetPtEta("HRecoJetPtEta","",100,-5.0,5.0,200,0,200);
  Tuple->Draw("jet_pt/1000:jet_eta>>HRecoJetPtEta","jet_truthJ==truth_VBFJ2");
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
  


  ///////////////////////////////////////////////
  ////////plot Cells 2D and dT
  ///////////////////////////////////////////////
  plotEventDisplayHitsXY(Tuple,outpath,"all","1",1); //all cells
  plotEventDisplayHitsXY(Tuple,outpath,"all","1",-1); //all cells
  
  plotEventDisplayHitsEtaPhi(Tuple,outpath,"allEtaPhi","1",1); //all cells
  plotEventDisplayHitsEtaPhi(Tuple,outpath,"allEtaPhi","1",-1); //all cells
  
  ///return;
  
  TString JetSelection="(cell_jet>=0)"; 
  plotEventDisplayHitsXY(Tuple,outpath,"jet",JetSelection,1);//all jets
  plotEventDisplayHitsXY(Tuple,outpath,"jet",JetSelection,-1);//all jets

  TString JetTruthSel="(jet_truthJ[cell_jet]==truth_VBFJ1)";
  plotEventDisplayHitsXY(Tuple,outpath,"signal",JetSelection+"&&"+JetTruthSel,1);
  plotEventDisplayHitsDT(Tuple,outpath,"signal",JetSelection+"&&"+JetTruthSel);

}




///////////////////
///Function used for ECFA
//////////////////
void plotEventDisplay(TChain*TREE, TString outpath,int eventid=1){
  TCanvas C;

  //Select an event with only one true jet in the positive forward region
  //TString BasePtSelection="(30000<truth_pt[truth_VBFJ1]&&truth_pt[truth_VBFJ1]<80000&&30000<truth_pt[truth_VBFJ2]&&njet>=1)";
  TString BasePtSelection="(30000<truth_pt[truth_VBFJ1]&&truth_pt[truth_VBFJ1]<80000&&njet>=1)";
  TString BaseEtaSelection="(2.7<truth_eta[truth_VBFJ1]&&truth_eta[truth_VBFJ1]<3.5)";
  TString BaseIsolation="((truth_Gluon==-1||abs(truth_eta[truth_Gluon])<2.0||abs(truth_eta[truth_Gluon])>5.0)&&truth_eta[truth_VBFJ2]<0)";

  /////////////////////
  ////this selects the other VBF jet in the HGTD acceptance
  /////////////////////
  TString EventType="(1)";
  //TString EventType="(-4.0<truth_eta[truth_VBFJ2]&&truth_eta[truth_VBFJ2]<-2.6)";
  
  /////////////////////
  ///this selects a reco jet of VBF Jet 1 with good time distributions
  ////////////////////
  ////Using jet core cells
  //TString EventType="((njet>0&&jet_pt[0]<80000&&jet_truthJ[0]==truth_VBFJ1&&jet_dTFitCore_ncell_window30[0]>0.02*jet_dTFitCore_ncell[0])||(njet>1&&jet_pt[1]<80000&&jet_truthJ[1]==truth_VBFJ1&&jet_dTFitCore_ncell_window30[1]>0.02*jet_dTFitCore_ncell[1])||(njet>2&&jet_pt[2]<80000&&jet_truthJ[2]==truth_VBFJ1&&jet_dTFitCore_ncell_window30[2]>0.02*jet_dTFitCore_ncell[2])||(njet>3&&jet_pt[3]<80000&&jet_truthJ[3]==truth_VBFJ1&&jet_dTFitCore_ncell_window30[3]>0.02*jet_dTFitCore_ncell[3])||(njet>4&&jet_pt[4]<80000&&jet_truthJ[4]==truth_VBFJ1&&jet_dTFitCore_ncell_window30[4]>0.02*jet_dTFitCore_ncell[4])||(njet>5&&jet_pt[5]<80000&&jet_truthJ[5]==truth_VBFJ1&&jet_dTFitCore_ncell_window30[5]>0.02*jet_dTFitCore_ncell[5])||(njet>6&&jet_pt[6]<80000&&jet_truthJ[6]==truth_VBFJ1&&jet_dTFitCore_ncell_window30[6]>0.02*jet_dTFitCore_ncell[6])||(njet>7&&jet_pt[7]<80000&&jet_truthJ[7]==truth_VBFJ1&&jet_dTFitCore_ncell_window30[7]>0.02*jet_dTFitCore_ncell[7])||(njet>8&&jet_pt[8]<80000&&jet_truthJ[8]==truth_VBFJ1&&jet_dTFitCore_ncell_window30[8]>0.02*jet_dTFitCore_ncell[8]))";

  ////Using cluster cells
  //TString EventType="((njet>0&&jet_truthJ[0]==truth_VBFJ1&&jet_dTFit_ncell_clus_window30[0]>0.15*jet_ncell_clus[0])||(njet>1&&jet_truthJ[1]==truth_VBFJ1&&jet_dTFit_ncell_clus_window30[1]>0.15*jet_ncell_clus[1])||(njet>2&&jet_truthJ[2]==truth_VBFJ1&&jet_dTFit_ncell_clus_window30[2]>0.15*jet_ncell_clus[2])||(njet>3&&jet_truthJ[3]==truth_VBFJ1&&jet_dTFit_ncell_clus_window30[3]>0.15*jet_ncell_clus[3])||(njet>4&&jet_truthJ[4]==truth_VBFJ1&&jet_dTFit_ncell_clus_window30[4]>0.15*jet_ncell_clus[4])||(njet>5&&jet_truthJ[5]==truth_VBFJ1&&jet_dTFit_ncell_clus_window30[5]>0.15*jet_ncell_clus[5])||(njet>6&&jet_truthJ[6]==truth_VBFJ1&&jet_dTFit_ncell_clus_window30[6]>0.15*jet_ncell_clus[6])||(njet>7&&jet_truthJ[7]==truth_VBFJ1&&jet_dTFit_ncell_clus_window30[7]>0.15*jet_ncell_clus[7])||(njet>8&&jet_truthJ[8]==truth_VBFJ1&&jet_dTFit_ncell_clus_window30[8]>0.15*jet_ncell_clus[8]))";


  TString BaseSelection=BasePtSelection+"&&"+BaseEtaSelection+"&&"+BaseIsolation+"&&"+EventType;
  cout<<"Base selection: "<<BaseSelection<<endl;


  ////////////////////////////////////////////
  //find a good event
  ///////////////////////////////////////////
  TH2F HEventId("HEventId","",20000,0.5,20000.5,1,2,5);//do not change this binning 
  TREE->Draw("truth_eta[truth_VBFJ1]:event>>HEventId",BaseSelection,"col");
  // C.Clear();
  // HEventId.Draw();
  // C.Print(outpath+"/EventDisplay_EventNumber.png");
  
  //print the list of events with VBF jet in this range
  for(int b=1;b<=HEventId.GetXaxis()->GetNbins();b++)
    if(HEventId.GetBinContent(b,1)>0) 
      cout<<"Event "<<b<<endl; 


  //////////////////////
  ///select the event for faster plotting
  //////////////////////
  TTree* Tuple = TREE->CopyTree(TString("event==")+(long)eventid);
  
  
  /*
  ////////////////////////////////////////////////
  ////////the vertex
  ///////////////////////////////////////////////
  TH1F HVertex("HVertex","",100,-150,150); 
  HVertex.GetXaxis()->SetTitle("vertex z"); 
  C.Clear();
  Tuple->Draw("vtx_z>>HVertex");
  C.Print(outpath+"/EventDisplay_VertexZ.png");


  ////////////////////////////////////////////////
  ////////the generated jet
  ///////////////////////////////////////////////
  TH2F HVBFJ1("HVBFJ1","",100,-3.15,3.15,100,2,5); 
  HVBFJ1.GetXaxis()->SetTitle("#phi"); 
  HVBFJ1.GetYaxis()->SetTitle("#eta");
  C.Clear();
  Tuple->Draw("truth_eta[truth_VBFJ1]:truth_phi[truth_VBFJ1]>>HVBFJ1","truth_VBFJ1>=0","col");
  C.Print(outpath+"/EventDisplay_TruthJet12D.png");

  TH1F HVBFJ1pT("HVBFJ1pT","",100,0,300); 
  HVBFJ1.GetXaxis()->SetTitle("Jet pT   [GeV]"); 
  C.Clear();
  Tuple->Draw("truth_pt[truth_VBFJ1]/1000>>HVBFJ1pT","truth_VBFJ1>=0");
  C.Print(outpath+"/EventDisplay_TruthJet1pT.png");

  ///////////////////////////////////////////////
  ////////the reco jets
  ///////////////////////////////////////////////
  C.Clear();
  Tuple->Draw("jet_pt/1000>>jetPt(300,0,300)"); 
  C.Print(outpath+"/EventDisplay_JetPt.png");

  TH2F HRecoJets("HRecoJets","",100,-3.15,3.15,30,2,5); 
  HRecoJets.GetXaxis()->SetTitle("#phi"); 
  HRecoJets.GetYaxis()->SetTitle("#eta");
  C.Clear();
  Tuple->Draw("jet_eta:jet_phi>>HRecoJets","","col"); 
  C.Print(outpath+"/EventDisplay_JetEtaPhi.png");
  


  ////////////////////////////////////////////////
  ////////the reco clusters  
  ///////////////////////////////////////////////
  TH2F HRecoClus("HRecoClus","",100,-3.15,3.15,100,2,5); 
  HRecoClus.GetXaxis()->SetTitle("#phi"); 
  HRecoClus.GetYaxis()->SetTitle("#eta");
  C.Clear();
  Tuple->Draw("clus_eta:clus_phi>>HRecoClus","clus_jet>=0&&jet_eta[clus_jet]>2.4&&jet_pt[clus_jet]>30000","col"); 
  C.Print(outpath+"/EventDisplay_Cluster2D.png");
  
  */

  ///Fill the jet list 
  //TH2F HRecoJetPtEta("HRecoJetPtEta","",100,-5,5,300,0,300);
  TH2F HRecoJetPtEta("HRecoJetPtEta","",100,-5.0,5.0,200,0,200);
  Tuple->Draw("jet_pt/1000:jet_eta>>HRecoJetPtEta","jet_truthJ>=0");//&&abs(jet_dTFitCore_signalmean-jet_truedt)<0.060
  // C.Clear();
  // HRecoJetPtEta.Draw();
  // C.Print(outpath+"/EventDisplay_JetPtEta.png");
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
  
  //return;


  ///////////////////////////////////////////////
  ////////cells
  ///////////////////////////////////////////////
  TString CellJetSelection="(cell_jet>=0)"; 
  plotEventDisplayDrawHits(Tuple,outpath,"jet",CellJetSelection);

  //TString CellJetCoreSelection1="(cell_jetdR<0.10)"; // cell is on jet core
  //plotEventDisplayDrawHits(Tuple,outpath,"core1",CellJetSelection+"*"+CellJetCoreSelection1);

  TString CellJetCoreSelection2="(cell_jetdR<0.20)"; // cell is on jet core
  plotEventDisplayDrawHits(Tuple,outpath,"core2",CellJetSelection+"*"+CellJetCoreSelection2);

  // TString CellClusSelection="(cell_clus>0)"; // cell is on cluster
  // plotEventDisplayDrawHits(Tuple,outpath,"cluster",CellJetSelection+"*"+CellClusSelection);

  ///with background subtraction
  //TString CellJetCoreSignal="(cell_jetdR<0.2)"; 
  //TString CellJetCoreBkg="(cell_jetdR>0.20)"; 
  //plotEventDisplayDrawHits(Tuple,outpath,"CoreSig",CellJetSelection+"*"+CellJetCoreSignal,CellJetCoreBkg);

}












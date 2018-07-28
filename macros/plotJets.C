
void plotJetPtEtaIDRSummer2017(TChain*TREE,TString outpath){

  //Plot the pT distribution 
  TH1F HJetPt("HJetPt","",30,0,150);
  HJetPt.Sumw2();
  TREE->Draw("jet_pt/1000>>HJetPt",JetEtaCut);

  TH1F HJetPtSig("HJetPtSig","",HJetPt.GetXaxis()->GetNbins(),HJetPt.GetXaxis()->GetXmin(),HJetPt.GetXaxis()->GetXmax());
  HJetPtSig.Sumw2();
  HJetPtSig.GetXaxis()->SetTitle("Jet Pt [GeV]");
  HJetPtSig.SetLineColor(JETTYPECOLOR[0]);
  TREE->Draw("jet_pt/1000>>HJetPtSig",JetEtaCut+"&&"+signaljetCUT);
  //HJetPtSig.Scale(1./TREE->GetEntries());

  TH1F HJetPtPU("HJetPtPU","",HJetPt.GetXaxis()->GetNbins(),HJetPt.GetXaxis()->GetXmin(),HJetPt.GetXaxis()->GetXmax());
  HJetPtPU.Sumw2();
  HJetPtPU.GetXaxis()->SetTitle("Jet Pt [GeV]");
  HJetPtPU.SetLineColor(JETTYPECOLOR[1]);
  HJetPtPU.SetLineStyle(2);
  TREE->Draw("jet_pt/1000>>HJetPtPU",JetEtaCut+"&&"+pujetCUT);
  //HJetPtPU.Scale(1./TREE->GetEntries());

  TLegend leg(0.75,0.7,0.9,0.85);
  leg.SetFillColor(0);
  leg.SetLineColor(0);
  leg.SetBorderSize(0);
  leg.AddEntry(&HJetPt,"total","lp");
  leg.AddEntry(&HJetPtSig,"h.s. jet","l");
  leg.AddEntry(&HJetPtPU,"p.u. jet","l");

  C1D.Clear();
  HJetPt.GetYaxis()->SetTitle("# of jets");
  HJetPt.GetXaxis()->SetTitle("jet p_{T}  [GeV]");
  HJetPt.GetYaxis()->SetRangeUser(0, HJetPt.GetMaximum()*1.2);
  HJetPt.Draw("histpe");
  HJetPtSig.Draw("histsame");
  HJetPtPU.Draw("histsame");
  leg.Draw();
  DrawVBFH(outpath);
  DrawECMSMu(outpath);
  text.DrawLatexNDC(0.75,0.6,etamin+" < |#eta| < "+etamax);
  C1D.Print(outpath+"/JetPt.png");
  C1D.Print(outpath+"/JetPt.pdf");
  C1D.Print(outpath+"/JetPt.eps");
  


  /////////////////////////////////////////
  TH1F HJetEta("HJetEta","",100,-5,5);
  HJetEta.GetXaxis()->SetTitle("Jet eta");
  TREE->Draw("jet_eta>>HJetEta","abs(jet_eta)<3.8");
  HJetEta.Draw("hist");
  C1D.Print(outpath+"/JetEta.png");  


  //Print the # of jets in the HGTD acceptance
  char txt[100]; 

  C1D.Clear();
  TH1F HJetN("HJetN","",20,-0.5,19.5);
  HJetN.GetYaxis()->SetTitle("Events");
  HJetN.GetXaxis()->SetTitle("# of jets per event");
  TREE->Draw("njet>>HJetN");
  HJetN.Draw("hist");
  text.DrawLatexNDC(0.6,0.85,"2.4 < |#eta| <3.8");
  text.DrawLatexNDC(0.6,0.80,"p_{T} > 30 GeV");
  sprintf(txt,"Avg. # of jets = %.1f",HJetEta.Integral()/TREE->GetEntries());
  text.DrawTextNDC(0.6,0.75,txt);
  DrawVBFH(outpath);
  DrawECMSMu(outpath);
  C1D.SetLogy(1);
  C1D.Print(outpath+"/JetN.png");
  C1D.Print(outpath+"/JetN.pdf");
  C1D.Print(outpath+"/JetN.eps");
  C1D.SetLogy(0);

}


void plotJets(TChain*TREE,TString outpath){
  TCanvas C;

  //Plot the number of jets
  C.Clear();
  TH1F HJetN("HJetN","",50,-0.5,49.5);
  HJetN.GetXaxis()->SetTitle("Number of Jets");
  TREE->Draw("njet>>HJetN");
  HJetN.Draw("hist");
  char txt[100]; 
  sprintf(txt,"<N>=%.1f",HJetN.GetMean());
  text.DrawTextNDC(0.6,0.8,txt);
  C.SetLogy(1);
  C.Print(outpath+"/JetN.png");
  C.SetLogy(0);

  //Print the # of jets in the HGTD acceptance
  



  //Plot the pT distribution 
  TH1F HJetPt("HJetPt","",30,0,150);
  HJetPt.Sumw2();
  HJetPt.GetXaxis()->SetTitle("Jet Pt [GeV]");
  TREE->Draw("jet_pt/1000>>HJetPt");
  C.Clear();
  HJetPt.Draw("hist");
  C.Print(outpath+"/JetPt.png");


  //Plot the eta distribution  
  C.cd();
  C.Clear();
  TH1F HJetEta("HJetEta","",100,-5,5);
  HJetEta.GetXaxis()->SetTitle("Jet eta");
  TREE->Draw("jet_eta>>HJetEta");
  HJetEta.Draw("hist");
  C.Print(outpath+"/JetEta.png");


  //Number of clusters vs eta
  TH2F HJetNclusVsEta("HJetNclusVsEta","",30,2,5,30,-0.5,29.5);
  HJetNclusVsEta.GetXaxis()->SetTitle("Jet eta");
  HJetNclusVsEta.GetYaxis()->SetTitle("# of clusters");
  TREE->Draw("jet_nclus:jet_eta>>HJetNclusVsEta");
  C2D.cd();
  C2D.Clear();
  HJetNclusVsEta.Draw("colz");
  C2D.Print(outpath+"/HJetNclusVsEta.png");
  


  /////////////////////////////////////////////////////////////////////////
  ///Plot the truth match composition
  TH1F HTruthMatch("HTruthMatch","",25,-1.5,23.5);
  TREE->Draw("truthjet_truth[jet_truthjhs_dr3pt10]>=0 ? abs(truth_pdg[truthjet_truth[jet_truthjhs_dr3pt10]]) : 0>>HTruthMatch",JetEtaCut+"&&"+signaljetCUT);
  C.cd();
  C.Clear();
  HTruthMatch.GetYaxis()->SetRangeUser(0.1,HTruthMatch.GetMaximum()*2);
  HTruthMatch.GetXaxis()->SetTitle("truth PDG Id");
  HTruthMatch.Draw("hist");
  C.SetLogy(1);
  C.Print(outpath+"/Jet_HS_TruthMatchType.png");
  C.SetLogy(0);



  ///////////////////////////////////////////////////////////////////
  //plot the jet calibration
  TH2F HJetUnCalibResponse("HJetUnCalibResponse","",40,0,200,60,0,3);
  TREE->Draw("(jet_uncalib_pt/truthjet_pt[jet_truthjhs_dr3pt10]):jet_pt/1000>>HJetUnCalibResponse",JetEtaCut+"&&"+signaljetCUT);
  C2D.cd();
  C2D.Clear();
  HJetUnCalibResponse.GetYaxis()->SetRangeUser(0,3);
  HJetUnCalibResponse.GetXaxis()->SetTitle("calibrated p_{T}   [GeV]");
  HJetUnCalibResponse.GetYaxis()->SetTitle("uncalibrated p_{T} / truth p_{T}");
  HJetUnCalibResponse.Draw("colz");
  line.SetLineColor(1);
  line.DrawLine(HJetUnCalibResponse.GetXaxis()->GetXmin(),1,HJetUnCalibResponse.GetXaxis()->GetXmax(),1);
  C2D.Print(outpath+"/Jet_UnCalibrationResponse_pt.png");

  TH2F HJetCalibResponse("HJetCalibResponse","",40,0,200,60,0,3);
  TREE->Draw("(jet_pt/truthjet_pt[jet_truthjhs_dr3pt10]):jet_pt/1000>>HJetCalibResponse",JetEtaCut+"&&"+signaljetCUT);
  C2D.cd();
  C2D.Clear();
  HJetCalibResponse.GetYaxis()->SetRangeUser(0,3);
  HJetCalibResponse.GetXaxis()->SetTitle("calibrated p_{T}   [GeV]");
  HJetCalibResponse.GetYaxis()->SetTitle("calibrated p_{T} / truth p_{T}");
  HJetCalibResponse.Draw("colz");
  line.SetLineColor(1);
  line.DrawLine(HJetCalibResponse.GetXaxis()->GetXmin(),1,HJetCalibResponse.GetXaxis()->GetXmax(),1);
  C2D.Print(outpath+"/Jet_CalibrationResponse_pt.png");


}

void plotJetComposition(TChain*TREE,TString outpath){
  setCuts(outpath);
  cout<<"Jet eta cut: "<<JetEtaCut<<endl;
  cout<<"Jet pT cut: "<<JetPtCut<<endl;
  cout<<"Signal jet: "<<signaljetCUT<<endl;
  cout<<"QCD jet: "<<qcdjetCUT<<endl;

  TH1F HJetPt("HJetPt","",30,0,150);
  HJetPt.Sumw2();
  HJetPt.GetXaxis()->SetTitle("jet p_{T} [GeV]");
  HJetPt.GetYaxis()->SetTitle("# of jets per event /  5 GeV");
  TREE->Draw("jet_pt/1000>>HJetPt",JetEtaCut);
  HJetPt.Scale(1./TREE->GetEntries());

  TH1F HJetPtSig("HJetPtSig","",HJetPt.GetXaxis()->GetNbins(),HJetPt.GetXaxis()->GetXmin(),HJetPt.GetXaxis()->GetXmax());
  HJetPtSig.Sumw2();
  HJetPtSig.GetXaxis()->SetTitle("Jet Pt [GeV]");
  HJetPtSig.SetFillColor(JETTYPECOLOR[0]);
  TREE->Draw("jet_pt/1000>>HJetPtSig",JetEtaCut+"&&"+signaljetCUT);
  HJetPtSig.Scale(1./TREE->GetEntries());

  TH1F HJetPtQCD("HJetPtQCD","",HJetPt.GetXaxis()->GetNbins(),HJetPt.GetXaxis()->GetXmin(),HJetPt.GetXaxis()->GetXmax());
  HJetPtQCD.Sumw2();
  HJetPtQCD.GetXaxis()->SetTitle("Jet Pt [GeV]");
  HJetPtQCD.SetFillColor(JETTYPECOLOR[1]);
  TREE->Draw("jet_pt/1000>>HJetPtQCD",JetEtaCut+"&&"+qcdjetCUT);
  HJetPtQCD.Scale(1./TREE->GetEntries());

  TH1F HJetPtStoc("HJetPtStoc","",HJetPt.GetXaxis()->GetNbins(),HJetPt.GetXaxis()->GetXmin(),HJetPt.GetXaxis()->GetXmax());
  HJetPtStoc.Sumw2();
  HJetPtStoc.GetXaxis()->SetTitle("Jet Pt [GeV]");
  HJetPtStoc.SetFillColor(JETTYPECOLOR[2]);
  TREE->Draw("jet_pt/1000>>HJetPtStoc",JetEtaCut+"&&"+stocjetCUT);
  HJetPtStoc.Scale(1./TREE->GetEntries());

  TH1F HJetPtOther("HJetPtOther","",HJetPt.GetXaxis()->GetNbins(),HJetPt.GetXaxis()->GetXmin(),HJetPt.GetXaxis()->GetXmax());
  HJetPtOther.Sumw2();
  HJetPtOther.GetXaxis()->SetTitle("Jet Pt [GeV]");
  HJetPtOther.SetFillColor(JETTYPECOLOR[3]);
  TREE->Draw("jet_pt/1000>>HJetPtOther",JetEtaCut+"&&"+otherjetCUT);
  HJetPtOther.Scale(1./TREE->GetEntries());

  THStack JetPtStack("JetPtStack","");
  JetPtStack.Add(&HJetPtOther);
  JetPtStack.Add(&HJetPtStoc,"hist");
  JetPtStack.Add(&HJetPtQCD,"hist");
  JetPtStack.Add(&HJetPtSig,"hist");
  JetPtStack.SetTitle("");
  
  TLegend leg(0.75,0.7,0.9,0.90);
  leg.SetFillColor(0);
  leg.SetLineColor(0);
  leg.SetBorderSize(0);
  leg.AddEntry(&HJetPt,"Total","lp");
  leg.AddEntry(&HJetPtSig,"H.S.","f");
  leg.AddEntry(&HJetPtQCD,"QCD","f");
  leg.AddEntry(&HJetPtStoc,"Stochastic","f");
  leg.AddEntry(&HJetPtOther,"Other","f");

  C1D.Clear();
  HJetPt.GetYaxis()->SetTitle("# of jets per event");
  HJetPt.GetXaxis()->SetTitle("jet p_{T}  [GeV]");
  HJetPt.GetYaxis()->SetRangeUser(0, HJetPt.GetMaximum()*1.2);
  HJetPt.Draw("histpe");
  JetPtStack.Draw("histsame");
  leg.Draw();
  DrawVBFH(outpath);
  DrawECMSMu(outpath);
  HJetPt.Draw("histpesame");
  text.DrawLatexNDC(0.75,0.6,etamin+" < |#eta| < "+etamax);
  C1D.Print(outpath+"/JetPt_TypeComposition.png");
  


  ////show the fractions
  TH1F*HSigFraction=makeHistRatio(&HJetPt,&HJetPtSig);
  HSigFraction->SetLineColor(JETTYPECOLOR[0]);   
  HSigFraction->SetMarkerColor(JETTYPECOLOR[0]);
  TH1F*HQCDFraction=makeHistRatio(&HJetPt,&HJetPtQCD);
  HQCDFraction->SetLineColor(JETTYPECOLOR[1]);     
  HQCDFraction->SetMarkerColor(JETTYPECOLOR[1]);
  TH1F*HStocFraction=makeHistRatio(&HJetPt,&HJetPtStoc);
  HStocFraction->SetLineColor(JETTYPECOLOR[2]);     
  HStocFraction->SetMarkerColor(JETTYPECOLOR[2]);
  TH1F*HOtherFraction=makeHistRatio(&HJetPt,&HJetPtOther);
  HOtherFraction->SetLineColor(JETTYPECOLOR[3]);     
  HOtherFraction->SetMarkerColor(JETTYPECOLOR[3]);

  TLegend legFrac(0.75,0.60,0.94,0.80);
  legFrac.SetFillColor(0);
  legFrac.SetLineColor(0);
  legFrac.SetBorderSize(0);
  legFrac.AddEntry(HSigFraction,"H.S.","lp");
  legFrac.AddEntry(HQCDFraction,"QCD","lp");
  legFrac.AddEntry(HStocFraction,"Stochastic","lp");
  legFrac.AddEntry(HOtherFraction,"Other","lp");

  C1D.Clear();
  HSigFraction->GetYaxis()->SetTitle("fraction of jets");
  HSigFraction->GetYaxis()->SetRangeUser(0,1.0);
  HSigFraction->GetXaxis()->SetTitle(" jet p_{T}  [GeV]");
  HSigFraction->Draw("histpe");
  HQCDFraction->Draw("histpesame");
  HStocFraction->Draw("histpesame");
  HOtherFraction->Draw("histpesame");
  legFrac.Draw();
  text.DrawLatexNDC(0.75,0.5,etamin+" < |#eta| < "+etamax);
  DrawVBFH(outpath);
  DrawECMSMu(outpath);
  C1D.Print(outpath+"/JetPt_TypeFractions.png");
  

 
  /////show the dR distributions w.r.t. the truth jet
  TH1F HJetdRSig("HJetdRSig","",35,0,0.35);
  HJetdRSig.Sumw2();
  HJetdRSig.GetXaxis()->SetTitle("#DeltaR(reco. jet - truth jet)");
  HJetdRSig.GetYaxis()->SetTitle("");
  TREE->Draw("sqrt(pow(jet_eta-truthjet_eta[jet_truthjhs_dr3pt10],2) + pow(abs(jet_phi-truthjet_phi[jet_truthjhs_dr3pt10])<3.14159?abs(jet_phi-truthjet_phi[jet_truthjhs_dr3pt10]): 2*3.14159 - abs(jet_phi-truthjet_phi[jet_truthjhs_dr3pt10]),2))>>HJetdRSig",JetEtaCut+"&&"+JetPtCut+"&&"+signaljetCUT);
  HJetdRSig.Scale(1./HJetdRSig.Integral());
  HJetdRSig.SetLineColor(JETTYPECOLOR[0]);

  TH1F HJetdRQCD("HJetdRQCD","",35,0,0.35);
  HJetdRQCD.Sumw2();
  HJetdRQCD.GetXaxis()->SetTitle("#DeltaR(reco. jet - truth jet)");
  HJetdRQCD.GetYaxis()->SetTitle("");
  TREE->Draw("sqrt(pow(jet_eta-truthjet_eta[jet_truthjpu_dr3pt10],2) + pow(abs(jet_phi-truthjet_phi[jet_truthjpu_dr3pt10])<3.14159?abs(jet_phi-truthjet_phi[jet_truthjpu_dr3pt10]): 2*3.14159 - abs(jet_phi-truthjet_phi[jet_truthjpu_dr3pt10]),2))>>HJetdRQCD",JetEtaCut+"&&"+JetPtCut+"&&"+qcdjetCUT);
  HJetdRQCD.Scale(1./HJetdRQCD.Integral());
  HJetdRQCD.SetLineColor(JETTYPECOLOR[1]);

  TLegend legdR(0.6,0.60,0.90,0.80);
  legdR.SetFillColor(0);
  legdR.SetLineColor(0);
  legdR.SetBorderSize(0);
  legdR.AddEntry(&HJetdRSig,"2.6 < |#eta| < 3.8","");
  legdR.AddEntry(&HJetdRSig,"30 GeV < p_{T} < 70 GeV","");
  legdR.AddEntry(&HJetdRSig,"H.S.","l");
  legdR.AddEntry(&HJetdRQCD,"QCD","l");
  C1D.Clear();
  HJetdRSig.GetYaxis()->SetTitle("number of jets (unit norm)");
  HJetdRSig.Draw("hist");
  HJetdRQCD.Draw("histsame");
  legdR.Draw();
  DrawVBFH(outpath);
  DrawECMSMu(outpath);
  C1D.Print(outpath+"/JetComposition_dRTruth.png");
 
  cout<<"Signal Efficiency for dR<0.1 = "<<HJetdRSig.Integral(1,10)/HJetdRSig.Integral(0,36)<<endl;
  cout<<"QCD Efficiency for dR<0.1 = "<<HJetdRQCD.Integral(1,10)/HJetdRQCD.Integral(0,36)<<endl;


  TH1F HJetPtSigdRTruth("HJetPtSigdRTruth","",HJetPt.GetXaxis()->GetNbins(),HJetPt.GetXaxis()->GetXmin(),HJetPt.GetXaxis()->GetXmax());
  HJetPtSigdRTruth.Sumw2();
  HJetPtSigdRTruth.GetXaxis()->SetTitle("Jet Pt [GeV]");
  HJetPtSigdRTruth.SetMarkerColor(JETTYPECOLOR[0]);
  HJetPtSigdRTruth.SetLineColor(JETTYPECOLOR[0]);
  TREE->Draw("jet_pt/1000>>HJetPtSigdRTruth",JetEtaCut+"&&"+signaljetCUT+"&&sqrt(pow(jet_eta-truthjet_eta[jet_truthjhs_dr3pt10],2) + pow(abs(jet_phi-truthjet_phi[jet_truthjhs_dr3pt10])<3.14159?abs(jet_phi-truthjet_phi[jet_truthjhs_dr3pt10]): 2*3.14159 - abs(jet_phi-truthjet_phi[jet_truthjhs_dr3pt10]),2))<0.1");
  HJetPtSigdRTruth.Scale(1./TREE->GetEntries());
  TH1F*HSigEff=makeHistRatio(&HJetPtSig,&HJetPtSigdRTruth);

  TH1F HJetPtQCDdRTruth("HJetPtQCDdRTruth","",HJetPt.GetXaxis()->GetNbins(),HJetPt.GetXaxis()->GetXmin(),HJetPt.GetXaxis()->GetXmax());
  HJetPtQCDdRTruth.Sumw2();
  HJetPtQCDdRTruth.GetXaxis()->SetTitle("Jet Pt [GeV]");
  HJetPtQCDdRTruth.SetMarkerColor(JETTYPECOLOR[1]);
  HJetPtQCDdRTruth.SetLineColor(JETTYPECOLOR[1]);
  TREE->Draw("jet_pt/1000>>HJetPtQCDdRTruth",JetEtaCut+"&&"+qcdjetCUT+"&&sqrt(pow(jet_eta-truthjet_eta[jet_truthjpu_dr3pt10],2) + pow(abs(jet_phi-truthjet_phi[jet_truthjpu_dr3pt10])<3.14159?abs(jet_phi-truthjet_phi[jet_truthjpu_dr3pt10]): 2*3.14159 - abs(jet_phi-truthjet_phi[jet_truthjpu_dr3pt10]),2))<0.1");
  HJetPtQCDdRTruth.Scale(1./TREE->GetEntries());
  TH1F*HQCDEff=makeHistRatio(&HJetPtQCD,&HJetPtQCDdRTruth);

  C1D.Clear();
  HSigEff->GetYaxis()->SetTitle("efficiency");
  HSigEff->Draw("histpe");
  HQCDEff->Draw("histpesame");
  C1D.Print(outpath+"/JetComposition_dRTruthEff.png");



  ///////////////////////////////////
  /// # of jets per event
  //////////////////////////////////
  TH1F HNjet("HNjet","",4,0,1);
  HNjet.SetBinContent(1,HJetPtSig.Integral(0,HJetPtSig.GetNbinsX()+1)); 
  HNjet.GetXaxis()->SetBinLabel(1,"H.S.");
  HNjet.SetBinContent(2,HJetPtQCD.Integral(0,HJetPtSig.GetNbinsX()+1));
  HNjet.GetXaxis()->SetBinLabel(2,"QCD");
  HNjet.SetBinContent(3,HJetPtStoc.Integral(0,HJetPtSig.GetNbinsX()+1));
  HNjet.GetXaxis()->SetBinLabel(3,"Stochastic");
  HNjet.SetBinContent(4,HJetPtOther.Integral(0,HJetPtSig.GetNbinsX()+1));
  HNjet.GetXaxis()->SetBinLabel(4,"other");
  C1D.Clear();
  HNjet.GetYaxis()->SetRangeUser(0.,0.8);
  HNjet.GetYaxis()->SetTitle("average # of jets / event ");
  HNjet.GetXaxis()->SetNdivisions(0);
  HNjet.GetXaxis()->SetLabelSize(0.06);
  HNjet.Draw("hist");
  DrawVBFH(outpath);
  DrawECMSMu(outpath);
  text.DrawLatexNDC(0.70,0.85,"Anti-k_{T} EM+JES R=0.4");
  text.DrawLatexNDC(0.70,0.80,"p_{T} > 30 GeV");
  text.DrawLatexNDC(0.70,0.75,etamin+" < |#eta| < "+etamax);
  C1D.Print(outpath+"/JetComposition_NJetPerEvent.png");
		      
}


void plotJetCells(TChain*TREE,TString outpath){
  TCanvas C;

  ///Plot the number of cells associated to jets
  C.Clear();
  TH1F HJetNcell("HJetNcell","",30,0,300);
  HJetNcell.GetXaxis()->SetTitle("Number of cells in jet");
  TREE->Draw("jet_ncell>>HJetNcell");
  HJetNcell.Draw("hist");
  C.Print(outpath+"/JetNcell.png");
  //C.SetLogy(1);
  //C.Print(outpath+"/JetNcell_log.png");
  //C.SetLogy(0);

  ///2-D distribution of jets in ncell vs eta 
  TH2F HJetNcellvsEta("HJetNcellvsEta","",100,-5,5,3001,-0.5,3000.5);
  HJetNcellvsEta.GetXaxis()->SetTitle("jet #eta");
  HJetNcellvsEta.GetYaxis()->SetTitle("Number of cells in jet");
  TREE->Draw("jet_ncell:jet_eta>>HJetNcellvsEta",signaljetCUT+"*"+JetPtCut);
  //HJetNcellvsEta.GetYaxis()->SetRangeUser(0,1000);
  C.Clear();
  HJetNcellvsEta.Draw("col");
  C.Print(outpath+"/JetNcellvsEta.png");

  TProfile * PJetNcellvsEta = HJetNcellvsEta.ProfileX("PJetNcellvsEta");
  PJetNcellvsEta->GetYaxis()->SetTitle("Average number of cells");
  //PJetNcellvsEta->GetYaxis()->SetRangeUser(0,250);
  C.Clear();
  PJetNcellvsEta->Draw("pe");
  C.Print(outpath+"/JetNcellvsEta_Profile.png");
  //C.Print(outpath+"/JetNcellvsEta_Profile.pdf");
  delete PJetNcellvsEta;


  /////////////////////////////
  ///plot the dR shapes
  /////////////////////////////
  TH1F HNjetPU("HNjetPU","",1,-10,10);
  TREE->Draw("jet_eta>>HNjetPU",pujetCUT+"*"+JetPtCut);
  TH1F*HdRPU=getdRDensityHisto(TString("HdRPU"));   
  TREE->Draw("(jet_pt>0)*0.025>>HdRPU",pujetCUT+"*"+JetPtCut+"*jet_ncell_dr00");
  TREE->Draw("(jet_pt>0)*0.075>>+HdRPU",pujetCUT+"*"+JetPtCut+"*jet_ncell_dr05");
  TREE->Draw("(jet_pt>0)*0.125>>+HdRPU",pujetCUT+"*"+JetPtCut+"*jet_ncell_dr10");
  TREE->Draw("(jet_pt>0)*0.175>>+HdRPU",pujetCUT+"*"+JetPtCut+"*jet_ncell_dr15");
  TREE->Draw("(jet_pt>0)*0.225>>+HdRPU",pujetCUT+"*"+JetPtCut+"*jet_ncell_dr20");
  TREE->Draw("(jet_pt>0)*0.275>>+HdRPU",pujetCUT+"*"+JetPtCut+"*jet_ncell_dr25");
  TREE->Draw("(jet_pt>0)*0.325>>+HdRPU",pujetCUT+"*"+JetPtCut+"*jet_ncell_dr30");
  TREE->Draw("(jet_pt>0)*0.375>>+HdRPU",pujetCUT+"*"+JetPtCut+"*jet_ncell_dr35");
  HdRPU->Scale(1./HNjetPU.Integral());
  dividedRDensityHisto(HdRPU);
  C.Clear();
  HdRPU->Draw("histpe");
  C.Print(outpath+"/JetCelldRShape.png");





}


void plotJetCelldPhidEta(TChain*TREE,TString outpath){

  TString Sel="cell_jet>=0&&jet_truth_pt[cell_jet]>0";
  TString SelPU="cell_jet>=0&&jet_truth_pt[cell_jet]<=0";
  TCanvas C;
  C.SetRightMargin(0.15);

  TString xtitle="#Delta#phi_{cell-jet}";
  TString ytitle="#Delta#eta_{cell-jet}";
  float titlesize=0.07;
  

  int etastep=(ETAMAX-ETAMIN)*10/ETABINS;
  int minetaidx=ETAMIN*10;
  int maxetaidx=ETAMAX*10 - etastep;
  int ptstep=(PTMAX-PTMIN)/(PTBINS*1000);
  int minptidx=PTMIN/1000;
  int maxptidx=PTMAX/1000-ptstep;

  for(long pt=minptidx;pt<=maxptidx;pt+=ptstep){
    for(long eta=minetaidx;eta<=maxetaidx;eta+=etastep){
      TString ptetasel=TString("(")+pt+"<jet_pt[cell_jet]/1000&&jet_pt[cell_jet]/1000<"+(pt+ptstep)+"&&"+eta+"<abs(jet_eta[cell_jet])*10&&abs(jet_eta[cell_jet])*10<"+(eta+etastep)+")";

      TH2F HCelldEtadPhiEta("HCelldEtadPhiEta","",100,-0.5,0.5,100,-0.5,0.5);
      HCelldEtadPhiEta.GetXaxis()->SetTitle(xtitle);
      HCelldEtadPhiEta.GetYaxis()->SetTitle(ytitle);
      HCelldEtadPhiEta.GetXaxis()->SetTitleSize(titlesize);
      HCelldEtadPhiEta.GetYaxis()->SetTitleSize(titlesize);
      HCelldEtadPhiEta.GetXaxis()->SetTitleOffset(0.9);
      HCelldEtadPhiEta.GetYaxis()->SetTitleOffset(0.9);
      TREE->Draw("(cell_eta-jet_eta[cell_jet]):(cell_phi-jet_phi[cell_jet])>>HCelldEtadPhiEta",Sel+"&&"+ptetasel);
      //HCelldEtadPhiEta.Scale(1./HCelldEtadPhiEta.Integral());
      HCelldEtadPhiEta.Scale(1./HCelldEtadPhiEta.GetMaximum());
      C2D.cd();
      C2D.Clear();
      HCelldEtadPhiEta.Draw("colz");
      C2D.Print(outpath+"/JetCelldEtadPhi_Sig_eta"+eta+"_pt"+pt+".png");
      C2D.Print(outpath+"/JetCelldEtadPhi_Sig_eta"+eta+"_pt"+pt+".pdf");


      TH2F HCelldEtadPhiEtaPU("HCelldEtadPhiEtaPU","",100,-0.5,0.5,100,-0.5,0.5);
      HCelldEtadPhiEtaPU.GetXaxis()->SetTitle(xtitle);
      HCelldEtadPhiEtaPU.GetYaxis()->SetTitle(ytitle);
      HCelldEtadPhiEtaPU.GetXaxis()->SetTitleSize(titlesize);
      HCelldEtadPhiEtaPU.GetYaxis()->SetTitleSize(titlesize);
      HCelldEtadPhiEtaPU.GetXaxis()->SetTitleOffset(0.9);
      HCelldEtadPhiEtaPU.GetYaxis()->SetTitleOffset(0.9);
      TREE->Draw("(cell_eta-jet_eta[cell_jet]):(cell_phi-jet_phi[cell_jet])>>HCelldEtadPhiEtaPU",SelPU+"&&"+ptetasel);
      //HCelldEtadPhiEtaPU.Scale(1./HCelldEtadPhiEtaPU.Integral());
      HCelldEtadPhiEtaPU.Scale(1./HCelldEtadPhiEtaPU.GetMaximum());
      C2D.Clear();
      HCelldEtadPhiEtaPU.Draw("colz");
      C2D.Print(outpath+"/JetCelldEtadPhi_PU_eta"+eta+"_pt"+pt+".png");
      C2D.Print(outpath+"/JetCelldEtadPhi_PU_eta"+eta+"_pt"+pt+".pdf");

    }
  }


  ////plot the fully inclusive jets
  TString ptetasel=TString("(")+(long)PTMIN+"<jet_pt[cell_jet]&&jet_pt[cell_jet]<"+(long)PTMAX+"&&"+(long)(ETAMIN*10)+"<abs(jet_eta[cell_jet])*10&&abs(jet_eta[cell_jet])*10<"+(long)(ETAMAX*10)+")";

  TH2F HCelldEtadPhiEta("HCelldEtadPhiEta","",100,-0.5,0.5,100,-0.5,0.5);
  HCelldEtadPhiEta.GetXaxis()->SetTitle(xtitle);
  HCelldEtadPhiEta.GetYaxis()->SetTitle(ytitle);
  HCelldEtadPhiEta.GetXaxis()->SetTitleSize(titlesize);
  HCelldEtadPhiEta.GetYaxis()->SetTitleSize(titlesize);
  HCelldEtadPhiEta.GetXaxis()->SetTitleOffset(0.9);
  HCelldEtadPhiEta.GetYaxis()->SetTitleOffset(0.9);
  TREE->Draw("(cell_eta-jet_eta[cell_jet]):(cell_phi-jet_phi[cell_jet])>>HCelldEtadPhiEta",Sel+"&&"+ptetasel);
  //HCelldEtadPhiEta.Scale(1./HCelldEtadPhiEta.Integral());
  HCelldEtadPhiEta.Scale(1./HCelldEtadPhiEta.GetMaximum());
  C2D.cd();
  C2D.Clear();
  HCelldEtadPhiEta.Draw("colz");
  C2D.Print(outpath+"/JetCelldEtadPhi_Sig.png");
  C2D.Print(outpath+"/JetCelldEtadPhi_Sig.pdf");


  TH2F HCelldEtadPhiEtaPU("HCelldEtadPhiEtaPU","",100,-0.5,0.5,100,-0.5,0.5);
  HCelldEtadPhiEtaPU.GetXaxis()->SetTitle(xtitle);
  HCelldEtadPhiEtaPU.GetYaxis()->SetTitle(ytitle);
  HCelldEtadPhiEtaPU.GetXaxis()->SetTitleSize(titlesize);
  HCelldEtadPhiEtaPU.GetYaxis()->SetTitleSize(titlesize);
  HCelldEtadPhiEtaPU.GetXaxis()->SetTitleOffset(0.9);
  HCelldEtadPhiEtaPU.GetYaxis()->SetTitleOffset(0.9);
  TREE->Draw("(cell_eta-jet_eta[cell_jet]):(cell_phi-jet_phi[cell_jet])>>HCelldEtadPhiEtaPU",SelPU+"&&"+ptetasel);
  //HCelldEtadPhiEtaPU.Scale(1./HCelldEtadPhiEtaPU.Integral());
  HCelldEtadPhiEtaPU.Scale(1./HCelldEtadPhiEtaPU.GetMaximum());
  C2D.cd();
  C2D.Clear();
  HCelldEtadPhiEtaPU.Draw("colz");
  C2D.Print(outpath+"/JetCelldEtadPhi_PU.png");
  C2D.Print(outpath+"/JetCelldEtadPhi_PU.pdf");



}



void plotJetNearJet(TChain*TREE,TString outpath){

  ///Inclusive plot show # of nearby jets
  TH1F HNSig("HNSig","",5,0,5);
  HNSig.GetXaxis()->SetTitle("# of jets #DeltaR<0.8");
  TREE->Draw(TString("jet_njetnear>>HNSig"),signaljetCUT);      
  C1D.Clear();
  HNSig.Draw();  
  C1D.Print(outpath+"/JetNearJet_N_Sig.png");

  TH1F HNPU("HNPU","",5,0,5);
  HNPU.GetXaxis()->SetTitle("# of jets #DeltaR<0.8");
  TREE->Draw(TString("jet_njetnear>>HNPU"),pujetCUT);      
  C1D.Clear();
  HNPU.Draw();  
  C1D.Print(outpath+"/JetNearJet_N_PU.png");


  TH1F HdRSig("HdRSig","",50,0,5);
  HdRSig.GetXaxis()->SetTitle("dR to nearest jet");
  TREE->Draw(TString("jet_nearjetdR>>HdRSig"),signaljetCUT+"*(jet_nearjet>=0)");      
  C1D.Clear();
  HdRSig.Draw();  
  C1D.Print(outpath+"/JetNearJet_dR_Sig.png");

  TH1F HdRPU("HdRPU","",50,0,5);
  HdRPU.GetXaxis()->SetTitle("dR to nearest jet");
  TREE->Draw(TString("jet_nearjetdR>>HdRPU"),pujetCUT+"*(jet_nearjet>=0)");      
  C1D.Clear();
  HdRPU.Draw();  
  C1D.Print(outpath+"/JetNearJet_dR_PU.png");



  /////////////////////////
  ////Scan all pT and eta regions
  ///////////////////////////
  long neta=3;
  long etastep=4;
  long minetaidx=26;

  long npt=2;
  long ptstep=20;
  long minptidx=30;

  char txt[100];

  for(long eta=0;eta<neta;eta++){
    for(long pt=0;pt<npt;pt++){

      long eta1=minetaidx+eta*etastep;
      long eta2=minetaidx+(eta+1)*etastep;
      long pt1=minptidx+pt*ptstep;
      long pt2=minptidx+(pt+1)*ptstep;
      
      TString ptetaselection = TString("(((")+eta1+"/10.)<abs(jet_eta)&&abs(jet_eta)<("+eta2+"/10.))"+"&&(("+pt1+"*1000.)<jet_pt&&jet_pt<("+pt2+"*1000.)))";
      if(outpath.Contains("_mu0"))      
	ptetaselection=TString("(((")+eta1+"/10.)<abs(jet_eta)&&abs(jet_eta)<("+eta2+"/10.))"+"&&(("+pt1+"*1000.)<jet_truth_pt&&jet_truth_pt<("+pt2+"*1000.)))";
      
      ///////////////////////////////////////////
      TString name=TString("HNearJetNjet_eta")+eta+"_pt"+pt;
      TH1F HN(name,"",10,0,10);
      HN.GetXaxis()->SetTitle("# of jets #DeltaR<0.8");
      TREE->Draw(TString("jet_njetnear>>")+name,signaljetCUT+"*"+ptetaselection);      
      C1D.Clear();
      HN.Draw();  

      sprintf(txt,"Mean = %.1f",HN.GetMean());
      text.DrawLatexNDC(0.7,0.7,txt);

      text.SetTextSize(0.045);
      sprintf(txt,"%.0f GeV<p_{T}^{J}<%.0f GeV",(float)pt1,(float)(pt2));
      text.DrawLatexNDC(0.67,0.75,txt);
      
      sprintf(txt,"%.1f<|#eta^{J}|<%.1f",float(eta1)/10,float(eta2)/10.);
      text.DrawLatexNDC(0.67,0.8,txt);

      C1D.Print(outpath+"/JetNearJet_N_"+eta+"_"+pt+"_Sig.png");

      TString namedR=TString("HNearJetdR_eta")+eta+"_pt"+pt;
      TH1F HdR(namedR,"",30,0,3);
      HdR.GetXaxis()->SetTitle("distance to nearest jet (#DeltaR)");
      TREE->Draw(TString("jet_nearjetdR>>")+namedR,signaljetCUT+"*"+ptetaselection+"*(jet_nearjet>=0)");
      C1D.Clear();
      HdR.Draw();
      C1D.Print(outpath+"/JetNearJet_NearJetdR_"+eta+"_"+pt+"_Sig.png");


      ////////////////////////////////
      TString namePU=TString("HNearJetNjetPU_eta")+eta+"_pt"+pt;
      TH1F HNPU(namePU,"",10,0,10);
      HNPU.GetXaxis()->SetTitle( HN.GetXaxis()->GetTitle());
      TREE->Draw(TString("jet_njetnear>>")+namePU,pujetCUT+"*"+ptetaselection);      
      C1D.Clear();
      HNPU.Draw();      

      sprintf(txt,"Mean = %.1f",HNPU.GetMean());
      text.DrawLatexNDC(0.7,0.7,txt);

      text.SetTextSize(0.045);
      sprintf(txt,"%.0f GeV<p_{T}^{J}<%.0f GeV",(float)pt1,(float)(pt2));
      text.DrawLatexNDC(0.67,0.75,txt);
      
      sprintf(txt,"%.1f<|#eta^{J}|<%.1f",float(eta1)/10,float(eta2)/10.);
      text.DrawLatexNDC(0.67,0.8,txt);

      C1D.Print(outpath+"/JetNearJet_N_"+eta+"_"+pt+"_PU.png");

      TString namePUdR=TString("HNearJetdRPU_eta")+eta+"_pt"+pt;
      TH1F HdRPU(namePUdR,"",30,0,3);
      HdRPU.GetXaxis()->SetTitle(HdR.GetXaxis()->GetTitle());
      TREE->Draw(TString("jet_nearjetdR>>")+namePUdR,pujetCUT+"*"+ptetaselection+"*(jet_nearjet>=0)");
      C1D.Clear();
      HdRPU.Draw();
      C1D.Print(outpath+"/JetNearJet_NearJetdR_"+eta+"_"+pt+"_PU.png");

    }
  }


}


void plotJetCelldR(TChain*TREE,TString outpath){

  long neta=3;
  long etastep=4;
  long minetaidx=26;
  long maxetaidx=34;

  long npt=1;
  long ptstep=40;
  long minptidx=30;
  long maxptidx=30;

  TH2F HFillCelldRNJet("HFillCelldRNJet","",neta,minetaidx,minetaidx+neta*etastep,npt,minptidx,minptidx+npt*ptstep);  
  TH2F HFillCelldRNJetPU("HFillCelldRNJetPU","",neta,minetaidx,minetaidx+neta*etastep,npt,minptidx,minptidx+npt*ptstep);  
  if(outpath.Contains("_mu0")){//for mu=0 jet calibration does not apply
    TREE->Draw("jet_truth_pt/1000.:abs(10*jet_eta)>>HFillCelldRNJet","jet_truth_pt>0");
    TREE->Draw("jet_truth_pt/1000.:abs(10*jet_eta)>>HFillCelldRNJetPU","jet_truth_pt<=0");
  }else{
    TREE->Draw("jet_pt/1000.:abs(10*jet_eta)>>HFillCelldRNJet","jet_truth_pt>0");
    TREE->Draw("jet_pt/1000.:abs(10*jet_eta)>>HFillCelldRNJetPU","jet_truth_pt<=0");
  }


  ////Fill distributions
  TH1F * HdR[10][10];
  TH1F * HdRPU[10][10];
  for(long eta=0;eta<neta;eta++){
    for(long pt=0;pt<npt;pt++){
      long eta1=minetaidx+eta*etastep;
      long eta2=minetaidx+(eta+1)*etastep;
      long pt1=minptidx+pt*ptstep;
      long pt2=minptidx+(pt+1)*ptstep;

      TString ptetaselection=TString("(((")+eta1+"/10.)<abs(jet_eta)&&abs(jet_eta)<("+eta2+"/10.))"+"&&(("+pt1+"*1000.)<jet_pt&&jet_pt<("+pt2+"*1000.)))";
      if(outpath.Contains("_mu0"))      //for mu=0 jet calibration does not apply
	ptetaselection=TString("(((")+eta1+"/10.)<abs(jet_eta)&&abs(jet_eta)<("+eta2+"/10.))"+"&&(("+pt1+"*1000.)<jet_truth_pt&&jet_truth_pt<("+pt2+"*1000.)))";
      
      
      TString name=TString("HdR_eta")+eta+"_pt"+pt;
      HdR[eta][pt]=getdRDensityHisto(name); 
      TREE->Draw(TString("(jet_pt>0)*0.025>>")+name,signaljetCUT+"*"+ptetaselection+"*jet_ncell_dr00");
      TREE->Draw(TString("(jet_pt>0)*0.075>>+")+name,signaljetCUT+"*"+ptetaselection+"*jet_ncell_dr05");
      TREE->Draw(TString("(jet_pt>0)*0.125>>+")+name,signaljetCUT+"*"+ptetaselection+"*jet_ncell_dr10");
      TREE->Draw(TString("(jet_pt>0)*0.175>>+")+name,signaljetCUT+"*"+ptetaselection+"*jet_ncell_dr15");
      TREE->Draw(TString("(jet_pt>0)*0.225>>+")+name,signaljetCUT+"*"+ptetaselection+"*jet_ncell_dr20");
      TREE->Draw(TString("(jet_pt>0)*0.275>>+")+name,signaljetCUT+"*"+ptetaselection+"*jet_ncell_dr25");
      TREE->Draw(TString("(jet_pt>0)*0.325>>+")+name,signaljetCUT+"*"+ptetaselection+"*jet_ncell_dr30");
      TREE->Draw(TString("(jet_pt>0)*0.375>>+")+name,signaljetCUT+"*"+ptetaselection+"*jet_ncell_dr35");


      TString namePU=TString("HdRPU_eta")+eta+"_pt"+pt;
      HdRPU[eta][pt]=getdRDensityHisto(namePU); 
      TREE->Draw(TString("(jet_pt>0)*0.025>>")+namePU,pujetCUT+"*"+ptetaselection+"*jet_ncell_dr00");
      TREE->Draw(TString("(jet_pt>0)*0.075>>+")+namePU,pujetCUT+"*"+ptetaselection+"*jet_ncell_dr05");
      TREE->Draw(TString("(jet_pt>0)*0.125>>+")+namePU,pujetCUT+"*"+ptetaselection+"*jet_ncell_dr10");
      TREE->Draw(TString("(jet_pt>0)*0.175>>+")+namePU,pujetCUT+"*"+ptetaselection+"*jet_ncell_dr15");
      TREE->Draw(TString("(jet_pt>0)*0.225>>+")+namePU,pujetCUT+"*"+ptetaselection+"*jet_ncell_dr20");
      TREE->Draw(TString("(jet_pt>0)*0.275>>+")+namePU,pujetCUT+"*"+ptetaselection+"*jet_ncell_dr25");
      TREE->Draw(TString("(jet_pt>0)*0.325>>+")+namePU,pujetCUT+"*"+ptetaselection+"*jet_ncell_dr30");
      TREE->Draw(TString("(jet_pt>0)*0.375>>+")+namePU,pujetCUT+"*"+ptetaselection+"*jet_ncell_dr35");


    }
  }


  TLegend legend(0.7,0.8,0.92,0.94);
  legend.SetFillColor(0);
  legend.SetBorderSize(0);
  char legtext[100];


  ///////////////////////////
  //// Number density
  ///////////////////////////
  float yrangemin=1;
  float yrangemax=9000;
  int cntr=0;
  for(long eta=0;eta<neta;eta++){
    C1D.Clear();
    long eta1=minetaidx+eta*etastep;
    long eta2=minetaidx+(eta+1)*etastep;

    for(long pt=0;pt<npt;pt++){
      long pt1=minptidx+pt*ptstep;
      long pt2=minptidx+(pt+1)*ptstep;

      ///// signal
      HdR[eta][pt]->Scale(1./HFillCelldRNJet.GetBinContent(eta+1,pt+1));
      dividedRDensityHisto(HdR[eta][pt]);      
      //HdR[eta][pt]->SetLineColor(ETACOLOR[cntr]);

      //// pile-up
      HdRPU[eta][pt]->Scale(1./HFillCelldRNJetPU.GetBinContent(eta+1,pt+1));
      dividedRDensityHisto(HdRPU[eta][pt]);
      //HdRPU[eta][pt]->SetLineColor(ETACOLOR[cntr]);

      if(cntr==0){
	legend.AddEntry(HdR[eta][pt],"h.s. jet","p");
	legend.AddEntry(HdRPU[eta][pt],"p.u. jet","l");
      }

      HdR[eta][pt]->GetYaxis()->SetRangeUser(yrangemin,yrangemax);
      HdR[eta][pt]->GetYaxis()->SetTitle("cell  density");
      HdR[eta][pt]->Draw("histp");
      HdRPU[eta][pt]->Draw("histsame");

      cntr++;
    }
    
    
    legend.Draw();
    DrawATLAS(0.20,0.85);
    DrawECMSMu(outpath);
        
    text.SetTextSize(0.045);
    sprintf(legtext,"%.0f GeV<p_{T}^{J}<%.0f GeV",(float)minptidx,(float)(minptidx+npt*ptstep));
    text.DrawLatexNDC(0.67,0.7,legtext);
    
    sprintf(legtext,"%.1f<|#eta^{J}|<%.1f",float(eta1)/10,float(eta2)/10.);
    text.DrawLatexNDC(0.67,0.75,legtext);

    C1D.Print(outpath+"/JetCell_dRDensity_"+eta+".png");
    
  }

  

  // ////////////////////////
  // ////////////PU
  // ////////////////////////
  // C1D.Clear();
  // cntr=0;
  // for(long eta=0;eta<neta;eta++){
  //   for(long pt=0;pt<npt;pt++){
  //     HdRPU[eta][pt]->Scale(1./HFillCelldRNJetPU.GetBinContent(eta+1,pt+1));
  //     dividedRDensityHisto(HdRPU[eta][pt]);
  //     HdRPU[eta][pt]->SetLineColor(ETACOLOR[cntr]);
  //     if(cntr==0){
  // 	HdRPU[eta][pt]->GetYaxis()->SetRangeUser(yrangemin,yrangemax);
  // 	HdRPU[eta][pt]->GetYaxis()->SetTitle("cell  density");
  // 	HdRPU[eta][pt]->Draw("hist");
  //     }else HdRPU[eta][pt]->Draw("histsame");
  //     cntr++;
  //   }
  // }

  // legend.Draw();
  // DrawATLAS(0.20,0.85);
  // DrawECMSMu(outpath);

  // text.SetTextSize(0.045);
  // sprintf(legtext,"%.0f GeV<p_{T}^{J}<%.0f GeV",(float)minptidx,(float)(minptidx+npt*ptstep));
  // text.DrawLatexNDC(0.67,0.7,legtext);

  // C1D.Print(outpath+"/JetCell_dRDensity_PU.png");


   
}



void fillJetCelldR(TChain*TREE,TString outpath){
  TString Sel="cell_jet>=0&&jet_truth_pt[cell_jet]>0";
  TString SelPU="cell_jet>=0&&jet_truth_pt[cell_jet]<=0";

  long etastep=2;
  long minetaidx=24;
  long maxetaidx=40;
  long ptstep=20;
  long minptidx=30;
  long maxptidx=70;

  TH2F HFillCelldRNJet("HFillCelldRNJet","",    0.5+(maxetaidx+etastep-minetaidx)/etastep,minetaidx/10.,(maxetaidx+etastep)/10.,0.5+(maxptidx+ptstep-minptidx)/ptstep,minptidx,maxptidx+ptstep);
  
  TH2F HFillCelldRNJetPU("HFillCelldRNJetPU","",0.5+(maxetaidx+etastep-minetaidx)/etastep,minetaidx/10.,(maxetaidx+etastep)/10.,0.5+(maxptidx+ptstep-minptidx)/ptstep,minptidx,maxptidx+ptstep);

  
  if(outpath.Contains("_mu0")){//for mu=0 jet calibration does not apply
    TREE->Draw("jet_truth_pt/1000.:abs(jet_eta)>>HFillCelldRNJet","jet_truth_pt>0");
    TREE->Draw("jet_truth_pt/1000.:abs(jet_eta)>>HFillCelldRNJetPU","jet_truth_pt<=0");
  }else{
    TREE->Draw("jet_pt/1000.:abs(jet_eta)>>HFillCelldRNJet","jet_truth_pt>0");
    TREE->Draw("jet_pt/1000.:abs(jet_eta)>>HFillCelldRNJetPU","jet_truth_pt<=0");
  }


  ////Fill distributions
  TH1F * HdR[50][100];
  TH1F * HdRPU[50][100];

  TH1F * HdREw[50][100];
  TH1F * HdREwPU[50][100];
  
  for(long eta=minetaidx;eta<=maxetaidx;eta+=etastep){
    for(long pt=minptidx;pt<=maxptidx;pt+=ptstep){

      TString ptetaselection=TString("((")+eta+"/10.)<abs(jet_eta[cell_jet])&&abs(jet_eta[cell_jet])<("+(eta+etastep)+"/10.))"+"&&(("+pt+"*1000.)<jet_pt[cell_jet]&&jet_pt[cell_jet]<("+(pt+ptstep)+"*1000.))";

      //for mu=0 jet calibration does not apply
      if(outpath.Contains("_mu0"))
	ptetaselection=TString("((")+eta+"/10.)<abs(jet_eta[cell_jet])&&abs(jet_eta[cell_jet])<("+(eta+etastep)+"/10.))"+"&&(("+pt+"*1000.)<jet_truth_pt[cell_jet]&&jet_truth_pt[cell_jet]<("+(pt+ptstep)+"*1000.))";
      
      
      HdR[eta][pt]=getdRDensityHisto(TString("HdR_eta")+eta+"_pt"+pt); //new TH1F(TString("HdR_eta")+eta+"_pt"+pt,"",10,.0,.5);
      TREE->Draw(TString("cell_jetdR>>HdR_eta")+eta+"_pt"+pt,Sel+"&&"+ptetaselection);
      
      HdRPU[eta][pt]=getdRDensityHisto(TString("HdRPU_eta")+eta+"_pt"+pt); //new TH1F(TString("HdRPU_eta")+eta+"_pt"+pt,"",10,.0,.5);
      TREE->Draw(TString("cell_jetdR>>HdRPU_eta")+eta+"_pt"+pt,SelPU+"&&"+ptetaselection);

      HdREw[eta][pt]=getdRDensityHisto(TString("HdREw_eta")+eta+"_pt"+pt); //new TH1F(TString("HdREw_eta")+eta+"_pt"+pt,"",10,.0,.5);
      TREE->Draw(TString("cell_jetdR>>HdREw_eta")+eta+"_pt"+pt,TString("(")+Sel+"&&"+ptetaselection+")*(cell_E)");
      
      HdREwPU[eta][pt]=getdRDensityHisto(TString("HdREwPU_eta")+eta+"_pt"+pt); //new TH1F(TString("HdREwPU_eta")+eta+"_pt"+pt,"",10,.0,.5);
      TREE->Draw(TString("cell_jetdR>>HdREwPU_eta")+eta+"_pt"+pt,TString("(")+SelPU+"&&"+ptetaselection+")*(cell_E)");
      
    }
  }



  ///Write histograms
  TFile FdR(outpath+"/dRTemplates.root","recreate");
  HFillCelldRNJet.Write();
  HFillCelldRNJetPU.Write();
  for(long pt=minptidx;pt<=maxptidx;pt+=ptstep){
    for(long eta=minetaidx;eta<=maxetaidx;eta+=etastep){
      HdR[eta][pt]->Write();
      HdRPU[eta][pt]->Write();

      HdREw[eta][pt]->Write();
      HdREwPU[eta][pt]->Write();
    }
  }
  FdR.ls();

}



void plotJetHitDensity(TChain*TREE,TString outpath){

  setCuts(outpath);
  
  TString extrasel="(1)";
  //TString extrasel="(abs(jet_dTFitCore_meandt_window45-jet_truedt)*1000>50)";//wrong time jets


  TH1F * G[ETABINS][PTBINS];
  TH1F * GQCD[ETABINS][PTBINS];
  TH1F * GStoc[ETABINS][PTBINS];

  for(long p=0;p<PTBINS;p++){
    TString ptsel=TString("(")+(long)(PTMIN+p*(PTMAX-PTMIN)/PTBINS)+"<jet_pt&&jet_pt<"+(long)(PTMIN+(p+1)*(PTMAX-PTMIN)/PTBINS)+")";
    cout<<p<<" : "<<ptsel<<endl;

  for(long e=0;e<ETABINS;e++){

    TString etasel=TString("(")+(long)(10*ETAMIN+e*10*(ETAMAX-ETAMIN)/ETABINS)+"<jet_eta*10&&jet_eta*10<"+(long)(10*ETAMIN+(e+1)*10*(ETAMAX-ETAMIN)/ETABINS)+")";
    cout<<e<<" : "<<etasel<<endl;


    G[e][p]=getdRDensityHisto(TString("dRDensity")+e+p);
    GQCD[e][p]=getdRDensityHisto(TString("dRDensityQCD")+e+p);
    GStoc[e][p]=getdRDensityHisto(TString("dRDensityStoc")+e+p);

    int counter=0;
    for(long i=0;i<8;i++){
      TString var=TString("jet_ncell_dr0")+(i*5);
      if(i>=2)var=TString("jet_ncell_dr")+(i*5);

      TH1F *HDensitydR=new TH1F(TString("HDensitydR_")+i+"_"+e+"_"+p,"",100,0,5000);
      TREE->Draw(var+">>HDensitydR_"+i+"_"+e+"_"+p,signaljetCUT+"&&"+ptsel+"&&"+etasel+"&&"+extrasel);
      G[e][p]->SetBinContent(i+1,HDensitydR->GetMean());
      G[e][p]->SetBinError(i+1,0.1);
      
      TH1F *HDensitydRQCD=new TH1F(TString("HDensitydRQCD_")+i+"_"+e+"_"+p,"",100,0,5000);
      TREE->Draw(var+">>HDensitydRQCD_"+i+"_"+e+"_"+p,qcdjetCUT+"&&"+ptsel+"&&"+etasel);
      GQCD[e][p]->SetBinContent(i+1,HDensitydRQCD->GetMean());
      GQCD[e][p]->SetBinError(i+1,0.1);

      TH1F *HDensitydRStoc=new TH1F(TString("HDensitydRStoc_")+i+"_"+e+"_"+p,"",100,0,5000);
      TREE->Draw(var+">>HDensitydRStoc_"+i+"_"+e+"_"+p,stocjetCUT+"&&"+ptsel+"&&"+etasel);
      GStoc[e][p]->SetBinContent(i+1,HDensitydRStoc->GetMean());
      GStoc[e][p]->SetBinError(i+1,0.1);

    }

  }
  }

  for(long p=0;p<PTBINS;p++){
  for(long e=0;e<ETABINS;e++){
    dividedRDensityHisto(G[e][p]);
    G[e][p]->SetFillColor(JETTYPECOLOR[0]);
    G[e][p]->SetLineColor(JETTYPECOLOR[0]);
    G[e][p]->SetMarkerColor(JETTYPECOLOR[0]);
    G[e][p]->SetFillStyle(3005);

    dividedRDensityHisto(GQCD[e][p]);
    GQCD[e][p]->SetFillColor(JETTYPECOLOR[1]);
    GQCD[e][p]->SetLineColor(JETTYPECOLOR[1]);
    GQCD[e][p]->SetMarkerColor(JETTYPECOLOR[1]);
    GQCD[e][p]->SetFillStyle(3004);

    dividedRDensityHisto(GStoc[e][p]);
    GStoc[e][p]->SetFillColor(JETTYPECOLOR[2]);
    GStoc[e][p]->SetLineColor(JETTYPECOLOR[2]);
    GStoc[e][p]->SetMarkerColor(JETTYPECOLOR[2]);
    GStoc[e][p]->SetFillStyle(3003);

    TLegend leg(0.2,0.25,0.4,0.45);
    leg.SetFillStyle (0);
    leg.SetFillColor (0);
    leg.SetBorderSize(0);
    leg.AddEntry(G[e][p],"H.S.","lp");
    leg.AddEntry(GQCD[e][p],"QCD","lp");
    leg.AddEntry(GStoc[e][p],"stochastic","lp");
 
    C1D.Clear();
    G[e][p]->GetYaxis()->SetRangeUser(0,outpath.Contains("Preshower")?20000:10000);
    G[e][p]->GetYaxis()->SetTitle("avg. cell density");
    G[e][p]->GetXaxis()->SetTitle("#DeltaR (cell - jet)");
    G[e][p]->Draw("histpe1");
    GQCD[e][p]->Draw("histpe1same");    
    GStoc[e][p]->Draw("histpe1same");    
    G[e][p]->Draw("histpe1same");
    leg.Draw();

    DrawATLAS(0.3,0.89);
    DrawHGTD(outpath,0,-1,0.3,0.83);
    DrawVBFH(outpath);
    DrawECMSMu(outpath);


    text.SetTextSize(0.033);
    char legtext[100];
    sprintf(legtext,"%.1f<|#eta|<%.1f",(ETAMIN+e*(ETAMAX-ETAMIN)/ETABINS),(ETAMIN+(e+1)*(ETAMAX-ETAMIN)/ETABINS));
    text.DrawLatexNDC(0.7,0.80,legtext);
    sprintf(legtext,"%.0f GeV<p_{T}<%.0f GeV",(PTMIN+p*(PTMAX-PTMIN)/PTBINS)/1000.,(PTMIN+(p+1)*(PTMAX-PTMIN)/PTBINS)/1000.);
    text.DrawLatexNDC(0.7,0.75,legtext);

    C1D.Print(outpath+"/JetCellDensity_eta"+e+"_pt"+p+".png");    


  }
  }


}




void plotJetHitDensitySummer2017(TChain*TREE,TString outpath){

  setCuts(outpath);  
  TH1F * G[ETABINS][PTBINS];
  TH1F * GPU[ETABINS][PTBINS];

  for(long p=0;p<PTBINS;p++){
    TString ptsel=TString("(")+(long)(PTMIN+p*(PTMAX-PTMIN)/PTBINS)+"<jet_pt&&jet_pt<"+(long)(PTMIN+(p+1)*(PTMAX-PTMIN)/PTBINS)+")";
    cout<<p<<" : "<<ptsel<<endl;

    for(long e=0;e<ETABINS;e++){
      TString etasel=TString("(")+(long)(10*ETAMIN+e*10*(ETAMAX-ETAMIN)/ETABINS)+"<jet_eta*10&&jet_eta*10<"+(long)(10*ETAMIN+(e+1)*10*(ETAMAX-ETAMIN)/ETABINS)+")";
      cout<<" "<<e<<" : "<<etasel<<endl;
      G[e][p]=getdRDensityHisto(TString("dRDensity")+e+p);
      GPU[e][p]=getdRDensityHisto(TString("dRDensityPU")+e+p);

      int counter=0;
      for(long i=0;i<8;i++){
	TString var=TString("jet_ncell_dr0")+(i*5);
	if(i>=2)var=TString("jet_ncell_dr")+(i*5);

	TH1F *HDensitydR=new TH1F(TString("HDensitydR_")+i+"_"+e+"_"+p,"",100,0,5000);
	TREE->Draw(var+">>HDensitydR_"+i+"_"+e+"_"+p,signaljetCUT+"&&"+ptsel+"&&"+etasel);
	G[e][p]->SetBinContent(i+1,HDensitydR->GetMean());
	G[e][p]->SetBinError(i+1,HDensitydR->GetRMS());
      
	TH1F *HDensitydRPU=new TH1F(TString("HDensitydRPU_")+i+"_"+e+"_"+p,"",100,0,5000);
	TREE->Draw(var+">>HDensitydRPU_"+i+"_"+e+"_"+p,pujetCUT+"&&"+ptsel+"&&"+etasel);
	GPU[e][p]->SetBinContent(i+1,HDensitydRPU->GetMean());
	GPU[e][p]->SetBinError(i+1,HDensitydRPU->GetRMS());
      }
    }
  }

  
  text.SetTextSize(0.033);
  char legtext[100];
  
  for(long e=0;e<ETABINS;e++){
     
    for(long p=0;p<PTBINS;p++){
      C1D.Clear(); 
    
      TLegend leg(0.7,0.75,0.9,0.85);
      leg.SetFillStyle (0);
      leg.SetFillColor (0);
      leg.SetBorderSize(0);
 
      dividedRDensityHisto(G[e][p]);
      G[e][p]->SetFillColor(JETTYPECOLOR[0]);
      G[e][p]->SetLineColor(JETTYPECOLOR[0]);
      G[e][p]->SetMarkerColor(JETTYPECOLOR[0]);
      G[e][p]->SetFillStyle(3005);

      dividedRDensityHisto(GPU[e][p]);
      GPU[e][p]->SetFillColor(JETTYPECOLOR[1]);
      GPU[e][p]->SetLineColor(JETTYPECOLOR[1]);
      GPU[e][p]->SetLineStyle(1);
      GPU[e][p]->SetMarkerColor(JETTYPECOLOR[1]);
      GPU[e][p]->SetFillStyle(3004);

      // sprintf(legtext,"%.0f <p_{T}<%.0f GeV",(PTMIN+p*(PTMAX-PTMIN)/PTBINS)/1000.,(PTMIN+(p+1)*(PTMAX-PTMIN)/PTBINS)/1000.);
      // leg.AddEntry(G[e][p],TString("h.s.,")+legtext,"lp");
      // leg.AddEntry(GPU[e][p],TString("p.u., ")+legtext,"lp");
      
      leg.AddEntry(G[e][p],TString("h.s. jet"),"lp");
      leg.AddEntry(GPU[e][p],TString("p.u. jet"),"lp");
          
      G[e][p]->GetYaxis()->SetRangeUser(0,outpath.Contains("Preshower")?20000:10000);
      G[e][p]->GetYaxis()->SetTitle("# of hits per unit #eta-#phi area ");
      G[e][p]->GetXaxis()->SetTitle("#DeltaR");
      G[e][p]->Draw("histpe2");
      //G[e][p]->Draw("histsame");

      GPU[e][p]->Draw("histpe2same");    
      //GPU[e][p]->Draw("histsame");    

      leg.Draw();
    
      DrawATLAS(0.3,0.89);
      DrawHGTD(outpath,0,-1,0.3,0.83,"1 mm cells");
      DrawVBFH(outpath);
      DrawECMSMu(outpath);
    
      sprintf(legtext,"Anti-k_{t} EM+JES (R=0.4) jets");
      text.DrawLatexNDC(0.2,0.35,legtext);
      sprintf(legtext,"%.1f<|#eta|<%.1f",(ETAMIN+e*(ETAMAX-ETAMIN)/ETABINS),(ETAMIN+(e+1)*(ETAMAX-ETAMIN)/ETABINS));
      text.DrawLatexNDC(0.2,0.30,legtext);
      sprintf(legtext,"%.0f GeV <p_{T}<%.0f GeV",PTMIN/1000.,PTMAX/1000.);
      text.DrawLatexNDC(0.2,0.25,legtext);
    
      C1D.Print(outpath+"/JetCellDensity_eta"+e+"_p"+p+".png");    
      C1D.Print(outpath+"/JetCellDensity_eta"+e+"_p"+p+".pdf");    
      C1D.Print(outpath+"/JetCellDensity_eta"+e+"_p"+p+".eps");    
    
    }
  }


}




void plotJetHitDensityIDRSummer2017(TChain*TREE,TString outpath){

  text.SetTextSize(0.033);
  char legtext[100];

  setCuts(outpath);  

  TH1F * G[ETABINS];
  TH1F * GPU[ETABINS];
  for(long e=0;e<ETABINS;e++){
    
    TString etasel=TString("(")+(long)(10*ETAMIN+e*10*(ETAMAX-ETAMIN)/ETABINS)+"<jet_eta*10&&jet_eta*10<"+(long)(10*ETAMIN+(e+1)*10*(ETAMAX-ETAMIN)/ETABINS)+")";
    cout<<" "<<e<<" : "<<etasel<<endl;
    G[e]=getdRDensityHisto(TString("dRDensity")+e);
    GPU[e]=getdRDensityHisto(TString("dRDensityPU")+e);
    
    int counter=0;
    for(long i=0;i<8;i++){
      TString var=TString("jet_ncell_dr0")+(i*5);
      if(i>=2)var=TString("jet_ncell_dr")+(i*5);
      
      TH1F *HDensitydR=new TH1F(TString("HDensitydR_")+i+"_"+e,"",100,0,5000);
      TREE->Draw(var+">>HDensitydR_"+i+"_"+e,signaljetCUT+"*"+JetPtCut+"*"+etasel);
      G[e]->SetBinContent(i+1,HDensitydR->GetMean());
      G[e]->SetBinError(i+1,HDensitydR->GetRMS());
      
      TH1F *HDensitydRPU=new TH1F(TString("HDensitydRPU_")+i+"_"+e,"",100,0,5000);
      TREE->Draw(var+">>HDensitydRPU_"+i+"_"+e,pujetCUT+"*"+JetPtCut+"*"+etasel);
      GPU[e]->SetBinContent(i+1,HDensitydRPU->GetMean());
      GPU[e]->SetBinError(i+1,HDensitydRPU->GetRMS());

    }



    C1D.Clear(); 
    
    TLegend leg(0.7,0.75,0.9,0.85);
    leg.SetFillStyle (0);
    leg.SetFillColor (0);
    leg.SetBorderSize(0);
 
    dividedRDensityHisto(G[e]);
    G[e]->SetFillColor(JETTYPECOLOR[0]);
    G[e]->SetLineColor(JETTYPECOLOR[0]);
    G[e]->SetMarkerColor(JETTYPECOLOR[0]);
    G[e]->SetFillStyle(3005);

    dividedRDensityHisto(GPU[e]);
    GPU[e]->SetFillColor(JETTYPECOLOR[1]);
    GPU[e]->SetLineColor(JETTYPECOLOR[1]);
    GPU[e]->SetLineStyle(1);
    GPU[e]->SetMarkerColor(JETTYPECOLOR[1]);
    GPU[e]->SetFillStyle(3004);
      
    leg.AddEntry(G[e],TString("h.s. jet"),"lp");
    leg.AddEntry(GPU[e],TString("p.u. jet"),"lp");
          
    G[e]->GetYaxis()->SetRangeUser(0,outpath.Contains("Preshower")?20000:10000);
    G[e]->GetYaxis()->SetTitle("# of hits per unit #eta-#phi area ");
    G[e]->GetXaxis()->SetTitle("#DeltaR");
    G[e]->Draw("histpe2");
    GPU[e]->Draw("histpe2same");    

    leg.Draw();    
    DrawATLAS(0.3,0.89);
    DrawHGTD(outpath,0,-1,0.3,0.83,"1 mm cells");
    DrawVBFH(outpath);
    DrawECMSMu(outpath);
    
    sprintf(legtext,"Anti-k_{t} EM+JES (R=0.4) jets");
    text.DrawLatexNDC(0.2,0.35,legtext);
 
    sprintf(legtext,"%.1f<|#eta|<%.1f",(ETAMIN+e*(ETAMAX-ETAMIN)/ETABINS),(ETAMIN+(e+1)*(ETAMAX-ETAMIN)/ETABINS));
    text.DrawLatexNDC(0.2,0.30,legtext);
 
    sprintf(legtext,"%.0f GeV<p_{T}<%.0f GeV",PTMIN/1000.,PTMAX/1000.);
    text.DrawLatexNDC(0.2,0.25,legtext);
    
    C1D.Print(outpath+"/JetCellDensity_eta"+e+".png");    
    C1D.Print(outpath+"/JetCellDensity_eta"+e+".pdf");    
    C1D.Print(outpath+"/JetCellDensity_eta"+e+".eps");    
    

  }
  
  
}





void plotJetTimingProbScan(TChain*TREE,TString outpath){
  
  float fitmin=-300;
  float fitmax=300;
  int nbins=60;

  TString FitFormula("[0]*exp(-0.5*(x-[1])*(x-[1])/([2]*[2]))");
  TString BkgFormula("[3]*exp(-0.5*(x-[4])*(x-[4])/([5]*[5]))");
  TF1 dTFit("dTFit",FitFormula+"+"+BkgFormula,fitmin,fitmax);
  dTFit.SetParLimits(1,-50,50);
  dTFit.SetParLimits(2,5,100);
  dTFit.SetParLimits(4,-300,300);
  dTFit.SetParLimits(5,100,1000);
  dTFit.SetLineColor(4);
  dTFit.SetNpx(500);
 

  TH1F* HEff[ETABINS];
  TH1F* HMean[ETABINS];
  TH1F* HSigma[ETABINS];
  for(long e=0;e<ETABINS;e++){
    float etalow=ETAMIN+(e)*(ETAMAX-ETAMIN)/ETABINS;
    float etahigh=ETAMIN+(e+1)*(ETAMAX-ETAMIN)/ETABINS;
    
    char cuttxt[100];
    sprintf(cuttxt,"%.2f",etalow);
    etamin=cuttxt;
    sprintf(cuttxt,"%.2f",etahigh);
    etamax=cuttxt;    
    jetPtL=100000;
    jetPtH=1000000;
    setCuts(outpath);

    HEff[e]=new TH1F(TString("HEff")+e,"",120,0,120);
    HMean[e]=new TH1F(TString("HMean")+e,"",120,0,120);
    HSigma[e]=new TH1F(TString("HSigma")+e,"",120,0,120);
 
    for(long W=15;W<=120;W+=15){

      TString dtvariable=TString("jet_dTFitCore_meandt_window")+W;

      TH1F HJetDTMatch("HJetDTMatch","",nbins,fitmin,fitmax);
      HJetDTMatch.GetXaxis()->SetTitle("jet t   -   truth t  [ps]     ");
      HJetDTMatch.GetYaxis()->SetTitle(TString("# of jets / ")+(long)((fitmax-fitmin)/nbins)+" ps");
      TREE->Draw(TString("(")+dtvariable+"-jet_truedt)*1000>>HJetDTMatch",signaljetCUT+"*"+JetEtaCut+"*"+JetPtCut);

      HJetDTMatch.Fit(&dTFit);

      C1D.Clear();
      HJetDTMatch.GetYaxis()->SetRangeUser(0,1.3*HJetDTMatch.GetMaximum());
      HJetDTMatch.Draw("histpe");
      dTFit.Draw("same");

      //show the mistimed jets
      TF1 FitBkg("FitBkg",FitFormula,fitmin,fitmax);
      FitBkg.SetParameter(0,dTFit.GetParameter(3));
      FitBkg.SetParameter(1,dTFit.GetParameter(4));
      FitBkg.SetParameter(2,dTFit.GetParameter(5));
      FitBkg.SetLineStyle(2);
      FitBkg.SetLineColor(1);
      FitBkg.Draw("same");

      text.SetTextSize(0.040);
      text.SetTextColor(1);
      text.DrawLatexNDC(0.7,0.85,"Anti-k_{T} EM+JES R=0.4");
      char txt[100];
      sprintf(txt," p_{T} > %.0f GeV",jetPtL/1000.);
      text.DrawLatexNDC(0.7,0.80,txt);
      sprintf(txt,"%.1f < |#eta| < %.1f",etalow,etahigh);
      text.DrawLatexNDC(0.7,0.75,txt);
      text.SetTextColor(4);
      text.DrawLatexNDC(0.7,0.55,TString("#sigma = ")+(long)(dTFit.GetParameter(2))+" ps");
      //text.DrawLatexNDC(0.7,0.6,TString("fraction = ")+(long)(dTFit.GetParameter(3)*100)+"%");
      DrawATLAS();
      DrawHGTD(outpath,0,-1);
      DrawVBFH(outpath);
      DrawECMSMu(outpath);
      C1D.Print(outpath+"/JetTimingProbScan_Eta"+e+"_Window"+W+".png");


      TF1 FitSignal("FitSignal",FitFormula,fitmin,fitmax);
      FitSignal.SetParameter(0,dTFit.GetParameter(0));
      FitSignal.SetParameter(1,dTFit.GetParameter(1));
      FitSignal.SetParameter(2,dTFit.GetParameter(2));
      for(int t=1;t<=nbins;t++){
	HEff[e]->AddBinContent(W,FitSignal.Eval(HJetDTMatch.GetXaxis()->GetBinCenter(t)));
      }
      HEff[e]->SetBinContent(W,HEff[e]->GetBinContent(W)/HJetDTMatch.Integral(0,nbins+1));
      HEff[e]->SetBinError(W,HEff[e]->GetBinContent(W)*dTFit.GetParError(0)/dTFit.GetParameter(0));

      HMean[e]->SetBinContent(W,dTFit.GetParameter(1));
      HMean[e]->SetBinError(W,dTFit.GetParError(1));

      HSigma[e]->SetBinContent(W,dTFit.GetParameter(2));
      HSigma[e]->SetBinError(W,dTFit.GetParError(2));

    }

    C1D.Clear();
    HEff[e]->GetYaxis()->SetRangeUser(0.5,1);
    HEff[e]->GetYaxis()->SetTitle("efficiency");
    HEff[e]->GetXaxis()->SetTitle("bin size  [ps]");
    HEff[e]->Draw("histpe");
    C1D.Print(outpath+"/JetTimingProbScan_Eta"+e+".png");

    C1D.Clear();
    HMean[e]->GetYaxis()->SetRangeUser(-20,20);
    HMean[e]->GetYaxis()->SetTitle("mean [ps]");
    HMean[e]->GetXaxis()->SetTitle("bin size  [ps]");
    HMean[e]->Draw("histpe");
    C1D.Print(outpath+"/JetTimingProbScanMean_Eta"+e+".png");


    C1D.Clear();
    HSigma[e]->GetYaxis()->SetRangeUser(0,100);
    HSigma[e]->GetYaxis()->SetTitle("sigma  [ps]");
    HSigma[e]->GetXaxis()->SetTitle("bin size  [ps]");
    HSigma[e]->Draw("histpe");
    C1D.Print(outpath+"/JetTimingProbScanSigma_Eta"+e+".png");
  }




}







void plotJetTimingProb(TChain*TREE,TString outpath){
  
  float fitmin=-300;
  float fitmax=300;
  int nbinsLow=60;
  int nbinsHigh=60;


  // ///for 1mm + 3mm cells
  // TString dtvariableLow=TString("jet_dTFitCore_meandt_window90");//optimized with LArHitNewITKJetsApril25/InclBrlNominal_mu200 (60ps 3mm cells) 
  // TString dtvariableHigh=TString("jet_dTFitCore_meandt_window45");//optimized with LArHitNewITKJetsApril25/InclBrlNominal_mu200 (30ps 1mm cells) 
 
  ///for 1mm cells
  TString dtvariableLow=TString("jet_dTFitCore_meandt_window30");//optimized with LArHitNewITKJetsJune12/InclBrlNominal_mu200 
  TString dtvariableHigh=TString("jet_dTFitCore_meandt_window30");//optimized with LArHitNewITKJetsJune12/InclBrlNominal_mu200 

  

  ///////////////////////////////////////////////////////
  /// double Gauss fit
  TString FitFormula("[0]*exp(-0.5*(x-[1])*(x-[1])/([2]*[2]))");
  TString BkgFormula("[3]*exp(-0.5*(x-[4])*(x-[4])/([5]*[5]))");
  TF1 dTFit("dTFit",FitFormula+"+"+BkgFormula,fitmin,fitmax);
  dTFit.SetParLimits(1,-50,50);
  dTFit.SetParLimits(2,5,100);
  dTFit.SetParLimits(4,-300,300);
  dTFit.SetParLimits(5,100,1000);
  dTFit.SetLineColor(4);
  dTFit.SetNpx(500);
 
  ////////////////////////////////////////////////
  ///vs pT
  /////////////////////////////////////////////////
  TH1F* HEff[ETABINS];
  for(long e=0;e<ETABINS;e++){
    float etalow=ETAMIN+(e)*(ETAMAX-ETAMIN)/ETABINS;
    float etahigh=ETAMIN+(e+1)*(ETAMAX-ETAMIN)/ETABINS;

    int ntbins= e==0 ? nbinsLow : nbinsHigh;
    
    char cuttxt[100];
    sprintf(cuttxt,"%.2f",etalow);
    etamin=cuttxt;
    sprintf(cuttxt,"%.2f",etahigh);
    etamax=cuttxt;    
    jetPtL=PTMIN;
    jetPtH=PTMAX;
    setCuts(outpath);

    TH1F HJetDTMatch("HJetDTMatch","",ntbins,fitmin,fitmax);
    HJetDTMatch.GetXaxis()->SetTitle("jet t   -   truth t  [ps]     ");
    HJetDTMatch.GetYaxis()->SetTitle(TString("# of jets / ")+(long)((fitmax-fitmin)/ntbins)+" ps");
    TREE->Draw(TString("(")+(e==0?dtvariableLow:dtvariableHigh)+"-jet_truedt)*1000>>HJetDTMatch",signaljetCUT+"*"+JetEtaCut+"*"+JetPtCut);

    HJetDTMatch.Fit(&dTFit);
    C1D.Clear();
    HJetDTMatch.GetYaxis()->SetRangeUser(0,1.3*HJetDTMatch.GetMaximum());
    HJetDTMatch.Draw("histpe");
    dTFit.Draw("same");

    //show the mistimed jets
    TF1 FitBkg("FitBkg",FitFormula,fitmin,fitmax);
    FitBkg.SetParameter(0,dTFit.GetParameter(3));
    FitBkg.SetParameter(1,dTFit.GetParameter(4));
    FitBkg.SetParameter(2,dTFit.GetParameter(5));
    FitBkg.SetLineStyle(2);
    FitBkg.SetLineColor(1);
    FitBkg.Draw("same");

    text.SetTextSize(0.040);
    text.SetTextColor(1);
    text.DrawLatexNDC(0.7,0.85,"Anti-k_{T} EM+JES R=0.4");
    char txt[100];
    //sprintf(txt," p_{T} > %.0f GeV",jetPtL/1000.);
    sprintf(txt,"%.0f GeV< p_{T} < %.0f GeV",jetPtL/1000.,jetPtH/1000.);
    text.DrawLatexNDC(0.7,0.80,txt);
    sprintf(txt,"%.1f < |#eta| < %.1f",etalow,etahigh);
    text.DrawLatexNDC(0.7,0.75,txt);
    text.SetTextColor(4);
    text.DrawLatexNDC(0.7,0.55,TString("#sigma = ")+(long)(dTFit.GetParameter(2))+" ps");
    //text.DrawLatexNDC(0.7,0.6,TString("fraction = ")+(long)(dTFit.GetParameter(3)*100)+"%");
    DrawATLAS();
    //DrawHGTD(outpath,0,-1);
    DrawHGTD(outpath,0,-1,0.20,0.82,"1 mm cells"); 
    DrawVBFH(outpath);
    DrawECMSMu(outpath);
    C1D.Print(outpath+"/JetTiming_"+e+".png");
    C1D.Print(outpath+"/JetTiming_"+e+".pdf");
    C1D.Print(outpath+"/JetTiming_"+e+".eps");

    HEff[e]=new TH1F(TString("HEff")+e,"",NJETTIMEPTBINS,ptbinning);
    for(long p=0;p<NJETTIMEPTBINS;p++){
      jetPtL=ptbinning[p]*1000;
      jetPtH=ptbinning[p+1]*1000;  
      setCuts(outpath);

      TH1F HJetDTMatchVsPt("HJetDTMatchVsPt","",ntbins,fitmin,fitmax);
      HJetDTMatchVsPt.GetXaxis()->SetTitle("jet t   -   truth t  [ps]     ");
      HJetDTMatchVsPt.GetYaxis()->SetTitle("# of jets");
      TREE->Draw(TString("(")+(e==0?dtvariableLow:dtvariableHigh)+"-jet_truedt)*1000>>HJetDTMatchVsPt",signaljetCUT+"*"+JetEtaCut+"*"+JetPtCut);
     
      if(HJetDTMatchVsPt.Integral()>0){
	HJetDTMatchVsPt.Fit(&dTFit);
	C1D.Clear();
	HJetDTMatchVsPt.GetYaxis()->SetRangeUser(0,1.3*HJetDTMatchVsPt.GetMaximum());
	HJetDTMatchVsPt.Draw("histpe");
	dTFit.Draw("same");

	text.SetTextSize(0.040);
	text.SetTextColor(1);
	text.DrawLatexNDC(0.7,0.85,"Anti-k_{T} EM+JES R=0.4");

	char txt[100];
	sprintf(txt,"%.0f GeV < p_{T} < %.0f GeV",jetPtL/1000.,jetPtH/1000.);
	text.DrawLatexNDC(0.7,0.80,txt);
	sprintf(txt,"%.1f < |#eta| < %.1f",etalow,etahigh);
	text.DrawLatexNDC(0.7,0.75,txt);

	text.SetTextColor(4);
	text.DrawLatexNDC(0.7,0.55,TString("#sigma = ")+(long)(dTFit.GetParameter(2))+" ps");
	//text.DrawLatexNDC(0.7,0.6,TString("fraction = ")+(long)(dTFit.GetParameter(3)*100)+"%");

	DrawATLAS();
	DrawHGTD(outpath,0,0);
	DrawVBFH(outpath);
	DrawECMSMu(outpath);
	C1D.Print(outpath+"/JetTiming_"+e+"_"+p+".png");


	TF1 FitSignal("FitSignal",FitFormula,fitmin,fitmax);
	FitSignal.SetParameter(0,dTFit.GetParameter(0));
	FitSignal.SetParameter(1,dTFit.GetParameter(1));
	FitSignal.SetParameter(2,dTFit.GetParameter(2));
	for(int t=1;t<=ntbins;t++){
	  HEff[e]->AddBinContent(p+1,FitSignal.Eval(HJetDTMatchVsPt.GetXaxis()->GetBinCenter(t)));
	}
	HEff[e]->SetBinContent(p+1,HEff[e]->GetBinContent(p+1)/HJetDTMatchVsPt.Integral(0,ntbins+1));
	HEff[e]->SetBinError(p+1,HEff[e]->GetBinContent(p+1)*dTFit.GetParError(0)/dTFit.GetParameter(0));
      }
      
    }
  }


  TLegend leg(0.7,0.25,0.9,0.35);
  leg.SetFillColor(0);
  leg.SetLineColor(0);
  leg.SetBorderSize(0);

  TH1F HEffFrame("HEffFrame","",NJETTIMEPTBINS,ptbinning);
  HEffFrame.GetXaxis()->SetTitle("jet p_{T}   [GeV]");
  HEffFrame.GetYaxis()->SetRangeUser(0.,1.2);
  HEffFrame.GetYaxis()->SetTitle("Efficiency");
  C1D.Clear();
  HEffFrame.Draw();
  for(long e=0;e<ETABINS;e++){
    float etalow=ETAMIN+(e)*(ETAMAX-ETAMIN)/ETABINS;
    float etahigh=ETAMIN+(e+1)*(ETAMAX-ETAMIN)/ETABINS;
    char txt[100];
    sprintf(txt,"%.1f < |#eta| < %.1f",etalow,etahigh);
    
    HEff[e]->SetLineColor(ETACOLOR[e]);
    HEff[e]->SetMarkerColor(ETACOLOR[e]);
    HEff[e]->Draw("histpesame");
    leg.AddEntry(HEff[e],txt,"lp");
  }
  
  leg.Draw();

  text.SetTextSize(0.040);
  text.SetTextColor(1);
  text.DrawLatexNDC(0.7,0.40,"Anti-k_{T} EM+JES R=0.4");

  DrawATLAS();
  //DrawHGTD(outpath,0,-1);
  DrawHGTD(outpath,0,-1,0.20,0.82,"1 mm cells"); 
  DrawVBFH(outpath);
  DrawECMSMu(outpath);
  C1D.Print(outpath+"/JetTiming_EffVspT.png");
  C1D.Print(outpath+"/JetTiming_EffVspT.pdf");
  C1D.Print(outpath+"/JetTiming_EffVspT.eps");


}




//////////////////////////////////////////////////////
/////Jet clusters
//////////////////////////////////////////////////////
void plotJetClusters(TChain*TREE,TString outpath){
 

  C1D.Clear();
  TH1F HClusterN("HClusterN","",30,0,30);
  HClusterN.GetXaxis()->SetTitle("Number of clusters per jet");
  TREE->Draw("jet_nclus>>HClusterN");
  HClusterN.Draw("hist");
  C1D.Print(outpath+"/JetClusterN.png");

  C1D.Clear();
  TH1F HClusterE("HClusterE","",100,0,1000);
  HClusterE.GetXaxis()->SetTitle("cluster energy [GeV]");
  TREE->Draw("clus_E/1000>>HClusterE");
  HClusterE.Draw("hist");
  C1D.Print(outpath+"/JetClusterE.png");

  C1D.Clear();
  TH1F HClusterEta("HClusterEta","",100,-5,5);
  HClusterEta.GetXaxis()->SetTitle("cluster eta");
  TREE->Draw("clus_eta>>HClusterEta");
  HClusterEta.Draw("hist");
  C1D.Print(outpath+"/JetClusterEta.png");


  //Number of clusters vs eta
  TH2F HJetNclusVsEta("HJetNclusVsEta","",30,2,5,30,-0.5,29.5);
  HJetNclusVsEta.GetXaxis()->SetTitle("Jet eta");
  HJetNclusVsEta.GetYaxis()->SetTitle("# of clusters");
  TREE->Draw("jet_nclus:jet_eta>>HJetNclusVsEta","jet_truth_pt>0");
  C2D.cd();
  C2D.Clear();
  HJetNclusVsEta.Draw("colz");
  C2D.Print(outpath+"/JetNclusVsEta.png");
  
  ////separate EM vs HAD clusters
  TH1F HJetClusEMProb("HJetClusEMProb","",20,0,1);
  HJetClusEMProb.GetXaxis()->SetTitle("jet cluster EM prob.");
  TREE->Draw("clus_emprob>>HJetClusEMProb","clus_jet>=0&&jet_truth_pt[clus_jet]>0");
  C1D.Clear();
  HJetClusEMProb.Draw("hist");
  C1D.Print(outpath+"/JetClusEMProb.png");

  TH1F HJetClusLambda("HJetClusLambda","",50,0,2000);
  HJetClusLambda.GetXaxis()->SetTitle("jet cluster pentration depth");
  TREE->Draw("clus_lambda>>HJetClusLambda","clus_jet>=0&&jet_truth_pt[clus_jet]>0");
  C1D.Clear();
  HJetClusLambda.Draw("hist");
  C1D.Print(outpath+"/JetClusLambda.png");

  TH2F HClusLambdaVsE("HClusLambdaVsE","",100,0,1000,50,0,2000);
  HClusLambdaVsE.GetXaxis()->SetTitle("jet cluster energy  [GeV]");
  HClusLambdaVsE.GetYaxis()->SetTitle("cluster penetration depth");
  HClusLambdaVsE.GetZaxis()->SetTitle("# of clusters");
  TREE->Draw("clus_lambda:clus_E/1000.>>HClusLambdaVsE","clus_jet>=0&&jet_truth_pt[clus_jet]>0");
  C2D.Clear();
  HClusLambdaVsE.Draw("histcolz");
  C2D.Print(outpath+"/JetClusLambdaVsE.png");

  ////Compare EM clusters
  TH1F HJetClusEMLambda("HJetClusEMLambda","",50,0,2000);
  TREE->Draw("clus_lambda>>HJetClusEMLambda","clus_jet>=0&&jet_truth_pt[clus_jet]>0&&clus_emprob>0.5");
  C1D.Clear();
  HJetClusLambda.Draw("hist");
  HJetClusEMLambda.SetLineColor(4);
  HJetClusEMLambda.Draw("histsame");
  C1D.Print(outpath+"/JetClusLambda_EM.png");


  TH1F HJetClusEMFrac("HJetClusEMFrac","",100,0,1);
  HJetClusEMFrac.GetXaxis()->SetTitle("jet cluster EM frac.");
  TREE->Draw("clus_emfrac>>HJetClusEMFrac","clus_jet>=0&&jet_truth_pt[clus_jet]>0");
  C1D.Clear();
  HJetClusEMFrac.Draw("hist");
  C1D.Print(outpath+"/JetClusEMFrac.png");



  ///////breakdown of the cluster energy in dR bins
  int nbins=50;
  float min=0;
  float max=1000;
  C1D.SetLogy(1);

  TString xtitle="#SigmaE_{clus}   [GeV]";
  TH1F HJetClusEdR20("HJetClusEdR20","",nbins,min,max);
  HJetClusEdR20.GetXaxis()->SetTitle(xtitle);
  TREE->Draw("(jet_clusE_dr20+jet_clusE_dr25)/1000>>HJetClusEdR20",signaljetCUT);
  HJetClusEdR20.Scale(1./HJetClusEdR20.Integral());
  TH1F HJetClusEdR30("HJetClusEdR30","",nbins,min,max);
  TREE->Draw("(jet_clusE_dr30+jet_clusE_dr35)/1000>>HJetClusEdR30",signaljetCUT);
  HJetClusEdR30.Scale(1./HJetClusEdR30.Integral());
  C1D.Clear();
  HJetClusEdR20.GetYaxis()->SetTitle("probability");
  HJetClusEdR20.Draw("hist");
  HJetClusEdR30.SetLineColor(4);
  HJetClusEdR30.Draw("histsame");
  C1D.Print(outpath+"/JetClusEdR_Sig.png");

  TH1F HJetClusEdR20PU("HJetClusEdR20PU","",nbins,min,max);
  HJetClusEdR20PU.GetXaxis()->SetTitle(xtitle);
  TREE->Draw("(jet_clusE_dr20+jet_clusE_dr25)/1000>>HJetClusEdR20PU",pujetCUT);
  HJetClusEdR20PU.Scale(1./HJetClusEdR20PU.Integral());
  TH1F HJetClusEdR30PU("HJetClusEdR30PU","",nbins,min,max);
  TREE->Draw("(jet_clusE_dr30+jet_clusE_dr35)/1000>>HJetClusEdR30PU",pujetCUT);
  HJetClusEdR30PU.Scale(1./HJetClusEdR30PU.Integral());
  C1D.Clear();
  HJetClusEdR20PU.GetYaxis()->SetTitle("probability");
  HJetClusEdR20PU.Draw("hist");
  HJetClusEdR30PU.SetLineColor(4);
  HJetClusEdR30PU.Draw("histsame");
  C1D.Print(outpath+"/JetClusEdR_PU.png");

  C1D.SetLogy(0);

  TH2F HJetClusEdRCorr("HJetClusEdRCorr","",2,-0.5,1.5,2,-0.5,1.5);
  HJetClusEdRCorr.GetZaxis()->SetTitle("% of jets");
  HJetClusEdRCorr.GetXaxis()->SetTitle("E_{clus}(0.2-0.3)  >  0");
  HJetClusEdRCorr.GetYaxis()->SetTitle("E_{clus}(0.3-0.4)  >  0");
  HJetClusEdRCorr.GetXaxis()->SetNdivisions(2);
  HJetClusEdRCorr.GetYaxis()->SetNdivisions(2);
  HJetClusEdRCorr.SetMarkerSize(2);
  TREE->Draw("((jet_clusE_dr30+jet_clusE_dr35)>0):((jet_clusE_dr20+jet_clusE_dr25)>0)>>HJetClusEdRCorr",signaljetCUT);
  HJetClusEdRCorr.Scale(1./HJetClusEdRCorr.Integral());
  C2D.Clear();
  HJetClusEdRCorr.Draw("histtext");
  C2D.Print(outpath+"/JetClusEdRCorrelation_Sig.png");


  TH2F HJetClusEdRCorrPU("HJetClusEdRCorrPU","",2,-0.5,1.5,2,-0.5,1.5);
  HJetClusEdRCorrPU.GetZaxis()->SetTitle("% of jets");
  HJetClusEdRCorrPU.GetXaxis()->SetTitle("E_{clus}(0.2-0.3)  >  0");
  HJetClusEdRCorrPU.GetYaxis()->SetTitle("E_{clus}(0.3-0.4)  >  0");
  HJetClusEdRCorrPU.GetXaxis()->SetNdivisions(2);
  HJetClusEdRCorrPU.GetYaxis()->SetNdivisions(2);
  HJetClusEdRCorrPU.SetMarkerSize(2);
  TREE->Draw("((jet_clusE_dr30+jet_clusE_dr35)>0):((jet_clusE_dr20+jet_clusE_dr25)>0)>>HJetClusEdRCorrPU",pujetCUT);
  HJetClusEdRCorrPU.Scale(1./HJetClusEdRCorrPU.Integral());
  C2D.Clear();
  HJetClusEdRCorrPU.Draw("histtext");
  C2D.Print(outpath+"/JetClusEdRCorrelation_PU.png");

}



void plotJetClusdR(TChain*TREE,TString outpath){

  TString Sel="clus_jet>=0&&jet_truth_pt[clus_jet]>0";
  TString SelPU="clus_jet>=0&&jet_truth_pt[clus_jet]<=0";

  //EM clusters
  //TString Sel="clus_lambda<300&&clus_jet>=0&&jet_truth_pt[clus_jet]>0";
  //TString SelPU="clus_lambda<300&&clus_jet>=0&&jet_truth_pt[clus_jet]<=0";



  long neta=3;
  long etastep=4;
  long minetaidx=26;
  long maxetaidx=34;
  long npt=1;
  long ptstep=40;
  long minptidx=30;
  long maxptidx=30;

  //TH2F HFillClusdRNJet("HFillClusdRNJet","",0.5+(maxetaidx+etastep-minetaidx)/etastep,minetaidx/10.,(maxetaidx+etastep)/10.,0.5+(maxptidx+ptstep-minptidx)/ptstep,minptidx,maxptidx+ptstep);
  TH2F HFillClusdRNJet("HFillClusdRNJet","",neta,minetaidx,minetaidx+neta*etastep,npt,minptidx,minptidx+npt*ptstep);  
  TH2F HFillClusdRNJetPU("HFillClusdRNJetPU","",neta,minetaidx,minetaidx+neta*etastep,npt,minptidx,minptidx+npt*ptstep);

  
  if(outpath.Contains("_mu0")){//for mu=0 jet calibration does not apply
    TREE->Draw("jet_truth_pt/1000.:abs(10*jet_eta)>>HFillClusdRNJet","jet_truth_pt>0");
    TREE->Draw("jet_truth_pt/1000.:abs(10*jet_eta)>>HFillClusdRNJetPU","jet_truth_pt<=0");
  }else{
    TREE->Draw("jet_pt/1000.:abs(10*jet_eta)>>HFillClusdRNJet","jet_truth_pt>0");
    TREE->Draw("jet_pt/1000.:abs(10*jet_eta)>>HFillClusdRNJetPU","jet_truth_pt<=0");
  }


  ////Fill distributions
  TH1F * HdR[50][100];
  TH1F * HdRPU[50][100];

  TH1F * HdREw[50][100];
  TH1F * HdREwPU[50][100];

  // for(long eta=minetaidx;eta<=maxetaidx;eta+=etastep){
  //   for(long pt=minptidx;pt<=maxptidx;pt+=ptstep){
   
  for(long eta=0;eta<neta;eta++){
    for(long pt=0;pt<npt;pt++){
      long eta1=minetaidx+eta*etastep;
      long eta2=minetaidx+(eta+1)*etastep;
      long pt1=minptidx+pt*ptstep;
      long pt2=minptidx+(pt+1)*ptstep;

      TString ptetaselection=TString("((")+eta1+"/10.)<abs(jet_eta[clus_jet])&&abs(jet_eta[clus_jet])<("+eta2+"/10.))"+"&&(("+pt1+"*1000.)<jet_pt[clus_jet]&&jet_pt[clus_jet]<("+pt2+"*1000.))";
      
      //for mu=0 jet calibration does not apply
      if(outpath.Contains("_mu0"))
	ptetaselection=TString("((")+eta1+"/10.)<abs(jet_eta[clus_jet])&&abs(jet_eta[clus_jet])<("+eta2+"/10.))"+"&&(("+pt1+"*1000.)<jet_truth_pt[clus_jet]&&jet_truth_pt[clus_jet]<("+pt2+"*1000.))";
      
      
      HdR[eta][pt]=getdRDensityHisto(TString("HdR_eta")+eta+"_pt"+pt); 
      TREE->Draw(TString("clus_jetdR>>HdR_eta")+eta+"_pt"+pt,Sel+"&&"+ptetaselection);
      
      HdRPU[eta][pt]=getdRDensityHisto(TString("HdRPU_eta")+eta+"_pt"+pt); 
      TREE->Draw(TString("clus_jetdR>>HdRPU_eta")+eta+"_pt"+pt,SelPU+"&&"+ptetaselection);


      HdREw[eta][pt]=getdRDensityHisto(TString("HdREw_eta")+eta+"_pt"+pt); 
      TREE->Draw(TString("clus_jetdR>>HdREw_eta")+eta+"_pt"+pt,TString("(")+Sel+"&&"+ptetaselection+")*(clus_E/1000)");

      HdREwPU[eta][pt]=getdRDensityHisto(TString("HdREwPU_eta")+eta+"_pt"+pt);
      TREE->Draw(TString("clus_jetdR>>HdREwPU_eta")+eta+"_pt"+pt,TString("(")+SelPU+"&&"+ptetaselection+")*(clus_E/1000)");


    }
  }


  TLegend legend(0.7,0.74,0.92,0.94);
  legend.SetFillColor(0);
  legend.SetBorderSize(0);
  char legtext[100];


  ///////////////////////////
  //// Number density
  ///////////////////////////
  float yrangemin=1;
  float yrangemax=1000;
  C1D.SetLogy(1);
  C1D.Clear();
  int cntr=0;
  for(long eta=0;eta<neta;eta++){
    for(long pt=0;pt<npt;pt++){
      long eta1=minetaidx+eta*etastep;
      long eta2=minetaidx+(eta+1)*etastep;
      long pt1=minptidx+pt*ptstep;
      long pt2=minptidx+(pt+1)*ptstep;

      HdR[eta][pt]->Scale(1./HFillClusdRNJet.GetBinContent(eta+1,pt+1));
      dividedRDensityHisto(HdR[eta][pt]);
      
      HdR[eta][pt]->SetLineColor(ETACOLOR[cntr]);

      sprintf(legtext,"%.1f<|#eta^{J}|<%.1f",float(eta1)/10,float(eta2)/10.);
      legend.AddEntry(HdR[eta][pt],legtext,"l");

      if(cntr==0){
	//HdR[eta][pt]->GetYaxis()->SetRangeUser(1,HdR[eta][pt]->GetMaximum()*100);
	HdR[eta][pt]->GetYaxis()->SetRangeUser(yrangemin,yrangemax);
	HdR[eta][pt]->GetYaxis()->SetTitle("cluster  density");
	HdR[eta][pt]->Draw("hist");
      }else HdR[eta][pt]->Draw("histsame");
      cntr++;
    }
  }

  
  legend.Draw();
  DrawATLAS(0.20,0.85);
  DrawECMSMu(outpath);

  text.SetTextSize(0.045);
  sprintf(legtext,"%.0f GeV<p_{T}^{J}<%.0f GeV",(float)minptidx,(float)(minptidx+npt*ptstep));
  text.DrawLatexNDC(0.67,0.7,legtext);

  C1D.Print(outpath+"/JetClus_dRDensity_Sig.png");


  C1D.Clear();
  cntr=0;
  for(long eta=0;eta<neta;eta++){
    for(long pt=0;pt<npt;pt++){
      HdRPU[eta][pt]->Scale(1./HFillClusdRNJetPU.GetBinContent(eta+1,pt+1));
      dividedRDensityHisto(HdRPU[eta][pt]);
      HdRPU[eta][pt]->SetLineColor(ETACOLOR[cntr]);
      if(cntr==0){
	HdRPU[eta][pt]->GetYaxis()->SetRangeUser(yrangemin,yrangemax);
	HdRPU[eta][pt]->GetYaxis()->SetTitle("cluster  density");
	HdRPU[eta][pt]->Draw("hist");
      }else HdRPU[eta][pt]->Draw("histsame");
      cntr++;
    }
  }

  legend.Draw();
  DrawATLAS(0.20,0.85);
  DrawECMSMu(outpath);

  text.SetTextSize(0.045);
  sprintf(legtext,"%.0f GeV<p_{T}^{J}<%.0f GeV",(float)minptidx,(float)(minptidx+npt*ptstep));
  text.DrawLatexNDC(0.67,0.7,legtext);

  C1D.Print(outpath+"/JetClus_dRDensity_PU.png");




  
  //////////////////////////////
  ////Energy density
  ///////////////////
  float yrangeminE=10;
  float yrangemaxE=1000000;
  C1D.Clear();
  cntr=0;
  for(long eta=0;eta<neta;eta++){
    for(long pt=0;pt<npt;pt++){
      HdREw[eta][pt]->Scale(1./HFillClusdRNJet.GetBinContent(eta+1,pt+1));
      dividedRDensityHisto(HdREw[eta][pt]);
      HdREw[eta][pt]->SetLineColor(ETACOLOR[cntr]);
      if(cntr==0){
	HdREw[eta][pt]->GetYaxis()->SetRangeUser(yrangeminE,yrangemaxE);
	HdREw[eta][pt]->GetYaxis()->SetTitle("cluster energy density  [GeV]");
	HdREw[eta][pt]->Draw("hist");
      }else HdREw[eta][pt]->Draw("histsame");
      cntr++;
    }
  }

  legend.Draw();
  DrawATLAS(0.20,0.85);
  DrawECMSMu(outpath);

  text.SetTextSize(0.045);
  sprintf(legtext,"%.0f GeV<p_{T}^{J}<%.0f GeV",(float)minptidx,(float)(minptidx+npt*ptstep));
  text.DrawLatexNDC(0.67,0.7,legtext);

  C1D.Print(outpath+"/JetClus_dRDensityEw_Sig.png");


  C1D.Clear();
  cntr=0;
  for(long eta=0;eta<neta;eta++){
    for(long pt=0;pt<npt;pt++){
      HdREwPU[eta][pt]->Scale(1./HFillClusdRNJetPU.GetBinContent(eta+1,pt+1));
      dividedRDensityHisto(HdREwPU[eta][pt]);
      HdREwPU[eta][pt]->SetLineColor(ETACOLOR[cntr]);
      if(cntr==0){
	HdREwPU[eta][pt]->GetYaxis()->SetRangeUser(yrangeminE,yrangemaxE);
	HdREwPU[eta][pt]->GetYaxis()->SetTitle("cluster energy density  [GeV]");
	HdREwPU[eta][pt]->Draw("hist");
      }else HdREwPU[eta][pt]->Draw("histsame");
      cntr++;
    }
  }

  legend.Draw();
  DrawATLAS(0.20,0.85);
  DrawECMSMu(outpath);

  text.SetTextSize(0.045);
  sprintf(legtext,"%.0f GeV<p_{T}^{J}<%.0f GeV",(float)minptidx,(float)(minptidx+npt*ptstep));
  text.DrawLatexNDC(0.67,0.7,legtext);

  C1D.Print(outpath+"/JetClus_dRDensityEw_PU.png");


  C1D.SetLogy(0);




  //////////Compare Sig and PU shapes  
  C1D.Clear();
  cntr=0;
  for(long eta=0;eta<neta;eta++){
    for(long pt=0;pt<npt;pt++){
      HdREwPU[eta][pt]->Divide(HdREw[eta][pt]);
      HdREwPU[eta][pt]->SetMarkerColor(ETACOLOR[cntr]);
      if(cntr==0){
	HdREwPU[eta][pt]->GetYaxis()->SetRangeUser(0,3.5);
	HdREwPU[eta][pt]->GetYaxis()->SetTitle("ratio");
	HdREwPU[eta][pt]->Draw("histpe");
      }else HdREwPU[eta][pt]->Draw("histpesame");
      cntr++;
    }
  }

  legend.Draw();
  DrawATLAS(0.20,0.85);
  DrawECMSMu(outpath);

  text.SetTextSize(0.045);
  sprintf(legtext,"%.0f GeV<p_{T}^{J}<%.0f GeV",(float)minptidx,(float)(minptidx+npt*ptstep));
  text.DrawLatexNDC(0.67,0.7,legtext);
  C1D.Print(outpath+"/JetClus_dRDensityEw_SigPURatio.png");


}



void plotJetTrks(TChain*TREE,TString outpath){
  TString JetSel="(jet_truth_pt>0&&abs(jet_eta)>2.8&&abs(jet_eta)<3.8&&jet_pt<70000)";
  TString JetTrkSel="(trk_ncell>0&&trk_jet>=0&&jet_truth_pt[trk_jet]>0&&abs(jet_eta[trk_jet])>2.8&&abs(jet_eta[trk_jet])<3.8&&jet_pt[trk_jet]<70000)";

  C1D.Clear();
  TH1F HJetNtrk("HJetNtrk","",50,0,50);
  HJetNtrk.GetXaxis()->SetTitle("Number of trks in jet");
  TREE->Draw("jet_ntrk>>HJetNtrk",JetSel);
  HJetNtrk.Draw("hist");
  C1D.Print(outpath+"/JetNtrk.png");

  C1D.Clear();
  TH1F HJetLTrkPt("HJetLTrkPt","",50,0,50);
  HJetLTrkPt.GetXaxis()->SetTitle("jet lead track  pT  [GeV]");
  TREE->Draw("trk_pt[jet_ltrk[trk_jet]]/1000>>HJetLTrkPt",JetTrkSel);
  HJetLTrkPt.Draw("hist");
  C1D.Print(outpath+"/JetLTrkPt.png");

  C1D.Clear();
  TH1F HJetLTrkdR("HJetLTrkdR","",50,0.,0.5);
  HJetLTrkdR.GetXaxis()->SetTitle("jet lead track  #DeltaR");
  TREE->Draw("trk_jetdR[jet_ltrk[trk_jet]]>>HJetLTrkdR",JetTrkSel);
  HJetLTrkdR.Draw("hist");
  C1D.Print(outpath+"/JetLTrkdR.png");


  /////////////////
  ///loop over jets, plot leading track dt 
  ////////////////
  TF1 FGauss("FGaus","[0]*exp(-0.5*(x-[1])**2/[2]**2)",-500,500);
  FGauss.SetParameters(0,100); 
  FGauss.SetParameters(1,1);  
  FGauss.SetParameters(2,100); 
  FGauss.SetParLimits(2,5,400);
  FGauss.SetLineColor(4);

  C1D.Clear();
  TH1F HJetLTrkdt("HJetLTrkdt","",120,-600,600);
  HJetLTrkdt.GetXaxis()->SetTitle(" track  t  - t_{exp}   [ps] ");
  TREE->Draw("1000*trk_dt>>HJetLTrkdt",JetTrkSel+"&&(trk_pt==trk_pt[jet_ltrk[trk_jet]])");
  HJetLTrkdt.Draw("hist");
  HJetLTrkdt.Fit(&FGauss);
  FGauss.Draw("same");
  char txt[100];
  sprintf(txt,FitGausTextSigma,FGauss.GetParameter(2));
  text.DrawLatexNDC(0.7,0.8,txt);
  C1D.Print(outpath+"/JetLTrkdt.png");


  //////////////////////////////////
  ///// loop over tracks which have a jet
  ///// plot diff in dt w.r.t leading trk  
  C1D.Clear();
  TH1F HJetdTTrksLTrk("HJetdTTrksLTrk","",120,-600,600);
  HJetdTTrksLTrk.GetXaxis()->SetTitle("track #Delta t - lead track #Delta t   [ps] ");
  TREE->Draw("1000*(trk_dt-trk_dt[jet_ltrk[trk_jet]])>>HJetdTTrksLTrk",JetTrkSel+"&&(trk_pt!=trk_pt[jet_ltrk[trk_jet]])");
  HJetdTTrksLTrk.Draw("hist");
  fitHist2Gaus(&HJetdTTrksLTrk,-600,600,0.7,0.8);
  C1D.Print(outpath+"/JetdTTrksLTrk.png");


  /////////////////////////
  /////jets at low and high eta 
  ///////////////////////////
  C1D.Clear();
  TH1F HJetdTTrksLTrkEtaLow("HJetdTTrksLTrkEtaLow","",120,-600,600);
  HJetdTTrksLTrkEtaLow.GetXaxis()->SetTitle("track #Delta t - lead track #Delta t   [ps] ");
  TREE->Draw("1000*(trk_dt-trk_dt[jet_ltrk[trk_jet]])>>HJetdTTrksLTrkEtaLow",JetTrkSel+"&&(trk_pt!=trk_pt[jet_ltrk[trk_jet]]&&abs(jet_eta[trk_jet])<3.2)");
  HJetdTTrksLTrkEtaLow.Draw("hist");
  fitHist2Gaus(&HJetdTTrksLTrkEtaLow,-600,600,0.7,0.8);
  C1D.Print(outpath+"/JetdTTrksLTrk_JetEtaLow.png");


  C1D.Clear();
  TH1F HJetdTTrksLTrkEtaHigh("HJetdTTrksLTrkEtaHigh","",120,-600,600);
  HJetdTTrksLTrkEtaHigh.GetXaxis()->SetTitle("track #Delta t - lead track #Delta t   [ps] ");
  TREE->Draw("1000*(trk_dt-trk_dt[jet_ltrk[trk_jet]])>>HJetdTTrksLTrkEtaHigh",JetTrkSel+"&&(trk_pt!=trk_pt[jet_ltrk[trk_jet]]&&abs(jet_eta[trk_jet])>3.2)");
  HJetdTTrksLTrkEtaHigh.Draw("hist");
  fitHist2Gaus(&HJetdTTrksLTrkEtaHigh,-600,600,0.7,0.8);
  C1D.Print(outpath+"/JetdTTrksLTrk_JetEtaHigh.png");


}




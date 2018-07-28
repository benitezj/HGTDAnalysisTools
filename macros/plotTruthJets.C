void plotTruthJets(TChain*TREE,TString outpath){

  //Plot the number of jets
  C1D.Clear();
  TH1F HTruthjetN("HTruthjetN","",50,-0.5,49.5);
  HTruthjetN.GetXaxis()->SetTitle("Number of Truthjets");
  HTruthjetN.GetYaxis()->SetTitle("# of events");
  TREE->Draw("ntruthjet>>HTruthjetN");
  HTruthjetN.Draw("histpe");
  C1D.SetLogy(1);
  C1D.Print(outpath+"/Truthjet_N.png");
  C1D.SetLogy(0);


  //Plot the number of jets vs interaction id 
  C1D.Clear();
  TH1F HTruthjetVtx("HTruthjetVtx","",251,-0.5,250.5);
  HTruthjetVtx.GetXaxis()->SetTitle("P.U. event id");
  HTruthjetVtx.GetYaxis()->SetTitle("# of truth jets");
  TREE->Draw("truthjet_vtx>>HTruthjetVtx");
  HTruthjetVtx.Draw("histpe");
  C1D.Print(outpath+"/Truthjet_puevent.png");


  //Plot the pT distribution 
  TH1F HTruthjetPt("HTruthjetPt","",30,0,150);
  HTruthjetPt.Sumw2();
  TREE->Draw("truthjet_pt/1000>>HTruthjetPt","truthjet_vtx==0&&abs(truthjet_eta)>2.4&&abs(truthjet_eta)<4.0");
  HTruthjetPt.Scale(1./TREE->GetEntries());
  TH1F HTruthjetPtPU("HTruthjetPtPU","",30,0,150);
  HTruthjetPtPU.Sumw2();
  TREE->Draw("truthjet_pt/1000>>HTruthjetPtPU","truthjet_vtx>0&&abs(truthjet_eta)>2.4&&abs(truthjet_eta)<4.0");
  HTruthjetPtPU.Scale(1./TREE->GetEntries());

  HTruthjetPt.GetXaxis()->SetTitle("Truth Jet Pt [GeV]");
  HTruthjetPt.GetYaxis()->SetTitle("# of truth jets per event");
  HTruthjetPt.GetYaxis()->SetRangeUser(0.,1.2*HTruthjetPtPU.GetMaximum());


  // C1D.Clear();
  // HTruthjetPt.Draw("histp");
  // HTruthjetPtPU.Draw("histsame");
  // C1D.Print(outpath+"/Truthjet_pt_HSandPU.png");

  HTruthjetPt.SetTitle("H.S. jets");
  HTruthjetPtPU.SetTitle("P.U. jets");
  compareWithRatio(&HTruthjetPt,&HTruthjetPtPU,outpath+"/Truthjet_pt_HSandPU_ratio.png",40,0,100,0);




  //Plot the eta distribution  
  C1D.Clear();
  TH1F HTruthjetEta("HTruthjetEta","",50,0,5);
  HTruthjetEta.GetXaxis()->SetTitle("Truth Jet |eta|");
  HTruthjetEta.GetYaxis()->SetTitle("# of truth jets");
  TREE->Draw("abs(truthjet_eta)>>HTruthjetEta");
  HTruthjetEta.Draw("hist");
  C1D.Print(outpath+"/Truthjet_eta.png");

  //Plot the pT distribution for truth jets matched to the VBF jets
  TH1F HTruthjetPt_HS("HTruthjetPt_HS","",HTruthjetPt.GetNbinsX(),HTruthjetPt.GetXaxis()->GetXmin(),HTruthjetPt.GetXaxis()->GetXmax());
  TH1F HTruthjetPt_HS_truthJ("HTruthjetPt_HS_truthJ","",HTruthjetPt.GetNbinsX(),HTruthjetPt.GetXaxis()->GetXmin(),HTruthjetPt.GetXaxis()->GetXmax());
  TREE->Draw("truthjet_pt/1000>>HTruthjetPt_HS","truthjet_vtx==0");
  HTruthjetPt_HS.SetTitle("all jets");
  TREE->Draw("truthjet_pt/1000>>HTruthjetPt_HS_truthJ","truthjet_vtx==0&&truthjet_truth>=0");
  HTruthjetPt_HS_truthJ.GetXaxis()->SetTitle("truth jet p_{T}  [GeV]");
  HTruthjetPt_HS_truthJ.GetYaxis()->SetTitle("# of h.s. jets");
  HTruthjetPt_HS_truthJ.SetTitle("VBF jets");
  compareWithRatio(&HTruthjetPt_HS,&HTruthjetPt_HS_truthJ,outpath+"/Truthjet_pt_HS_truthJ.png");


}



/////////////////////////////////////////////////////////////////
///////////Event info
/////////////////////////////////////////////////////////////////
void plotPrimVertex(TChain*TREE, TString outpath){
  TCanvas C;


  TF1 VtxFit("VtxFit",FitGausFormula.Data(),-200,200);
  VtxFit.SetLineColor(4);
  VtxFit.SetParameter(0,1); 
  VtxFit.SetParameter(1,0); 
  VtxFit.SetParLimits(1,-100,100);
  VtxFit.SetParameter(2,10);
  VtxFit.SetParLimits(2,1,200);
  char Formula_Gaus[100];
  text.SetTextSize(0.04);


  C.Clear();
  TH2F HVtxXY("HVtxXY","",100,-0.1,0.1,100,-0.1,0.1);
  HVtxXY.GetXaxis()->SetTitle("Vertex X [mm]");
  HVtxXY.GetYaxis()->SetTitle("Vertex Y [mm]");
  TREE->Draw("vtx_y:vtx_x>>HVtxXY");
  HVtxXY.Draw("colz");
  C.Print(outpath+"/VtxXY.png");



  /////////////////////////////////////
  ///Vertex distribution

  char RMSText[100];

  TH1F HVtxZ("HVtxZ","",40,-200,200);
  HVtxZ.GetXaxis()->SetTitle("Reco. Vertex Z [mm]");
  TREE->Draw("vtx_z>>HVtxZ");
  //HVtxZ.Fit("VtxFit");  
  C.Clear();
  HVtxZ.GetYaxis()->SetRangeUser(0,HVtxZ.GetMaximum()*1.3);
  HVtxZ.Draw("hist");
  //VtxFit.Draw("lsame");
  //sprintf(Formula_Gaus,FitGausTextSigma.Data(),VtxFit.GetParameter(2),VtxFit.GetParError(2));
  //text.DrawLatexNDC(0.24,.85, Formula_Gaus);  
  sprintf(RMSText,"RMS = %d mm",(int)(HVtxZ.GetRMS()));
  text.DrawLatexNDC(0.7,.80, RMSText);  
  C.Print(outpath+"/HVtxZ.png");
 
  ///true vertex
  TH1F HTrueVtxZ("HTrueVtxZ","",40,-200,200);
  HTrueVtxZ.GetXaxis()->SetTitle("True Primary Vertex Z [mm]");
  TREE->Draw("truevtx_z>>HTrueVtxZ");
  //HTrueVtxZ.Fit("VtxFit");  
  C.Clear();
  HTrueVtxZ.GetYaxis()->SetRangeUser(0,HTrueVtxZ.GetMaximum()*1.3);
  HTrueVtxZ.Draw("hist");
  sprintf(RMSText,"RMS = %d mm",(int)(HTrueVtxZ.GetRMS()));
  text.DrawLatexNDC(0.7,.80, RMSText);  
  C.Print(outpath+"/HTrueVtxZ.png");
 
  ///difference between true and reco vertex
  TH1F HDeltaVtxZ("HDeltaVtxZ","",80,-200,200);
  HDeltaVtxZ.GetXaxis()->SetTitle("Reco. - True Primary Vertex Z [mm]");
  TREE->Draw("vtx_z-truevtx_z>>HDeltaVtxZ");
  C.Clear();
  HDeltaVtxZ.Draw("hist");
  C.Print(outpath+"/HDeltaVtxZ.png");
 


  //true vertex time
  TF1 VtxTFit("VtxTFit",FitGausFormula.Data(),-500,500);
  VtxTFit.SetLineColor(4);
  //VtxTFit.SetParameter(0,1); 
  VtxTFit.SetParameter(1,0); 
  VtxTFit.SetParLimits(1,-100,100);
  VtxTFit.SetParameter(2,10);
  VtxTFit.SetParLimits(2,1,100);


  TH1F HTrueVtxT("HTrueVtxT","",120,-600,600);
  HTrueVtxT.GetXaxis()->SetTitle("true vertex time [ps]");
  HTrueVtxT.GetYaxis()->SetTitle("Avg. # of vertices per event / 10 ps");
  HTrueVtxT.SetLineColor(4);
  TREE->Draw("(1000*truevtx_t)>>HTrueVtxT");
  HTrueVtxT.Scale(200./TREE->GetEntries());

  C.Clear();
  HTrueVtxT.GetYaxis()->SetRangeUser(0,HTrueVtxT.GetMaximum()*1.3);
  HTrueVtxT.Draw("hist");
  text.SetTextColor(1);
  text.DrawLatexNDC(0.22,.85, "ATLAS simulation");  
  text.SetTextColor(4);
  sprintf(RMSText,"RMS = %d ps",(int)(HTrueVtxT.GetRMS()));
  text.DrawLatexNDC(0.7,.80, RMSText);  
  C.Print(outpath+"/HTrueVtxT.png");
 

}

void plotTruth(TChain*TREE,TString outpath){
  TCanvas C;

  if(HVBFJ1_eta){
    C.Clear();
    HVBFJ1_eta->SetLineWidth(2);
    HVBFJ2_eta->SetLineWidth(2);
    HVBFG_eta->SetLineWidth(2);
    HVBFJ1_eta->GetYaxis()->SetRangeUser(0,HVBFJ1_eta->GetMaximum()*1.2);
    HVBFJ1_eta->GetXaxis()->SetTitle("truth  |eta| ");
    HVBFJ1_eta->SetLineColor(1);
    HVBFJ1_eta->Draw("hist");
    HVBFJ2_eta->SetLineColor(4);
    HVBFJ2_eta->Draw("histsame");
    HVBFG_eta->SetLineColor(3);
    //HVBFG_eta->Draw("histsame");
    line.DrawLine(-4.0,0,-4.0,HVBFJ1_eta->GetMaximum());
    line.DrawLine(-2.6,0,-2.6,HVBFJ1_eta->GetMaximum());
    line.DrawLine(2.6,0,2.6,HVBFJ1_eta->GetMaximum());
    line.DrawLine(4.0,0,4.0,HVBFJ1_eta->GetMaximum());
    C.Print(outpath+"/Truth_Eta_VBFJetsAndGluon.png");
    
    
    ///fraction of jets in acceptance:
    cout<<"% of jets in acceptance:  "
	<<HVBFJ2_eta->Integral(20,27)/HVBFJ2_eta->Integral(1,80)
	<<"+/-"<<sqrt(HVBFJ2_eta->Integral(20,27))/HVBFJ2_eta->Integral(1,80)<<endl;
  }

  if(HVBFJ1_pT){
    C.Clear();
    HVBFJ1_pT->SetLineWidth(2);
    HVBFJ2_pT->SetLineWidth(2);
    HVBFG_pT->SetLineWidth(2);
    HVBFJ1_pT->GetYaxis()->SetRangeUser(0,HVBFG_pT->GetMaximum()*1.1);
    HVBFJ1_pT->GetXaxis()->SetTitle("truth  pT   [GeV]");
    HVBFJ1_pT->SetLineColor(1);
    HVBFJ1_pT->Draw("hist");
    HVBFJ2_pT->SetLineColor(4);
    HVBFJ2_pT->Draw("histsame");
    HVBFG_pT->SetLineColor(3);
    //HVBFG_pT->Draw("histsame");
    C.Print(outpath+"/Truth_PT_VBFJetsAndGluon.png");
  }





  // /////////////////////////////////////////////////////////
  // ///Forward VBF Jet
  // TH1F HEta_VBFJF("HEta_VBFJF","",40,0,8);
  // TREE->Draw("abs(truth_eta[truth_VBFJ1])>abs(truth_eta[truth_VBFJ2]) ? abs(truth_eta[truth_VBFJ1]):abs(truth_eta[truth_VBFJ2])>>HEta_VBFJF");
  // TH1F HPt_VBFJF("HPt_VBFJF","",60,0,300);
  // TREE->Draw("abs(truth_eta[truth_VBFJ1])>abs(truth_eta[truth_VBFJ2]) ? truth_pt[truth_VBFJ1]/1000:truth_pt[truth_VBFJ2]/1000>>HPt_VBFJF");

  // ///"Central" VBF Jet
  // TH1F HEta_VBFJC("HEta_VBFJC","",40,0,8);
  // TREE->Draw("abs(truth_eta[truth_VBFJ1])>abs(truth_eta[truth_VBFJ2]) ? abs(truth_eta[truth_VBFJ2]):abs(truth_eta[truth_VBFJ1])>>HEta_VBFJC");
  // TH1F HPt_VBFJC("HPt_VBFJC","",60,0,300);
  // TREE->Draw("abs(truth_eta[truth_VBFJ1])>abs(truth_eta[truth_VBFJ2]) ? truth_pt[truth_VBFJ2]/1000:truth_pt[truth_VBFJ1]/1000>>HPt_VBFJC");

  // ///Gluon
  // TH1F HEta_Gluon("HEta_Gluon","",40,0,8);
  // TREE->Draw("abs(truth_eta[truth_Gluon])>>HEta_Gluon","truth_Gluon>=0");
  // TH1F HPt_Gluon("HPt_Gluon","",60,0,300);
  // TREE->Draw("truth_pt[truth_Gluon]/1000>>HPt_Gluon","truth_Gluon>=0");

  
  // C.Clear();
  // HEta_VBFJF.GetYaxis()->SetRangeUser(0, HEta_VBFJC.GetMaximum()*1.2);
  // HEta_VBFJF.GetXaxis()->SetTitle("truth  |eta| ");
  // HEta_VBFJF.SetLineColor(1);
  // HEta_VBFJF.Draw("histp");
  // HEta_VBFJC.SetLineColor(4);
  // HEta_VBFJC.Draw("histsame");
  // HEta_Gluon.SetLineColor(3);
  // HEta_Gluon.Draw("histsame");
  // C.Print(outpath+"/Truth_Eta_VBFJetsAndGluon.png");


  // C.Clear();
  // HPt_VBFJF.GetYaxis()->SetRangeUser(0, HPt_Gluon.GetMaximum()*1.2);
  // HPt_VBFJF.GetXaxis()->SetTitle("truth  p_{T}   [GeV] ");
  // HPt_VBFJF.SetLineColor(1);
  // HPt_VBFJF.Draw("histp");
  // HPt_VBFJC.SetLineColor(4);
  // HPt_VBFJC.Draw("histsame");
  // HPt_Gluon.SetLineColor(3);
  // HPt_Gluon.Draw("histsame");
  // C.Print(outpath+"/Truth_Pt_VBFJetsAndGluon.png");


  // C.Clear();
  // HPt_VBFJF.GetYaxis()->SetRangeUser(0, HPt_Gluon.GetMaximum()*1.2);
  // HPt_VBFJF.GetXaxis()->SetTitle("truth  p_{T}   [GeV] ");
  // HPt_VBFJF.SetLineColor(1);
  // HPt_VBFJF.Draw("histp");
  // HPt_VBFJC.SetLineColor(4);
  // HPt_VBFJC.Draw("histsame");
  // HPt_Gluon.SetLineColor(3);
  // HPt_Gluon.Draw("histsame");
  // C.Print(outpath+"/Truth_Pt_VBFJetsAndGluon.png");



  // /////////////////////////////////////////////////////////
  // /// Forward Lepton //there is some issue with the Truth collection, sometimes the W decays to taus and the tau decay is corrupted so no lepton is found
  // TH1F HEta_LeptonF("HEta_LeptonF","",40,0,8);
  // TREE->Draw("abs(truth_eta[truth_Lp])>abs(truth_eta[truth_Lm]) ? abs(truth_eta[truth_Lp]):abs(truth_eta[truth_Lm])>>HEta_LeptonF","truth_Lp>=0&&truth_Lm>=0");
  // TH1F HPt_LeptonF("HPt_LeptonF","",60,0,300);
  // TREE->Draw("abs(truth_eta[truth_Lp])>abs(truth_eta[truth_Lm]) ? truth_pt[truth_Lp]/1000:truth_pt[truth_Lm]/1000>>HPt_LeptonF","truth_Lp>=0&&truth_Lm>=0");

  // ///"Central" Lepton
  // TH1F HEta_LeptonC("HEta_LeptonC","",40,0,8);
  // TREE->Draw("abs(truth_eta[truth_Lp])>abs(truth_eta[truth_Lm]) ? abs(truth_eta[truth_Lm]):abs(truth_eta[truth_Lp])>>HEta_LeptonC","truth_Lp>=0&&truth_Lm>=0");
  // TH1F HPt_LeptonC("HPt_LeptonC","",60,0,300);
  // TREE->Draw("abs(truth_eta[truth_Lp])>abs(truth_eta[truth_Lm]) ? truth_pt[truth_Lm]/1000:truth_pt[truth_Lp]/1000>>HPt_LeptonC","truth_Lp>=0&&truth_Lm>=0");

  // C.Clear();
  // HEta_LeptonF.GetYaxis()->SetRangeUser(0, HEta_LeptonC.GetMaximum()*1.2);
  // HEta_LeptonF.GetXaxis()->SetTitle("truth  |eta| ");
  // HEta_LeptonF.SetLineColor(1);
  // HEta_LeptonF.Draw("histp");
  // HEta_LeptonC.SetLineColor(4);
  // HEta_LeptonC.Draw("histsame");
  // C.Print(outpath+"/Truth_Eta_Leptons.png");

  // C.Clear();
  // HPt_LeptonF.GetYaxis()->SetRangeUser(0, HEta_LeptonC.GetMaximum()*1.2);
  // HPt_LeptonF.GetXaxis()->SetTitle("truth  p_{T}   [GeV] ");
  // HPt_LeptonF.SetLineColor(1);
  // HPt_LeptonF.Draw("histp");
  // HPt_LeptonC.SetLineColor(4);
  // HPt_LeptonC.Draw("histsame");
  // C.Print(outpath+"/Truth_Pt_Leptons.png");


}




void plotJetNCellsIDR(TChain*TREE,TString outpath){ ///plot the distribution of # of cells per jet vs jet eta

  TString plottag="_Timing";
  if(outpath.Contains("Preshower"))
    plottag="_Preshower";

  if(outpath.Contains("_mu0"))
    plottag+="_mu0";
  else plottag+="_mu200";


  int nhitmin=0;
  //int nhitmax=800;   int nhitbins=16; // for dR=0.4
  int nhitmax=400;   int nhitbins=16; // for dR=0.2

  TH1F HFrame("HFrame","",nhitbins,nhitmin,nhitmax);
  HFrame.GetXaxis()->SetTitle("number of cells per jet");
  HFrame.GetYaxis()->SetTitle("Probability");
  HFrame.GetYaxis()->SetRangeUser(0,0.6);

  TLegend legend(0.7,0.7,0.92,0.9);
  legend.SetFillColor(0);
  legend.SetBorderSize(0);
  char legtext[100];

  TCanvas C;
  C.Clear();
  HFrame.Draw();
  int color=0;

  //low eta
  int etamin=10*ETAMIN;
  int etamax=10*ETAMAX;
  int etastep=0.5+10*(ETAMAX-ETAMIN)/ETABINS;
  for(long e=etamin;e<etamax;e+=etastep){

    TString selection=TString("(")+e+"<abs(jet_eta*10)&&abs(jet_eta*10)<"+(e+etastep)+")";

    ///test removing square corners 
    //TString selection=TString("(")+e+"<abs(jet_eta*10)&&abs(jet_eta*10)<"+(e+etastep)+"&&!(fabs(jet_phi+3*3.14159/4)<3.14159/8||fabs(jet_phi+3.14159/4)<3.14159/8||fabs(jet_phi-3.14159/4)<3.14159/8||fabs(jet_phi-3*3.14159/4)<3.14159/8))";
    
    cout<<e<<" "<<selection<<endl;

    TH1F * HEta=new TH1F(TString("HEta")+e,"",nhitbins,nhitmin,nhitmax);
    //TREE->Draw(TString("jet_ncell>>HEta")+e,signaljetCUT+"*"+JetPtCut+"*"+selection,"same");
    TREE->Draw(TString("jet_ncell_dr00+jet_ncell_dr05+jet_ncell_dr10+jet_ncell_dr15>>HEta")+e,signaljetCUT+"*"+JetPtCut+"*"+selection,"same");
    HEta->Scale(1./HEta->Integral());

    HEta->SetLineColor(ETACOLOR[color]);
    HEta->SetLineWidth(3);

    sprintf(legtext,"%.1f<|#eta^{J}|<%.1f",float(e)/10,float(e+etastep)/10.);
    legend.AddEntry(HEta,legtext,"l");

    color++;
  }

  legend.Draw();
  DrawATLAS(0.20,0.85);
  DrawHGTD(outpath,0,0,0.20,0.80);
  DrawECMSMu(outpath);

  text.SetTextSize(0.045);
  sprintf(legtext,"%.0f GeV<p_{T}^{J}<%.0f GeV",jetPtL/1000.,jetPtH/1000.);
  text.DrawLatexNDC(0.65,0.6,legtext);

  text.SetTextSize(0.045);
  //text.DrawLatexNDC(0.80,0.50,"#DeltaR<0.4");
  text.DrawLatexNDC(0.80,0.50,"#DeltaR<0.2");

  C.Print(outpath+"/JetNcellvsEta_IDR"+plottag+".png");
  C.Print(outpath+"/JetNcellvsEta_IDR"+plottag+".eps");
  C.Print(outpath+"/JetNcellvsEta_IDR"+plottag+".pdf");

}


void drawJetCelldEtadPhiLabels(TString outpath,TString label){
  DrawATLAS(0.2,0.835);
  DrawHGTD(outpath,0,0,0.185,0.76);
  DrawECMSMu(outpath,0.54,0.765);
  text.DrawLatexNDC(0.69,0.25,label);
  text.DrawLatexNDC(0.63,0.2,TString("")+etamin+"<|#eta^{J}|<"+etamax);
}

void printJetCelldEtadPhi(TChain*TREE,TString outpath){

 TCanvas C("C","",700,700);
  C.SetRightMargin(0.18);
  C.SetTopMargin(0.18);
  C.SetLeftMargin(0.17);
  C.SetBottomMargin(0.17);

  TString xtitle="#phi^{cell} - #phi^{J}";
  TString ytitle="|#eta^{cell}| - |#eta^{J}|";
  TString ztitle="avg. # of cells per jet / (0.02x0.02)";
  float titlesize=0.05;
  
  int nbins=50;
  float min=-0.5;
  float max=0.5;

  TH2F HCelldEtadPhiEta("HCelldEtadPhiEta","",nbins,min,max,nbins,min,max);
  HCelldEtadPhiEta.GetXaxis()->SetTitle(xtitle);
  HCelldEtadPhiEta.GetYaxis()->SetTitle(ytitle);
  HCelldEtadPhiEta.GetZaxis()->SetTitle(ztitle);
  HCelldEtadPhiEta.GetXaxis()->SetTitleSize(titlesize);
  HCelldEtadPhiEta.GetYaxis()->SetTitleSize(titlesize);
  HCelldEtadPhiEta.GetZaxis()->SetTitleSize(0.8*titlesize);
  HCelldEtadPhiEta.GetXaxis()->SetTitleOffset(1.1);
  HCelldEtadPhiEta.GetYaxis()->SetTitleOffset(1.1);
  HCelldEtadPhiEta.GetZaxis()->SetTitleOffset(1.2);
  HCelldEtadPhiEta.GetXaxis()->SetLabelSize(0.7*titlesize);
  HCelldEtadPhiEta.GetYaxis()->SetLabelSize(0.7*titlesize);
  HCelldEtadPhiEta.GetZaxis()->SetLabelSize(0.6*titlesize);


  TH1F HJetsNorm("HJetsNorm","",100,-10,10);

  //TString dEta="(abs(cell_eta)-abs(jet_eta[cell_jet]))";
  //TString dPhi="(cell_phi-jet_phi[cell_jet])";
  //TString dPhi="(cell_phi-jet_phi[cell_jet]-(abs(cell_phi-jet_phi[cell_jet])>3.14159)*((cell_phi-jet_phi[cell_jet])/abs(cell_phi-jet_phi[cell_jet]))*2*3.14159)";


  ///Signal jet
  HCelldEtadPhiEta.Scale(0.);
  HJetsNorm.Scale(0.);
  TREE->Draw(dEta+":"+dPhi+">>HCelldEtadPhiEta",CUTSignalJetCell+"*"+CUTPtJetCell+"*"+CUTEtaJetCell);
  TREE->Draw("jet_eta>>HJetsNorm",signaljetCUT+"*"+JetPtCut+"*"+JetEtaCut);
  HCelldEtadPhiEta.Scale(1./HJetsNorm.Integral());
  C.Clear();
  HCelldEtadPhiEta.Draw("colz");
  drawJetCelldEtadPhiLabels(outpath,"h.s. jet");
  C.Print(outpath+"_Sig.png");
  C.Print(outpath+"_Sig.eps");
  C.Print(outpath+"_Sig.pdf");


  //PU jet
  HCelldEtadPhiEta.Scale(0.);
  HJetsNorm.Scale(0.);
  TREE->Draw(dEta+":"+dPhi+">>HCelldEtadPhiEta",CUTPUJetCell+"*"+CUTPtJetCell+"*"+CUTEtaJetCell);
  HJetsNorm.Scale(0.);
  TREE->Draw("jet_eta>>HJetsNorm",pujetCUT+"*"+JetPtCut+"*"+JetEtaCut);
  HCelldEtadPhiEta.Scale(1./HJetsNorm.Integral());
  C.Clear();
  HCelldEtadPhiEta.Draw("colz");
  drawJetCelldEtadPhiLabels(outpath,"p.u. jet");
  C.Print(outpath+"_PU.png");
  C.Print(outpath+"_PU.eps");
  C.Print(outpath+"_PU.pdf");


  ///Signal jet with timing cut:
  TString TimingCut=TString("(abs(cell_dt-jet_truedt[cell_jet])<")+timecut+")";
  HCelldEtadPhiEta.Scale(0.);
  HJetsNorm.Scale(0.);
  TREE->Draw(dEta+":"+dPhi+">>HCelldEtadPhiEta",CUTSignalJetCell+"*"+CUTPtJetCell+"*"+CUTEtaJetCell+"&&"+TimingCut);
  TREE->Draw("jet_eta>>HJetsNorm",signaljetCUT+"*"+JetPtCut+"*"+JetEtaCut);
  HCelldEtadPhiEta.Scale(1./HJetsNorm.Integral());
  C.Clear();
  HCelldEtadPhiEta.Draw("colz");
  drawJetCelldEtadPhiLabels(outpath,"h.s. jet");
  C.Print(outpath+"_Sig_TimingCut.png");
  C.Print(outpath+"_Sig_TimingCut.eps");
  C.Print(outpath+"_Sig_TimingCut.pdf");




}



void printJetCelldEtadPhiOutsideCone(TChain*TREE,TString outpath){

 TCanvas C("C","",700,700);
  C.SetRightMargin(0.18);
  C.SetTopMargin(0.18);
  C.SetLeftMargin(0.17);
  C.SetBottomMargin(0.17);

  TString xtitle="#phi^{cell} - #phi^{J}";
  TString ytitle="|#eta^{cell}| - |#eta^{J}|";
  TString ztitle="avg. # of cells per jet / (0.02x0.02)";
  float titlesize=0.05;
  
  int nbins=100;
  float min=-1.0;
  float max=1.0;

  TH2F HCelldEtadPhiEta("HCelldEtadPhiEta","",nbins,min,max,nbins,min,max);
  HCelldEtadPhiEta.GetXaxis()->SetTitle(xtitle);
  HCelldEtadPhiEta.GetYaxis()->SetTitle(ytitle);
  HCelldEtadPhiEta.GetZaxis()->SetTitle(ztitle);
  HCelldEtadPhiEta.GetXaxis()->SetTitleSize(titlesize);
  HCelldEtadPhiEta.GetYaxis()->SetTitleSize(titlesize);
  HCelldEtadPhiEta.GetZaxis()->SetTitleSize(0.8*titlesize);
  HCelldEtadPhiEta.GetXaxis()->SetTitleOffset(1.1);
  HCelldEtadPhiEta.GetYaxis()->SetTitleOffset(1.1);
  HCelldEtadPhiEta.GetZaxis()->SetTitleOffset(1.2);
  HCelldEtadPhiEta.GetXaxis()->SetLabelSize(0.7*titlesize);
  HCelldEtadPhiEta.GetYaxis()->SetLabelSize(0.7*titlesize);
  HCelldEtadPhiEta.GetZaxis()->SetLabelSize(0.6*titlesize);


  ///TH1F HJetsNorm("HJetsNorm","",100,-10,10);

  
  HCelldEtadPhiEta.Scale(0.);
  TREE->Draw("(abs(cell_eta)-abs(truth_eta[truth_VBFJ1])):(cell_phi-truth_phi[truth_VBFJ1])>>HCelldEtadPhiEta","cell_eta>0&&(truth_Gluon==-1||abs(truth_phi[truth_Gluon]-truth_phi[truth_VBFJ1])>1.0)&&3.6<abs(truth_eta[truth_VBFJ1])&&abs(truth_eta[truth_VBFJ1])<3.8");
  C.Clear();
  HCelldEtadPhiEta.Draw("colz");
  drawJetCelldEtadPhiLabels(outpath,"h.s. jet");
  C.Print(outpath+"_SigVBFJ1.png");


  HCelldEtadPhiEta.Scale(0.);
  TREE->Draw("(abs(cell_eta)-abs(truth_eta[truth_VBFJ2])):(cell_phi-truth_phi[truth_VBFJ2])>>HCelldEtadPhiEta","cell_eta<0&&(truth_Gluon==-1||abs(truth_phi[truth_Gluon]-truth_phi[truth_VBFJ2])>1.0)&&3.6<abs(truth_eta[truth_VBFJ2])&&abs(truth_eta[truth_VBFJ2])<3.8");
  C.Clear();
  HCelldEtadPhiEta.Draw("colz");
  drawJetCelldEtadPhiLabels(outpath,"h.s. jet");
  C.Print(outpath+"_SigVBFJ2.png");



}




void plotJetCelldEtadPhiIDR(TChain*TREE,TString outpath){//2D hit distribution w.r.t jet axis

  TString plottag="_Timing";
  if(outpath.Contains("Preshower"))
    plottag="_Preshower";

  if(outpath.Contains("_mu0"))
    plottag+="_mu0";
  else plottag+="_mu200";


  ////Low eta
  etamin="2.8";
  etamax="3.0";
  timecut="0.180";
  setCuts(outpath);
  printJetCelldEtadPhi(TREE,outpath+"/JetCelldEtadPhi_LowEta_IDR"+plottag);

  // ////High eta
  etamin="3.6";
  etamax="3.8";
  timecut="0.090";
  setCuts(outpath);
  printJetCelldEtadPhi(TREE,outpath+"/JetCelldEtadPhi_HighEta_IDR"+plottag);



  // //////////////////////////
  // ////High eta large cone use truth Jets
  // etamin="3.6";
  // etamax="3.8";
  // setCuts(outpath);
  // printJetCelldEtadPhiOutsideCone(TREE,outpath+"/JetCelldEtadPhi_HighEta_OutsideCone"+plottag);


  // ////inclusive
  // etamin="2.8";
  // etamax="3.8";
  // timecut="0.135";
  // setCuts(outpath);
  // printJetCelldEtadPhi(TREE,outpath+"/JetCelldEtadPhi_IDR"+plottag);


}

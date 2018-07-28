void plotCelldR(TString file){
  
  TFile F(file.Data(),"read");

  TTree*t=(TTree*)F.Get("tuple");
  if(!t) return;

  TCanvas C;

  int NBINS=6;
  float xmin=0.;
  float xmax=0.6;
  
  ////Number of cells vs deltaR 
  TH1F HN_signal("HN_signal","",NBINS,xmin,xmax);
  t->Draw("cell_jetdR>>HN_signal","(cell_jet>=0&&jet_truthJ[cell_jet]>=0)");
  HN_signal.Scale(1./HN_signal.Integral());

  TH1F HN_pu("HN_pu","",NBINS,xmin,xmax);
  t->Draw("cell_jetdR>>HN_pu","(cell_jet>=0&&jet_truthJ[cell_jet]==-1)");
  HN_pu.Scale(1./HN_pu.Integral());  

  C.Clear();
  HN_signal.GetYaxis()->SetTitle(" number of cells ");
  HN_signal.GetXaxis()->SetTitle(" cell deltaR ");
  HN_signal.Draw("histp");
  HN_pu.Draw("histsame");
  C.Print("cell_jetdR_SignalAndPU.png");



  ////Number of cells vs deltaR energy weighted
  TH1F HE_signal("HE_signal","",NBINS,xmin,xmax);
  t->Draw("cell_jetdR>>HE_signal","(cell_jet>=0&&jet_truthJ[cell_jet]>=0)*(cell_E)");
  HE_signal.Scale(1./HE_signal.Integral());

  TH1F HE_pu("HE_pu","",NBINS,xmin,xmax);
  t->Draw("cell_jetdR>>HE_pu","(cell_jet>=0&&jet_truthJ[cell_jet]==-1)*(cell_E)");
  HE_pu.Scale(1./HE_pu.Integral());  

  C.Clear();
  HE_signal.GetYaxis()->SetTitle(" number of cells E weighted ");
  HE_signal.GetXaxis()->SetTitle(" cell deltaR ");
  HE_signal.Draw("histp");
  HE_pu.Draw("histsame");
  C.Print("cell_jetdR_Eweight_SignalAndPU.png");


  gROOT->ProcessLine(".q");
}

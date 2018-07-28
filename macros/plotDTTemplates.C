
void plotDTTemplates(){
  gROOT->ProcessLine(".x ./HGTDAnalysisTools/macros/rootlogon.C");

  TFile FINPUT("HGTDAnalysisTools/share/DTTemplates.root","read");

  TH1F* Sig10=(TH1F*)FINPUT.Get("Signal_10");
  TH1F* PU10=(TH1F*)FINPUT.Get("PU_10");

  TH1F* Sig20=(TH1F*)FINPUT.Get("Signal_20"); 
  TH1F* PU20=(TH1F*)FINPUT.Get("PU_20");

  TH1F* Sig30=(TH1F*)FINPUT.Get("Signal_30"); 
  TH1F* PU30=(TH1F*)FINPUT.Get("PU_30");

  TH1F* Sig50=(TH1F*)FINPUT.Get("Signal_50"); 
  TH1F* PU50=(TH1F*)FINPUT.Get("PU_50");

  TH1F* Sig100=(TH1F*)FINPUT.Get("Signal_100"); 
  TH1F* PU100=(TH1F*)FINPUT.Get("PU_100");

  Sig20->SetLineColor(2);  PU20->SetLineColor(Sig20->GetLineColor());
  Sig30->SetLineColor(3);  PU30->SetLineColor(Sig30->GetLineColor());
  Sig50->SetLineColor(4);  PU50->SetLineColor(Sig50->GetLineColor());
  Sig100->SetLineColor(7);  PU100->SetLineColor(Sig100->GetLineColor());

  TCanvas C;

  C.Clear();
  Sig10->GetXaxis()->SetTitle("cell #DeltaT   [ps]");
  Sig10->Draw();
  Sig20->Draw("same");
  Sig30->Draw("same");
  Sig50->Draw("same");
  Sig100->Draw("same");
  C.Print("DTTemplates_Sig.gif");
  
  C.Clear();
  PU10->GetXaxis()->SetTitle("cell #DeltaT   [ps]");
  PU10->Draw();
  PU20->Draw("same");
  PU30->Draw("same");
  PU50->Draw("same");
  PU100->Draw("same");
  C.Print("DTTemplates_PU.gif");
  
  
  gROOT->ProcessLine(".q");
}

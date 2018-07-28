
void plotElectrons(TChain*TREE,TString outpath){
  TCanvas C;

  //Plot the number of jets
  C.Clear();
  TH1F HEleN("HEleN","",20,-0.5,19.5);
  HEleN.GetXaxis()->SetTitle("Number of Electrons");
  TREE->Draw("nele>>HEleN");
  HEleN.Draw("hist");
  C.Print(outpath+"/EleN.png");

  //Plot the pT distribution
  C.Clear();
  TH1F HElePt("HElePt","",25,0,125);
  HElePt.GetXaxis()->SetTitle("Ele Pt [GeV]");
  TREE->Draw("ele_pt/1000>>HElePt");
  HElePt.Draw("hist");
  C.Print(outpath+"/ElePt.png");

  //Plot the eta distribution           
  C.Clear();
  TH1F HEleEta("HEleEta","",100,-5,5);
  HEleEta.GetXaxis()->SetTitle("Electron eta");
  TREE->Draw("ele_eta>>HEleEta");
  HEleEta.Draw("hist");
  C.Print(outpath+"/EleEta.png");


  //Plot the pT distribution for signal and pu 
  TH1F HElePtSig("HElePtSig","",25,0,125);
  TREE->Draw("ele_pt/1000>>HElePtSig","ele_truth>=0");
  HElePtSig.Scale(1./HElePtSig.Integral());
  TH1F HElePtPU("HElePtPU","",25,0,125);
  TREE->Draw("ele_pt/1000>>HElePtPU","ele_truth<0");
  HElePtPU.Scale(1./HElePtPU.Integral());
  C.Clear();
  HElePtPU.GetXaxis()->SetTitle("Ele Pt [GeV]");
  HElePtPU.Draw("hist");
  HElePtSig.Draw("histpsame");
  C.Print(outpath+"/ElePt_SigAndPU.png");

  //Plot the eta distribution for signal and pu 
  TH1F HEleEtaSig("HEleEtaSig","",50,0,5);
  TREE->Draw("ele_eta>>HEleEtaSig","ele_truth>=0");
  HEleEtaSig.Scale(1./HEleEtaSig.Integral());
  TH1F HEleEtaPU("HEleEtaPU","",50,0,5);
  TREE->Draw("ele_eta>>HEleEtaPU","ele_truth<0");
  HEleEtaPU.Scale(1./HEleEtaPU.Integral());
  C.Clear();
  HEleEtaPU.GetXaxis()->SetTitle("Ele Eta");
  HEleEtaPU.Draw("hist");
  HEleEtaSig.Draw("histpsame");
  C.Print(outpath+"/EleEta_SigAndPU.png");



}

void plotEleCells(TChain*TREE,TString outpath){
  TCanvas C;

  //Plot the number of cells associated to eles
  C.Clear();
  TH1F HEleNcell("HEleNcell","",30,0,500);
  HEleNcell.GetXaxis()->SetTitle("Number of cells in ele");
  TREE->Draw("ele_ncell>>HEleNcell");
  HEleNcell.Draw("hist");
  C.Print(outpath+"/EleNcell.png");
  C.SetLogy(1);
  C.Print(outpath+"/EleNcell_log.png");
  C.SetLogy(0);

  ///2-D distribution of eles in ncell vs eta 
  C.Clear();
  TH2F HEleNcellvsEta("HEleNcellvsEta","",100,-5,5,501,-0.5,500.5);
  HEleNcellvsEta.GetXaxis()->SetTitle("Ele |eta|");
  HEleNcellvsEta.GetYaxis()->SetTitle("Number of cells in ele");
  TREE->Draw("ele_ncell:ele_eta>>HEleNcellvsEta");
  HEleNcellvsEta.Draw("colz");
  C.Print(outpath+"/EleNcellvsEta.png");

  TProfile * PEleNcellvsEta = HEleNcellvsEta.ProfileX("PEleNcellvsEta");
  PEleNcellvsEta->GetYaxis()->SetTitle("Average number of cells");
  //PEleNcellvsEta->GetYaxis()->SetRangeUser(0,200);
  C.Clear();
  PEleNcellvsEta->Draw("pe");
  C.Print(outpath+"/EleNcellvsEta_Profile.png");
  delete PEleNcellvsEta;


}



//////////////////////////////////////////////////////
/////ele clusters
//////////////////////////////////////////////////////
void plotEleClusters(TChain*TREE,TString outpath){
 
  ////separate EM vs HAD clusters
  TH1F HEleClusEMProb("HEleClusEMProb","",20,0,1);
  TREE->Draw("ele_clus_emprob>>HEleClusEMProb","ele_truth>=0");
  C1D.Clear();
  HEleClusEMProb.Draw("hist");
  C1D.Print(outpath+"/EleClusEMProb.png");

  TH1F HEleClusLambda("HEleClusLambda","",50,0,2000);
  TREE->Draw("ele_clus_lambda>>HEleClusLambda","ele_truth>=0");
  C1D.Clear();
  HEleClusLambda.Draw("hist");
  C1D.Print(outpath+"/EleClusLambda.png");

  TH1F HEleClusEMFrac("HEleClusEMFrac","",100,0,1);
  TREE->Draw("ele_clus_emfrac>>HEleClusEMFrac","ele_truth>=0");
  C1D.Clear();
  HEleClusEMFrac.Draw("hist");
  C1D.Print(outpath+"/EleClusEMFrac.png");


  TH2F HEleClusLambdaVsE("HEleClusLambdaVsE","",100,0,1000,50,0,2000);
  HEleClusLambdaVsE.GetXaxis()->SetTitle("ele. cluster energy  [GeV]");
  HEleClusLambdaVsE.GetYaxis()->SetTitle("cluster penetration depth");
  HEleClusLambdaVsE.GetZaxis()->SetTitle("# of clusters");
  TREE->Draw("ele_clus_lambda:ele_clus_E/1000.>>HEleClusLambdaVsE","ele_truth>=0");
  C2D.Clear();
  HEleClusLambdaVsE.Draw("histcolz");
  C2D.Print(outpath+"/EleClusLambdaVsE.png");
 
}

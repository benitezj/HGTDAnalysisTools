

void plotClusterCells(TChain*TREE,TString outpath){
  TCanvas C;

  C.Clear();
  TH1F HClusterNcell("HClusterNcell","",100,0,200);
  HClusterNcell.GetXaxis()->SetTitle("Number of cells in cluster");
  TREE->Draw("clus_ncell>>HClusterNcell");
  HClusterNcell.Draw("hist");
  C.Print(outpath+"/ClusterNcell.png");
  C.SetLogy(1);
  C.Print(outpath+"/ClusterNcell_log.png");
  C.SetLogy(0);

  ///2-D distribution of ncell vs eta
  C.Clear();
  TH2F HClusterNcellvsEta("HClusterNcellvsEta","",100,-5,5,301,-0.5,300.5);
  HClusterNcellvsEta.GetXaxis()->SetTitle("Cluster eta");
  HClusterNcellvsEta.GetYaxis()->SetTitle("Number of cells in cluster");
  TREE->Draw("clus_ncell:clus_eta>>HClusterNcellvsEta");
  HClusterNcellvsEta.Draw("colz");
  C.Print(outpath+"/ClusterNcellvsEta.png");

  TProfile * PClusterNcellvsEta = HClusterNcellvsEta.ProfileX("PClusterNcellvsEta");
  PClusterNcellvsEta->GetYaxis()->SetTitle("Average number of cells");
  //PClusterNcellvsEta->GetYaxis()->SetRangeUser(0,70);
  C.Clear();
  PClusterNcellvsEta->Draw("pe");
  C.Print(outpath+"/ClusterNcellvsEtaProfile.png");
  delete PClusterNcellvsEta;


}


void plotClusterCellsDensity(TChain*TREE,TString outpath){

  //////////////////////////////
  TH1F HClusE("HClusE","",100,0,1000);
  TREE->Draw("clus_E/1000>>HClusE",CUTSignalJetClus);
  C1D.Clear();
  HClusE.Draw("histp");
  C1D.Print(outpath+"/ClusterClusE.png");


  ////////////////////////////////////////
  /// cell dEta:dPhi map
  //////////////////////////////////////
  TString xtitle="#phi_{cell} - #phi_{cluster}";
  TString ytitle="|#eta_{cell}| - |#eta_{cluster}|";
  TH2F HCelldEtadPhiEta("HCelldEtadPhiEta","",60,-0.15,0.15,60,-0.15,0.15);
  HCelldEtadPhiEta.GetXaxis()->SetTitle(xtitle);
  HCelldEtadPhiEta.GetYaxis()->SetTitle(ytitle);
  HCelldEtadPhiEta.GetZaxis()->SetTitle("avg. # of cells / 0.005 x 0.005");
  TREE->Draw(dEtaCellClus+":"+dPhiCellClus+">>HCelldEtadPhiEta",CUTSignalJetClusterCell);
  
  HCelldEtadPhiEta.Scale(1./HClusE.Integral());
  C2D.Clear();
  HCelldEtadPhiEta.Draw("histcolz");
  C2D.Print(outpath+"/ClusterCell_dEtadPhi.png");


  //////////////////////////////
  TH1F HCelldR("HCelldR","",15,0,0.15);
  HCelldR.Sumw2();
  HCelldR.GetXaxis()->SetTitle("#DeltaR");
  HCelldR.GetYaxis()->SetTitle("avg. # of cells per cluster / 0.01");
  TREE->Draw("cell_clusdR>>HCelldR",CUTSignalJetClusterCell);
  HCelldR.Scale(1./HClusE.Integral());
  C1D.Clear();
  HCelldR.Draw("hist");
  C1D.Print(outpath+"/ClusterCelldR.png");

  ///// Density for signal clusters 
  dividedRDensityHisto(&HCelldR);
  HCelldR.GetYaxis()->SetTitle("cell density per cluster");
  C1D.Clear();
  HCelldR.Draw("histpe");
  C1D.Print(outpath+"/ClusterCelldRDensity.png");


}




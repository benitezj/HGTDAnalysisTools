
void plotEventDisplayHitsEtaPhi(TTree *Tuple, TString outpath,TString tag, TString Selection, long side=1){

  TCanvas C("C","",800,400);
  C.SetRightMargin(0.15);
  C.SetTopMargin(0.05);
  TString ZTitle="# of hits / 0.1 x 0.1";


  TString outname=outpath+"/EventDisplay_CellsEtaPhi"+(side==1?"P":"N")+"_"+tag;

  TH2F HRecoCells("HRecoCells","",63,-TMath::Pi(),TMath::Pi(),30,side==1?2:-5,side==1?5:-2); 
  HRecoCells.GetXaxis()->SetTitle("#phi"); 
  HRecoCells.GetYaxis()->SetTitle("#eta");
  HRecoCells.GetZaxis()->SetTitle(ZTitle);
  HRecoCells.GetZaxis()->SetTitleOffset(0.7);
  //HRecoCells.GetXaxis()->SetNdivisions(0);
  //HRecoCells.GetYaxis()->SetNdivisions(0);
  Tuple->Draw("cell_eta:cell_phi>>HRecoCells",Selection,""); 

  
  ////// show the jets as circles of dR=0.4
  TH2F HRecoJets("HRecoJets","",1000,-TMath::Pi(),TMath::Pi(),1000,side==1?2:-5,side==1?5:-2); 
  Tuple->Draw("jet_eta:jet_phi>>HRecoJets"); 
  HRecoJets.SetMarkerColor(1);
  HRecoJets.SetMarkerStyle(24);
  //HRecoJets.SetMarkerSize(5);//0.2
  HRecoJets.SetMarkerSize(10);//0.4
  //HRecoJets.SetLineWidth(8);
  
  // HRecoJets.Fill(-3.14158,2.0); //check circle size

  C.Clear();    
  HRecoCells.Draw("HIST COlZ");
  HRecoJets.Draw("same");
  C.Print(outname+".png");    

}


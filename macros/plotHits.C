

/////////////////////////////////////////////////////////////////
///////////Hit plots
/////////////////////////////////////////////////////////////////
void plotRawHitSpatial(TChain*TREE,TString outpath){
  TCanvas C;

  C.Clear();
  TH2F HHitXY("HHitXY","",240,-600,600,240,-600,600);
  HHitXY.GetXaxis()->SetTitle("Hit X [mm]");
  HHitXY.GetYaxis()->SetTitle("Hit Y [mm]");
  TREE->Draw("hit_y:hit_x>>HHitXY");
  HHitXY.Draw("colz");
  C.Print(outpath+"/HitXY.png");

  C.Clear();
  TH1F HHitZ("HHitZ","",500,3430,3530);
  HHitZ.GetXaxis()->SetTitle("Hit Z [mm]");
  TREE->Draw("abs(hit_z)>>HHitZ");
  HHitZ.Draw("colz");
  C.Print(outpath+"/HitZ.png");
 
  C.Clear();
  TH1F HHitS("HHitS","",24,-5.5,5.5);
  HHitS.GetXaxis()->SetTitle("layer #");
  HHitS.GetYaxis()->SetTitle("# of hits");
  TREE->Draw("hit_S>>HHitS");
  HHitS.Draw("colz");
  C.Print(outpath+"/HitS.png");
  
  TH1F HHitEta("HHitEta","",60,2,5);
  HHitEta.GetXaxis()->SetTitle("Hit |eta|");
  TREE->Draw("hit_eta>>HHitEta");
  HHitEta.Draw("hist");
  C.Print(outpath+"/HitEta.png");

  TH1F HHitR("HHitR","",120,0,600);
  HHitR.GetXaxis()->SetTitle("Hit radius");
  TREE->Draw("hit_r>>HHitR");
  HHitR.Draw("hist");
  C.Print(outpath+"/HitR.png");

  TH2F HHitRVsL("HHitRVsL","",11,-5.5,5.5,120,0,600);
  HHitRVsL.GetYaxis()->SetTitle("Hit radius (5mm)");
  HHitRVsL.GetXaxis()->SetTitle("layer #");
  TREE->Draw("sqrt(hit_y*hit_y+hit_x*hit_x):hit_S>>HHitRVsL");
  HHitRVsL.Draw("colz");
  C.Print(outpath+"/HitRVsL.png");
}


void plotRawHitTime(TChain*TREE,TString outpath){
  TCanvas C;

  /////////////////////////////////////////
  ///Hit raw time distribution
  C.Clear();
  TH1F HHitT("HHitT","",40,-10,150);
  HHitT.GetXaxis()->SetTitle("Hit time [ns]");
  TREE->Draw("hit_t>>HHitT");
  HHitT.Draw("hist");
  C.Print(outpath+"/HitT.png");
  C.SetLogy(1);
  C.Print(outpath+"/HitT_log.png");
  C.SetLogy(0);


  ///Hit distance to primary vertex
  C.Clear();
  TH1F HHitD("HHitD","",100,3000,4000);
  HHitD.GetXaxis()->SetTitle("Hit distance to prim. vertex [mm]");
  TREE->Draw("hit_D>>HHitD");
  HHitD.Draw("hist");
  C.Print(outpath+"/HitD.png");

  ///Hit delta T distribution
  C.Clear();
  TH1F HHitDT("HHitDT","",40,-10,150);
  HHitDT.GetXaxis()->SetTitle("Hit time [ns]");
  HHitDT.SetLineColor(2);
  HHitDT.SetMarkerSize(0.5);
  TREE->Draw("hit_dt>>HHitDT");
  HHitDT.Draw("hist");
  HHitT.Draw("histsame");
  C.Print(outpath+"/HitDT.png");
  C.SetLogy(1);
  C.Print(outpath+"/HitDT_log.png");
  C.SetLogy(0);
  cout<<"HHitT Mean = "<<HHitT.GetMean()<<" RMS="<<HHitT.GetRMS()<<endl;
  cout<<"HHitDT Mean = "<<HHitDT.GetMean()<<" RMS="<<HHitDT.GetRMS()<<endl;


}

void plotRawHitEnergy(TChain*TREE,TString outpath){
  TCanvas C;

  //////////////////////////////
  ///Hit energy 
  C.Clear();
  TH1F HHitE("HHitE","",100,0,10);
  HHitE.GetXaxis()->SetTitle("Hit energy [MeV]");
  TREE->Draw("hit_E>>HHitE");
  HHitE.Draw("hist");
  C.Print(outpath+"/HitE.png");
  C.SetLogy(1);
  C.Print(outpath+"/HitE_log.png");
  C.SetLogy(0);

  C.Clear();
  TH1F HHitELow("HHitELow","",100,0,1);
  HHitELow.GetXaxis()->SetTitle("Hit energy [MeV]");
  TREE->Draw("hit_E>>HHitELow");
  HHitELow.GetYaxis()->SetRangeUser(0.1,HHitELow.GetMaximum()*1.4);
  HHitELow.Draw("hist");
  line.DrawLine(0.05,0.1,0.05,HHitELow.GetMaximum());
  C.Print(outpath+"/HitELow.png");
  C.SetLogy(1);
  C.Print(outpath+"/HitELow_log.png");
  C.SetLogy(0);


  ///Hit delta T vs energy correlation
  C.Clear();
  TH2F HHitDTvsE("HHitDTvsE","",100,-1,20,100,0,10);
  HHitDTvsE.GetXaxis()->SetTitle("Hit time - expected time [ns]");
  HHitDTvsE.GetYaxis()->SetTitle("Hit energy [MeV]");
  TREE->Draw("hit_E:hit_dt>>HHitDTvsE");
  HHitDTvsE.Draw("colz");
  C.SetLogz(1);
  C.Print(outpath+"/HitDTvsE.png");
  C.SetLogz(0);

}

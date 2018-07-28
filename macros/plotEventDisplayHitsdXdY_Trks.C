void plotEventDisplayHitsdXdY_Trks(TTree *Tuple, TString outpath,TString tag, TString Selection){
  
  TCanvas C("CdEtadPhi","",800,800);
  C.SetRightMargin(0.15);
  C.SetTopMargin(0.15);



  TH2F HTrkPtEta("HTrkPtEta_Trk","",100,0,50,100,-5,5);
  HTrkPtEta.GetYaxis()->SetTitle("track eta");
  HTrkPtEta.GetXaxis()->SetTitle("track pT   [GeV]");
  Tuple->Draw("trk_eta:trk_pt/1000>>HTrkPtEta_Trk"); 
  C.Clear();  
  HTrkPtEta.Draw("col");
  C.Print(outpath+"/EventDisplay_TrksPtEta_"+tag+".png");


  int nbins=20;
  float range=10;
  TString dEtaTrk="(cell_y-trk_hgtd0_y)"; //Use the first track
  TString dPhiTrk="(cell_x-trk_hgtd0_x)";

  
  // TH2F HCells("HCells_Trk","",nbins,-range,range,nbins,-range,range);
  // HCells.GetYaxis()->SetTitle("y_{cell} - y_{trk}   [mm]");
  // HCells.GetXaxis()->SetTitle("x_{cell} - x_{trk}   [mm]");
  // Tuple->Draw(dEtaTrk+":"+dPhiTrk+">>HCells_Trk",Selection); 
  // C.Clear();  
  // HCells.GetZaxis()->SetTitle("# of hits");
  // HCells.Draw("col");
  // C.Print(outpath+"/EventDisplay_HitsdXdYTrks_"+tag+".png");


  TH2F HTrksXY("HTrksXY_Trk","",1200,-600,600,1200,-600,600);
  HTrksXY.GetYaxis()->SetTitle("y   [mm]");
  HTrksXY.GetXaxis()->SetTitle("x   [mm]");
  Tuple->Draw("trk_hgtd0_y:trk_hgtd0_x>>HTrksXY_Trk"); 
  C.Clear();  
  HTrksXY.Draw("scat");
  drawSensors(1,1);
  drawSensors(-1,1);
  C.Print(outpath+"/EventDisplay_TrksXY_"+tag+".png");


  TH2F HCells("HCells","",1200,-600,600,1200,-600,600);
  HCells.GetYaxis()->SetTitle("y  [mm]");
  HCells.GetXaxis()->SetTitle("x  [mm]");
  Tuple->Draw("cell_y:cell_x>>HCells","(abs(cell_S)==1)"); 
  HCells.SetMarkerColor(2);
  HCells.SetMarkerSize(0.3);
  C.Clear();  
  HTrksXY.Draw("scat");
  HCells.Draw("scatsame");
  drawSensors(1,1);
  drawSensors(-1,1);
  C.Print(outpath+"/EventDisplay_Trks_CellsXY_"+tag+".png");


  C.Clear();  
  HTrksXY.GetXaxis()->SetRangeUser(0,600);
  HTrksXY.GetYaxis()->SetRangeUser(0,600);
  HTrksXY.Draw("scat");
  HCells.Draw("scatsame");
  //drawSensors(1,1);
  //drawSensors(-1,1);
  C.Print(outpath+"/EventDisplay_Trks_CellsXY_Q1_"+tag+".png");



}



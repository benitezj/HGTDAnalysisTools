
void plotEventDisplayTrksXY(TTree *Tuple, TString outpath,TString tag, TString SelectionXY){
  TCanvas C("C","",700,780);
  long side=1;

  TString outname=outpath+"/EventDisplay_TrksXY_"+tag;
  
  int nxybins=144;
  int xymin=-650;
  int xymax=650;
  TString XYZTitle="# of hits / 9mm x 9mm";

  int extrafortitle=126;//applied to top 
  int extrafortitlebins=14;//keep same binning

  TH2F HRecoCellsXY("HRecoCellsXY","",nxybins,xymin,xymax,nxybins+extrafortitlebins,xymin,xymax+extrafortitle); 
  HRecoCellsXY.GetXaxis()->SetTitle("x [mm]"); 
  HRecoCellsXY.GetYaxis()->SetTitle("y [mm]");
  HRecoCellsXY.GetZaxis()->SetTitle(XYZTitle);
  HRecoCellsXY.GetXaxis()->SetNdivisions(7);
  HRecoCellsXY.GetYaxis()->SetNdivisions(9);
  HRecoCellsXY.SetMarkerSize(0.5);
  HRecoCellsXY.SetMarkerColor(outpath.Contains("mu0")?2:1);


  C.Clear();    
  Tuple->Draw("trk_hgtd0_y:trk_hgtd0_x>>HRecoCellsXY",SelectionXY+"&&(trk_eta>0)",""); 
  
  //drawCellGridDetGeo();
  drawSensors(1,1);
  drawSensors(-1,1);
  DrawATLAS(0.20,0.89);
  //DrawHGTD(outpath,0,1,0.20,0.835);
  //DrawVBFH(outpath);
  //DrawECMSMu(outpath);
  
  
  // text.SetTextColor(1);
  // text.SetTextSize(0.035);
  // text.DrawTextNDC(0.76,0.84,side==1?"z = 3.5 m":"z = -3.5 m");
  // text.DrawLatexNDC(0.76,0.81,side==1?"2.5<#eta<4.2":"-4.2<#eta<-2.5");
  


  C.Print(outname+".png");    
  //C.Print(outname+".eps");    
  //C.Print(outname+".pdf");    





  int nxybinsQ1=1000;
  int xyminQ1=280;
  int xymaxQ1=450;

  TH2F HRecoCellsXYQ1("HRecoCellsXYQ1","",nxybinsQ1,xyminQ1,xymaxQ1,nxybinsQ1,xyminQ1,xymaxQ1); 
  HRecoCellsXYQ1.GetXaxis()->SetTitle("x [mm]"); 
  HRecoCellsXYQ1.GetYaxis()->SetTitle("y [mm]");
  HRecoCellsXYQ1.GetZaxis()->SetTitle(XYZTitle);
  HRecoCellsXYQ1.GetXaxis()->SetNdivisions(7);
  HRecoCellsXYQ1.GetYaxis()->SetNdivisions(9);
  HRecoCellsXYQ1.SetMarkerSize(0.5);
  HRecoCellsXYQ1.SetMarkerColor(1);
  C.Clear();    
  Tuple->Draw("trk_hgtd0_y:trk_hgtd0_x>>HRecoCellsXYQ1",SelectionXY+"&&(trk_eta>0)",""); 
  drawSensors(1,0);
  DrawATLAS(0.20,0.89);
  C.Print(outname+"_Q1.png");    



}



void plotEventDisplayHitsXY(TTree *Tuple, TString outpath,TString tag, TString SelectionXY, long side){
  TCanvas C("C","",700,780);

  TString outname=outpath+"/EventDisplay_CellsXY"+(side>=0?"P":"N")+"_"+tag;
  
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
  Tuple->Draw("cell_y:cell_x>>HRecoCellsXY",SelectionXY+"&&(cell_S=="+side+"||"+side+"==0)",""); 
  
  //drawCellGridDetGeo();
 
  ////old 1mm + 3mm (ASU) geometry
  // drawSensors(1,1);
  // drawSensors(-1,1);
  
  drawSensors1mmGeometry(1);


  DrawATLAS(0.18,0.89);
  DrawHGTD(outpath,0,-1,0.18,0.835);
  DrawVBFH(outpath);
  DrawECMSMu(outpath);
  
  
  text.SetTextColor(1);
  text.SetTextSize(0.030);

  //text.DrawTextNDC(0.77,0.87,side>=1?"z = 3.5 m":"z = -3.5 m");
  //text.DrawLatexNDC(0.77,0.83,side>=1?"2.5<#eta<4.2":"-4.2<#eta<-2.5");

  text.DrawTextNDC(0.17,0.20,side>=1?"z = 3.5 m":"z = -3.5 m");
  
  TLatex latex;
  latex.SetTextSize(0.032);
  latex.DrawLatexNDC(0.75,0.86,"jet p_{T}>30 GeV");
  latex.DrawLatexNDC(0.82,0.83,"#DeltaR<0.4");


	
  TFile * InFile=NULL;
  TString outnamemu0=outname;
  if(outpath.Contains("mu200")){
    outnamemu0("mu200")="mu0";
    InFile = new TFile(outnamemu0+".root","read"); 
    if(!InFile->IsZombie()){
      TH1F * H = (TH1F*)InFile->Get("HRecoCellsXY");
      if(H){
	H->SetMarkerColor(2);
	H->SetMarkerSize(0.6);
  	H->Draw("same");
	text.SetTextSize(0.03);
	text.SetTextColor(1);
	text.DrawLatexNDC(0.84,0.23,"#bullet pile-up");
	text.SetTextColor(2);
	text.DrawLatexNDC(0.84,0.20,"#bullet signal");
      }
    }
  }

  C.Print(outname+".png");    
  C.Print(outname+".eps");    
  C.Print(outname+".pdf");    


  //write out the histogram for the mu=200 plot
  TFile OutFile(outname+".root","recreate"); 
  HRecoCellsXY.Write();
  OutFile.Close();
  if(InFile)InFile->Close();



  /////////////////
  ////////Only Q1
  ////////////////
  
  // int nxybinsQ1=650;
  // int xyminQ1=0;
  // int xymaxQ1=650;

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
  HRecoCellsXYQ1.SetMarkerColor(2);
  C.Clear();    
  Tuple->Draw("cell_y:cell_x>>HRecoCellsXYQ1",SelectionXY+"&&(cell_S=="+side+"||"+side+"==0)",""); 
  drawSensors(1,0);
  DrawATLAS(0.20,0.89);
  DrawHGTD(outpath,0,1,0.20,0.835);
  C.Print(outname+"_Q1.png");    


  TH2F HRecoCellsXYQ1L2("HRecoCellsXYQ1L2","",nxybinsQ1,xyminQ1,xymaxQ1,nxybinsQ1,xyminQ1,xymaxQ1); 
  HRecoCellsXYQ1L2.GetXaxis()->SetTitle("x [mm]"); 
  HRecoCellsXYQ1L2.GetYaxis()->SetTitle("y [mm]");
  HRecoCellsXYQ1L2.GetZaxis()->SetTitle(XYZTitle);
  HRecoCellsXYQ1L2.GetXaxis()->SetNdivisions(7);
  HRecoCellsXYQ1L2.GetYaxis()->SetNdivisions(9);
  HRecoCellsXYQ1L2.SetMarkerSize(0.5);
  HRecoCellsXYQ1L2.SetMarkerColor(1);
  C.Clear();    
  Tuple->Draw("cell_y:cell_x>>HRecoCellsXYQ1L2",SelectionXY+"&&(cell_S==2)",""); 
  drawSensors(1,0);
  DrawATLAS(0.20,0.89);
  DrawHGTD(outpath,0,2,0.20,0.835);
  C.Print(outname+"_Q1L2.png");    

  TH2F HRecoCellsXYQ1L3("HRecoCellsXYQ1L3","",nxybinsQ1,xyminQ1,xymaxQ1,nxybinsQ1,xyminQ1,xymaxQ1); 
  HRecoCellsXYQ1L3.GetXaxis()->SetTitle("x [mm]"); 
  HRecoCellsXYQ1L3.GetYaxis()->SetTitle("y [mm]");
  HRecoCellsXYQ1L3.GetZaxis()->SetTitle(XYZTitle);
  HRecoCellsXYQ1L3.GetXaxis()->SetNdivisions(7);
  HRecoCellsXYQ1L3.GetYaxis()->SetNdivisions(9);
  HRecoCellsXYQ1L3.SetMarkerSize(0.5);
  HRecoCellsXYQ1L3.SetMarkerColor(3);
  C.Clear();    
  Tuple->Draw("cell_y:cell_x>>HRecoCellsXYQ1L3",SelectionXY+"&&(cell_S==3)",""); 
  drawSensors(1,0);
  DrawATLAS(0.20,0.89);
  DrawHGTD(outpath,0,3,0.20,0.835);
  C.Print(outname+"_Q1L3.png");    

  TH2F HRecoCellsXYQ1L4("HRecoCellsXYQ1L4","",nxybinsQ1,xyminQ1,xymaxQ1,nxybinsQ1,xyminQ1,xymaxQ1); 
  HRecoCellsXYQ1L4.GetXaxis()->SetTitle("x [mm]"); 
  HRecoCellsXYQ1L4.GetYaxis()->SetTitle("y [mm]");
  HRecoCellsXYQ1L4.GetZaxis()->SetTitle(XYZTitle);
  HRecoCellsXYQ1L4.GetXaxis()->SetNdivisions(7);
  HRecoCellsXYQ1L4.GetYaxis()->SetNdivisions(9);
  HRecoCellsXYQ1L4.SetMarkerSize(0.5);
  HRecoCellsXYQ1L4.SetMarkerColor(4);
  C.Clear();    
  Tuple->Draw("cell_y:cell_x>>HRecoCellsXYQ1L4",SelectionXY+"&&(cell_S==4)",""); 
  drawSensors(1,0);
  DrawATLAS(0.20,0.89);
  DrawHGTD(outpath,0,3,0.20,0.835);
  C.Print(outname+"_Q1L4.png");    


  C.Clear();    
  HRecoCellsXYQ1L4.Draw("p");
  HRecoCellsXYQ1L3.Draw("psame");
  HRecoCellsXYQ1L2.Draw("psame");
  HRecoCellsXYQ1.Draw("psame");
  drawSensors(1,0);
  DrawATLAS(0.20,0.89);
  DrawHGTD(outpath,0,0,0.20,0.835);
  C.Print(outname+"_Q1LAll.png");    



}


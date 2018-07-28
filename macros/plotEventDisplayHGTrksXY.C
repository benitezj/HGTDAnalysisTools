
void plotEventDisplayHGTrksXY(TTree *Tuple, TString outpath,TString tag, TString SelectionXY){
  TCanvas C("C","",700,780);
  long side=1;

  TString outname=outpath+"/EventDisplay_HGTrksXY_"+tag;
  
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
  Tuple->Draw("hgtrk_y:hgtrk_x>>HRecoCellsXY",SelectionXY+"&&(hgtrk_eta>0)",""); 
  
  //drawCellGridDetGeo();
  drawSensors(1,1);
  drawSensors(-1,1);
  DrawATLAS(0.20,0.89);
  DrawHGTD(outpath,0,1,0.20,0.835);
  DrawVBFH(outpath);
  DrawECMSMu(outpath);
  
  
  // text.SetTextColor(1);
  // text.SetTextSize(0.035);
  // text.DrawTextNDC(0.76,0.84,side==1?"z = 3.5 m":"z = -3.5 m");
  // text.DrawLatexNDC(0.76,0.81,side==1?"2.5<#eta<4.2":"-4.2<#eta<-2.5");
  


  C.Print(outname+".png");    
  //C.Print(outname+".eps");    
  //C.Print(outname+".pdf");    




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
  Tuple->Draw("hgtrk_y:hgtrk_x>>HRecoCellsXYQ1",SelectionXY+"&&(hgtrk_eta>0)",""); 
  drawSensors(1,0);
  DrawATLAS(0.20,0.89);
  DrawHGTD(outpath,0,1,0.20,0.835);
  C.Print(outname+"_Q1.png");    


  TH2F HRecoCellsXYQ1Eta("HRecoCellsXYQ1Eta","",nxybinsQ1,xyminQ1,xymaxQ1,nxybinsQ1,xyminQ1,xymaxQ1); 
  Tuple->Draw("hgtrk_y:hgtrk_x>>HRecoCellsXYQ1Eta",SelectionXY+"*(hgtrk_eta>0)*(hgtrk_deta)"); 

  TH2F HRecoCellsXYQ1Phi("HRecoCellsXYQ1Phi","",nxybinsQ1,xyminQ1,xymaxQ1,nxybinsQ1,xyminQ1,xymaxQ1); 
  Tuple->Draw("hgtrk_y:hgtrk_x>>HRecoCellsXYQ1Phi",SelectionXY+"*(hgtrk_eta>0)*(hgtrk_dphi)"); 


  C.Clear(); 
  HRecoCellsXYQ1.Draw("p");   
  drawSensors(1,0);
  DrawATLAS(0.20,0.89);
  DrawHGTD(outpath,0,1,0.20,0.835);

  TLine line;
  line.SetLineColor(1);
  for(int bx=1;bx<=HRecoCellsXYQ1.GetXaxis()->GetNbins();bx++){
    for(int by=1;by<=HRecoCellsXYQ1.GetYaxis()->GetNbins();by++){
      if(HRecoCellsXYQ1.GetBinContent(bx,by)>0){

	TVector3 Vec;
	Vec.SetPtEtaPhi(1,HRecoCellsXYQ1Eta.GetBinContent(bx,by),HRecoCellsXYQ1Phi.GetBinContent(bx,by));
	float dist= HRecoCellsXYQ1Eta.GetBinContent(bx,by) > 0 ? 4*8.2/Vec.Z() : 0;

	line.DrawLine(HRecoCellsXYQ1.GetXaxis()->GetBinCenter(bx),
		      HRecoCellsXYQ1.GetYaxis()->GetBinCenter(by),
		      HRecoCellsXYQ1.GetXaxis()->GetBinCenter(bx)+dist*Vec.X(),
		      HRecoCellsXYQ1.GetYaxis()->GetBinCenter(by)+dist*Vec.Y());

      }
    }
  }

  C.Print(outname+"_Dir_Q1.png");    



  C.Clear();    
  HRecoCellsXYQ1Eta.Draw("text");
  C.Print(outname+"_Values.png");    

}


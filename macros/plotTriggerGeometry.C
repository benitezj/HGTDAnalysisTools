#include "HGTDAnalysisTools/macros/globals.h"
//#include "HGTDAnalysisTools/macros/plotEventDisplay.C"
#include "HGTDAnalysisTools/macros/drawCellGridDetGeo.C"

void plotTriggerGeometry(){
  SetAtlasStyle();
  gStyle->SetPalette(1,0);

  TCanvas C("C","",700,700);
  C.SetTopMargin(0.15);
  C.SetRightMargin(0.15);
  C.SetLeftMargin(0.15);
  C.SetBottomMargin(0.15);

  int nxybins=1;
  int xymax=650;

  TH2F HFrame("HFrame","",nxybins,0,xymax,nxybins,0,xymax); 
  HFrame.GetXaxis()->SetTitle("x [mm]"); 
  HFrame.GetYaxis()->SetTitle("");//y [mm]");
  HFrame.GetZaxis()->SetTitle("");
  HFrame.GetXaxis()->SetNdivisions(7);
  HFrame.GetYaxis()->SetNdivisions(9);
  HFrame.SetMarkerSize(0.3);
  HFrame.GetXaxis()->SetLabelSize(0.03);
  HFrame.GetYaxis()->SetLabelSize(0.03);

  TGaxis EtaAxis(-70,FEtaVsR.GetXmin(),-70,FEtaVsR.GetXmax(),"FRVsEta");
  EtaAxis.SetNdivisions(11);
  EtaAxis.SetLabelSize(0.03);


  ////First just draw one quadrant and eta axis
  C.Clear();    
  HFrame.Draw("");
  drawTriggerTiles(1);
  EtaAxis.Draw();
  C.Print(OUTDIR+"/TriggerGeometry.png");


  //////
  TH2F HFrameFull("HFrameFull","",nxybins,-xymax,xymax,nxybins,-xymax,xymax); 
  HFrameFull.GetXaxis()->SetTitle("x [mm]"); 
  HFrameFull.GetYaxis()->SetTitle("y [mm]");
  HFrameFull.GetZaxis()->SetTitle("");
  HFrameFull.GetXaxis()->SetNdivisions(7);
  HFrameFull.GetYaxis()->SetNdivisions(9);
  HFrameFull.SetMarkerSize(0.3);
  HFrameFull.GetXaxis()->SetLabelSize(0.04);
  HFrameFull.GetYaxis()->SetLabelSize(0.04);
  C.Clear();    
  HFrameFull.Draw("");
  drawTriggerTiles(1,1);
  drawTriggerTiles(-1,1);
  C.Print(OUTDIR+"/TriggerGeometry_Full.png");


  // float r1;
  // float r2;
  // // ///Print the tile sizes in eta distances:
  // // for(int r=0;r<5;r++){
  // //   r1=(0.5+r*0.5)*ASU;
  // //   r2=(0.5+(r+1)*0.5)*ASU;
  // //   int divisor=(6-r);
  // //   cout<<"eta1="<<FEtaVsR.Eval(r2)<<", eta2="<<FEtaVsR.Eval(r1)<<": "<< (FEtaVsR.Eval(r1)-FEtaVsR.Eval(r2))/divisor<<endl;
  // // }
  
  // r1=(0.5)*ASU;  r2=(1.0)*ASU;
  // cout<<"eta1="<<FEtaVsR.Eval(r2)<<", eta2="<<FEtaVsR.Eval(r1)<<": "<< (FEtaVsR.Eval(r1)-FEtaVsR.Eval(r2))/3<<endl;

  // r1=(1.0)*ASU;  r2=(1.5)*ASU;
  // cout<<"eta1="<<FEtaVsR.Eval(r2)<<", eta2="<<FEtaVsR.Eval(r1)<<": "<< (FEtaVsR.Eval(r1)-FEtaVsR.Eval(r2))/2<<endl;

  // r1=(1.5)*ASU;  r2=(2.0)*ASU;
  // cout<<"eta1="<<FEtaVsR.Eval(r2)<<", eta2="<<FEtaVsR.Eval(r1)<<": "<< (FEtaVsR.Eval(r1)-FEtaVsR.Eval(r2))/2<<endl;

  // r1=(2.0)*ASU;  r2=(2.5)*ASU;
  // cout<<"eta1="<<FEtaVsR.Eval(r2)<<", eta2="<<FEtaVsR.Eval(r1)<<": "<< (FEtaVsR.Eval(r1)-FEtaVsR.Eval(r2))/1<<endl;

  // r1=(2.5)*ASU;  r2=(3.0)*ASU;
  // cout<<"eta1="<<FEtaVsR.Eval(r2)<<", eta2="<<FEtaVsR.Eval(r1)<<": "<< (FEtaVsR.Eval(r1)-FEtaVsR.Eval(r2))/1<<endl;


}

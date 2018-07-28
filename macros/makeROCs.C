#include "HGTDAnalysisTools/macros/globals.h"
//#include "HGTDAnalysisTools/macros/plotJets.C"

void DrawdT(TTree*TREE,TString outpath,TString dtvariable,TString CellSelection, TString Tag){
  TCanvas C;

  TString dtpeak=TString("(cell_dt-")+dtvariable+"[cell_jet])*1000";

  for(long e=0;e<ETABINS;e++){
    for(long p=0;p<PTBINS;p++){
      char cuttxt[100];
      sprintf(cuttxt,"%.2f",ETAMIN+(e)*(ETAMAX-ETAMIN)/ETABINS);
      etamin=cuttxt;
      sprintf(cuttxt,"%.2f",ETAMIN+(e+1)*(ETAMAX-ETAMIN)/ETABINS);
      etamax=cuttxt;
      jetPtL=PTMIN+(p)*(PTMAX-PTMIN)/PTBINS;
      jetPtH=PTMIN+(p+1)*(PTMAX-PTMIN)/PTBINS;
      setCuts(outpath);


      TH1F HSignal("HSignal","",35,-500,500);
      TREE->Draw(dtpeak+">>HSignal",CUTSignalJetCell+"*"+CUTEtaJetCell+"*"+CUTPtJetCell+"*"+CellSelection);

      TH1F HPU("HPU","",35,-500,500);
      TREE->Draw(dtpeak+">>HPU",CUTPUJetCell+"*"+CUTEtaJetCell+"*"+CUTPtJetCell+"*"+CellSelection);

      HSignal.Scale(1./HSignal.Integral());
      HPU.Scale(1./HPU.Integral());

      C.Clear();
      HSignal.GetXaxis()->SetTitle("t - t_{exp} - t_{peak}    [ps]");
      HSignal.GetYaxis()->SetTitle("# of hits (unit norm)");
      HSignal.GetYaxis()->SetRangeUser(0,1.3*HSignal.GetMaximum());
      HSignal.Draw("histp");
      HPU.Draw("histsame");

      // line.SetLineColor(2);
      // line.DrawLine(-TIMEWINDOW,0,-TIMEWINDOW,HSignal.GetBinContent(HSignal.GetMaximumBin()));
      // line.DrawLine(TIMEWINDOW,0,TIMEWINDOW,HSignal.GetBinContent(HSignal.GetMaximumBin()));

      DrawDiscriminatorLegend(&HSignal,&HPU);
      DrawHGTD(outpath);
      //DrawATLAS(200,outpath);
      C.Print(outpath+"/dT_"+Tag+"_"+e+"_"+p+".png");
      //C.Print(outpath+"/dT_"+Tag+".eps");

    }  
  }


}


void DrawDiscriminator(TTree*TREE, TString outpath, TString Discriminator, int nbins, float xmin, float xmax, TString Tag){

  //TCanvas C;

  ///draw for the different pT and eta slices
  for(long e=0;e<ETABINS;e++){
    for(long p=0;p<PTBINS;p++){
      char cuttxt[100];

      float etalow=ETAMIN+(e)*(ETAMAX-ETAMIN)/ETABINS;
      float etahigh=ETAMIN+(e+1)*(ETAMAX-ETAMIN)/ETABINS;

      sprintf(cuttxt,"%.2f",etalow);
      etamin=cuttxt;
      sprintf(cuttxt,"%.2f",etahigh);
      etamax=cuttxt;

      jetPtL=PTMIN+(p)*(PTMAX-PTMIN)/PTBINS;
      jetPtH=PTMIN+(p+1)*(PTMAX-PTMIN)/PTBINS;

      setCuts(outpath);

      TH1F HSignal("HSignal","",nbins,xmin,xmax);
      HSignal.Sumw2();
      TREE->Draw(Discriminator+">>HSignal",signaljetCUT+"*"+JetEtaCut+"*"+JetPtCut);
 
      TH1F HPU("HPU","",nbins,xmin,xmax);
      HPU.Sumw2();
      TREE->Draw(Discriminator+">>HPU",stocjetCUT+"*"+JetEtaCut+"*"+JetPtCut);
 
      HSignal.Scale(1./HSignal.Integral());
      HPU.Scale(1./HPU.Integral());
  
      C1D.Clear();
      HPU.GetYaxis()->SetTitle(" # of jets (unit norm) ");
      HPU.GetXaxis()->SetTitle("# of hits at maximum bin");
      HPU.GetYaxis()->SetRangeUser(0,1.3*(HPU.GetMaximum()>HSignal.GetMaximum()?HPU.GetMaximum():HSignal.GetMaximum()));
      HPU.SetMarkerColor(4);
      HPU.SetLineColor(4);
      HPU.SetLineStyle(2);
      HPU.Draw("hist");
      HSignal.SetMarkerColor(2);
      HSignal.SetLineColor(2);
      HSignal.Draw("histsame");


      DrawATLAS(0.2,0.89);
      DrawHGTD(outpath,0,-1,0.2,0.83);
      DrawVBFH(outpath);
      DrawECMSMu(outpath);
      DrawDiscriminatorLegend(&HSignal,&HPU);

      C1D.Print(outpath+"/Discriminator_"+Tag+"_"+e+"_"+p+".png");
      C1D.Print(outpath+"/Discriminator_"+Tag+"_"+e+"_"+p+".pdf");
      C1D.Print(outpath+"/Discriminator_"+Tag+"_"+e+"_"+p+".eps");



    }
  }




  return;
}





void ScanDiscriminator(TChain*TREE,TString outpath,TString Discriminator,TString unit,TString Tag){
  
  TH2F HSignal("HSignal","",ETABINS,ETAMIN,ETAMAX,PTBINS,PTMIN/1000,PTMAX/1000);
  TREE->Draw("jet_pt/1000:abs(jet_eta)>>HSignal",signaljetCUT);

  TH2F HPU("HPU","",ETABINS,ETAMIN,ETAMAX,PTBINS,PTMIN/1000,PTMAX/1000);
  TREE->Draw("jet_pt/1000:abs(jet_eta)>>HPU",pujetCUT);

  TH2F HQCD("HQCD","",ETABINS,ETAMIN,ETAMAX,PTBINS,PTMIN/1000,PTMAX/1000);
  TREE->Draw("jet_pt/1000:abs(jet_eta)>>HQCD",qcdjetCUT);

  TH2F HSTOC("HSTOC","",ETABINS,ETAMIN,ETAMAX,PTBINS,PTMIN/1000,PTMAX/1000);
  TREE->Draw("jet_pt/1000:abs(jet_eta)>>HSTOC",stocjetCUT);

  TFile ROCFile(outpath+"/Discriminator_"+Tag+".root","recreate");
  gROOT->cd();
  setROCTree(&ROCFile);

  ///Apply selection
  cout<<"scan "<<Tag<<": ";
  for(roc_nhit=1;roc_nhit<NROCSCANPOINTS;roc_nhit++){
    //cout<<roc_nhit<<",";
    TString cut=TString("(")+Discriminator+">="+(long)(roc_nhit)+"*"+unit+")";//save the cut on the % value
    
    TH2F HSignal_CUT(TString("HSignal_")+(long)roc_nhit,"",ETABINS,ETAMIN,ETAMAX,PTBINS,PTMIN/1000,PTMAX/1000);
    TREE->Draw(TString("jet_pt/1000:abs(jet_eta)>>HSignal_")+(long)roc_nhit,signaljetCUT+"*"+cut);

    TH2F HPU_CUT(TString("HPU_")+(long)roc_nhit,"",ETABINS,ETAMIN,ETAMAX,PTBINS,PTMIN/1000,PTMAX/1000);
    TREE->Draw(TString("jet_pt/1000:abs(jet_eta)>>HPU_")+(long)roc_nhit,pujetCUT+"*"+cut);

    TH2F HQCD_CUT(TString("HQCD_")+(long)roc_nhit,"",ETABINS,ETAMIN,ETAMAX,PTBINS,PTMIN/1000,PTMAX/1000);
    TREE->Draw(TString("jet_pt/1000:abs(jet_eta)>>HQCD_")+(long)roc_nhit,qcdjetCUT+"*"+cut);

    TH2F HSTOC_CUT(TString("HSTOC_")+(long)roc_nhit,"",ETABINS,ETAMIN,ETAMAX,PTBINS,PTMIN/1000,PTMAX/1000);
    TREE->Draw(TString("jet_pt/1000:abs(jet_eta)>>HSTOC_")+(long)roc_nhit,stocjetCUT+"*"+cut);


    
    for(int e=1;e<=ETABINS;e++){
      for(int p=1;p<=PTBINS;p++){
	roc_eta = HSignal.GetXaxis()->GetBinCenter(e);
	roc_pt = HSignal.GetYaxis()->GetBinCenter(p);

	roc_eff_sig = HSignal_CUT.GetBinContent(e,p) / HSignal.GetBinContent(e,p);
	roc_eff_sig_err = HSignal_CUT.GetBinError(e,p) / HSignal.GetBinContent(e,p) ;

	roc_eff_pu = HPU_CUT.GetBinContent(e,p) / HPU.GetBinContent(e,p);
	roc_eff_pu_err = HPU_CUT.GetBinError(e,p) / HPU.GetBinContent(e,p) ;

	roc_eff_qcd = HQCD_CUT.GetBinContent(e,p) / HQCD.GetBinContent(e,p);
	roc_eff_qcd_err = HQCD_CUT.GetBinError(e,p) / HQCD.GetBinContent(e,p) ;

	roc_eff_stoc = HSTOC_CUT.GetBinContent(e,p) / HSTOC.GetBinContent(e,p);
	roc_eff_stoc_err = HSTOC_CUT.GetBinError(e,p) / HSTOC.GetBinContent(e,p) ;
	
	//cout<<e<<" "<<p<<" "<<cut<<endl;
	ROCTree->Fill();
      }
    }
  }
  cout<<"done"<<endl;

  cout<<"Number of ROC tree events : "<<ROCTree->GetEntries()<<endl;
  ROCFile.cd();
  ROCTree->Write();
  ROCFile.ls();
  ROCFile.Close();

}



void makeROCs(TString SAMPLE="June29/SiHit_June15_mu200_3CELLSIZE_60CELLENERGY_10TIMESMEAR/mc15_13TeV.301399.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_ZZ4nu.evgen.EVNT.e4397", 
	      TString WINDOW="45"){
  gROOT->ProcessLine(".x ./HGTDAnalysisTools/macros/rootlogon.C");

  setCuts(SAMPLE);

  /////
  loadChain(INDIR+"/"+SAMPLE);
  if(!TREE || TREE->GetEntries()<=0){
    cout<<"TREE was not loaded properly. "<<endl;
    return;
  }

  ////clean the output path
  TString outpath=OUTDIR+"/ROC/"+SAMPLE+"_Window"+WINDOW;
  system((TString("rm -rf ")+outpath).Data());
  system((TString("mkdir -p ")+outpath).Data());


  TString discriminator;
  TString discname;
  TString cellselection;
  TString celldR="0.20";
 

  /////////////////////////////////////////////////////////////
  ////// Using jet core cells
  /////////////////////////////////////////////////////////////
  discname="dTFitCoreNcell";
  discriminator=TString("jet_dTFitCore_ncell_window")+WINDOW;
  DrawDiscriminator(TREE,outpath,discriminator,20,0,200,discname);
  ScanDiscriminator(TREE,outpath,discriminator,"2",discname);


  /////////////////////////////////////////////////////////////
  ////// Using jet core cells fraction
  /////////////////////////////////////////////////////////////
  // discname="dTFitCore";
  // discriminator=TString("jet_dTFitCore_ncell_window")+WINDOW+"/jet_dTFitCore_ncell";
  // DrawDiscriminator(TREE,outpath,discriminator,40,0.,0.50,"jet_dTFitCore_signalmean",discname);
  // ScanDiscriminator(TREE,outpath,discriminator,"0.003",discname);


  /////////////////////////////////////////////////////////////
  ////// Using jet core hgtracks
  /////////////////////////////////////////////////////////////
  // discname="dTFitCoreHGTrk";
  // discriminator=TString("jet_dTFitCoreHGTrk_ntrk/jet_dTFitCoreHGTrk_ntrktot");
  // DrawDiscriminator(TREE,outpath,discriminator,40,0.,0.50,"jet_dTFitCoreHGTrk_dt",discname);
  // ScanDiscriminator(TREE,outpath,discriminator,"0.003",discname);



  /////////////////////////////////////////////////////////////
  ////// ratio of cells at 0.3<dr<0.4/0.2<dr<0.3
  /////////////////////////////////////////////////////////////
  //discname="dRRatio";
  //discriminator="jet_ncell_dr3";
  //DrawDiscriminator(TREE,outpath,discriminator,40,0,3000,"jet_truedt",discname);

  // discname="dRRatio35over20";
  // discriminator="(jet_ncell_dr35/jet_ncell_dr20)";
  // DrawDiscriminator(TREE,outpath,discriminator,15,0,3,"jet_truedt",discname);
  // //ScanDiscriminator(TREE,outpath,discriminator,"0.15",discname);
 
  // discname="dRRatio35over25";
  // discriminator="(jet_ncell_dr35/jet_ncell_dr25)";
  // DrawDiscriminator(TREE,outpath,discriminator,15,0,3,"jet_truedt",discname);
  // //ScanDiscriminator(TREE,outpath,discriminator,"0.15",discname);

  // discname="dRRatio35over2025";
  // discriminator="(jet_ncell_dr35/(jet_ncell_dr20+jet_ncell_dr25))";
  // DrawDiscriminator(TREE,outpath,discriminator,15,0,3,"jet_truedt",discname);
  // //ScanDiscriminator(TREE,outpath,discriminator,"0.15",discname);

  // discname="dRDensity35over20";
  // discriminator="((jet_ncell_dr35/0.1178)/((jet_ncell_dr20/0.07068)))";
  // DrawDiscriminator(TREE,outpath,discriminator,15,0,3,"jet_truedt",discname);
  // ScanDiscriminator(TREE,outpath,discriminator,"0.15",discname);

  // discname="dRDensity35over25";
  // discriminator="((jet_ncell_dr35/0.1178)/((jet_ncell_dr25/0.08639)))";
  // DrawDiscriminator(TREE,outpath,discriminator,15,0,3,"jet_truedt",discname);
  // ScanDiscriminator(TREE,outpath,discriminator,"0.15",discname);

  // discname="dREdge35";
  // discriminator="(jet_ncell_dr35)";
  // DrawDiscriminator(TREE,outpath,discriminator,15,0,1000,"jet_truedt",discname);
  // ScanDiscriminator(TREE,outpath,discriminator,"6",discname);


  // discname="dRDensity35over2025";
  // discriminator="((jet_ncell_dr35/0.1178)/((jet_ncell_dr20/0.07068+jet_ncell_dr25/0.08639)/2))";
  // DrawDiscriminator(TREE,outpath,discriminator,15,0,3,"jet_truedt",discname);
  // //ScanDiscriminator(TREE,outpath,discriminator,"0.15",discname);


  // discname="dRRatio3035over20";
  // discriminator="((jet_ncell_dr30+jet_ncell_dr35)/jet_ncell_dr20)";
  // DrawDiscriminator(TREE,outpath,discriminator,15,0,3,"jet_truedt",discname);
  // //ScanDiscriminator(TREE,outpath,discriminator,"0.15",discname);
  
  // discname="dRRatio3035over2025";
  // discriminator="((jet_ncell_dr30+jet_ncell_dr35)/(jet_ncell_dr20+jet_ncell_dr25))";
  // DrawDiscriminator(TREE,outpath,discriminator,15,0,3,"jet_truedt",discname);
  // //ScanDiscriminator(TREE,outpath,discriminator,"0.15",discname);
 
  // discname="dRRatio00over15";
  // discriminator="(jet_ncell_dr00/jet_ncell_dr15)";
  // DrawDiscriminator(TREE,outpath,discriminator,15,0,3,"jet_truedt",discname);
  // //ScanDiscriminator(TREE,outpath,discriminator,"0.15",discname);

  // discname="dRRatio0005over15";
  // discriminator="((jet_ncell_dr00+jet_ncell_dr05)/jet_ncell_dr15)";
  // DrawDiscriminator(TREE,outpath,discriminator,15,0,3,"jet_truedt",discname);
  // //ScanDiscriminator(TREE,outpath,discriminator,"0.15",discname);

  // discname="dRRatio0005over1015";
  // discriminator="((jet_ncell_dr00+jet_ncell_dr05)/(jet_ncell_dr10+jet_ncell_dr15))";
  // DrawDiscriminator(TREE,outpath,discriminator,15,0,3,"jet_truedt",discname);
  // //ScanDiscriminator(TREE,outpath,discriminator,"0.15",discname);


  // discname="dRCore000510";
  // discriminator="(jet_ncell_dr00+jet_ncell_dr05+jet_ncell_dr10)";
  // DrawDiscriminator(TREE,outpath,discriminator,15,0,1000,"jet_truedt",discname);
  // ScanDiscriminator(TREE,outpath,discriminator,"5",discname);

  // discname="dRCore00051015";
  // discriminator="(jet_ncell_dr00+jet_ncell_dr05+jet_ncell_dr10+jet_ncell_dr15)";
  // DrawDiscriminator(TREE,outpath,discriminator,15,0,1500,"jet_truedt",discname);
  // ScanDiscriminator(TREE,outpath,discriminator,"9",discname);

  // discname="dRDensity000510over20";
  // discriminator="((jet_ncell_dr00/0.007854+jet_ncell_dr05/0.02356+jet_ncell_dr10/0.03927)/(3*jet_ncell_dr20/0.07068))";
  // DrawDiscriminator(TREE,outpath,discriminator,15,0,3,"jet_truedt",discname);
  // ScanDiscriminator(TREE,outpath,discriminator,"0.2",discname);


  ////Use the 4 points above dR=0.2 (centers: 0.225, 0.275, 0.325 , 0.375)
  //Formula m = [sum(xi*yi) - sum(xi)*yave]/[sum(xi*xi) - sum(xi)*xave]
  // sum(xi) = 1.2         sum(xi*xi)= 0.3725     xave=0.3  => denom = 0.0125
  //ring areas: 0.07068, 0.08639, 0.1021, 0.1178
  // discname="dRslope";
  // discriminator="(40000+((0.225*jet_ncell_dr20/0.07068+0.275*jet_ncell_dr25/0.08639+0.325*jet_ncell_dr30/0.1021+0.375*jet_ncell_dr35/0.1178)-0.3*(jet_ncell_dr20/0.07068+jet_ncell_dr25/0.08639+jet_ncell_dr30/0.1021+jet_ncell_dr35/0.1178))/0.0125)";
  // DrawDiscriminator(TREE,outpath,discriminator,15,0,70000,"jet_truedt",discname);
  // ScanDiscriminator(TREE,outpath,discriminator,"200",discname);

  // discname="dRslopeFit";
  // discriminator="(40000+jet_shape_p3)";
  // DrawDiscriminator(TREE,outpath,discriminator,15,0,70000,"jet_truedt",discname);
  // ScanDiscriminator(TREE,outpath,discriminator,"500",discname);

  //discname="dRslopeComb";
  //discriminator="(80000 + jet_shape_p3 - jet_shape_p1)";
  //DrawDiscriminator(TREE,outpath,discriminator,15,0,140000,"jet_truedt",discname);
  //ScanDiscriminator(TREE,outpath,discriminator,"0.15",discname);

  
  ////Function is not good need to normalize terms due to large difference in y vs. x axis so angles are not at limits.
  //discname="dRKnee";
  //discriminator="(jet_shapeknee_p2-jet_shapeknee_p1)";
  //DrawDiscriminator(TREE,outpath,discriminator,100,-0.5,2.5*3.14159,"jet_truedt",discname);

  //discriminator="(jet_shapeknee_p2)";
  //DrawDiscriminator(TREE,outpath,discriminator,15,0,2*3.14,"jet_truedt",discname);

  //ScanDiscriminator(TREE,outpath,discriminator,"0.15",discname);


  ////Use the 4 points below dR=0.2 (centers: 0.025, 0.075, 0.125 , 0.175)
  //Formula m = [sum(xi*yi) - sum(xi)*yave]/[sum(xi*xi) - sum(xi)*xave]
  // sum(xi) = 0.4         sum(xi*xi)= 0.0525     xave=0.1  => denom = 0.0125
  //ring areas: 0.00785398, 0.02356, 0.03927, 0.05497
  //discname="dRslopeCore";
  //discriminator="(40000-((0.025*jet_ncell_dr00/0.007854+0.075*jet_ncell_dr05/0.02356+0.125*jet_ncell_dr10/0.03927+0.175*jet_ncell_dr15/0.05497)-0.1*(jet_ncell_dr00/0.007854+jet_ncell_dr05/0.02356+jet_ncell_dr10/0.03927+jet_ncell_dr15/0.05497))/0.0125)";
  //DrawDiscriminator(TREE,outpath,discriminator,15,0,100000,"jet_truedt",discname);
  //ScanDiscriminator(TREE,outpath,discriminator,"400",discname);




  ///this is the test with cluster
  // discname="dRRatio35over25Clus";
  // discriminator="(jet_clusE_dr35>0&&jet_clusE_dr25>0)*(jet_clusE_dr35/jet_clusE_dr25)";
  // DrawDiscriminator(TREE,outpath,discriminator,30,0.01,3,"jet_truedt",discname);

  //discname="dRRatio30over20Clus";
  //discriminator="((jet_clusE_dr30+jet_clusE_dr35)>0&&(jet_clusE_dr20+jet_clusE_dr25)>0)*((jet_clusE_dr30+jet_clusE_dr35)/(jet_clusE_dr20+jet_clusE_dr25))";
  //DrawDiscriminator(TREE,outpath,discriminator,15,0.01,4,"jet_truedt",discname);


  /////////////////////////////////////////////////////////////
  ////// dR fit in time slices
  /////////////////////////////////////////////////////////////
  // discname="dRdTFit";
  //cellselection=TString("(cell_jetdR<")+celldR+")";
  //DrawdT(TREE,outpath,"jet_dRdTFit_signalmean",cellselection,discname);

  //discriminator=TString("jet_dRdTFit_sig");
  //DrawDiscriminator(TREE,outpath,discriminator,30,0,4,"jet_dRdTFit_signalmean",discname);

  //discriminator=TString("jet_dRdTFit_sig/jet_dRdTFit_siger");
  //DrawDiscriminator(TREE,outpath,discriminator,30,0,30,"jet_dRdTFit_signalmean",discname);

  //discriminator=TString("jet_dRdTFit_bkg");
  //DrawDiscriminator(TREE,outpath,discriminator,30,-1000,2000,"jet_dRdTFit_signalmean",discname);

  //discriminator=TString("jet_dRdTFit_bkg/jet_dRdTFit_bkger");
  //DrawDiscriminator(TREE,outpath,discriminator,30,-10,20,"jet_dRdTFit_signalmean",discname);

  //discriminator=TString("jet_dRdTFit_signalmean");
  //DrawDiscriminator(TREE,outpath,discriminator,50,-1,1,"jet_dRdTFit_signalmean",discname);

  ///ScanDiscriminator(TREE,outpath,discriminator,"0.02",discname);


  /////////////////////////////////////////////////////////////
  ////// Using jet core tracks
  /////////////////////////////////////////////////////////////
  //discname="TrkCore";
  //cellselection=TString("(cell_jetdR<")+celldR+")";
  //DrawdT(TREE,outpath,"jet_dTFitCore_signalmean",cellselection,discname);

  //discriminator=TString("jet_TrkCore_ncellpeak")+"/jet_TrkCore_ncell";
  //DrawDiscriminator(TREE,outpath,discriminator,40,0.,0.50,"jet_TrkCore_signalmean",discname);
  //ScanDiscriminator(TREE,outpath,discriminator,"0.003",discname);

  /////////////////////////////////////////////////////////////
  ////// jet core truth peak 
  /////////////////////////////////////////////////////////////
  // discname="dTFitCoreTrue";
  // discriminator="jet_dTFitCore_truencell";
  // DrawDiscriminator(TREE,outpath,discriminator,30,0,300,"jet_truedt",discname);
  // ScanDiscriminator(TREE,outpath,discriminator,"1.5",discname);


  /////////////////////////////////////////////////////////////
  ////// jet core with background subtraction
  /////////////////////////////////////////////////////////////
  //discname="dTFitCoreBkgSub";
  //cellselection=TString("(cell_jetdR<")+celldR+")";
  //DrawdT(TREE,outpath,"jet_dTFitCoreBkgSub_signalmean",cellselection,discname);

  //discriminator=TString("jet_dTFitCoreBkgSub_ncell_window")+WINDOW+"/jet_dTFitCoreBkgSub_ncell";
  //DrawDiscriminator(TREE,outpath,discriminator,40,0,0.5,"jet_dTFitCoreBkgSub_signalmean",discname);

  ////discriminator=TString("jet_dTFitCoreBkgSub_ncellsig-jet_dTFitCoreBkgSub_maxbkg");
  //discriminator=TString("jet_dTFitCoreBkgSub_ncellsig");
  //discriminator=TString("jet_dTFitCoreBkgSub_maxbkg");
  //DrawDiscriminator(TREE,outpath,discriminator,30,0,300,"jet_dTFitCoreBkgSub_signalmean",discname);

  //discriminator=TString("jet_dTFitCoreBkgSub_ncellbkg");
  //discriminator=TString("jet_dTFitCoreBkgSub_avgbkg");
  //DrawDiscriminator(TREE,outpath,discriminator,40,-5,15,"jet_dTFitCoreBkgSub_signalmean",discname);

  //discriminator=TString("jet_dTFitCoreBkgSub_sF");
  //DrawDiscriminator(TREE,outpath,discriminator,40,0,1,"jet_dTFitCoreBkgSub_signalmean",discname);

  //discriminator=TString("jet_dTFitCoreBkgSub_ncellsig/jet_dTFitCoreBkgSub_maxbkg");
  //DrawDiscriminator(TREE,outpath,discriminator,40,0,5,"jet_dTFitCoreBkgSub_signalmean",discname);

  //ScanDiscriminator(TREE,outpath,discriminator,"1.5",discname);



  // /////////////////////////////////////////////////////////
  // ///// **  Using Jet cells  **
  // /////////////////////////////////////////////////////////
  //ScanDiscriminator(TREE,outpath,TString("jet_dTFitJetCell_ncell_window")+WINDOW+"/jet_ncell","JetCellDTFitFraction","fraction of cells",20,0,0.05,TString("0.000075"));

 
  // /////////////////////////////////////////////////////////////
  // //////Using Cluster Cells 
  // /////////////////////////////////////////////////////////////
  // discriminator=TString("jet_dTFit_ncell_clus_window")+WINDOW+"/jet_ncell_clus";
  // discname="dTFit";
  // cellselection=TString("(cell_clusdR<")+celldR+")";
  // //DrawdT(TREE,outpath,"jet_dTFit_signalmean",cellselection,discname);
  // DrawDiscriminator(TREE,outpath,discriminator,40,0,0.80,discname);
  // ScanDiscriminator(TREE,outpath,discriminator,"0.002",discname);


  // ///////////////////////////////////////////////////////////
  // ///////// ***  Using Cluster Cells  Weight by cluster energy
  // ///////////////////////////////////////////////////////////
  // discriminator=TString("jet_dTFitEcw_Ecell_clus_window")+WINDOW+"/jet_dTFitEcw_Etot_clus";
  // discname="ClustersEcw";
  // DrawdT(TREE,outpath,"(cell_dt-jet_dTFitEcw_signalmean[clus_jet[cell_clus]])",CUTSignalJetClusterCell+"*"+CUTEtaJetClusterCell+"*(cell_clus>=0?clus_E[cell_clus]:0.)",CUTPUJetClusterCell+"*"+CUTEtaJetClusterCell+"*(cell_clus>=0?clus_E[cell_clus]:0.)",discname);
  // DrawDiscriminator(TREE,outpath,discriminator,20,0,0.40,discname);
  // //ScanDiscriminator(TREE,outpath,discriminator,"0.0015",discname);



//   /////////////////////////////////////////////////////////////
//   ////// simple dR fit
//   /////////////////////////////////////////////////////////////
//   discriminator=TString("jet_dRFit_sig/jet_dRdTFit_siger");
//   discname="dRFit";
// //cellselection="(cell_jetdR<0.2)";
// //DrawdT(TREE,outpath,"jet_dRdTFit_signalmean",CUTSignalJetCell+"*"+CUTEtaJetCell+"*"+cellselection,CUTPUJetCell+"*"+CUTEtaJetCell+"*"+cellselection,discname);
//   DrawDiscriminator(TREE,outpath,discriminator,25,0,5,discname);


//   /////////////////////////////////////////////////////////////
//   ////// dT scan using 
//   /////////////////////////////////////////////////////////////
//   discriminator=TString("jet_dRdTFit_ncell_window")+WINDOW+"/jet_dRdTFit_ncell";
//   discname="dRdTFit";
//   cellselection="(cell_jetdR<0.2)";
//   DrawdT(TREE,outpath,"jet_dRdTFit_signalmean",CUTSignalJetCell+"*"+CUTEtaJetCell+"*"+cellselection,CUTPUJetCell+"*"+CUTEtaJetCell+"*"+cellselection,discname);
//   DrawDiscriminator(TREE,outpath,discriminator,25,0,0.50,discname);
//   //ScanDiscriminator(TREE,outpath,discriminator,"0.0001",discname);



  gROOT->ProcessLine(".q");
}


#include "HGTDAnalysisTools/macros/globals.h"
#include "HGTDAnalysisTools/macros/drawCellGridDetGeo.C"
#include "HGTDAnalysisTools/macros/plotTrigger.C"
#include <map>

map<long,long> jetpt;
map<long,float> jeteta;

void drawXYTrigger(TTree *Tuple, TString outpath,TString tag, TString SelectionXY){
  TCanvas C("C","",700,780);

  //TString outname=outpath+"/TrigEventDisplay_CellsXY"+(side==1?"P":"N")+"_"+tag;
  TString outname=outpath+"/TrigEventDisplay_CellsXY_"+tag;
  
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
  HRecoCellsXY.SetMarkerSize(0.3);
  HRecoCellsXY.SetMarkerColor(outpath.Contains("mu0")?2:1);

  Tuple->Draw("cell_y:cell_x>>HRecoCellsXY",SelectionXY+"*(cell_S==1)"); 

  ///get mu=0 distribution
  TH1F * Hmu0=NULL;
  TString outnamemu0=outname;
  if(outpath.Contains("mu200")){
    outnamemu0("mu200")="mu0";
    TFile * InFile = new TFile(outnamemu0+".root","read"); 
    gROOT->cd();
    if(!InFile->IsZombie()){
      Hmu0 = (TH1F*)(InFile->Get("HRecoCellsXY")->Clone("Hmu0"));
    }
    InFile->Close();
  }



  C.Clear();    
  HRecoCellsXY.Draw("");
  drawTriggerTiles(1,1);
  drawTriggerTiles(-1,1);

  DrawATLAS(0.20,0.89);
  DrawHGTD(outpath,0,1,0.20,0.835);
  DrawVBFH(outpath);
  DrawECMSMu(outpath);
    
  //text.SetTextColor(1);
  //text.SetTextSize(0.035);
  //text.DrawTextNDC(0.76,0.84,side==1?"z = 3.5 m":"z = -3.5 m");
  //text.DrawLatexNDC(0.76,0.81,side==1?"2.5<#eta<4.2":"-4.2<#eta<-2.5");
 

  //////Draw the signal hits
  if(Hmu0){
    Hmu0->SetMarkerColor(2);
    Hmu0->SetMarkerSize(0.5);
    Hmu0->Draw("same");
    text.SetTextSize(0.03);
    text.SetTextColor(1);
    text.DrawLatexNDC(0.8,0.25,"#bullet pile-up");
    text.SetTextColor(2);
    text.DrawLatexNDC(0.8,0.22,"#bullet signal");
  }

  C.Print(outname+".png");    
  //C.Print(outname+".eps");    
  //C.Print(outname+".pdf");   


  //write out the histogram for the mu=200 plot
  TFile OutFile(outname+".root","recreate"); 
  HRecoCellsXY.Write();
  OutFile.Close();


  ///////# of hits per tile
  TH2F * HRecoCellsXYCount = getTrigger2D(TString("HRecoCellsXYCount")+"_"+tag,"# of hits per tile",0);
  Tuple->Draw(TString("trig_y:trig_x>>HRecoCellsXYCount")+"_"+tag,"(trig_eta>0&&trig_r<600)*(trig_nhit)");//for now select by hand the positive side
  plotTrigger2D(HRecoCellsXYCount,outname+"_Counts.png",0);
   
  TH2F * HRecoCellsXYCountPeak = getTrigger2D(TString("HRecoCellsXYCountPeak")+"_"+tag,"# of peak hits per tile",0);
  Tuple->Draw(TString("trig_y:trig_x>>HRecoCellsXYCountPeak")+"_"+tag,"(trig_eta>0&&trig_r<600)*(trig_nhit_peak)");//for now select by hand the positive side
  plotTrigger2D(HRecoCellsXYCountPeak,outname+"_CountsPeak.png",0);
   

}



void DrawHitsTrigger(TTree *Tuple, TString outpath,TString tag, TString Selection){
  
  drawXYTrigger(Tuple,outpath,tag,Selection);

  TCanvas C;
  
  //////////////////
  /////Plot the deltaT distributions
  ////////////
  long ntbins=17; //17=120ps, 33=60ps
  int tmin=-1000;
  int tmax=1000;
  TString dTYTitle="number of hits / 120 ps";

  TFile OutFile(outpath+"/TrigEventDisplay_CellsdT_"+tag+".root","recreate"); //save the dT distributions for plotting mu=0 in mu=200

  long counter=1;
  for(map<long,long>::const_iterator it = jetpt.begin(); it!=jetpt.end(); ++it){
    //cout<<tag<<" dTCells: jet="<<it->first<<" pt="<<it->second<<endl;

    if(fabs(jeteta[counter])>3.2){
      ntbins=33;//60ps binning
      dTYTitle="number of hits / 60 ps";
    }
  
    TH1F HCellDT(TString("HCellDT_Jet")+it->first,"",ntbins,tmin,tmax);
    //Tuple->Draw(TString("cell_dt*1000>>HCellDT_Jet")+it->first,Selection+"*(cell_jet>=0&&abs(jet_pt[cell_jet]/1000-"+(it->second)+")<1.)"); 
    Tuple->Draw(TString("cell_dt*1000>>HCellDT_Jet")+it->first,Selection); 
    

    ////////////////////////////////////////////////////
    //open the mu=0 root file and plot dT distribution
    ////////////////////////////////////////////////////
    TFile * InFile=NULL;
    TH1F * H=NULL;
    TH1F * HDR=NULL;
    if(outpath.Contains("mu200")){
      TString outpathmu0=outpath;
      outpathmu0("mu200")="mu0";
      InFile = new TFile(outpathmu0+"/TrigEventDisplay_CellsdT_"+tag+".root","read"); 
      gROOT->cd();
      if(!InFile->IsZombie()){
	H = (TH1F*)InFile->Get(TString("HCellDT_Jet")+it->first);
	HDR = (TH1F*)InFile->Get(TString("HCellDR_Jet")+it->first);
      }
    }

 
    C.Clear();
    HCellDT.GetXaxis()->SetTitle(" t - t_{exp}   [ps]"); 
    HCellDT.GetYaxis()->SetTitle(dTYTitle); 
    //HCellDT.GetYaxis()->SetRangeUser(0,100);
    HCellDT.GetYaxis()->SetRangeUser(0,1.3*HCellDT.GetMaximum());
    HCellDT.Draw("hist");

    DrawATLAS(0.20,0.89);
    DrawHGTD(outpath,0,0,0.20,0.835);
    DrawVBFH(outpath);
    DrawECMSMu(outpath);
  
    float xpostxt=0.68;
    text.SetTextSize(0.04);

    // text.SetTextColor(1);
    // if(outpath.Contains("_mu200"))
    //   text.DrawLatexNDC(xpostxt,0.87,"#topbar signal jet, #mu=200");
    
    if(H){
      H->SetLineColor(2);
      H->Draw("histsame");
      text.SetTextColor(2);
      text.DrawLatexNDC(xpostxt,0.82,"#topbar signal jet, #mu=0");
    }
    

    C.RedrawAxis();
    C.Print(outpath+"/TrigEventDisplay_CellsdT_"+tag+"_Jet"+it->first+".png");
    //C.Print(outpath+"/TrigEventDisplay_CellsdT_"+tag+"_Jet"+it->first+".eps");
    //C.Print(outpath+"/TrigEventDisplay_CellsdT_"+tag+"_Jet"+it->first+".pdf");


    
    //////////////////////////////////
    ////Show the full dR distribution   
    //////////////////////////////////
    TH1F HCellDR(TString("HCellDR_Jet")+it->first,"",40,0,4);
    Tuple->Draw(TString("sqrt((cell_eta-truth_eta[truth_VBFJ1])*(cell_eta-truth_eta[truth_VBFJ1])  + pow(abs(cell_phi - truth_phi[truth_VBFJ1])<3.1416 ? cell_phi - truth_phi[truth_VBFJ1] : cell_phi - 2*3.1416 - truth_phi[truth_VBFJ1],2))>>HCellDR_Jet")+it->first,Selection); 
    C.Clear();
    HCellDR.GetXaxis()->SetTitle(" cell #Delta r  "); 
    HCellDR.GetYaxis()->SetTitle(" # of cells / 0.1  "); 
    HCellDR.GetYaxis()->SetRangeUser(0,HCellDR.GetMaximum()*1.3);
    HCellDR.Draw("hist");
    DrawHGTD(outpath,0,0,0.76,0.87);
    DrawVBFH(outpath);
    DrawECMSMu(outpath);
    if(HDR){
      HDR->SetLineColor(2);
      HDR->Draw("histsame");
      text.SetTextColor(2);
      text.DrawLatexNDC(0.76,0.74,"#topbar #mu=0");
    }
    C.Print(outpath+"/TrigEventDisplay_CellsdR_"+tag+"_Jet"+it->first+".png");



    /////////Close input and output files
    if(InFile) delete InFile;
    ///Save the signal histogram
    OutFile.cd();
    HCellDT.Write();
    HCellDR.Write();


    counter++;
  }
  OutFile.Close();
	
}
  


void plotTriggerEventDisplay(TString SAMPLE="LArHitNov29/LArHit_July26_mu0",TString TAG=""){

  long eventid=2354; //839 for low eta,  9 for high eta

  SetAtlasStyle();
  gStyle->SetPalette(1,0);
  setCuts(SAMPLE);

  TString sample = "mc15_13TeV.301399.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_ZZ4nu.evgen.EVNT.e4397";
  TString samplealias = "VBFH125inv";

  loadChain(INDIR+"/"+SAMPLE+"/"+sample);
  if(TREE->GetEntries()<=0) return;

  TString outpath=OUTDIR+"/"+samplealias+"_"+SAMPLE;
  if(TAG.CompareTo("")!=0) outpath=outpath+"_"+TAG;
  system((TString("rm -rf ")+outpath).Data());
  system((TString("mkdir -p ")+outpath).Data());



  //Select an event with only one true jet in the positive forward region
  TString BasePtSelection="(30000<truth_pt[truth_VBFJ1]&&truth_pt[truth_VBFJ1]<100000)";
  //TString BaseEtaSelection="(2.8<truth_eta[truth_VBFJ1]&&truth_eta[truth_VBFJ1]<3.0)";
  TString BaseEtaSelection="(3.5<truth_eta[truth_VBFJ1]&&truth_eta[truth_VBFJ1]<3.8)";
  TString BaseIsolation="((truth_Gluon==-1)&&truth_eta[truth_VBFJ2]<0)";

  TString BaseSelection=BasePtSelection+"*"+BaseEtaSelection+"*"+BaseIsolation+"*(njet>0)";
  cout<<"Base selection: "<<BaseSelection<<endl;


  ////////////////////////////////////////////
  //find a good event
  ///////////////////////////////////////////
  TH2F HEventId("HEventId","",20000,0.5,20000.5,1,2,5);//do not change this binning 
  TREE->Draw("truth_eta[truth_VBFJ1]:event>>HEventId",BaseSelection,"col");
  
  //print the list of events with VBF jet in this range
  for(int b=1;b<=HEventId.GetXaxis()->GetNbins();b++)
    if(HEventId.GetBinContent(b,1)>0) 
      cout<<"Event "<<b<<endl; 


  //////////////////////
  ///select the event for faster plotting
  //////////////////////
  TTree* Tuple = TREE->CopyTree(TString("event==")+eventid);
  
  
  ////Fill the jet list %
  TH2F HRecoJetPtEta("HRecoJetPtEta","",1000,-5.0,5.0,200,0,200);
  float etastep=(HRecoJetPtEta.GetXaxis()->GetXmax()-HRecoJetPtEta.GetXaxis()->GetXmin())/HRecoJetPtEta.GetXaxis()->GetNbins();
  Tuple->Draw("jet_pt/1000:jet_eta>>HRecoJetPtEta","jet_truthJ==truth_VBFJ1");
  long jetcntr=1;
  for(int bx=1;bx<HRecoJetPtEta.GetNbinsX();bx++)
    for(int by=1;by<HRecoJetPtEta.GetNbinsY();by++)
      if(HRecoJetPtEta.GetBinContent(bx,by)>0){
	float eta=(HRecoJetPtEta.GetXaxis()->GetXmin()+bx*etastep);
  	cout<<"Jet: pT="<<by<<" GeV  ,  eta="<<eta<<endl;
  	jetpt[jetcntr]=by; 
  	jeteta[jetcntr]=eta;
  	jetcntr++;
      }
  if(jetpt.size()==0) return;
  

  //////////////////////////
  //////// cells
  //////////////////////////
  //DrawHitsTrigger(Tuple,outpath,"trigtile","(cell_trig>=0&&trig_jet[cell_trig]>=0&&jet_truth_pt[trig_jet[cell_trig]]>0)");
  //DrawHitsTrigger(Tuple,outpath,"alltiles","()");
  DrawHitsTrigger(Tuple,outpath,"trigtile","(cell_trig>=0&&trig_jet[cell_trig]>=0&&jet_truthJ[trig_jet[cell_trig]]==truth_VBFJ1)");//show the signal jet tile only
  DrawHitsTrigger(Tuple,outpath,"alltiles","(cell_trig>=0&&trig_eta[cell_trig]*truth_eta[truth_VBFJ1]>0)");//show hits only in the side of signal jet

  gROOT->ProcessLine(".q");
}

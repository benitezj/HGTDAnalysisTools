
void plotEventDisplayDrawHits(TTree *Tuple, TString outpath,TString tag, TString Selection, TString SelectionBkg=""){
  
  //plotEventDisplayHitsXY(Tuple,outpath,tag,Selection,1);
  //plotEventDisplayHitsXY(Tuple,outpath,tag,Selection,-1);

  TCanvas C;
  
  //////////////////
  /////Plot the deltaT distributions
  ////////////

  int ntbins=33;//66=30ps, 33=60ps
  int tmin=-1000;
  int tmax=1000;
  TString dTYTitle="number of hits / 60 ps";
  if(tag.Contains("Ecw")) dTYTitle = TString("weighted ")+dTYTitle;
  
  char txt[100];
  
  TFile OutFile(outpath+"/EventDisplay_CellsdT_"+tag+".root","recreate"); //save the dT distributions for plotting mu=0 in mu=200

  for(map<long,long>::const_iterator it = jetpt.begin(); it!=jetpt.end(); ++it){
    //cout<<tag<<" dTCells: jet="<<it->first<<" pt="<<it->second<<endl;

    TH1F HCellDT(TString("HCellDT_Jet")+it->first,"",ntbins,tmin,tmax);
    Tuple->Draw(TString("cell_dt*1000>>HCellDT_Jet")+it->first,Selection+"*(cell_jet>=0&&abs(jet_pt[cell_jet]/1000-"+(it->second)+")<1.)"); 
 
 
    C.Clear();

    HCellDT.GetXaxis()->SetTitle(" t - t_{exp}   [ps]"); 
    HCellDT.GetYaxis()->SetTitle(dTYTitle); 
    HCellDT.GetYaxis()->SetRangeUser(0,100);
    HCellDT.Draw("hist");

    DrawATLAS(0.20,0.89);
    DrawHGTD(outpath,0,0,0.20,0.835);
    DrawVBFH(outpath);
    DrawECMSMu(outpath);
  

    float xpostxt=0.68;
    text.SetTextSize(0.04);
    text.SetTextColor(1);
    text.DrawLatexNDC(xpostxt,0.87,"#topbar signal jet, #mu=200");

    ////////////////////////////////////////////////////
    //open the mu=0 root file and plot dT distribution
    ////////////////////////////////////////////////////
    TFile * InFile=NULL;
    TH1F * H=NULL;
    if(outpath.Contains("mu200")){// && tag.CompareTo("jet")==0){
      TString outpathmu0=outpath;
      outpathmu0("mu200")="mu0";
      InFile = new TFile(outpathmu0+"/EventDisplay_CellsdT_"+tag+".root","read"); 
      if(!InFile->IsZombie()){
	H = (TH1F*)InFile->Get(TString("HCellDT_Jet")+it->first);
    	if(H){
    	  H->SetLineColor(2);
    	  H->Draw("histsame");

	  text.SetTextColor(2);
	  text.DrawLatexNDC(xpostxt,0.82,"#topbar signal jet, #mu=0");
    	}
      }
    }
    
    text.SetTextColor(1);
    if(tag.CompareTo("jet")==0)   text.DrawLatexNDC(xpostxt,0.75,"#Delta R < 0.4");
    if(tag.CompareTo("core2")==0) text.DrawLatexNDC(xpostxt,0.75,"#Delta R < 0.2");
    if(tag.CompareTo("core1")==0) text.DrawLatexNDC(xpostxt,0.75,"#Delta R < 0.1");


    ////Show the background 
    TH1F HCellDTBkg(TString("HCellDTBkg_Jet")+it->first,"",ntbins,tmin,tmax);
    HCellDTBkg.SetLineColor(4);
    if(SelectionBkg.CompareTo("")!=0){
      Tuple->Draw(TString("cell_dt*1000>>HCellDTBkg_Jet")+it->first,SelectionBkg+"*(cell_jet>=0&&abs(jet_pt[cell_jet]/1000-"+(it->second)+")<1.)","histsame");      
    }

    C.RedrawAxis();
    C.Print(outpath+"/EventDisplay_CellsdT_"+tag+"_Jet"+it->first+".png");
    C.Print(outpath+"/EventDisplay_CellsdT_"+tag+"_Jet"+it->first+".eps");
    C.Print(outpath+"/EventDisplay_CellsdT_"+tag+"_Jet"+it->first+".pdf");


    ////Show the background subtracted distribution
    if(SelectionBkg.CompareTo("")!=0){
      HCellDT.Add(&HCellDTBkg,-1);
      C.Clear();
      HCellDT.Draw("hist");
      if(H) H->Draw("histsame");
      C.Print(outpath+"/EventDisplay_CellsdT_"+tag+"_Jet"+it->first+"_BkgSub.png");
    }



    if(InFile) delete InFile;

    OutFile.cd();
    HCellDT.Write();
  }
  OutFile.Close();
	
}
  

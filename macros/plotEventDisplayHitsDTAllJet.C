
void plotEventDisplayHitsDTAllJet(TTree *Tuple, TString outpath,TString tag, TString Selection){
  
  TCanvas C("C","",1000,1000);
  
  //////////////////
  /////Plot the deltaT distributions
  ////////////
  int ntbins=17;
  int tmin=-1000;
  int tmax=1000;

  TFile OutFile(outpath+"/EventDisplay_CellsdTAllJet_"+tag+".root","recreate"); //save the dT distributions for plotting mu=0 in mu=200


  ////////////////////////////////////////////////////
  //show the signal distribution
  ////////////////////////////////////////////////////
  TFile * InFile=NULL;
  if(outpath.Contains("mu200")){// && tag.CompareTo("jet")==0){
    TString outpathmu0=outpath;
    outpathmu0("mu200")="mu0";
    InFile = new TFile(outpathmu0+"/EventDisplay_CellsdTAllJet_"+tag+".root","read"); 
    gROOT->cd();
  }


  C.Clear();  
  C.Divide(3,3);
  int counter=1;
  for(map<long,long>::const_iterator it = jetpt.begin(); it!=jetpt.end(); ++it){
    C.cd(counter);

    cout<<"dTCells: jet="<<it->first<<" pt="<<jetpt[counter]<<" eta="<<jeteta[counter]<<" phi="<<jetphi[counter]<<endl;
 
    TString dTYTitle="number of hits / 120 ps";
    if(fabs(jeteta[counter])>3.2){
      ntbins=33;
      dTYTitle="number of hits / 60 ps";
    }

    TString histname=TString("HCellDT_Jet_")+(long)abs(10*jeteta[counter])+"_"+(long)abs(10*jetphi[counter]);

    TH1F HCellDT(histname,"",ntbins,tmin,tmax);
    Tuple->Draw(TString("cell_dt*1000>>")+histname,Selection+"*(cell_jet>=0&&abs(jet_pt[cell_jet]/1000-"+(it->second)+")<1)"); 

    OutFile.cd();//save histo at this point, below there is a background subtraction
    HCellDT.Write();

  
    ////////////////////////////
    HCellDT.GetXaxis()->SetTitle(" t - t_{exp}   [ps]"); 
    HCellDT.GetYaxis()->SetTitle(dTYTitle); 
    HCellDT.GetYaxis()->SetRangeUser(0,HCellDT.GetMaximum()*1.3);
    HCellDT.DrawClone("hist");
  
    DrawATLAS(0.20,0.89);
    DrawHGTD(outpath,0,0,0.20,0.835);
    DrawVBFH(outpath);
    DrawECMSMu(outpath);
    
    float xpostxt=0.68;
    text.SetTextColor(1);

    TH1F * HSig=NULL;
    if(InFile && !InFile->IsZombie()){
      HSig = (TH1F*)InFile->Get(histname);
      if(!HSig) HSig = (TH1F*)InFile->Get(TString("HCellDT_Jet_")+(long)abs(10*jeteta[counter])+"_"+(long)abs(10*jetphi[counter]-1));
      if(!HSig) HSig = (TH1F*)InFile->Get(TString("HCellDT_Jet_")+(long)abs(10*jeteta[counter])+"_"+(long)abs(10*jetphi[counter]+1));
    }
    if(HSig){
      HSig->SetLineColor(2);
      HSig->Draw("histsame");
      text.SetTextColor(2);
      text.DrawLatexNDC(xpostxt,0.82,"#topbar signal jet, #mu=0");
    }

    counter++;
  }

  C.RedrawAxis();
  C.Print(outpath+"/EventDisplay_CellsdTAllJet_"+tag+".png");



  //if(InFile) delete InFile;
  //OutFile.ls();
  OutFile.Close();
}
  

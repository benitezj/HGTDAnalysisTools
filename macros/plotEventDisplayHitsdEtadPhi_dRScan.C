
void plotEventDisplayHitsdEtadPhi_dRScan(TTree *Tuple, TString outpath,TString tag, TString Selection){

  TCanvas C("CdEtadPhi","",600,600);
  C.SetRightMargin(0.15);
  C.SetTopMargin(0.15);


  int nbins=100;//gives bin size of 0.01, previously used 20->0.05
  float range=0.5;
  TH2F HCellsdEtadPhi("HCellsdEtadPhi_Jet","",nbins,-range,range,nbins,-range,range);

  ////////////////////////
  /// scan time bins
  ////////////////////////
  ///Choose time resolution
  long dtwindow=120;
  if(fabs(jeteta[1])>3.2){
    dtwindow=60;
  }

  TH2F*H[10];
  float max=0.;
  for(long t=0;t<10;t++){
    long tmin=-600+t*2*dtwindow;
    long tmax=-600+(t+1)*2*dtwindow;
    H[t] = (TH2F*)(HCellsdEtadPhi.Clone(TString("HCellsdEtadPhi_Jet_dT")+t));
    H[t]->Scale(0.);
    Tuple->Draw(dEta+":"+dPhi+">>HCellsdEtadPhi_Jet_dT"+t,TString("(")+Selection+"&&("+tmin+"<cell_dt*1000&&cell_dt*1000<"+tmax+")"+")"); 

    if(H[t]->GetMaximum()>max) max = H[t]->GetMaximum();
   }


  TCanvas C2("CdEtadPhi","",1500,600);
  C2.SetRightMargin(0);
  C2.SetTopMargin(0);
  C2.SetLeftMargin(0);
  C2.SetBottomMargin(0);
  C2.Divide(5,2);
  for(long t=0;t<10;t++){
    C2.cd(t+1);
    C2.GetPad(t+1)->SetRightMargin(0.05);
    C2.GetPad(t+1)->SetTopMargin(0.05);
    C2.GetPad(t+1)->SetLeftMargin(0.05);
    C2.GetPad(t+1)->SetBottomMargin(0.05);

    long tmin=-600+t*2*dtwindow;
    long tmax=-600+(t+1)*2*dtwindow;

    H[t]->GetYaxis()->SetTitle("");
    H[t]->GetXaxis()->SetTitle("");
    H[t]->GetZaxis()->SetTitle("");
    H[t]->GetZaxis()->SetRangeUser(0,max);
    H[t]->Draw("colz");

    text.SetTextColor(1);
    text.SetTextSize(0.06);
    text.DrawLatexNDC(0.12,0.87,TString("[")+tmin+","+tmax+"]");
  }
  C2.Print(outpath+"/EventDisplaydR_CellsdEtadPhivsdT_"+tag+".png");
  

}
  

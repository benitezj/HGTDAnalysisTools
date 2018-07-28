
void plotEventDisplayJetShape(TTree *Tuple, TString outpath, TString tag, TString Selection){
  
  TCanvas C;
  
  TH1F*H=getdRDensityHisto("Density");
  Tuple->Draw(TString("cell_jetdR>>Density"),+Selection); 
  dividedRDensityHisto(H);


  //TF1* F=new TF1("F","[0]+(x-0.2)*((x<0.2)*tan(3.14159/2+[1])+(x>0.2)*tan(3.14159/2+[2]))",0,0.4);
  TF1* F=new TF1("F","[0]+(x-0.2)*(2000*(x<0.2)*tan([1])+3000*(x>0.2)*tan([2]))",0,0.4);
  F->SetParLimits(1,0.6*3.14159,1.4*3.14159);
  F->SetParLimits(2,1.6*3.14159,2.4*3.14159);
  F->SetLineColor(2);

  H->Fit(F,"","",0,0.4);  

  C.Clear();  
  H->GetXaxis()->SetTitle(" #DeltaR "); 
  H->GetYaxis()->SetTitle("cell density"); 
  H->GetYaxis()->SetRangeUser(0,10000);
  //H->GetYaxis()->SetRangeUser(0,H->GetMaximum()*1.3);
  H->Draw("histpe");

  F->Draw("lsame");

  long ANGLE=180*F->GetParameter(1)/TMath::Pi();
  long SUBANGLE=180*F->GetParameter(2)/TMath::Pi();
  text.SetTextSize(0.04);
  text.SetTextColor(2);
  text.DrawLatexNDC(0.75,0.85,TString("#theta_{1}=")+ANGLE);    
  text.DrawLatexNDC(0.75,0.80,TString("#theta_{2}=")+SUBANGLE);    


  DrawATLAS(0.20,0.89);
  DrawHGTD(outpath,0,0,0.20,0.835);
  DrawVBFH(outpath);
  DrawECMSMu(outpath);
    
  C.RedrawAxis();
  C.Print(outpath+"/EventDisplayJetShape_"+tag+".png");
  //C.Print(outpath+"/EventDisplaydR_CellsdT_"+tag+".eps");
  //C.Print(outpath+"/EventDisplaydR_CellsdT_"+tag+".pdf");
  

}







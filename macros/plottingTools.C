//#include "plotterGlobals.h"


TH1F* makeHistRatio(TH1* HDenom,TH1* HNum,float maxerr=0.5){
  if(HNum==NULL || HDenom == NULL){ 
    cout<<"Histograms are NULL"<<endl;
    return 0;
  }
  if(HNum->GetNbinsX() != HDenom->GetNbinsX()){ 
    cout<<"Histograms bins don't match"<<endl;
    return 0;
  }
  
  TH1F* HRatio=(TH1F*)HNum->Clone("HRatio");
  for(int b=0;b<=HNum->GetNbinsX()+1;b++){
    if(HDenom->GetBinContent(b)>0. &&       
       HNum->GetBinContent(b)>0. &&
       HNum->GetBinError(b)/HDenom->GetBinContent(b)<(maxerr>0?maxerr:1)
       ){
      float ratio=HNum->GetBinContent(b)/HDenom->GetBinContent(b);
      // if(ratio>1+maxerr) HRatio->SetBinContent(b,1+maxerr-maxerr/100);
      // else if(ratio<1-maxerr) HRatio->SetBinContent(b,1-maxerr+maxerr/100);
      // else 
      HRatio->SetBinContent(b,ratio);

      HRatio->SetBinError(b,HNum->GetBinError(b)/HDenom->GetBinContent(b));
    }else {
      HRatio->SetBinContent(b,0.);
      HRatio->SetBinError(b,0.);
    }
  }

  return HRatio;
}



TH1F* compareWithRatio(TH1* HDenom, TH1* HNum, TString outfilename, float yrange=0.5, float xmin=0, float xmax=0, float logy=0.){
  if(!HDenom || !HNum){
    std::cout<<"compareWithRatio: Input histograms are NULL"<<std::endl;
    return 0 ;
  }

  TCanvas C("C","C");  
  TPad pad1("pad1","",0,0.2,1,1);
  TPad pad2("pad2","",0,0,1,0.2);
  
  float maxHDenom=0;
  float maxHNum=0;
  
  if(xmin!=xmax){
    for(int b=1;b<=HDenom->GetNbinsX();b++){
      if(xmin<HDenom->GetBinCenter(b) && HDenom->GetBinCenter(b)<xmax){
	if(maxHDenom<HDenom->GetBinContent(b)) maxHDenom = HDenom->GetBinContent(b);
	if(maxHNum<HNum->GetBinContent(b)) maxHNum = HNum->GetBinContent(b);
      }
    }
  }else{
    maxHDenom=(HDenom->GetMaximum());//+HDenom->GetBinError(HDenom->GetMaximumBin()));
    maxHNum=(HNum->GetMaximum());//+HNum->GetBinError(HNum->GetMaximumBin()));
  }


  

  TLegend leg(0.72,0.75,0.90,0.92);
  leg.SetFillColor(0);
  leg.SetFillStyle(0);
  leg.SetBorderSize(0);
  leg.SetTextSize(0.05);
  leg.AddEntry(HDenom,HDenom->GetTitle(),"p");//Use y-axis titles for legend, remove y-axis titles below
  leg.AddEntry(HNum,HNum->GetTitle(),"p");




  //HDenom is the reference 
  

  pad1.cd();
  HDenom->SetTitle("");
  HDenom->GetXaxis()->SetTitleSize(0.05);
  //HDenom->GetYaxis()->SetTitle("");
  HDenom->GetYaxis()->SetRangeUser(0, maxHDenom > maxHNum ? maxHDenom*1.2 : maxHNum*1.2);
  HDenom->SetFillColor(0);
  HDenom->SetLineColor(1);
  HDenom->SetMarkerStyle(20);

  if(xmin!=xmax) HDenom->GetXaxis()->SetRangeUser(xmin,xmax);
  HDenom->Draw("histpE1");//draw with error bars to compare uncertainties

  // HNum->GetYaxis()->SetTitle("");
  HNum->SetFillColor(2);
  HNum->SetLineColor(2);
  HNum->SetMarkerColor(2);
  HNum->SetMarkerStyle(20);
  HNum->Draw("histpE1same");//
  leg.Draw();


  pad2.cd();
  pad2.SetGrid();
  //TH1D* ratio=(TH1D*)HNum->Clone("ratio");
  //ratio->Divide(HDenom);
  TH1F* ratio=makeHistRatio(HDenom,HNum,yrange);
  //TH1F* ratioRef=makeHistRatio(H1,H1,0.8);
  ratio->SetTitle("");
  ratio->GetXaxis()->SetTitle("");
  if(yrange>0)ratio->GetYaxis()->SetRangeUser(1-(yrange<=1.?yrange:1),1.+yrange);
  ratio->GetYaxis()->SetTitle("ratio");
  ratio->GetYaxis()->SetNdivisions(5);
  ratio->GetYaxis()->SetLabelSize(0.10);
  ratio->GetYaxis()->SetTitleSize(0.15);
  ratio->GetYaxis()->SetTitleOffset(0.20);
  ratio->GetXaxis()->SetNdivisions(-1);
  ratio->GetXaxis()->SetLabelSize(0.0001);
  if(xmin!=xmax) ratio->GetXaxis()->SetRangeUser(xmin,xmax);
  ratio->Draw("hist pe");
  //ratioRef->Draw("histe1same");
  //ratio->Draw("histpesame");
  TLine line;
  if(xmin!=xmax)line.DrawLine(xmin,1,xmax,1);
  else line.DrawLine(ratio->GetXaxis()->GetXmin(),1,ratio->GetXaxis()->GetXmax(),1);

  C.Clear();
  pad1.Draw();
  pad2.Draw();
  C.Update();
  if(logy!=0.){
    HDenom->GetYaxis()->SetRangeUser(logy,maxHDenom *10);
    pad1.SetLogy(logy);
    C.Update();
  }
  C.Print(outfilename);   


  return ratio;
}


void histOverlap(TH1* H1,TH1* H2,float& Overlap,float& OverlapErr){
  if(!H1 || !H2)
    return;
  
  if(H1->GetNbinsX() != H2->GetNbinsX())
    return;
  
  float norm=0.;
  float overlap=0.;
  float error=0.;
  for(int b=1;b<=H1->GetNbinsX();b++){
    //cout<<H1->GetBinContent(b)<<"+/-"<<H1->GetBinError(b)<<"  ,  "<<H2->GetBinContent(b)<<"+/-"<<H2->GetBinError(b)<<endl;
    norm += H2->GetBinContent(b)*H2->GetBinContent(b);
    overlap += H1->GetBinContent(b)*H2->GetBinContent(b);
    error +=  H1->GetBinError(b) * H1->GetBinError(b) * H2->GetBinContent(b) * H2->GetBinContent(b) 
      +  H2->GetBinError(b) * H2->GetBinError(b) * H1->GetBinContent(b) * H1->GetBinContent(b); 
  }
  overlap=overlap/norm;
  error=sqrt(error)/norm;
  
  cout<<"Overlap = "<<overlap<<" +- "<<error<<endl;
  Overlap=overlap;
  OverlapErr=error;
}


TF1* fitTrkMatchEffVsPt(TH1F* H, float xmin, float xmax){
  if(!H){
    cout<<"No histogram "<<endl;
    return 0;
  }

  TF1*F=new TF1(TString("FPOL")+H->GetName(),"[0]-[1]*exp(-[2]*x)",xmin,xmax); //
  F->SetParLimits(0,0.5,1);
  F->SetParLimits(1,0.001,100);
  F->SetParLimits(2,0.1,20);
  F->SetLineWidth(2);
  F->SetLineColor(4);
  H->Fit(F,"","",xmin,xmax);
  //F->Draw("same");

  return F;
}


void fitHistGaus(TH1F* H, float xmin, float xmax, float XNDC=0.8, float YNDC=0.8){
  if(!H){
    cout<<"No histogram "<<endl;
    return;
  }
  TF1*F=new TF1(TString("FGAUSS")+H->GetName(),"[0]*exp(-0.5*(x-[1])**2/[2]**2)",xmin,xmax);
  F->SetParLimits(0,H->GetMaximum()/100,10*H->GetMaximum());
  //F->SetParLimits(1,xmin,xmax);
  F->SetParLimits(2,fabs(xmax-xmin)/100,10*fabs(xmax-xmin));
  F->SetLineColor(4);

  H->Fit(F,"","",xmin,xmax);
  F->Draw("same");
  char txt[100];
  text.SetTextColor(4);
  text.SetTextSize(0.04);
  sprintf(txt,"Mean: %0.1f",F->GetParameter(1));
  text.DrawLatexNDC(XNDC,YNDC,txt);
  sprintf(txt,"Sigma: %0.1f",F->GetParameter(2));
  text.DrawLatexNDC(XNDC,YNDC-0.05,txt);
}


TF1* fitHist2Gaus(TH1F* H, float xmin, float xmax, float XNDC=0.8, float YNDC=0.8){

  if(!H){
    cout<<"No histogram "<<endl;
    return 0;
  }
  TF1*F=new TF1(TString("F2GAUSS")+H->GetName(),"[0]*((1-[3])*exp(-0.5*(x-[1])**2/[2]**2)/sqrt(2*3.14159*[2]*[2]) + [3]*exp(-0.5*(x-[1])**2/[4]**2)/sqrt(2*3.14159*[4]*[4]))",xmin,xmax);
  F->SetParLimits(0,H->GetMaximum()/10000,100*H->GetMaximum());
  
  //F->SetParLimits(1,xmin,xmax);
  F->SetParameter(1,H->GetMean());

  F->SetParLimits(2,fabs(xmax-xmin)/100,10*fabs(xmax-xmin));
  F->SetParameter(2,0.5*H->GetRMS());

  F->SetParLimits(3,0.00001,0.2);

  F->SetParLimits(4,fabs(xmax-xmin)/1000,10*fabs(xmax-xmin));
  F->SetParameter(4,2*H->GetRMS());

  F->SetLineColor(4);
  F->SetNpx(500);
  
  H->Fit(F,"","pe",xmin,xmax);
  H->GetYaxis()->SetRangeUser(0,1.3*H->GetMaximum());
  H->Draw("histpe");
  F->Draw("same");

  
  //show the broad component
  TF1 * F2=new TF1(TString("F2GAUSS2")+H->GetName(),"[0]*[3]*exp(-0.5*(x-[1])**2/[2]**2)/sqrt(2*3.14159*[2]*[2])",xmin,xmax);
  F2->SetParameter(0,F->GetParameter(0));
  F2->SetParameter(1,F->GetParameter(1));
  F2->SetParameter(2,F->GetParameter(4));
  F2->SetParameter(3,F->GetParameter(3));
  F2->SetLineColor(1); 
  F2->SetLineStyle(2);
  F2->Draw("same");
  
  // char txt[100];
  // text.SetTextColor(4);
  // text.SetTextSize(0.04);
  // sprintf(txt,"Mean: %0.1e",F->GetParameter(1));
  // text.DrawLatexNDC(XNDC,YNDC,txt);
  // sprintf(txt,"Sigma1: %0.1e",F->GetParameter(2));
  // text.DrawLatexNDC(XNDC,YNDC-0.05,txt);
  //sprintf(txt,"Sigma2: %0.1e",F->GetParameter(4));
  //text.DrawLatexNDC(XNDC,YNDC-0.10,txt);
  //sprintf(txt,"Frac.2: %0.1e",F->GetParameter(3));
  //text.DrawLatexNDC(XNDC,YNDC-0.15,txt);

  return F;
}

TF1* fitHistTrackdT(TH1F* H, float xmin, float xmax, float XNDC=0.8, float YNDC=0.8){

  if(!H){
    cout<<"No histogram "<<endl;
    return 0;
  }
  TF1*F=new TF1(TString("FTrackdT")+H->GetName(),"[0]*((1-[3])*exp(-0.5*(x-[1])**2/[2]**2)/sqrt(2*3.14159*[2]*[2]) + [3]*exp(-0.5*(x-[1])**2/[4]**2)/sqrt(2*3.14159*[4]*[4]))",xmin,xmax);
  F->SetParLimits(0,H->GetMaximum()/10000,100*H->GetMaximum());
  
  //F->SetParLimits(1,xmin,xmax);
  F->SetParameter(1,H->GetMean());

  F->SetParLimits(2,fabs(xmax-xmin)/100,10*fabs(xmax-xmin));
  F->SetParameter(2,0.5*H->GetRMS());

  F->SetParLimits(3,0.00001,0.2);

  //F->SetParLimits(4,fabs(xmax-xmin)/1000,10*fabs(xmax-xmin));
  //F->SetParameter(4,2*H->GetRMS());
  F->FixParameter(4,175);

  F->SetLineColor(4);


  
  H->Fit(F,"","pe",xmin,xmax);
  H->GetYaxis()->SetRangeUser(0,1.2*H->GetMaximum());
  H->Draw("histpe");
  F->Draw("same");

  
  //show the broad component
  TF1 * F2=new TF1(TString("FTrackdT")+H->GetName(),"[0]*[3]*exp(-0.5*(x-[1])**2/[2]**2)/sqrt(2*3.14159*[2]*[2])",xmin,xmax);
  F2->SetParameter(0,F->GetParameter(0));
  F2->SetParameter(1,F->GetParameter(1));
  F2->SetParameter(2,F->GetParameter(4));
  F2->SetParameter(3,F->GetParameter(3));
  F2->SetLineColor(1); 
  F2->SetLineStyle(2);
  F2->Draw("same");
  return F;
}



TF1* fitMatchResolution(TH1F* H, float xmin, float xmax, float XNDC=0.8, float YNDC=0.8){

  if(!H){
    cout<<"No histogram "<<endl;
    return 0;
  }
  
  ///////////////////////////
  //// Need to fit tail first 
  ///////////////////////////
  TH1F *HTail=(TH1F*)H->Clone(TString("HTail")+H->GetName());
  //float taillow=(HTail->GetXaxis()->GetXmax()+HTail->GetXaxis()->GetXmin())-2*H->GetRMS();
  //float tailhigh=(HTail->GetXaxis()->GetXmax()+HTail->GetXaxis()->GetXmin())+2*H->GetRMS();

  float taillow  =-5;
  float tailhigh = 5;

  for(int b=1;b<=HTail->GetNbinsX();b++){
    if(taillow<HTail->GetBinCenter(b) && HTail->GetBinCenter(b)<tailhigh){
      HTail->SetBinContent(b,0);
      HTail->SetBinError(b,0);
    }
  }

  TF1* FTail=new TF1(TString("MatchResolutionTail")+H->GetName(),
		     "[0]*exp(-0.5*(x-[1])**2/[2]**2)",
		     HTail->GetXaxis()->GetXmin(),HTail->GetXaxis()->GetXmax());
  FTail->SetLineColor(4);
  FTail->SetParLimits(0,0,1000*H->GetMaximum());
  //FTail->SetParLimits(1,xmin,xmax);
  FTail->FixParameter(1,0);
  FTail->SetParLimits(2,5,100);
  HTail->Fit(FTail,"","",HTail->GetXaxis()->GetXmin(),HTail->GetXaxis()->GetXmax());
  float tailfrac=FTail->Integral(xmin,xmax)/(H->GetXaxis()->GetBinWidth(1)*H->Integral());

  //return FTail;


  TF1*F=new TF1(TString("MatchResolution")+H->GetName(),"[0]*((1-[3])*exp(-0.5*(x-[1])**2/[2]**2)/sqrt(2*3.14159*[2]*[2]) + [3]*exp(-0.5*(x-[1])**2/[4]**2)/sqrt(2*3.14159*[4]*[4]))",xmin,xmax);
  F->SetParLimits(0,H->GetMaximum()/10000,100*H->GetMaximum());

  F->SetParLimits(1,xmin,xmax);
  F->SetParameter(1,H->GetMean());

  F->SetParLimits(2,fabs(xmax-xmin)/100,10*fabs(xmax-xmin));
  F->SetParameter(2,0.5*H->GetRMS());

  //F->SetParLimits(3,0.00001,0.9);
  F->FixParameter(3,tailfrac);

  //F->SetParLimits(4,fabs(xmax-xmin)/1000,10*fabs(xmax-xmin));
  //F->SetParameter(4,2*H->GetRMS());
  F->FixParameter(4,FTail->GetParameter(2));

  F->SetLineColor(4);

  H->Fit(F,"","same",xmin,xmax);

  cout<<"Fit Tail :"<<F->GetParameter(3)<<endl;

  //F->Draw("same");

  // char txt[100];
  // text.SetTextColor(4);
  // text.SetTextSize(0.04);
  // sprintf(txt,"Mean: %0.1e",F->GetParameter(1));
  // text.DrawLatexNDC(XNDC,YNDC,txt);
  // sprintf(txt,"Sigma: %0.1e",F->GetParameter(2));
  // text.DrawLatexNDC(XNDC,YNDC-0.05,txt);

  // sprintf(txt,"Sigma2: %0.1e",F->GetParameter(4));
  // text.DrawLatexNDC(XNDC,YNDC-0.10,txt);
  // sprintf(txt,"Frac.2: %0.1e",F->GetParameter(3));
  // text.DrawLatexNDC(XNDC,YNDC-0.15,txt);


  H->Draw("histpe");
  F->Draw("same");

  // FTail->Draw("same");
  // TLine line;
  // line.DrawLine(taillow,0,taillow,H->GetMaximum());
  // line.DrawLine(tailhigh,0,tailhigh,H->GetMaximum());

  return F;
}



TF1* fitMatchResolutionNew(TH1F* H, float xmin, float xmax, float XNDC=0.8, float YNDC=0.8){
  
  if(!H){
    cout<<"No histogram "<<endl;
    return 0;
  }

  TH1F *HTail=(TH1F*)H->Clone(TString("HTail")+H->GetName());
  for(int b=1;b<=HTail->GetNbinsX();b++){
    if(-5<HTail->GetXaxis()->GetBinCenter(b) && HTail->GetXaxis()->GetBinCenter(b)<5){
      HTail->SetBinContent(b,0);
      HTail->SetBinError(b,0);
    }
  }
  TF1* FTail=new TF1(TString("MatchResolutionTail")+H->GetName(),"[0]/([1]+fabs(x))",xmin,xmax);
  FTail->SetParLimits(1,0.00001,100);
  HTail->Fit(FTail,"","",xmin,xmax);


  TF1*F=new TF1(TString("MatchResolution")+H->GetName(),
		//"[0]*exp(-0.5*(x-[1])**2/[2]**2) + [3]*exp(-0.5*(x-[1])**2/[4]**2)",
		"[0]*exp(-0.5*(x-[1])**2/[2]**2) + [3]/([4]+fabs(x))",
		xmin,xmax);
  F->SetNpx(200);
  F->SetParLimits(0,H->GetMaximum()/10000,100*H->GetMaximum());

  F->SetParLimits(1,xmin,xmax);
  F->SetParameter(1,H->GetMean());

  F->SetParLimits(2,0.1,5);
  //F->SetParameter(2,0.5*H->GetRMS());

  //F->SetParLimits(3,H->GetMaximum()/10000,1000*H->GetMaximum());
  //F->SetParLimits(3,0.001,0.95);
  //F->FixParameter(3,tailfrac);

  //F->SetParLimits(4,0.001,100);
  //F->SetParLimits(4,3,50);
  //F->SetParameter(4,2*H->GetRMS());
  F->FixParameter(4,FTail->GetParameter(1));


  F->SetLineColor(4);

  H->Fit(F,"","",xmin,xmax);  

  H->Draw("histpe");
  F->Draw("same");


  FTail->SetParameter(0,F->GetParameter(3));
  FTail->SetParameter(1,F->GetParameter(4));
  FTail->SetLineColor(1);
  FTail->SetLineStyle(2);
  FTail->Draw("same");

  return F;
}





TF1* fitResolutionVsPt(TH1F* H, float xmin, float xmax, float XNDC=0.8, float YNDC=0.8){

  if(!H){
    cout<<"No histogram "<<endl;
    return 0;
  }
  TF1*F=new TF1(TString("ResolutionVsPt")+H->GetName(),"[0]+[1]*x+[2]*exp(-[3]*x)",xmin,xmax);
  F->SetParLimits(2,H->GetMaximum()/100,2*H->GetMaximum());
  F->SetParLimits(3,fabs(xmax-xmin)/1000,fabs(xmax-xmin)/2);
  F->SetLineColor(4);


  H->Fit(F,"","",xmin,xmax);
  F->Draw("same");
  char txt[100];
  text.SetTextColor(4);
  text.SetTextSize(0.04);
  sprintf(txt,"p0=%.2e",F->GetParameter(0));
  text.DrawLatexNDC(XNDC,YNDC,txt);
  sprintf(txt,"p1=%.2e",F->GetParameter(1));
  text.DrawLatexNDC(XNDC,YNDC-0.05,txt);
  sprintf(txt,"p2=%.2e",F->GetParameter(2));
  text.DrawLatexNDC(XNDC,YNDC-0.10,txt);
  sprintf(txt,"p3=%.2e",F->GetParameter(3));
  text.DrawLatexNDC(XNDC,YNDC-0.15,txt);

  return F;
}


TF1* fitTrkTruthT(TH1F* H, float xmin, float xmax, float XNDC=0.8, float YNDC=0.8){

  if(!H){
    cout<<"No histogram "<<endl;
    return 0;
  }

  /////////////////////////////////
  ///going to fit only the tails
  TH1F* HFit=new TH1F(TString("HfitTrkTruthT")+H->GetName(),"",H->GetXaxis()->GetNbins(),H->GetXaxis()->GetXmin(),H->GetXaxis()->GetXmax());
  HFit->Sumw2();
  for(int b=1;b<=HFit->GetNbinsX();b++){
    if(HFit->GetBinCenter(b)<xmin||xmax<HFit->GetBinCenter(b)){
      HFit->SetBinContent(b,H->GetBinContent(b));
      HFit->SetBinError(b,H->GetBinError(b));
    }
  }
  
  if(HFit->Integral()<=0){
    cout<<"Fit region is empty. Hist="<<H->GetName()<<endl;
    return 0;
  }


  ///Fit function is a power law * exponential decay
  TF1*F=new TF1(TString("FTrkTruthT")+H->GetName(),"[0]*((x-[2])^6)*exp(-[1]*(x-[2])/1000)",H->GetXaxis()->GetXmin(),H->GetXaxis()->GetXmax());
  F->SetParLimits(2,-4000,H->GetXaxis()->GetXmin());
  F->SetLineColor(4);
   

  HFit->Fit(F);

  //F->Draw("same");

  // char txt[100];
  // text.SetTextColor(4);
  // text.SetTextSize(0.04);
  // sprintf(txt,"Mean=%.2e",F->GetParameter(1));
  // text.DrawLatexNDC(XNDC,YNDC,txt);
  // sprintf(txt,"Sigma=%.2e",F->GetParameter(2));
  // text.DrawLatexNDC(XNDC,YNDC-0.05,txt);


  return F;
}


TH1F* fitTrkTruthTTemplate(TH1F* H, TH1F* HTemplate, float xmin, float xmax, float XNDC=0.8, float YNDC=0.8){

  if(!H || !HTemplate){
    cout<<"No histogram "<<endl;
    return 0;
  }
  

  //clone and remove signal region
  TH1F*HFit=(TH1F*)H->Clone(TString("HfitTrkTruthT")+H->GetName());
  for(int b=1;b<=HFit->GetNbinsX();b++){
    if(xmin<HFit->GetBinCenter(b)&&HFit->GetBinCenter(b)<xmax){
      HFit->SetBinContent(b,0);
      HFit->SetBinError(b,0);
    }
  }


  HTemplate->Scale(HFit->Integral()/HTemplate->Integral());

  // char txt[100];
  // text.SetTextColor(4);
  // text.SetTextSize(0.04);
  // sprintf(txt,"Mean=%.2e",F->GetParameter(1));
  // text.DrawLatexNDC(XNDC,YNDC,txt);

  return HTemplate;
}





// TGraphErrors* makeGraphRatio(TH1* HNum,TH1* HDenom,float maxerr=0.5,bool yerr=1,bool xerr=1){
//   if(HNum==NULL || HDenom == NULL){ 
//     cout<<"Histograms are NULL"<<endl;
//     return 0;
//   }
//   if(HNum->GetNbinsX() != HDenom->GetNbinsX()){ 
//     cout<<"Histograms bins don't match"<<endl;
//     return 0;
//   }
  
//   //TH1F* HRatio=(TH1F*)HNum->Clone("HRatio");
//   TGraphErrors * HRatio=new TGraphErrors();
//   int counter=0;
//   for(int b=0;b<=HNum->GetNbinsX()+1;b++){
//     if(HDenom->GetBinContent(b)>0. &&
//        HNum->GetBinError(b)/HNum->GetBinContent(b)<maxerr
//        ){  

//       float ratio=HNum->GetBinContent(b)/HDenom->GetBinContent(b);
//       HRatio->SetPoint(counter,HNum->GetBinCenter(b),ratio);


//       //float ERROR = HNum->GetBinError(b)/HDenom->GetBinContent(b);//only include numerator unc
//       float ERROR = sqrt(HNum->GetBinError(b)*HNum->GetBinError(b) + HDenom->GetBinError(b)*HDenom->GetBinError(b))/HDenom->GetBinContent(b);//include both 

//       HRatio->SetPointError(counter,
// 			    xerr ? HNum->GetBinWidth(b)/2:0.,
// 			    yerr ? ERROR : 0.);

      
//       std::cout<<ratio<<"*("<<HNum->GetBinLowEdge(b)<<"<x&&x<"<<HNum->GetBinLowEdge(b)+HNum->GetBinWidth(b)<<")+";  
//       counter++;
//     }
//   }

//   std::cout<<std::endl;

//   return HRatio;
// }

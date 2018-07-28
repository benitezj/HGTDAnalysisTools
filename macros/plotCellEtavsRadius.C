
////////Current parameters (r->eta)
// p0                        =      5.59809   +/-   0.00408999  
// p1                        =   -0.0188504   +/-   7.43826e-05 
// p2                        =  6.66887e-05   +/-   5.05347e-07 
// p3                        = -1.48097e-07   +/-   1.61169e-09 
// p4                        =  1.75012e-10   +/-   2.42919e-12 
// p5                        = -8.38774e-14   +/-   1.39386e-15 
// 5.59809-0.0188504*x+6.66887e-05*x*x-1.48097e-07*x*x*x+1.75012e-10*x*x*x*x-8.38774e-14*x*x*x*x*x

// /////////////////
// p0                        =      4691.33   +/-   0.093721    
// p1                        =     -2791.86   +/-   0.0392189   
// p2                        =        342.9   +/-   0.01213     
// p3                        =      128.261   +/-   0.00349912  
// p4                        =     -41.0255   +/-   0.000940926 
// p5                        =      3.33265   +/-   0.000204489 
// 4691.33-2791.86*x+342.9*x*x+128.261*x*x*x-41.0255*x*x*x*x+3.33265*x*x*x*x*x

void plotCellEtavsRadius(){

  TFile File("/data/det-hgtd/tuple/LArHitNov11/LArHit_July26_mu0_3CELLSIZE_40CELLENERGY_30TIMESMEAR/mc15_13TeV.301399.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_ZZ4nu.evgen.EVNT.e4397/tuple_0.root","read");
  TTree * T=(TTree*)File.Get("tuple");

  TF1 F("F","[0]+[1]*x+[2]*x*x+[3]*x*x*x+[4]*x*x*x*x+[5]*x*x*x*x*x",0,650);

  TH2F HEtaVsR("HEtaVsR","",1000,0,650,1000,2.4,4.5);
  T->Draw("abs(cell_eta):cell_r>>HEtaVsR");  
  HEtaVsR.Fit(&F);

  TH2F HRVsEta("HRVsEta","",1000,2.4,4.5,1000,0,650);
  T->Draw("cell_r:abs(cell_eta)>>HRVsEta");  
  HRVsEta.Fit(&F);


  TCanvas C;
  C.Clear();
  HEtaVsR.Draw();
  C.Print("HEtaVsR.png");

  C.Clear();
  HRVsEta.Draw();
  C.Print("HRVsEta.png");


}

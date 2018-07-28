#include "HGTDAnalysisTools/macros/globals.h"
#include "HGTDAnalysisTools/macros/plottingTools.C"

//////
///Note: PP1 and Moderator need to be last as they overlap and otherwise get covered.
///For eta, phi plots need to exclude LArMgr

TString exclude="Atlas,IDET,CALO,Tile,Muon,MUON,InDetServMat";
#define NDET 6
std::string detList[NDET]={"BeamPipe","Pixel","SCT","LArMgr","PP1","Moderator"};
int detColor[NDET]={1,2,4,7,6,3};
int detColorP[NDET+1]={0,1,2,4,7,6,3};


// TString exclude="Atlas,IDET,CALO,Tile,Muon,MUON,InDetServMat,LArMgr";
// #define NDET 5
// std::string detList[NDET]={"BeamPipe","Pixel","SCT","PP1","Moderator"};
// int detColor[NDET]={1,2,4,6,3};
// int detColorP[NDET+1]={0,1,2,4,6,3};


// TString exclude="Atlas,IDET,CALO,Tile,Muon,MUON,BeamPipe,SCT,InDetServMat";
// #define NDET 4
// std::string detList[NDET]={"Pixel","PP1","Moderator","LArMgr"};
// int detColor[NDET]={1,2,3,4};
// int detColorP[NDET+1]={0,1,2,3,4};



// TString exclude="Atlas,IDET,CALO,Tile,Muon,MUON,BeamPipe,Pixel,PP1,InDetServMat";
// #define NDET 3
// std::string detList[NDET]={"SCT","LArMgr","Moderator"};
// int detColor[NDET]={1,2,3};
// int detColorP[NDET+1]={0,1,2,3};


// TString exclude="Atlas,IDET,CALO,Tile,Muon,MUON,BeamPipe,Pixel,SCT,InDetServMat,LArMgr";
// #define NDET 2
// std::string detList[NDET]={"PP1","Moderator"};
// int detColor[NDET]={6,3};
// int detColorP[NDET+1]={0,6,3};


// TString exclude="Atlas,IDET,CALO,Tile,Muon,MUON,BeamPipe,Pixel,SCT,InDetServMat,PP1,LArMgr";
// #define NDET 1
// std::string detList[NDET]={"Moderator"};
// int detColor[NDET]={1};
// int detColorP[NDET+1]={0,3};



TObjArray * getObjList(TFile * inFile, TString Type, TString require, TString reject){
  
  TObjArray* list=new TObjArray();

  TDirectory*dir=(TDirectory*)inFile->Get("intLen");
  TKey *key;
  TIter next(dir->GetListOfKeys());
  while( (key = (TKey*)next()) ){

    ////////////////
    // Select TProfile2D objects
    /////////////////
    TObject * obj = key->ReadObj();
    if(!obj) continue;
    TString objclass(obj->ClassName());
    if(objclass.CompareTo(Type.Data())!=0) continue;


    TString name = key->GetName();

    ////////////////
    ///check its the desired Detectors/Materials
    ////////////////
    bool pass=1;
    TObjArray * reqtokens=require.Tokenize(",");
    if(!reqtokens) continue;
    int i=0;
    while(reqtokens->At(i)){
      TString token=((TObjString*)(reqtokens->At(i)))->GetString();
      if(!name.Contains(token.Data()))  pass=0;
      i++;
    }
    if(pass==0) continue;


    ////////////////
    ///remove other TProfiles with similar names
    ////////////////
    pass=1;
    TObjArray * rejtokens=reject.Tokenize(",");
    if(!rejtokens) continue;
    int j=0;
    while(rejtokens->At(j)){
      TString token=((TObjString*)(rejtokens->At(j)))->GetString();
      if(name.Contains(token.Data()))  pass=0;
      j++;
    }
    if(pass==0) continue;


    //////////////////////////////////
    //// exclude some detectors
    /////////////////////////////////////
    pass=1;
    TObjArray * excludetokens=exclude.Tokenize(",");
    if(!excludetokens) continue;
    int k=0;
    while(excludetokens->At(k)){
      TString token=((TObjString*)(excludetokens->At(k)))->GetString();
      if(name.Contains(token.Data()))  pass=0;
      k++;
    }
    if(pass==0) continue;


    list->Add(obj);    
    cout<<objclass<<" "<<name<<endl;
  }


  
  ///reorder the list:
  if(list->GetLast()!=NDET-1){
    cout<<"Array list "<<list->GetLast()<<" does not match number of expected detectors "<<NDET<<endl;
    return NULL;
  }
  gROOT->cd();
  TObjArray* orderlist=new TObjArray();
  for(int i=0;i<NDET;i++){
    int j=0;
    for(int j=0;j<=list->GetLast();j++){
      TProfile*obj=(TProfile*)(list->At(j));
      if(TString(obj->GetName()).Contains(detList[i].c_str())){
	cout<<i<<" "<<detList[i].c_str()<<" "<<j<<" "<<obj->GetName()<<endl;

	orderlist->Add(obj->Clone());
      }
    }
  }


  return orderlist;
}





void plot1D(TFile * inFile, TString outfile, TString require, TString reject,
	    TString title, float xmin=0, float xmax=0){

  if(!inFile) return;
  TObjArray * list = getObjList(inFile,"TProfile",require,reject);
  if(!list) return;


  TLegend leg_stack(0.71, 0.4, 0.95, 0.9);
  leg_stack.SetFillColor(0);
  leg_stack.SetShadowColor(0);
  leg_stack.SetBorderSize(0);

  THStack h_stack("plot1DStack","");
  int n_colour = 0;
  float range=0;
  int i=0;
  while(list->At(i)){
    TProfile*obj=(TProfile*)(list->At(i));

    n_colour=detColor[i];
    //if(n_colour==10) n_colour++;

    TH1D* h_plot = ((TProfile*)obj)->ProjectionX();
    h_plot->SetFillColor(n_colour);
    h_plot->SetLineColor(n_colour);
    h_plot->SetMarkerColor(n_colour);

    TObjArray * nametokens=TString(obj->GetName()).Tokenize("_");
    h_plot->SetTitle(((TObjString*)(nametokens->At(1)))->GetString());

    h_stack.Add(h_plot);

    leg_stack.AddEntry(h_plot,h_plot->GetTitle(),"f");

    if(range<h_plot->GetMaximum()) 
      range = h_plot->GetMaximum();

    i++;
  }


  TH1D* h_temp_last = (TH1D*)list->Last();
  h_temp_last->GetYaxis()->SetRangeUser(0.001,range*1.3);


  ////////////////////////////////////////
  TCanvas Canv("Canv","Title",800,600);
  Canv.SetFillColor(0);
  Canv.SetRightMargin(0.3);
  //Canv.SetLogy(1);
  Canv.Clear();

  h_stack.SetTitle("");
  h_stack.Draw("hist");
  h_stack.GetYaxis()->SetTitle("interaction lengths");
  h_stack.GetXaxis()->SetTitle(title);
  if(xmin!=0 || xmax!=0)
    h_stack.GetXaxis()->SetRangeUser(xmin,xmax);


  leg_stack.Draw();

  Canv.Update();
  Canv.Print(outfile+".png");
  Canv.Print(outfile+".eps");

}



void setProfileColor(TProfile2D*P, TH2F * PCol, float coloridx){
  if(!P||!PCol) return;
  
  for(int i=1;i<=P->GetXaxis()->GetNbins();i++){
    for(int j=1;j<=P->GetYaxis()->GetNbins();j++){
      if(P->GetBinContent(i,j)>1e-5){//need to use a threshold otherwise e.g. Moderator will cover Pixel
  	if(coloridx>0) 
	  PCol->SetBinContent(i,j,coloridx);
	else 
	  PCol->SetBinContent(i,j,P->GetBinContent(i,j));
      }
    }
  }
}

void plot2D(TFile * inFile, TString outfile, TString require, TString reject,
	    TString titlex, TString titley, float xmin=0, float xmax=0, float ymin=0, float ymax=0){

  if(!inFile) return;

  bool ColorPlot=1;

  TObjArray * list = getObjList(inFile,"TProfile2D",require,reject);
  if(!list) return;


  ////////////////////////////////////////
  TCanvas Canv("Canv","Title",1000,600);
  Canv.SetFillColor(0);
  //Canv.SetRightMargin(0.35);
  Canv.SetRightMargin(0.2);


  TLegend leg_stack(0.81,0.2,1.0, 0.9);
  leg_stack.SetFillColor(0);
  leg_stack.SetFillStyle(0);
  leg_stack.SetShadowColor(0);
  leg_stack.SetBorderSize(0);


  int i=0;
  while(list->At(i)){
    TProfile2D*h=(TProfile2D*)(list->At(i));

    TObjArray * nametokens=TString(h->GetName()).Tokenize("_");
    h->SetFillColor(detColor[i]);
    leg_stack.AddEntry(h,((TObjString*)(nametokens->At(2)))->GetString(),"f");

    i++;
  }

  //////////////////////////
  TProfile2D * P0=(TProfile2D*)(list->At(0));
  if(!P0) return;

  ///create a new total 2D plot
  TH2F* P0Col=new TH2F(TString("Col")+P0->GetName(),"",
		      P0->GetXaxis()->GetNbins(),P0->GetXaxis()->GetXmin(),P0->GetXaxis()->GetXmax(),
		      P0->GetYaxis()->GetNbins(),P0->GetYaxis()->GetXmin(),P0->GetYaxis()->GetXmax()
		      );

  i=0;
  while( TProfile2D * P = (TProfile2D*)(list->At(i))){
    setProfileColor(P,P0Col,ColorPlot ? i+1.0 : 0);
    //cout<<P->GetName()<<" "<<i+1.0<<" "<<detColorP[i+1]<<endl;

    i++;
  }


  if(xmin!=0 || xmax!=0)P0Col->GetXaxis()->SetRangeUser(xmin,xmax);
  if(ymin!=0 || ymax!=0)P0Col->GetYaxis()->SetRangeUser(ymin,ymax);
  //P0Col->GetZaxis()->SetRangeUser(0.001,range*1.3);
  P0Col->GetZaxis()->SetRangeUser(0,NDET+1);
  //P0Col->GetZaxis()->SetTitle("interaction lengths");
  P0Col->GetXaxis()->SetTitle(titlex);
  P0Col->GetYaxis()->SetTitle(titley);
  P0Col->GetXaxis()->SetNdivisions(5);
  P0Col->GetYaxis()->SetNdivisions(5);
  P0Col->GetYaxis()->SetTickSize(0);
  P0Col->GetXaxis()->SetTickSize(0);


  Canv.Clear();
  P0Col->Draw(ColorPlot ? "col" : "colz");
  if(ColorPlot){
    leg_stack.Draw();
    gStyle->SetPalette(NDET+1,detColorP);
  }
  Canv.Update();
  Canv.Print(outfile+".png");
  Canv.Print(outfile+".eps");

}




void G4ScanPlots() {

  SetAtlasStyle();
  gStyle->SetPalette(1,0);


  //TString infile="/data/det-hgtd/hits/G4Scan_s3072/G4Scan_histo.root";
  //TString outdir="/afs/cern.ch/user/b/benitezj/www/HGTDStudies/MaterialScans/s3072";

  TString infile="/nfs/uiowapc/data/det-hgtd/hits/G4Scan_s3072_extended/G4Scan_histo.root";
  TString outdir="/afs/cern.ch/user/b/benitezj/www/HGTDStudies/MaterialScans/G4Scan_s3072_extended";

  // TString infile="/nfs/uiowapc/data/det-hgtd/hits/G4Scan_s3072_extended_removeITKMat/G4Scan_histo.root";
  // TString outdir="/afs/cern.ch/user/b/benitezj/www/HGTDStudies/MaterialScans/G4Scan_s3072_extended_removeITKMat";

  //TString infile="/nfs/uiowapc/data/det-hgtd/hits/G4Scan_s3072_extended_removeITKMatAndSCT/G4Scan_histo.root";
  //TString outdir="/afs/cern.ch/user/b/benitezj/www/HGTDStudies/MaterialScans/G4Scan_s3072_extended_removeITKMatAndSCT";



  ////////////////////////////////////////////////////////////////
  //system(TString("rm -rf ")+outdir);
  system(TString("mkdir ")+outdir);
  TFile inFile(infile.Data(),"read");


  //RZdistribution by Detector
  //plot2D(&inFile,outdir+"/Detector_rz","RZIntLen_D_","","z  [mm]","r  [mm]"); 
  plot2D(&inFile,outdir+"/Detector_rz_Forward","RZIntLen_D_","","z  [mm]","r  [mm]",0,3700,0,700); 
  plot2D(&inFile,outdir+"/Detector_rz_HGTD","RZIntLen_D_","","z  [mm]","r  [mm]",2700,3700,0,700); 

  //Eta distribution by Detector
  //plot1D(&inFile,outdir+"/Detector_eta","D_","Phi_,R_,Z_","#eta",-5,5); 
  plot1D(&inFile,outdir+"/Detector_eta_Forward","D_","Phi_,R_,Z_","#eta",0,5); 

  //Phi distribution by Detector
  plot1D(&inFile,outdir+"/Detector_phi","D_,Phi_","Z_,R_","#phi"); 

  // //Radial distribution by Detector
  // plot1D(&inFile,outdir+"/Detector_r","D_,R_","Phi_,Z_","radius   [mm]"); 

  // //Z distribution by Detector
  // plot1D(&inFile,outdir+"/Detector_z","D_,Z_","Phi_,R_","z   [mm]"); 


  //string searchstring = "D_";
  //string searchstring = "M_";
  //string searchstring = "E_";
  //string searchstring = "DM_Pixel_";
  //string searchstring = "DE_Pixel_";
  //RZIntLen_D_Tile

  gROOT->ProcessLine(".q");
}

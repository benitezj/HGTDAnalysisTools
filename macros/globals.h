#include <iostream>
#include <iomanip>
#include <sys/stat.h>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h> 
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <TMath.h>
#include <map>
using namespace std;

#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TH1F.h>
#include <TF1.h>
#include <TDirectory.h>
#include <TString.h>
#include <TLegend.h>
#include <TProfile.h>
#include <TLatex.h>
#include <TGaxis.h>


//#include "AtlasLabels.h"
//#include "AtlasStyle.h"
#include "/nfs/home/benitezj/Notes/atlasstyle/AtlasStyle.C"
#include "/nfs/home/benitezj/Notes/atlasstyle/AtlasUtils.C"
#include "/nfs/home/benitezj/Notes/atlasstyle/AtlasLabels.C"

TString INDIR="/data/det-hgtd/tuple";
TString OUTDIR="/afs/cern.ch/user/b/benitezj/www/HGTDStudies/HitPlots";
TChain* TREE = NULL; 

TCanvas C1D("C1D","",900,600);
TCanvas C2D("C2D","",700,700);

#define NMAXINPUTFILES 1000
#define ETAMIN 2.6
#define ETAMAX 3.8
#define ETABINS 2
#define PTMIN 30000
#define PTMAX 70000
#define PTBINS 2

int ETACOLOR[ETABINS]={1,2};

TString etamin="2.6";
TString etamax="3.8";
long jetPtL=PTMIN;
long jetPtH=PTMAX;
int TIMEWINDOW=30;
TString timecut="0.090";
TString plottag="_Timing";

#define NJETTIMEPTBINS 10
float ptbinning[NJETTIMEPTBINS+1]={0,10,20,30,40,50,60,80,100,140,200};

#define NTRKMATCHEFFPTBINS 10
float ptbinstrkmatch[NTRKMATCHEFFPTBINS+1]={0,1,2,3,4,5,6,8,10,15,25};

//#define NTRKMATCHEFFPTBINS 8
//float ptbinstrkmatch[NTRKMATCHEFFPTBINS+1]={0,1,2,3,5,6,10,15,25};


///2D dEta x dPhi displays:
TString dEta="(abs(cell_eta)-abs(jet_eta[cell_jet]))";
TString dPhi="(cell_phi-jet_phi[cell_jet]-(abs(cell_phi-jet_phi[cell_jet])>3.14159)*((cell_phi-jet_phi[cell_jet])/abs(cell_phi-jet_phi[cell_jet]))*2*3.14159)";

TString dEtaClus="(abs(clus_eta)-abs(jet_eta[clus_jet]))";
TString dPhiClus="(clus_phi-jet_phi[clus_jet]-(abs(clus_phi-jet_phi[clus_jet])>3.14159)*((clus_phi-jet_phi[clus_jet])/abs(clus_phi-jet_phi[clus_jet]))*2*3.14159)";

TString dEtaCellClus="(abs(cell_eta)-abs(clus_eta[cell_clus]))";
TString dPhiCellClus="(cell_phi-clus_phi[cell_clus]-(abs(cell_phi-clus_phi[cell_clus])>3.14159)*((cell_phi-clus_phi[cell_clus])/abs(cell_phi-clus_phi[cell_clus]))*2*3.14159)";

TString dEtaHGTrk="(abs(hgtrk_deta)>0.5?abs(hgtrk_eta)-abs(jet_eta[hgtrk_jet]):0)";
TString dPhiHGTrk="(abs(hgtrk_deta)>0.5?hgtrk_phi-jet_phi[hgtrk_jet]-(abs(hgtrk_phi-jet_phi[hgtrk_jet])>3.14159)*((hgtrk_phi-jet_phi[hgtrk_jet])/abs(hgtrk_phi-jet_phi[hgtrk_jet]))*2*3.14159:0)";
TString dEtadHGTrk="(abs(hgtrk_deta)>0.5?abs(hgtrk_deta)-abs(jet_eta[hgtrk_jet]):0)";
TString dPhidHGTrk="(abs(hgtrk_deta)>0.5?hgtrk_dphi-jet_phi[hgtrk_jet]-(abs(hgtrk_dphi-jet_phi[hgtrk_jet])>3.14159)*((hgtrk_dphi-jet_phi[hgtrk_jet])/abs(hgtrk_dphi-jet_phi[hgtrk_jet]))*2*3.14159:0)";

TString dEtaCellTrk="(abs(cell_eta)-abs(trk_eta[cell_trk]))";
TString dPhiCellTrk="(cell_phi-trk_phi[cell_trk]-(abs(cell_phi-trk_phi[cell_trk])>3.14159)*((cell_phi-trk_phi[cell_trk])/abs(cell_phi-trk_phi[cell_trk]))*2*3.14159)";

////Trigger stuff
TF1 FEtaVsR("FEtaVsR","5.59809-0.0188504*x+6.66887e-05*x*x-1.48097e-07*x*x*x+1.75012e-10*x*x*x*x-8.38774e-14*x*x*x*x*x",100,650);
TF1 FRVsEta("FRVsEta","4691.33-2791.86*x+342.9*x*x+128.261*x*x*x-41.0255*x*x*x*x+3.33265*x*x*x*x*x",FEtaVsR.Eval(FEtaVsR.GetXmax()),FEtaVsR.Eval(FEtaVsR.GetXmin())); 
TString TriggerTruthSelection="(trig_r<=600&&trig_jet>=0&&jet_truth_pt[trig_jet]>10000&&jet_pt[trig_jet]<90000)";//
TString TriggerCut;//only for tiles
TString TriggerCutJet;//usable for jets

//////
TLatex text;
TLine line;
TString FitGausFormula("[0]*exp(-0.5*(x-[1])**2/[2]**2)");
TString FitGausText("Mean=%.2f+/-%.2f,  Sigma=%.2f+/-%.2f");
TString FitGausTextSigma("Sigma=%.0f ps");


////dR fit functions (from fit with 0.1 binning)
TF1 dRFitL("dRFitL","[0]+[1]*3809.38*exp(-6.09719*x^0.5)",0,0.4);
TF1 dRFitM("dRFitM","[0]+[1]*7065.16*exp(-6.53396*x^0.5)",0,0.4);
TF1 dRFitH("dRFitH","[0]+[1]*8413.39*exp(-6.63098*x^0.5)",0,0.4);

/////event displays
map<long,long> jetpt;
map<long,float> jeteta;
map<long,float> jetphi;

/////Jet selections
TString JetEtaCut;
TString JetPtCut;

TString pujetCUT;
TString signaljetCUT;
TString qcdjetCUT;
TString stocjetCUT;
TString otherjetCUT;
TString nonsigqcdjetCUT; //not signal and not QCD

int JETTYPECOLOR[4]={2,4,3,5};

TString CUTEtaJetCell;
TString CUTPtJetCell;
TString CUTPUJetCell;
TString CUTSignalJetCell;

TString CUTPUJetClus;
TString CUTSignalJetClus;

TString CUTEtaJetClusterCell;
TString CUTPtJetClusterCell;
TString CUTSignalJetClusterCell;
TString CUTPUJetClusterCell;


TString CUTEtaJetHGTrk;
TString CUTPtJetHGTrk;
TString CUTPUJetHGTrk;
TString CUTSignalJetHGTrk;

//////////////////////////////////////
void DrawATLAS(float x=.21,float y=0.88){
  //ATLASLabel(x,y,"Simulation Preliminary",1); 
  ATLASLabel(x,y,"Simulation internal",1); 
}

void DrawITK(float x=.21,float y=0.88){
  text.SetTextColor(1);
  text.SetTextSize(0.040);
  text.DrawLatexNDC(x,y,"ITK Inclined");
}
void DrawHGTD(TString outpath="",long tres=30,long layer=0,float x=0.20, float y=0.82,TString comment=""){//outpath defines Timing vs Preshower, layer=0 : all layers
  text.SetTextColor(1);

  text.SetTextSize(0.040);
  text.DrawLatexNDC(x,y,outpath.Contains("Preshower") ? "#it{HGTD-SiW}" : "#it{HGTD-Si}");
  
  if(comment.CompareTo("")!=0) 
    text.DrawLatexNDC(x,y-0.05,comment);

  text.SetTextSize(0.030);
  if(layer>0) 
    text.DrawLatexNDC(x+0.13,y,TString("Layer ")+(layer-1));
  else if(layer==0) 
    text.DrawLatexNDC(x+0.13,y,TString("All layers "));

  text.SetTextSize(0.030);
  if(tres!=0)
    text.DrawLatexNDC(x,y-0.06,TString("#sigma_{t} = ")+tres+" ps"); 
}

void DrawVBFH(TString outpath="",long M=125){
  text.SetTextColor(1);
  text.SetTextSize(0.035);
  float ypos=0.96;
  text.DrawLatexNDC(0.21,ypos,TString("VBF h(")+M+" GeV)#rightarrowinv.");
  // if(outpath.Contains("_mu0"))text.DrawLatexNDC(0.67,ypos,TString("#sqrt{s}=13 TeV, #mu=0"));
  // if(outpath.Contains("_mu200"))text.DrawLatexNDC(0.67,ypos,TString("#sqrt{s}=13 TeV, #mu=200"));
}

void DrawECMSMu(TString outpath="",float x=0.68,float y=0.96){
  text.SetTextColor(1);
  text.SetTextSize(0.035);
  if(outpath.Contains("_mu0"))
    text.DrawLatexNDC(x,y,TString("#sqrt{s}=14 TeV, #mu=0"));
  if(outpath.Contains("_mu200"))
    text.DrawLatexNDC(x,y,TString("#sqrt{s}=14 TeV, #mu=200"));
}
void DrawSinglePi(long pt=0, float x=0.6,float y=0.97){
  text.SetTextColor(1);
  text.SetTextSize(0.035);
  if(pt>0)
    text.DrawLatexNDC(x,y,TString("single #pi^{+},  p_{T} = ")+pt+" GeV,  #mu=0");
  else text.DrawLatexNDC(x,y,"single #pi^{+},  p_{T} = 1 - 20 GeV,  #mu=0");
}


void DrawDiscriminatorLegend(TH1F*HSignal,TH1F*HPU){
  if(!HSignal || !HPU) return;

  text.SetTextSize(0.035);
  text.DrawLatexNDC(0.65,0.86,"Anti-k_{t} EM+JES (R=0.4) jets");  
  text.SetTextSize(0.040);
  text.DrawLatexNDC(0.65,0.81,TString("")+(jetPtL/1000)+" GeV<p_{T}<"+(jetPtH/1000)+" GeV");
  text.DrawLatexNDC(0.65,0.76,etamin+"<|#eta|<"+etamax);


  TLegend Leg;
  Leg.AddEntry(HSignal,"h.s. jet","l");
  Leg.AddEntry(HPU,"p.u. jet","l");
  Leg.SetFillStyle (0);
  Leg.SetFillColor (0);
  Leg.SetBorderSize(0);
  Leg.SetTextSize(.04);
  Leg.SetX1NDC(0.70);  
  Leg.SetY1NDC(0.60);
  Leg.SetX2NDC(0.90);  
  Leg.SetY2NDC(0.72);
  Leg.DrawClone();


}

////////////////////////////////////////////////////////////////
void setCuts(TString SAMPLE){

  C1D.SetTopMargin(0.05); 
  C1D.SetRightMargin(0.05);
  C1D.SetBottomMargin(0.20); 
  C1D.SetLeftMargin(0.15);

  C2D.SetTopMargin(0.15); 
  C2D.SetRightMargin(0.15);
  C2D.SetBottomMargin(0.15); 
  C2D.SetLeftMargin(0.15);

  gStyle->SetLabelSize  (0.040,"X");
  gStyle->SetTitleSize  (0.055,"X");
  gStyle->SetTitleOffset(1.100,"X");
  gStyle->SetLabelSize  (0.040,"Y");
  gStyle->SetTitleSize  (0.055,"Y");
  //gStyle->SetTitleOffset(1.200,"Y");

  gStyle->SetLabelSize  (0.040,"Z");
  //gStyle->SetTitleSize  (0.055,"X");



  /////get a plot tag depending on the sample
  if(SAMPLE.Contains("Preshower"))
    plottag="_Preshower";

  if(SAMPLE.Contains("_mu0"))
    plottag+="_mu0";
  else plottag+="_mu200";


  JetEtaCut=TString("(")+etamin+"<abs(jet_eta)&&abs(jet_eta)<"+etamax+")"; 
  
  ////nominal jet eta and pt cuts
  // signaljetCUT=TString("(jet_truthJ>=0&&truthjet_vtx[jet_truthJ]==0&&truthjet_pt[jet_truthJ]>10000)");
  // qcdjetCUT=TString("(jet_truthJ>=0&&truthjet_vtx[jet_truthJ]>0&&truthjet_pt[jet_truthJ]>10000)");
  // stocjetCUT=TString("(jet_truthJ<0||(jet_truthJ>=0&&truthjet_vtx[jet_truthJ]>0&&truthjet_pt[jet_truthJ]<10000))");
  // otherjetCUT=TString("(!")+signaljetCUT+"&&!"+stocjetCUT+"&&!"+qcdjetCUT+")";  
  // pujetCUT=TString("(")+stocjetCUT+"||"+qcdjetCUT+")";

  signaljetCUT=TString("(jet_truthjhs_dr3pt10>=0)");
  qcdjetCUT=TString("(jet_truthjhs_dr3pt4<0&&jet_truthjpu_dr3pt10>=0)");
  stocjetCUT=TString("(jet_truthjhs_dr3pt4<0&&jet_truthjpu_dr6pt10<0)");
  pujetCUT=TString("(jet_truthjhs_dr3pt4<0)");

  /////new tighter definitions requireing H.S. and QCD with dR<0.1 to truth jet
  // signaljetCUT=TString("(jet_truthjhs_dr3pt10>=0&&sqrt(pow(jet_eta-truthjet_eta[jet_truthjhs_dr3pt10],2) + pow(abs(jet_phi-truthjet_phi[jet_truthjhs_dr3pt10])<3.14159?abs(jet_phi-truthjet_phi[jet_truthjhs_dr3pt10]): 2*3.14159 - abs(jet_phi-truthjet_phi[jet_truthjhs_dr3pt10]),2))<0.1)");
  // qcdjetCUT=TString("(jet_truthjhs_dr3pt4<0&&jet_truthjpu_dr3pt10>=0&&sqrt(pow(jet_eta-truthjet_eta[jet_truthjpu_dr3pt10],2) + pow(abs(jet_phi-truthjet_phi[jet_truthjpu_dr3pt10])<3.14159?abs(jet_phi-truthjet_phi[jet_truthjpu_dr3pt10]): 2*3.14159 - abs(jet_phi-truthjet_phi[jet_truthjpu_dr3pt10]),2))<0.1)");
  // stocjetCUT=TString("(jet_truthjhs_dr3pt4<0")+"&&!"+qcdjetCUT+")";
  // otherjetCUT=TString("(!")+signaljetCUT+"&&!"+qcdjetCUT+"&&!"+stocjetCUT+")";  
  // pujetCUT=TString("(jet_truthjhs_dr3pt4<0)");
  

  CUTPUJetCell=TString("(cell_jet>=0&&jet_truthJ[cell_jet]<0)");
  CUTSignalJetCell=TString("(cell_jet>=0&&jet_truth_pt[cell_jet]>10000)");  

  CUTPUJetClus=TString("(clus_jet>=0&&jet_truthJ[clus_jet]<0)");
  CUTSignalJetClus=TString("(clus_jet>=0&&jet_truthJ[clus_jet]>=0&&truthjet_pt[jet_truthJ[clus_jet]]>10000)");  

  CUTPUJetClusterCell=TString("(cell_clus>=0&&clus_jet[cell_clus]>=0&&jet_truthJ[clus_jet[cell_clus]]<0)");
  CUTSignalJetClusterCell=TString("(cell_clus>=0&&clus_jet[cell_clus]>=0&&jet_truthJ[clus_jet[cell_clus]]>=0&&truthjet_pt[jet_truthJ[clus_jet[cell_clus]]]>10000)");

  CUTPUJetHGTrk=TString("(hgtrk_jet>=0&&hgtrk_jetdR<0.2&&jet_truthJ[hgtrk_jet]<0)");
  CUTSignalJetHGTrk=TString("(hgtrk_jet>=0&&hgtrk_jetdR<0.2&&jet_truthJ[hgtrk_jet]>=0&&truthjet_pt[jet_truthJ[hgtrk_jet]]>10000)");  

  if(SAMPLE.Contains("_mu0")){
    ///Note must use truth jet pT because calibrated pT uses calibration for mu=200 so many very low pT jets would pass the selection
    JetPtCut=TString("(")+jetPtL+"<jet_truth_pt&&jet_truth_pt<"+jetPtH+")";
  
    CUTEtaJetCell=TString("(cell_jet>=0&&")+etamin+"<abs(jet_eta[cell_jet])&&abs(jet_eta[cell_jet])<"+etamax+")"; 
    CUTPtJetCell=TString("(cell_jet>=0&&")+jetPtL+"<jet_truth_pt[cell_jet]&&jet_truth_pt[cell_jet]<"+jetPtH+")";  

    CUTEtaJetHGTrk=TString("(hgtrk_jet>=0&&")+etamin+"<abs(jet_eta[hgtrk_jet])&&abs(jet_eta[hgtrk_jet])<"+etamax+")"; 
    CUTPtJetHGTrk=TString("(hgtrk_jet>=0&&")+jetPtL+"<jet_truth_pt[hgtrk_jet]&&jet_truth_pt[hgtrk_jet]<"+jetPtH+")";  

    
  }else {
    JetPtCut=TString("(")+jetPtL+"<jet_pt&&jet_pt<"+jetPtH+")";
    
    CUTEtaJetCell=TString("(cell_jet>=0&&")+etamin+"<abs(jet_eta[cell_jet])&&abs(jet_eta[cell_jet])<"+etamax+")"; 
    CUTPtJetCell=TString("(cell_jet>=0&&")+jetPtL+"<jet_pt[cell_jet]&&jet_pt[cell_jet]<"+jetPtH+")";  
    
    // CUTPUJetClus=TString("(clus_jet>=0&&jet_truth_pt[clus_jet]<=0)");
    // CUTSignalJetClus=TString("(clus_jet>=0&&jet_truth_pt[clus_jet]>0)");  

    CUTEtaJetHGTrk=TString("(hgtrk_jet>=0&&")+etamin+"<abs(jet_eta[hgtrk_jet])&&abs(jet_eta[hgtrk_jet])<"+etamax+")"; 
    CUTPtJetHGTrk=TString("(hgtrk_jet>=0&&")+jetPtL+"<jet_pt[hgtrk_jet]&&jet_pt[hgtrk_jet]<"+jetPtH+")";  
  }


  ///Trigger
  if(SAMPLE.Contains("Preshower")){
    if(SAMPLE.Contains("_mu0")){
      //this is for 50% ==> Needs updating after fixing tile-jet matching 
      TriggerCut="((trig_size<40)*(24*(110<trig_r&&trig_r<=120)+29*(120<trig_r&&trig_r<=130)+28*(140<trig_r&&trig_r<=150)+28*(150<trig_r&&trig_r<=160)+29*(160<trig_r&&trig_r<=170)+19*(180<trig_r&&trig_r<=190)+19*(190<trig_r&&trig_r<=200))+(40<trig_size&&trig_size<60)*(41*(170<trig_r&&trig_r<=180)+29*(210<trig_r&&trig_r<=220)+22*(220<trig_r&&trig_r<=230)+19*(230<trig_r&&trig_r<=240)+27*(240<trig_r&&trig_r<=250)+22*(250<trig_r&&trig_r<=260)+29*(270<trig_r&&trig_r<=280)+26*(280<trig_r&&trig_r<=290)+46*(290<trig_r&&trig_r<=300)+56*(320<trig_r&&trig_r<=330))+(60<trig_size&&trig_size<120)*(67*(350<trig_r&&trig_r<=360)+47*(370<trig_r&&trig_r<=380)+36*(420<trig_r&&trig_r<=430)+26*(490<trig_r&&trig_r<=500))+(trig_size>120)*(58*(500<trig_r&&trig_r<=510)+28*(570<trig_r&&trig_r<=580)))";
    }else{
      //this is for 50%
      TriggerCut="((trig_size<40)*(43*(110<trig_r&&trig_r<=120)+42*(120<trig_r&&trig_r<=130)+42*(140<trig_r&&trig_r<=150)+43*(150<trig_r&&trig_r<=160)+35*(160<trig_r&&trig_r<=170)+29*(180<trig_r&&trig_r<=190)+27*(190<trig_r&&trig_r<=200))+(40<trig_size&&trig_size<60)*(58*(170<trig_r&&trig_r<=180)+45*(210<trig_r&&trig_r<=220)+38*(220<trig_r&&trig_r<=230)+38*(230<trig_r&&trig_r<=240)+36*(240<trig_r&&trig_r<=250)+35*(250<trig_r&&trig_r<=260)+46*(270<trig_r&&trig_r<=280)+51*(280<trig_r&&trig_r<=290)+60*(290<trig_r&&trig_r<=300)+61*(320<trig_r&&trig_r<=330))+(60<trig_size&&trig_size<120)*(121*(350<trig_r&&trig_r<=360)+104*(370<trig_r&&trig_r<=380)+76*(420<trig_r&&trig_r<=430)+54*(490<trig_r&&trig_r<=500))+(trig_size>120)*(165*(500<trig_r&&trig_r<=510)+88*(570<trig_r&&trig_r<=580)))";
    }

  }else{
    if(SAMPLE.Contains("_mu0")){
      ///this is for 50% ==> Needs updating after fixing tile-jet matching 
      TriggerCut="((trig_size<40)*(23*(110<trig_r&&trig_r<=120)+24*(120<trig_r&&trig_r<=130)+28*(140<trig_r&&trig_r<=150)+36*(150<trig_r&&trig_r<=160)+24*(160<trig_r&&trig_r<=170)+17*(180<trig_r&&trig_r<=190)+18*(190<trig_r&&trig_r<=200))+(40<trig_size&&trig_size<60)*(39*(170<trig_r&&trig_r<=180)+25*(210<trig_r&&trig_r<=220)+25*(220<trig_r&&trig_r<=230)+25*(230<trig_r&&trig_r<=240)+19*(240<trig_r&&trig_r<=250)+19*(250<trig_r&&trig_r<=260)+16*(270<trig_r&&trig_r<=280)+19*(280<trig_r&&trig_r<=290)+13*(290<trig_r&&trig_r<=300)+13*(320<trig_r&&trig_r<=330))+(60<trig_size&&trig_size<120)*(21*(350<trig_r&&trig_r<=360)+17*(370<trig_r&&trig_r<=380)+11*(420<trig_r&&trig_r<=430)+9*(490<trig_r&&trig_r<=500))+(trig_size>120)*(18*(500<trig_r&&trig_r<=510)+10*(570<trig_r&&trig_r<=580)))";
    }else{
      //this is 50%
      TriggerCut="((trig_size<40)*(42*(110<trig_r&&trig_r<=120)+44*(120<trig_r&&trig_r<=130)+44*(140<trig_r&&trig_r<=150)+43*(150<trig_r&&trig_r<=160)+36*(160<trig_r&&trig_r<=170)+31*(180<trig_r&&trig_r<=190)+26*(190<trig_r&&trig_r<=200))+(40<trig_size&&trig_size<60)*(59*(170<trig_r&&trig_r<=180)+44*(210<trig_r&&trig_r<=220)+39*(220<trig_r&&trig_r<=230)+38*(230<trig_r&&trig_r<=240)+41*(240<trig_r&&trig_r<=250)+34*(250<trig_r&&trig_r<=260)+31*(270<trig_r&&trig_r<=280)+29*(280<trig_r&&trig_r<=290)+24*(290<trig_r&&trig_r<=300)+22*(320<trig_r&&trig_r<=330))+(60<trig_size&&trig_size<120)*(61*(350<trig_r&&trig_r<=360)+55*(370<trig_r&&trig_r<=380)+37*(420<trig_r&&trig_r<=430)+27*(490<trig_r&&trig_r<=500))+(trig_size>120)*(81*(500<trig_r&&trig_r<=510)+43*(570<trig_r&&trig_r<=580)))";
    }
  }

  //cout<<"Trigger Cut: "<<TriggerCut<<endl;

  //fix trigger cut for jets
  TriggerCutJet=TriggerCut;
  TriggerCutJet.ReplaceAll("trig_size","trig_size[jet_trig]");
  TriggerCutJet.ReplaceAll("trig_r","trig_r[jet_trig]");
  //cout<<"Jet Trigger Cut: "<<TriggerCutJet<<endl;


}


///////////////////////////////////////////////////////
TH1F* HNCELLvsR = NULL; 
TH1F* HVBFH_pT=0;
TH1F* HVBFH_eta=0;
TH1F* HVBFJ1_pT=0;
TH1F* HVBFJ1_eta=0;
TH1F* HVBFJ2_pT=0;
TH1F* HVBFJ2_eta=0;
TH1F* HVBFG_pT=0;
TH1F* HVBFG_eta=0;


void loadHistos(TString inpath){
  
  for(long i=0;i<=NMAXINPUTFILES;i++){

    TString fname=inpath+"/tuple_"+(long)i;
    struct stat st;
    if(stat((fname+".root").Data(),&st) != 0){
      continue;
    }  
    TFile file((fname+".root").Data(),"read");
    if(file.IsZombie()){
      continue;
    }
    if(!file.GetListOfKeys()){
      continue;
    }
    if(file.GetListOfKeys()->GetSize()==0){
      continue;            
    }
    gROOT->cd();

    if(!HNCELLvsR){
      HNCELLvsR = (TH1F*) file.Get("HNCell"); //Use only one file
      if(HNCELLvsR){
	HNCELLvsR = (TH1F*) HNCELLvsR->Clone("HNCELLvsR");
      }
    }

    if(file.Get("HVBFJ1_pT")){
      if(!HVBFJ1_pT)HVBFJ1_pT=(TH1F*)((TH1F*)(file.Get("HVBFJ1_pT"))->Clone("HVBFJ1_pT_"));
      else HVBFJ1_pT->Add((TH1F*)(file.Get("HVBFJ1_pT")));
    }
    if(file.Get("HVBFJ1_eta")){
      if(!HVBFJ1_eta) HVBFJ1_eta=(TH1F*)((TH1F*)(file.Get("HVBFJ1_eta"))->Clone("HVBFJ1_eta_"));
      else HVBFJ1_eta->Add((TH1F*)(file.Get("HVBFJ1_eta")));
    }
    
    if(file.Get("HVBFJ2_pT")){
      if(!HVBFJ2_pT)HVBFJ2_pT=(TH1F*)((TH1F*)(file.Get("HVBFJ2_pT"))->Clone("HVBFJ2_pT_"));
      else HVBFJ2_pT->Add((TH1F*)(file.Get("HVBFJ2_pT")));
    }
    if(file.Get("HVBFJ2_eta")){
      if(!HVBFJ2_eta) HVBFJ2_eta=(TH1F*)((TH1F*)(file.Get("HVBFJ2_eta"))->Clone("HVBFJ2_eta_"));
      else HVBFJ2_eta->Add((TH1F*)(file.Get("HVBFJ2_eta")));
    }
    
    if(file.Get("HVBFG_pT")){
      if(!HVBFG_pT)HVBFG_pT=(TH1F*)((TH1F*)(file.Get("HVBFG_pT"))->Clone("HVBFG_pT_"));
      else HVBFG_pT->Add((TH1F*)(file.Get("HVBFG_pT")));
    }
    if(file.Get("HVBFG_eta")){
      if(!HVBFG_eta) HVBFG_eta=(TH1F*)((TH1F*)(file.Get("HVBFG_eta"))->Clone("HVBFG_eta_"));
      else HVBFG_eta->Add((TH1F*)(file.Get("HVBFG_eta")));
    }
    

    
  }


  if(!HVBFJ1_pT) cout<<"HVBFJ1_pT not found"<<endl;
  if(!HVBFJ1_eta) cout<<"HVBFJ1_eta not found"<<endl;
  if(!HVBFJ2_pT) cout<<"HVBFJ2_pT not found"<<endl;
  if(!HVBFJ2_eta) cout<<"HVBFJ2_eta not found"<<endl;
  if(!HVBFG_pT) cout<<"HVBFG_pT not found"<<endl;
  if(!HVBFG_eta) cout<<"HVBFG_eta not found"<<endl;

  if(!HNCELLvsR)  cout<<" No file was found for loading histograms"<<endl;


  cout<<"Histograms have been loaded"<<endl;
}


void loadChain(TString inpath){
  cout<<"Loading tuples : "<<inpath<<endl;
  
  if(!TREE)//allow for previous load 
    TREE = new TChain("tuple");

  int badFiles=0;
  for(long i=0;i<=NMAXINPUTFILES;i++){

    TString fname=inpath+"/tuple_"+(long)i;
    struct stat st;
    if(stat((fname+".root").Data(),&st) != 0){
      if(i==0) cout<<(fname+".root")<<" Not found"<<endl;     
      continue;
    }  
    TFile file((fname+".root").Data(),"read");
    if(file.IsZombie()){
      cout<<(fname+".root")<<" is Zombie"<<endl;        badFiles++;       continue;
    }
    if(!file.GetListOfKeys()){
      cout<<(fname+".root")<<" has no Keys"<<endl;            badFiles++;       continue;
    }
    if(file.GetListOfKeys()->GetSize()==0){
      cout<<(fname+".root")<<" KeysSize = 0"<<endl;            badFiles++;       continue;            
    }
    TTree*t=(TTree*)file.Get("tuple");
    if(t==0){
      cout<<(fname+".root")<<" TTree == NULL"<<endl;            badFiles++;       continue;            
    }
    if(t->GetEntries()==0){
      cout<<(fname+".root")<<" TTree empty"<<endl;            badFiles++;       continue;            
    }
    
    TREE->Add((fname+".root").Data());    
  }

  cout<<"Number of input events "<<TREE->GetEntries()<<endl;
  cout<<"Number of bad input files "<<badFiles<<endl;

  
  //loadHistos(inpath);


}



//////////////////////////////////////////////////////////////
///for the ROC curves 
#define NROCSCANPOINTS 100
TTree * ROCTree;
int roc_nhit;
float roc_eta;
float roc_pt;
float roc_eff_sig; 
float roc_eff_sig_err; 
float roc_eff_pu; 
float roc_eff_pu_err;   
float roc_eff_qcd; 
float roc_eff_qcd_err;   
float roc_eff_stoc; 
float roc_eff_stoc_err;   
void setROCTree(TFile * File){
  if(!File){
    cout<<" No File provided for setROCTree"<<endl;
    return;
  }

  ROCTree =new TTree("tuple","tuple");
  ROCTree->SetDirectory(File);  
  ROCTree->Branch("nhit",&roc_nhit,"nhit/I");
  ROCTree->Branch("eta",&roc_eta,"eta/F");
  ROCTree->Branch("pt",&roc_pt,"pt/F");

  ROCTree->Branch("eff_sig",&roc_eff_sig,"eff_sig/F");
  ROCTree->Branch("eff_sig_err",&roc_eff_sig_err,"eff_sig_err/F");

  ROCTree->Branch("eff_pu",&roc_eff_pu,"eff_pu/F");
  ROCTree->Branch("eff_pu_err",&roc_eff_pu_err,"eff_pu_err/F");

  ROCTree->Branch("eff_qcd",&roc_eff_qcd,"eff_qcd/F");
  ROCTree->Branch("eff_qcd_err",&roc_eff_qcd_err,"eff_qcd_err/F");

  ROCTree->Branch("eff_stoc",&roc_eff_stoc,"eff_stoc/F");
  ROCTree->Branch("eff_stoc_err",&roc_eff_stoc_err,"eff_stoc_err/F");
}


TGraph * getROCGraph(TTree* tree, TString selection, int type=0){
  if(!tree)return 0;

  TTree* copy=tree->CopyTree(selection.Data());
  if(!copy){
    cout<<"Tree Copy() failed"<<endl;
    return 0;
  }

  if(copy->GetEntries()==0){
    cout<<"Tree Copy() failed, 0 entries"<<endl;
    return 0;
  }

  float eff_sig;
  float eff_pu;
  
  copy->SetBranchAddress("eff_sig",&eff_sig);
  if(type==0)      copy->SetBranchAddress("eff_pu",&eff_pu);
  else if(type==1) copy->SetBranchAddress("eff_qcd",&eff_pu);
  else if(type==2) copy->SetBranchAddress("eff_stoc",&eff_pu);
  else {cout<<"Wrong jet type "<<type<<endl;}

  TGraph * G=new TGraph();
  G->SetLineColor(type+1);
  G->SetMarkerColor(type+1);
  
  for(int i=0;i<=copy->GetEntries();i++){
    copy->GetEntry(i);
    G->SetPoint(i,eff_sig,eff_pu);
    ///cout<<i<<" "<<eff_sig<<" "<<eff_pu<<endl;
  }
  
  delete copy;
  return G;
}

void drawROCBoundary(){
  TGraph * GBoundary=new TGraph();
  GBoundary->SetPoint(0,0,0);
  GBoundary->SetPoint(1,1,1);
  GBoundary->SetLineStyle(2);
  GBoundary->Draw("lsame");
}


/////////////////////////////////////////////////////////////////////
//////////Other tools 
/////////////////////////////////////////////////////////////////////
double deltaR(float eta1, float phi1, float eta2, float phi2){
  double deta= fabs(eta1 - eta2);      
  double dphi= fabs(phi1 - phi2);
  if (dphi > TMath::Pi() ) dphi = 2*(TMath::Pi()) - dphi;
  return sqrt((dphi*dphi)+(deta*deta));
}

/////////////////////////////////////
/////create density histo
///////////////////////////////////
TH1F * getdRDensityHisto(TString name){
  TH1F * h = new TH1F(name,"",8,0,0.4); 
  h->Sumw2();
  h->GetXaxis()->SetTitle("#Delta R");
  h->GetYaxis()->SetTitle("density");
  return h;
}
void dividedRDensityHisto(TH1F* h){
  if(!h) return;
  for(int b=1; b<=h->GetNbinsX(); b++){
    float area = TMath::Pi()*( pow(h->GetBinLowEdge(b)+h->GetBinWidth(b),2) - pow(h->GetBinLowEdge(b),2) );
    h->SetBinError(b,h->GetBinError(b)/area);
    h->SetBinContent(b,h->GetBinContent(b)/area);
  }
}


///////////////////////
/// Event display tools
//////////////////////
void drawJetCircles(bool all=1){

  TEllipse* circle=new TEllipse(0,0,0.4,0.4);
  circle->SetFillColor(0);
  circle->SetFillStyle(0);
  circle->SetLineColor(1);
  circle->Draw();

  if(all){
  TEllipse* circle1=new TEllipse(0,0,0.1,0.1);
  circle1->SetFillColor(0);
  circle1->SetFillStyle(0);
  circle1->SetLineColor(1);
  circle1->Draw();

  TEllipse* circle2=new TEllipse(0,0,0.2,0.2);
  circle2->SetFillColor(0);
  circle2->SetFillStyle(0);
  circle2->SetLineColor(1);
  circle2->Draw();

  TEllipse* circle3=new TEllipse(0,0,0.3,0.3);
  circle3->SetFillColor(0);
  circle3->SetFillStyle(0);
  circle3->SetLineColor(1);
  circle3->Draw();
  }
  
}

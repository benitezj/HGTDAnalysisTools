
void collectDTTemplates(){

  TString inpath="/afs/cern.ch/user/b/benitezj/www/HGTDStudies/HitPlots/May12";

  TFile FSig10(inpath+"/SiHit_April12_mu0_3CELLSIZE_60CELLENERGY_10TIMESMEAR_mc15_13TeV.341080.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_WWlvlv_EF_15_5.evgen.EVNT.e4397/DTTemplates_ClusterCell.root","read");
  TH1F* HSig10=(TH1F*)FSig10.Get("HCELLDT_Clus_Signal");
  TFile FSig20(inpath+"/SiHit_April12_mu0_3CELLSIZE_60CELLENERGY_20TIMESMEAR_mc15_13TeV.341080.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_WWlvlv_EF_15_5.evgen.EVNT.e4397/DTTemplates_ClusterCell.root","read");
  TH1F* HSig20=(TH1F*)FSig20.Get("HCELLDT_Clus_Signal");
  TFile FSig30(inpath+"/SiHit_April12_mu0_3CELLSIZE_60CELLENERGY_30TIMESMEAR_mc15_13TeV.341080.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_WWlvlv_EF_15_5.evgen.EVNT.e4397/DTTemplates_ClusterCell.root","read");
  TH1F* HSig30=(TH1F*)FSig30.Get("HCELLDT_Clus_Signal");
  TFile FSig50(inpath+"/SiHit_April12_mu0_3CELLSIZE_60CELLENERGY_50TIMESMEAR_mc15_13TeV.341080.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_WWlvlv_EF_15_5.evgen.EVNT.e4397/DTTemplates_ClusterCell.root","read");
  TH1F* HSig50=(TH1F*)FSig50.Get("HCELLDT_Clus_Signal");
  TFile FSig100(inpath+"/SiHit_April12_mu0_3CELLSIZE_60CELLENERGY_100TIMESMEAR_mc15_13TeV.341080.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_WWlvlv_EF_15_5.evgen.EVNT.e4397/DTTemplates_ClusterCell.root","read");
  TH1F* HSig100=(TH1F*)FSig100.Get("HCELLDT_Clus_Signal");


  TFile FPU10(inpath+"/SiHit_April12_mu200_3CELLSIZE_60CELLENERGY_10TIMESMEAR_mc15_13TeV.341080.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_WWlvlv_EF_15_5.evgen.EVNT.e4397/DTTemplates_ClusterCell.root","read");
  TH1F* HPU10=(TH1F*)FPU10.Get("HCELLDT_Clus_PU");
  TFile FPU20(inpath+"/SiHit_April12_mu200_3CELLSIZE_60CELLENERGY_20TIMESMEAR_mc15_13TeV.341080.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_WWlvlv_EF_15_5.evgen.EVNT.e4397/DTTemplates_ClusterCell.root","read");
  TH1F* HPU20=(TH1F*)FPU20.Get("HCELLDT_Clus_PU");
  TFile FPU30(inpath+"/SiHit_April12_mu200_3CELLSIZE_60CELLENERGY_30TIMESMEAR_mc15_13TeV.341080.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_WWlvlv_EF_15_5.evgen.EVNT.e4397/DTTemplates_ClusterCell.root","read");
  TH1F* HPU30=(TH1F*)FPU30.Get("HCELLDT_Clus_PU");
  TFile FPU50(inpath+"/SiHit_April12_mu200_3CELLSIZE_60CELLENERGY_50TIMESMEAR_mc15_13TeV.341080.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_WWlvlv_EF_15_5.evgen.EVNT.e4397/DTTemplates_ClusterCell.root","read");
  TH1F* HPU50=(TH1F*)FPU50.Get("HCELLDT_Clus_PU");
  TFile FPU100(inpath+"/SiHit_April12_mu200_3CELLSIZE_60CELLENERGY_100TIMESMEAR_mc15_13TeV.341080.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_WWlvlv_EF_15_5.evgen.EVNT.e4397/DTTemplates_ClusterCell.root","read");
  TH1F* HPU100=(TH1F*)FPU100.Get("HCELLDT_Clus_PU");


  
  TFile DTTemplates("DTTemplates.root","recreate");
  HSig10->SetName("Signal_10"); HSig10->Write();  HPU10->SetName("PU_10"); HPU10->Write();
  HSig20->SetName("Signal_20"); HSig20->Write();  HPU20->SetName("PU_20"); HPU20->Write();
  HSig30->SetName("Signal_30"); HSig30->Write();  HPU30->SetName("PU_30"); HPU30->Write();
  HSig50->SetName("Signal_50"); HSig50->Write();  HPU50->SetName("PU_50"); HPU50->Write();
  HSig100->SetName("Signal_100"); HSig100->Write();  HPU100->SetName("PU_100"); HPU100->Write();
  DTTemplates.ls();
 
  gROOT->ProcessLine(".q");
}

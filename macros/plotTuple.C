#include "HGTDAnalysisTools/macros/globals.h"
#include "HGTDAnalysisTools/macros/plottingTools.C"

#include "HGTDAnalysisTools/macros/drawCellGrid.C"
#include "HGTDAnalysisTools/macros/drawCellGridDetGeo.C"
#include "HGTDAnalysisTools/macros/plotEventDisplayDrawHits.C"
#include "HGTDAnalysisTools/macros/plotEventDisplayHitsXY.C"
#include "HGTDAnalysisTools/macros/plotEventDisplayHitsEtaPhi.C"
#include "HGTDAnalysisTools/macros/plotEventDisplayHitsDT.C"
#include "HGTDAnalysisTools/macros/plotEventDisplayHitsDTAllJet.C"

#include "HGTDAnalysisTools/macros/plotEventDisplayHitsDT_dRScan.C"
#include "HGTDAnalysisTools/macros/plotEventDisplayHitsDR_dRScan.C"
#include "HGTDAnalysisTools/macros/plotEventDisplayHitsdEtadPhi_dRScan.C"
#include "HGTDAnalysisTools/macros/plotEventDisplayHitsdEtadPhi_HGTracks.C"
#include "HGTDAnalysisTools/macros/plotEventDisplayHitsdXdY_Trks.C"
#include "HGTDAnalysisTools/macros/plotEventDisplayTrksXY.C"
#include "HGTDAnalysisTools/macros/plotEventDisplayClusdEtadPhi.C"
#include "HGTDAnalysisTools/macros/plotEventDisplayHGTracksdEtadPhi.C"
#include "HGTDAnalysisTools/macros/plotEventDisplayHGTrksXY.C"
#include "HGTDAnalysisTools/macros/plotEventDisplayJetShape.C"

#include "HGTDAnalysisTools/macros/plotEventDisplay.C"
#include "HGTDAnalysisTools/macros/plotEventDisplayDecIDR.C"
#include "HGTDAnalysisTools/macros/plotEventDisplayTest.C"
#include "HGTDAnalysisTools/macros/plotEventDisplayTrks.C"

#include "HGTDAnalysisTools/macros/plotEvent.C"
#include "HGTDAnalysisTools/macros/plotHits.C"
#include "HGTDAnalysisTools/macros/plotCells.C"
#include "HGTDAnalysisTools/macros/plotJets.C"
#include "HGTDAnalysisTools/macros/plotTruthJets.C"
#include "HGTDAnalysisTools/macros/plotJetNCellsIDR.C"
#include "HGTDAnalysisTools/macros/plotClusters.C"
#include "HGTDAnalysisTools/macros/plotElectrons.C"
#include "HGTDAnalysisTools/macros/plotHGTracks.C"
#include "HGTDAnalysisTools/macros/plotJetHGTrksIDR.C"
#include "HGTDAnalysisTools/macros/plotVBF.C"
#include "HGTDAnalysisTools/macros/plotTracks.C"
//#include "HGTDAnalysisTools/macros/plotTrigger.C"


/////////////////////////////////////////////////////////////////////////
void plotTuple(TString SAMPLE="LArHitDec13IDR/LArHit_July26_mu0",TString TAG="",long EVT=0){

  SetAtlasStyle();
  gStyle->SetPalette(1,0);

  setCuts(SAMPLE);

  std::vector<std::string> samples;

  //TString sample = "mc15_valid.361034.Pythia8EvtGen_A2MSTW2008LO_minbias_inelastic_low.evgen.EVNT.e4800_Last20Files";
  //TString samplealias = "Minbias";

  // TString sample = "mc15_13TeV.341080.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_WWlvlv_EF_15_5.evgen.EVNT.e3871";
  // TString samplealias = "VBFH125_WWlvlv";

  //TString sample = "mc15_13TeV.305174.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH500_ZZ4nu.evgen.EVNT.e5019";
  //TString samplealias = "VBFHinv";
  

  TString samplealias = "VBFH125inv";
  samples.push_back("mc15_13TeV.301399.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_ZZ4nu.evgen.EVNT.e4397");

  //TString sample = "mc15_13TeV.361106.PowhegPythia8EvtGen_AZNLOCTEQ6L1_Zee.evgen.EVNT.e3601";
  //TString samplealias = "Zee";

  //////14 TeV samples
  //TString samplealias = "VBFH125inv";
  //samples.push_back("mc15_14TeV.301399.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_ZZ4nu.recon.AOD.e4956_s3072_s3059_r8956");//Nominal,InclBrl,Timing,mu0
  //samples.push_back("mc15_14TeV.301399.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_ZZ4nu.recon.AOD.e4956_s3072_s3059_r9073");  //Nominal,InclBrl,Timing,mu200
  //samples.push_back("mc15_14TeV.301399.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_ZZ4nu.recon.AOD.e4956_s3069_s3058_r8955");//Nominal,InclBrl,PS,mu0
  //samples.push_back("mc15_14TeV.301399.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_ZZ4nu.recon.AOD.e4956_s3069_s3058_r9072");//Nominal,InclBrl,PS,mu200
  

  // ////single pions
  // TString samplealias = "SinglePiPlus";

  // samples.push_back("mc15_14TeV.415016.ParticleGun_single_piplus_Pt1_etaFlatp23_43.recon.AOD.e5580_s3072_s3059_r8956");// AOD's need use xAOD::TrackParticle
  // samples.push_back("mc15_14TeV.415017.ParticleGun_single_piplus_Pt2_etaFlatp23_43.recon.AOD.e5580_s3072_s3059_r8956");
  // samples.push_back("mc15_14TeV.415018.ParticleGun_single_piplus_Pt5_etaFlatp23_43.recon.AOD.e5580_s3072_s3059_r8956");
  // samples.push_back("mc15_14TeV.415019.ParticleGun_single_piplus_Pt20_etaFlatp23_43.recon.AOD.e5580_s3072_s3059_r8956");

  // samples.push_back("mc15_14TeV.415016.ParticleGun_single_piplus_Pt1_etaFlatp23_43.recon.ESD.e5580_s3072_s3059_r8956");//ESD contain Trk::Trk
  // samples.push_back("mc15_14TeV.415017.ParticleGun_single_piplus_Pt2_etaFlatp23_43.recon.ESD.e5580_s3072_s3059_r8956");
  // samples.push_back("mc15_14TeV.415018.ParticleGun_single_piplus_Pt5_etaFlatp23_43.recon.ESD.e5580_s3072_s3059_r8956");
  // samples.push_back("mc15_14TeV.415019.ParticleGun_single_piplus_Pt20_etaFlatp23_43.recon.ESD.e5580_s3072_s3059_r8956");
  
  // samples.push_back("mc15_14TeV.415016.ParticleGun_single_piplus_Pt1_etaFlatp23_43.evgen.EVNT.e5580");// samples with ITK material removed (PP and Moderator)
  // samples.push_back("mc15_14TeV.415017.ParticleGun_single_piplus_Pt2_etaFlatp23_43.evgen.EVNT.e5580");
  // samples.push_back("mc15_14TeV.415018.ParticleGun_single_piplus_Pt5_etaFlatp23_43.evgen.EVNT.e5580");
  // samples.push_back("mc15_14TeV.415019.ParticleGun_single_piplus_Pt20_etaFlatp23_43.evgen.EVNT.e5580");
  

  ////
  for(std::vector<std::string>::const_iterator s=samples.begin();s!=samples.end();s++){
    cout<<"Loading :"<<(*s).c_str()<<endl;
    loadChain(INDIR+"/"+SAMPLE+"/"+(*s).c_str());
  }
  if(!TREE) return;
  if(TREE->GetEntries()<=0) return;


  //clean the output path
  TString outpath=OUTDIR+"/"+samplealias+"_"+SAMPLE;
  if(TAG.CompareTo("")!=0) outpath=outpath+"_"+TAG;
  //if(EVT!=0) outpath=outpath+"_"+EVT;
  system((TString("rm -rf ")+outpath).Data());
  system((TString("mkdir -p ")+outpath).Data());

  ////////// Event display
  //plotEventDisplay(TREE,outpath,8521);  /// XY display 260, cells dT 4503  (used for ECFA)
  plotEventDisplayDecIDR(TREE,outpath,EVT);  /// for Dec. IDR XY  260, dT 4503;   for Summer2017 IDR  dT  6295 (/data/det-hgtd/tuple/LArHitDec13IDR/)
  //plotEventDisplayTest(TREE,outpath,EVT); /// global event displays
  //plotEventDisplayTrks(TREE,outpath,EVT); 


  ///////////Event
  //plotTruth(TREE,outpath);
  //plotPrimVertex(TREE,outpath);

  /////////////inclusive Cells
  //plotCellEnergy(TREE,outpath);
  //plotCellSpatial(TREE,outpath);
  //plotCellTime(TREE,outpath);

  //////////// truth jets
  //plotTruthJets(TREE,outpath);

  //////////// Jets 
  //plotJets(TREE,outpath);
  //plotJetCells(TREE,outpath);
  //plotJetClusters(TREE,outpath);
  //plotJetClusdR(TREE,outpath);
  //plotJetCelldR(TREE,outpath);
  //plotCellMoments(TREE,outpath);
  //plotJetCelldPhidEta(TREE,outpath);
  //plotJetNearJet(TREE,outpath);
  //plotJetTrks(TREE,outpath);
  //plotJetComposition(TREE,outpath);
  //plotJetTimingProbScan(TREE,outpath);  //optimize the time binning
  
  

  ////Plots from December 2016
  //plotJetNCellsIDR(TREE,outpath);       // IDR plots (mu=0)
  //plotJetCelldEtadPhiIDR(TREE,outpath); // IDR plots
  //fillJetCelldR(TREE,outpath);          // IDR plots , actual plots are done with separate macro
 
  ///////plots for June 2017
  //plotJetHitDensity(TREE,outpath); 
  //plotJetTimingProb(TREE,outpath);
  //plotJetPtEtaIDRSummer2017(TREE,outpath);
  //plotJetHitDensitySummer2017(TREE,outpath);// two pT bins
  //plotJetHitDensityIDRSummer2017(TREE,outpath);//single pT bin
  

  ///// HGTD tracks
  //plotHGTracks(TREE,outpath);
  //plotHGTrackCells(TREE,outpath);
  //plotJetNHGTrksIDR(TREE,outpath);
  //plotJetHGTrkdEtadPhiIDR(TREE,outpath);
  //plotJetHGTrkdR(TREE,outpath);


  /////ITK tracks
  //plotTracks(TREE,outpath);
  //plotTrackCells(TREE,outpath);
  //plotTrackCellResolutionVsPt(TREE,outpath,1);
  //plotBadResTracks(TREE,outpath);
  //plotTrackHGTrks(TREE,outpath); 
  
  ////// Public Track timing plots
  //plotTrackMatchingEff(TREE,outpath);       // public mu=0 matching
  //plotTrackdXPublicPlot(TREE,outpath);    // public mu=0 dX resolution (special plot not removing matching selection)
  //plotTrackdTPublicPlot(TREE,outpath);    // public mu=200 dT resolution
  //plotTrackTimeMatchingEff(TREE,outpath);   // public mu=200 dT matching 



  /////////////Electrons
  //plotElectrons(TREE,outpath);
  //plotEleCells(TREE,outpath);
  //plotEleClusters(TREE,outpath);

  //////////// Clusters
  //plotClusters(TREE,outpath);
  //plotClusterCells(TREE,outpath);
  //plotClusterCellsDensity(TREE,outpath);


  // //// Trigger
  // plotTrigger(TREE,outpath);
  // plotTriggerDiagnostics(TREE,outpath);
  // //////first must determine the cut for some signal efficiency
  // plotTriggerCut(TREE,outpath,"(trig_size<40)",0.5,"tile1");
  // plotTriggerCut(TREE,outpath,"(40<trig_size&&trig_size<60)",0.5,"tile2");
  // plotTriggerCut(TREE,outpath,"(60<trig_size&&trig_size<120)",0.5,"tile3");
  // plotTriggerCut(TREE,outpath,"(trig_size>120)",0.5,"tile4");
  // plotTriggerProbabilities2D(TREE,outpath);
  // plotTriggerJets(TREE,outpath);


  //// VBF analysis
  // plotVBF(TREE,outpath);

  
  gROOT->ProcessLine(".q");

}


#ifndef HGTDANALYSISTOOLS_LArHitTupleDetGeo_H
#define HGTDANALYSISTOOLS_LArHitTupleDetGeo_H 1

#include "AthAnalysisBaseComps/AthAnalysisAlgorithm.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "JetCalibTools/IJetCalibrationTool.h"
#include "LArSimEvent/LArHitContainer.h"
#include "CaloIdentifier/HGTD_ID.h"

#include "TrkExInterfaces/IExtrapolator.h"
#include "CaloTrackingGeometry/ICaloSurfaceBuilder.h"
#include "TrkSurfaces/DiscSurface.h"

#define ZPOSL0_Preshower 3506.3 
#define ZPOSL1_Preshower 3518.0 
#define ZPOSL2_Preshower 3529.7 
#define ZPOSL3_Preshower 3541.4 

#define ZPOSL0_Timing 3516.9 
#define ZPOSL1_Timing 3525.1 
#define ZPOSL2_Timing 3533.3 
#define ZPOSL3_Timing 3541.5 

#define MAXCELL1 12000000 //needed for 1x1mm 
#define MAXCELL2 2000000 //needed for 3x3mm 
#define MAXCELL  500000 //final cell list (cells with hits per event) on average <20%

#define MAXTRK1 3000000  
#define MAXTRK2 350000 
#define MAXTRK  50000   

#define MAXTRIG1 288
#define MAXTRIG2 288
#define MAXTRIG3 128
#define MAXTRIG4 72 
#define MAXTRIG 776

#define MAXTRUTH 100
#define MAXTRUTHJETS 150
#define MAXJETS 100
#define MAXCLUS 10000
#define MAXELES 300
#define MAXTRKS 1000
#define JETMATCHDR 0.3
#define ELEMATCHDR 0.3

//#define ASU 198

#define speed_light 2.99792e2 //in mm/ns

#define NTWINDOW 8
#define NMAXTBINS 200

class TTree;
class TFile;
class TH1F;
class TF1;
class TF2;


class LArHitTupleDetGeo: public ::AthAnalysisAlgorithm { 
 public: 
  LArHitTupleDetGeo( const std::string& name, ISvcLocator* pSvcLocator );
  virtual ~LArHitTupleDetGeo(); 

  virtual StatusCode  initialize();
  virtual StatusCode  execute();
  virtual StatusCode  finalize();
  
  virtual StatusCode beginInputFile();

 private: 

  ///  
  int eventCount=0; 


  float ZPOSL[4];
  
  //
  float ASU;//198
  float LRDETSeparation=1.5;//separation between L and R halfs
  float ASU_boundary=2.25;
  float SENSOR_boundary=0.75;//Between two sensors: GR=0.5 + Spacer=0.5 + GR=0.5
  int HGTDHEIGHTIN;//=int(ASU/2+ASU); // inner radius + 1 ASU, assume inner radius is half ASU for now
  int HGTDHEIGHTOUT;//=600;//mm outer radius
  bool passDeadZone(float x,float y);//for done for G4 hits
  bool passDeadZoneStripStandard(float x,float y);//for done for G4 hits

  int m_deadZoneType=1;//

  TF1 * FEtaVsR=NULL;

  //Input
  std::string m_geometry;
  std::string m_VertexCollection;
  std::string m_HGTDHitCollection;
  std::string m_JetCollection;
  std::string m_METCollection;
  std::string m_METSoftTerm;
  std::string m_ElectronCollection;


  float m_HitTimeSmear1=0.01;
  float m_HitTimeSmear2=0.02;
  long TSTEP=15;
  float scanwindow=1.000;//ns
  float m_dTSigma=0.06;//ns bin size
  float bincount[NMAXTBINS];
  float binmean[NMAXTBINS];

  int m_dTVertex=0;//choose true, (0,0,0,), or reco. 

  int m_CellSize1=1;
  int m_CellSize2=3;
  float m_CellMinEnergy=0;
  int m_WriteAllCells=0;

  float m_ClusterMinEta=0;
  float m_ClusterMinHitDR=0;

  float m_JetPtCut=0.;
  float m_JetPtCutMax=0.;
  float m_JetMinEta=0;
  float m_JetMinHitDR=0;

  float m_EleMinHitDR=0;
  float m_EleMinEta=0; 


  float m_TrackMatchRadius=5;

  ////OUTPUT
  TTree * m_tree=0; 
  TFile * m_outfile=0;

  int m_selectJetEvents=0;
  int m_selectElectronEvents=0;

  int cutflow[100];


  int run;
  long long event;
  int mu;
  int bcid;


  int ntruth=0;
  float truth_m[MAXTRUTH];
  float truth_p[MAXTRUTH];
  float truth_pt[MAXTRUTH];
  float truth_eta[MAXTRUTH];
  float truth_phi[MAXTRUTH];
  int truth_status[MAXTRUTH];
  int truth_pdg[MAXTRUTH];

  int truth_Higgs;
  int truth_HiggsB;
  int truth_HiggsBbar;
  int truth_VBFJ1;
  int truth_VBFJ2;
  int truth_Gluon;
  int truth_Wp ;
  int truth_Wm ;
  int truth_Lp ;
  int truth_Lm ;
  int truth_Z; 
  int truth_Z_Lp; 
  int truth_Z_Lm; 
  int truth_Z_e1; 
  int truth_Z_e2; 

  float truevtx_t;
  float truevtx_x;
  float truevtx_y;
  float truevtx_z;

  int nvtx;
  float vtx_x;
  float vtx_y;
  float vtx_z;

  float met;
  float sumet;
  float met_phi;


  //////////Truth Jet information
  int   ntruthjet;
  int   truthjet_vtx[MAXTRUTHJETS];
  float truthjet_pt[MAXTRUTHJETS];
  float truthjet_eta[MAXTRUTHJETS];
  float truthjet_phi[MAXTRUTHJETS];
  float truthjet_E[MAXTRUTHJETS];
  int   truthjet_truth[MAXTRUTHJETS];

  ////Inner Cell array
  int NARRAYCELL1;
  int NCELLXY1;
  float cell1_x[MAXCELL1];
  float cell1_y[MAXCELL1];
  float cell1_z[MAXCELL1];
  int cell1_S[MAXCELL1];
  float cell1_r[MAXCELL1];
  float cell1_eta[MAXCELL1];
  float cell1_phi[MAXCELL1];
  float cell1_E[MAXCELL1];
  float cell1_t[MAXCELL1];
  int cell1_nhit[MAXCELL1];
  int cell1_jet[MAXCELL1];
  float cell1_jetdR[MAXCELL1];
  int cell1_clus[MAXCELL1];
  float cell1_clusdR[MAXCELL1];
  int cell1_ele[MAXCELL1];
  float cell1_eledR[MAXCELL1];
  int cell1_elePU[MAXCELL1];

  int cell1_hx[MAXCELL1];
  int cell1_hy[MAXCELL1];

  int cell1_trkidx[MAXCELL1];

  //Outer cell array
  int NARRAYCELL2;
  int NCELLXY2;
  float cell2_x[MAXCELL2];
  float cell2_y[MAXCELL2];
  float cell2_z[MAXCELL2];
  int cell2_S[MAXCELL2];
  float cell2_r[MAXCELL2];
  float cell2_eta[MAXCELL2];
  float cell2_phi[MAXCELL2];
  float cell2_E[MAXCELL2];
  float cell2_t[MAXCELL2];
  int cell2_nhit[MAXCELL2];
  int cell2_jet[MAXCELL2];
  float cell2_jetdR[MAXCELL2];
  int cell2_clus[MAXCELL2];
  float cell2_clusdR[MAXCELL2];
  int cell2_ele[MAXCELL2];
  float cell2_eledR[MAXCELL2];
  int cell2_elePU[MAXCELL2];

  int cell2_hx[MAXCELL2];
  int cell2_hy[MAXCELL2];

  int cell2_trkidx[MAXCELL2];

  ///finall cell collection
  int ncell;
  float cell_x[MAXCELL];
  float cell_y[MAXCELL];
  float cell_z[MAXCELL];
  int cell_S[MAXCELL];
  float cell_r[MAXCELL];
  float cell_D[MAXCELL];
  float cell_DZ0[MAXCELL];
  float cell_trueD[MAXCELL];
  float cell_Dreco[MAXCELL];
  float cell_eta[MAXCELL];
  float cell_phi[MAXCELL];
  float cell_E[MAXCELL];
  float cell_t[MAXCELL];
  float cell_tr[MAXCELL];
  float cell_dt[MAXCELL];
  float cell_dtZ0[MAXCELL];
  float cell_dttrue[MAXCELL];
  float cell_dtreco[MAXCELL];
  float cell_truedt[MAXCELL];
  int cell_nhit[MAXCELL];
  int cell_jet[MAXCELL];
  float cell_jetdR[MAXCELL];
  int cell_clus[MAXCELL];
  float cell_clusdR[MAXCELL];
  int cell_ele[MAXCELL];
  float cell_eledR[MAXCELL];
  int cell_elePU[MAXCELL];
  int cell_trig[MAXCELL];

  int cell_hx[MAXCELL];
  int cell_hy[MAXCELL];

  int cell_type[MAXCELL];
  int cell_trkidx[MAXCELL1];

  int cell_trk[MAXCELL1];
  float cell_trkdR[MAXCELL1];

  int ncellL0;
  int ncellL1;
  int ncellL2;
  int ncellL3;


  /////////////////////////////////////
  /// HGTD Track arrays
  /////////////////////////////////////
  // void fillHGTracks();

  ////Inner Track array
  int NARRAYTRK1;
  // float trk1_x[MAXTRK1];
  // float trk1_y[MAXTRK1];
  // float trk1_z[MAXTRK1];
  // float trk1_r[MAXTRK1];
  // float trk1_eta[MAXTRK1];
  // float trk1_phi[MAXTRK1];
  // float trk1_E[MAXTRK1];
  // float trk1_t[MAXTRK1];
  // float trk1_tr[MAXTRK1];
  // float trk1_dt[MAXTRK1];
  // float trk1_truedt[MAXTRK1];
  // int trk1_nhit[MAXTRK1];

  //Outer array
  int NARRAYTRK2;
  // float trk2_x[MAXTRK2];
  // float trk2_y[MAXTRK2];
  // float trk2_z[MAXTRK2];
  // float trk2_r[MAXTRK2];
  // float trk2_eta[MAXTRK2];
  // float trk2_phi[MAXTRK2];
  // float trk2_E[MAXTRK2];
  // float trk2_t[MAXTRK2];
  // float trk2_tr[MAXTRK2];
  // float trk2_dt[MAXTRK2];
  // float trk2_truedt[MAXTRK2];
  // int trk2_nhit[MAXTRK2];


  /////////////////////////////////////
  /// better HGTD Track arrays
  /////////////////////////////////////
  void fillHGTDTracks();

  ////Inner Track array
  float hgtrk1_x[MAXTRK1][9];
  float hgtrk1_y[MAXTRK1][9];
  float hgtrk1_z[MAXTRK1][9];
  float hgtrk1_r[MAXTRK1][9];
  float hgtrk1_eta[MAXTRK1][9];
  float hgtrk1_phi[MAXTRK1][9];
  float hgtrk1_E[MAXTRK1][9];
  float hgtrk1_t[MAXTRK1][9];
  float hgtrk1_tr[MAXTRK1][9];
  float hgtrk1_dt[MAXTRK1][9];
  float hgtrk1_truedt[MAXTRK1][9];
  int hgtrk1_nhit[MAXTRK1][9];
  int hgtrk1_c1[MAXTRK1][9];
  int hgtrk1_c2[MAXTRK1][9];


  ////Inner Track array
  float hgtrk2_x[MAXTRK2][9];
  float hgtrk2_y[MAXTRK2][9];
  float hgtrk2_z[MAXTRK2][9];
  float hgtrk2_r[MAXTRK2][9];
  float hgtrk2_eta[MAXTRK2][9];
  float hgtrk2_phi[MAXTRK2][9];
  float hgtrk2_E[MAXTRK2][9];
  float hgtrk2_t[MAXTRK2][9];
  float hgtrk2_tr[MAXTRK2][9];
  float hgtrk2_dt[MAXTRK2][9];
  float hgtrk2_truedt[MAXTRK2][9];
  int hgtrk2_nhit[MAXTRK2][9];
  int hgtrk2_c1[MAXTRK2][9];
  int hgtrk2_c2[MAXTRK2][9];


  ///final track collection
  int nhgtrk;
  float hgtrk_x[MAXTRK];
  float hgtrk_y[MAXTRK];
  float hgtrk_z[MAXTRK];
  float hgtrk_r[MAXTRK];
  float hgtrk_eta[MAXTRK];
  float hgtrk_phi[MAXTRK];
  float hgtrk_E[MAXTRK];
  float hgtrk_t[MAXTRK];
  float hgtrk_tr[MAXTRK];
  float hgtrk_dt[MAXTRK];
  float hgtrk_truedt[MAXTRK];
  int hgtrk_nhit[MAXTRK];
  int hgtrk_jet[MAXTRK];
  float hgtrk_jetdR[MAXTRK];

  float hgtrk_deta[MAXTRK];
  float hgtrk_dphi[MAXTRK];

  int hgtrk_type[MAXTRK];
  int hgtrk_trkidx[MAXTRK];
  int hgtrk_idx2[MAXTRK];
  int hgtrk_c1[MAXTRK];
  int hgtrk_c2[MAXTRK];


  /////////////////////////////////////
  ////Trigger tiles
  /////////////////////////////////////
  float TRIGBOUNDARY1=1*ASU;
  float TRIGBOUNDARY2=1.5*ASU;
  float TRIGBOUNDARY3=2*ASU;
  float TRIGBOUNDARY4=3*ASU;

  float TRIGSIZE1=ASU/6.;
  float TRIGSIZE2=ASU/4.;
  float TRIGSIZE3=ASU/2.;//= 1 sensor
  float TRIGSIZE4=ASU;

  int NARRAYTRIGGER1;
  int NTRIGGERXY1;
  float trig1_x[MAXTRIG1];
  float trig1_y[MAXTRIG1];
  float trig1_z[MAXTRIG1];
  //int trig1_S[MAXTRIG1];

  int NARRAYTRIGGER2;
  int NTRIGGERXY2;
  float trig2_x[MAXTRIG2];
  float trig2_y[MAXTRIG2];
  float trig2_z[MAXTRIG2];
  //int trig2_S[MAXTRIG2];

  int NARRAYTRIGGER3;
  int NTRIGGERXY3;
  float trig3_x[MAXTRIG3];
  float trig3_y[MAXTRIG3];
  float trig3_z[MAXTRIG3];
  //int trig3_S[MAXTRIG3];

  int NARRAYTRIGGER4;
  int NTRIGGERXY4;
  float trig4_x[MAXTRIG4];
  float trig4_y[MAXTRIG4];
  float trig4_z[MAXTRIG4];
  //int trig4_S[MAXTRIG4];


  int ntrig;
  float trig_x[MAXTRIG];
  float trig_y[MAXTRIG];
  float trig_z[MAXTRIG];
  //int trig_S[MAXTRIG];
  float trig_size[MAXTRIG];
  float trig_sizeeta[MAXTRIG];
  float trig_r[MAXTRIG];
  float trig_D[MAXTRIG];
  float trig_eta[MAXTRIG];
  float trig_phi[MAXTRIG];
  float trig_dt[MAXTRIG];
  float trig_truedt[MAXTRIG];
  float trig_E[MAXTRIG];
  float trig_E_peak[MAXTRIG];
  int trig_nhit[MAXTRIG];
  int trig_nhit_peak[MAXTRIG];
  int trig_jet[MAXTRIG];

  int ntrig_cut;
  int ntrig_5;
  int ntrig_10;
  int ntrig_15;
  int ntrig_20;
  int ntrig_25;
  int ntrig_30;
  int ntrig_35;
  int ntrig_40;
  int ntrig_45;
  int ntrig_50;

  StatusCode initTrigger();//define tiles
  void processTrigger();
  void fitTriggerTileDT(int idx);//basic peak finding

  std::string m_TrigCut;
  TF2 * TrigCut=NULL;

  /////////////////////////////////////
  ///Jet clusters
  /////////////////////////////////////
  int nclus;
  float clus_pt[MAXCLUS];
  float clus_eta[MAXCLUS];
  float clus_phi[MAXCLUS];
  float clus_E[MAXCLUS];
  float clus_w[MAXCLUS];
  float clus_lambda[MAXCLUS];
  float clus_emfrac[MAXCLUS];
  float clus_emprob[MAXCLUS];
  float clus_sig[MAXCLUS];
  int clus_ncell[MAXCLUS];
  int clus_jet[MAXCLUS];
  float clus_jetdR[MAXCLUS];
  int clus_ncell_window[NTWINDOW][MAXCLUS];
  int clus_ncell_clean[MAXCLUS];//# of cells in time, fixed window required

  //////////Jet information
  int njet;
  int jet_truthJ[MAXJETS];
  float jet_pt[MAXJETS];
  float jet_eta[MAXJETS];
  float jet_phi[MAXJETS];
  float jet_E[MAXJETS];
  float jet_m[MAXJETS];
  int jet_nclus[MAXJETS];
  int jet_ntrk[MAXJETS];
  int jet_nhgtrk[MAXJETS];
  int jet_trig[MAXJETS];
  int jet_ltrk[MAXJETS];
  int jet_dRtrk[MAXJETS];

  //jet classification
  int jet_truthjhs_dr3pt10[MAXJETS];//-> h.s.
  int jet_truthjhs_dr3pt4[MAXJETS]; //-> p.u. inclusive
  int jet_truthjpu_dr3pt10[MAXJETS];//-> p.u. QCD
  int jet_truthjpu_dr6pt10[MAXJETS];//-> p.u. Stochastic


  //jet cone cells
  int jet_ncell[MAXJETS]; //withing dR<0.4 , no time cut
  int jet_ncell_window[NTWINDOW][MAXJETS];  //number of cells within dR<0.4 and some time cut 
  float jet_cellR1_window[NTWINDOW][MAXJETS];//1nd moment of cell distribution inside jet
  float jet_cellR2_window[NTWINDOW][MAXJETS];//2nd moment of cell distribution inside jet
  float jet_cellE_window[NTWINDOW][MAXJETS];//total cell energy
  float jet_cellR1Ew_window[NTWINDOW][MAXJETS];//1nd moment of cell distribution inside jet, energy weighted
  float jet_cellR2Ew_window[NTWINDOW][MAXJETS];//2nd moment of cell distribution inside jet, energy weighted

  ///for different dR rings
  int jet_ncell_dr00[MAXJETS];
  int jet_ncell_dr05[MAXJETS];
  int jet_ncell_dr10[MAXJETS];
  int jet_ncell_dr15[MAXJETS];
  int jet_ncell_dr20[MAXJETS];
  int jet_ncell_dr25[MAXJETS];
  int jet_ncell_dr30[MAXJETS];
  int jet_ncell_dr35[MAXJETS];

  ///clus energy for different dR rings
  float jet_clusE_dr00[MAXJETS];
  float jet_clusE_dr05[MAXJETS];
  float jet_clusE_dr10[MAXJETS];
  float jet_clusE_dr15[MAXJETS];
  float jet_clusE_dr20[MAXJETS];
  float jet_clusE_dr25[MAXJETS];
  float jet_clusE_dr30[MAXJETS];
  float jet_clusE_dr35[MAXJETS];

  ///clus energy for different dR rings
  float jet_trk_dr00[MAXJETS];
  float jet_trk_dr05[MAXJETS];
  float jet_trk_dr10[MAXJETS];
  float jet_trk_dr15[MAXJETS];
  float jet_trk_dr20[MAXJETS];
  float jet_trk_dr25[MAXJETS];
  float jet_trk_dr30[MAXJETS];
  float jet_trk_dr35[MAXJETS];


  //
  float jet_shape_p0[MAXJETS];
  float jet_shape_p1[MAXJETS];
  float jet_shape_p0er[MAXJETS];
  float jet_shape_p1er[MAXJETS];
  float jet_shape_p2[MAXJETS];
  float jet_shape_p3[MAXJETS];
  float jet_shape_p2er[MAXJETS];
  float jet_shape_p3er[MAXJETS];
  TF1 *FJetShape=NULL;
  TH1F * HJetShape=NULL;
  void fitJetShape(int idx);  
  void dividedRDensityHisto(TH1F* h);


  //Fit with knee function
  float jet_shapeknee_chi2pndf[MAXJETS];
  float jet_shapeknee_p0[MAXJETS];
  float jet_shapeknee_p0er[MAXJETS];
  float jet_shapeknee_p1[MAXJETS];
  float jet_shapeknee_p1er[MAXJETS];
  float jet_shapeknee_p2[MAXJETS];
  float jet_shapeknee_p2er[MAXJETS];
  TF1 *FJetShapeKnee=NULL;
  

  //jet cluster cells
  int jet_ncell_clus[MAXJETS];  //number of cells associate to clusters within dR<0.05 
  int jet_ncell_clus_window[NTWINDOW][MAXJETS];  //number of cells associate to clusters within dR<0.05 and some time cut 
  
  //good clusters
  int jet_nclus_window[NTWINDOW][MAXJETS];   

  //uncalibrated jet 
  float jet_uncalib_pt[MAXJETS];
  float jet_uncalib_eta[MAXJETS];
  float jet_uncalib_phi[MAXJETS];
  float jet_uncalib_m[MAXJETS];
  float jet_uncalib_E[MAXJETS];

  ///nearby jets
  int jet_njetnear[MAXJETS];
  int jet_nearjet[MAXJETS];
  float jet_nearjetdR[MAXJETS];


  float jet_truedt[MAXJETS];

  //results of the deltaT fit with flexible mean using all cells
  float jet_dTFitJetCell_signalmean[MAXJETS];
  int jet_dTFitJetCell_ncell_window[NTWINDOW][MAXJETS]; 

  //results of the deltaT fit with fixed mean
  float jet_dTFitFix_sfrac[MAXJETS];
  float jet_dTFitFix_sbratio[MAXJETS];
  float jet_dTFitFix_minnll[MAXJETS];

  //results of the deltaT fit with flexible mean
  float jet_dTFit_signalmean[MAXJETS];
  int jet_dTFit_ncell_clus[MAXJETS]; 
  int jet_dTFit_ncell_clus_window[NTWINDOW][MAXJETS]; 

  int jet_dTFit_wp70[MAXJETS];

  //results of peak finding with background subtraction
  float m_dTFitCoreBkgSubRadius=0.2;
  float m_dTFitCoreBkgSubRadiusSideband=0.2;
  float jet_dTFitCoreBkgSub_signalmean[MAXJETS];
  float jet_dTFitCoreBkgSub_ncellbkg[MAXJETS];
  float jet_dTFitCoreBkgSub_ncellsig[MAXJETS];
  float jet_dTFitCoreBkgSub_avgbkg[MAXJETS]; 
  float jet_dTFitCoreBkgSub_maxbkg[MAXJETS]; 
  float jet_dTFitCoreBkgSub_sF[MAXJETS];
  int jet_dTFitCoreBkgSub_ncell[MAXJETS]; 
  int jet_dTFitCoreBkgSub_ncell_window[NTWINDOW][MAXJETS]; 

 
  //results of dR fit with sliding time window, uses dR templates below
  float jet_dRdTFit_signalmean[MAXJETS];
  float jet_dRdTFit_sig[MAXJETS];
  float jet_dRdTFit_siger[MAXJETS];
  float jet_dRdTFit_chi2[MAXJETS];
  float jet_dRdTFit_bkg[MAXJETS];
  float jet_dRdTFit_bkger[MAXJETS];
  float jet_dRdTFit_slp[MAXJETS];
  float jet_dRdTFit_slper[MAXJETS];

  //results of the deltaT fit with flexible mean
  int jet_dTFitCore_ncell[MAXJETS];
  int jet_dTFitCore_truencell[MAXJETS];
  float jet_dTFitCore_signalmean[MAXJETS];
  int jet_dTFitCore_ncell_window[NTWINDOW][MAXJETS]; 
  float jet_dTFitCore_meandt_window[NTWINDOW][MAXJETS]; 

  //peak finding on jet core with tracklets
  int jet_dTFitCoreHGTrk_ntrktot[MAXJETS];
  int jet_dTFitCoreHGTrk_ntrk[MAXJETS]; 
  float jet_dTFitCoreHGTrk_dt[MAXJETS];

  //results of the deltaT fit with flexible mean and E weighted
  float jet_dTFitEw_signalmean[MAXJETS];
  float jet_dTFitEw_Ecell_clus_window[NTWINDOW][MAXJETS]; 
  float jet_dTFitEw_Etot_clus[MAXJETS]; 

  //results of the deltaT fit with flexible mean and E weighted
  float jet_dTFitEcw_signalmean[MAXJETS];
  float jet_dTFitEcw_Ecell_clus_window[NTWINDOW][MAXJETS]; 
  float jet_dTFitEcw_Etot_clus[MAXJETS]; 
  int jet_dTFitEcw_wp70[MAXJETS];

  //results deltaT fit with multiple signals
  float jet_dTFitMultiSignal_sfrac[MAXJETS];
  float jet_dTFitMultiSignal_sbratio[MAXJETS];
  float jet_dTFitMultiSignal_minnll[MAXJETS];


  ///additional jet and cluster counters
  int njet_hgtd;
  int njet_pt20_hgtd;

  int nclus_hgtd;
  int nclus_hgtd_clean;


  ///////////////////////////////////////
  ////// Electron information
  ///////////////////////////////////////
  int nele;
  int ele_truth[MAXELES];
  float ele_pt[MAXELES];
  float ele_eta[MAXELES];
  float ele_phi[MAXELES];
  float ele_m[MAXELES];
  float ele_E[MAXELES];
  int ele_ncell[MAXELES];
  int ele_ncell_clean[MAXELES];
  int ele_ncell_window[NTWINDOW][MAXELES];
  float ele_cellE_window[NTWINDOW][MAXELES];//total cell energy
  int ele_clus[MAXELES];
  float ele_clus_pt[MAXELES];
  float ele_clus_phi[MAXELES];
  float ele_clus_eta[MAXELES];
  float ele_clus_E[MAXELES];
  float ele_clus_lambda[MAXELES];
  float ele_clus_emfrac[MAXELES];
  float ele_clus_emprob[MAXELES];
  float ele_clus_sig[MAXELES];
  int ele_ncellPU[MAXELES];
  float ele_truedt[MAXELES];
  float ele_dTFit_signalmean[MAXELES];
  float ele_cellEPU[MAXELES];
  int ele_ntrk[MAXELES];
  int ele_ntrkpv[MAXELES];
  int ele_trk[MAXELES];

  ///////////////////////////////////////
  ///  ID tracks
  ///////////////////////////////////////
  int ntrk;
  float trk_e[MAXTRKS];
  float trk_pt[MAXTRKS];
  float trk_eta[MAXTRKS];
  float trk_phi[MAXTRKS];
  float trk_d0[MAXTRKS];
  float trk_z0[MAXTRKS];
  int trk_charge[MAXTRKS];
  int trk_ele[MAXTRKS];
  int trk_jet[MAXTRKS];
  float trk_jetdR[MAXTRKS];

  int trk_ncell[MAXTRKS];
  int trk_ncellL0[MAXTRKS];
  int trk_cell0[MAXTRKS];
  int trk_cell1[MAXTRKS];
  int trk_cell2[MAXTRKS];
  int trk_cell3[MAXTRKS];

  float trk_t[MAXTRKS];
  float trk_dt[MAXTRKS];
  float trk_truedt[MAXTRKS];

  int trk_outlier[MAXTRKS];

  float trk_tr[MAXTRKS];
  float trk_truetr[MAXTRKS];
  float trk_trtrue[MAXTRKS];

  float trk_ext_eta[MAXTRKS];
  float trk_ext_phi[MAXTRKS];
  float trk_ext_x[MAXTRKS];
  float trk_ext_y[MAXTRKS];
  float trk_ext_z[MAXTRKS];
  float trk_ext_peta[MAXTRKS];
  float trk_ext_pphi[MAXTRKS];

  float trk_ext_dx[MAXTRKS];
  float trk_ext_dy[MAXTRKS];
  float trk_ext_dtheta[MAXTRKS];
  float trk_ext_dphi[MAXTRKS];

  float trk_hgtd_x[4][MAXTRKS];//corrected positions for different layers
  float trk_hgtd_y[4][MAXTRKS];

  int   trk_truth_pdg[MAXTRKS];
  float trk_truth_pt[MAXTRKS];
  float trk_truth_eta[MAXTRKS];
  float trk_truth_phi[MAXTRKS];
  float trk_truth_vtxt[MAXTRKS];
  float trk_truth_vtxz[MAXTRKS];

  int trk_hgtrk[MAXTRKS];
  float trk_hgtrkdR[MAXTRKS];

  ////////////
  //VBF selection
  ////////////
  float m_VBFMinPt=30000;
  float m_VBFMinMjj=500000;
  std::string m_VBFJetWPAlgo;
  std::string m_VBFJetWPInputFile;
  float VBFJetWP[6][3]; // 6 values vs eta (2.4->4.8/0.4), 3 values in pt (30-40,40-50,50-70)
  int WPWindowIdx=0;
  int getJetWPEtaBin(float eta){
    if(fabs(eta)<2.4||fabs(eta)>4.8) return -1; 
    return int((fabs(eta)-2.4)/0.4) ; 
  }
  int getJetWPPtBin(float pt){
    if(pt<=30000) return -1; 
    else if(30000<pt&&pt<=40000) return 0; 
    else if(40000<pt&&pt<=50000) return 1; 
    else if(50000<pt&&pt<=70000) return 2; 
    else return -1;
  }
  bool passJetHGTDCut(int idx);


  int vbf_j1;
  int vbf_j2;
  float vbf_mjj=0.;//will be the mjj of the highest pair

  int vbfdt1_j1;//require wp only on subleading jet
  int vbfdt1_j2;
  float vbfdt1_mjj=0.;

  int vbfdt2_j1;//require wp on both jets
  int vbfdt2_j2;
  float vbfdt2_mjj=0.;

  int vbfcoin_j1;//require wp on both jets and coincidence
  int vbfcoin_j2;
  float vbfcoin_mjj=0.;

  int vbfcoinnowp_j1;//require only coincidence 
  int vbfcoinnowp_j2;
  float vbfcoinnowp_mjj=0.;

  int vbfcoinnowpj1_j1;//require only coincidence and wp on subleading jet
  int vbfcoinnowpj1_j2;
  float vbfcoinnowpj1_mjj=0.;

  int vbfcjv_j1;
  int vbfcjv_j2;
  float vbfcjv_mjj=0.;

  int vbfdt2cjv_j1;
  int vbfdt2cjv_j2;
  float vbfdt2cjv_mjj=0.;


  void findVBFJets();


  /////
  double deltaR(float eta1, float phi1, float eta2, float phi2){
    double deta= fabs(eta1 - eta2);      double dphi= fabs(phi1 - phi2);
    if (dphi > TMath::Pi() ) dphi = 2*(TMath::Pi()) - dphi;
    return sqrt((dphi*dphi)+(deta*deta));
  }
  double evalGaus(float mean, float sigma, float x){
    return (1./(sigma*2.5066))*exp(-0.5*(x-mean)*(x-mean)/(sigma*sigma));
  }

  ////
  const xAOD::TruthParticleContainer* Truth = 0;
  void printTruthTree(unsigned int idx);
  StatusCode processTruth();
  StatusCode processEventInfo();
  StatusCode processTruthJets();
  StatusCode processJets();
  StatusCode processMET();
  StatusCode processElectrons();

  StatusCode processTracks();
  void setObjectTracks();
  void associateTrackCells();
  void setTrackCells();
  void setTrackHGTDTrks();
  TF1* FTrkMatch1mm=0;
  TF1* FTrkMatch3mm=0;

  float getZPosition(int S);
  void resetCells();
  StatusCode processCells();  

  void associateClusterCells();
  void setClusterCells();

  void associateElectronCells();
  void setElectronCells();

  void associateJetCells();
  void setJetCells();

  void associateTriggerCells();
  void setTriggerCells();



  ///
  ToolHandle<IJetCalibrationTool> m_jetCalibration;


  ///track selection
  TF1 *FTrackZ0Cut=0;

  //track extrapolation
  int m_RunTrkExtraPol=0;
  double m_CALO_INNER_Z=0;//3641.;
  Trk::Surface* m_discSurface_atCaloEntrance_positiveZ=0;
  Trk::Surface* m_discSurface_atCaloEntrance_negativeZ=0;
  ToolHandle< Trk::IExtrapolator > m_extrapolator;


  ///for the deltaT fitting
  std::string m_DTTemplatesFilePath;
  std::string m_DTTemplateHistoSig;
  std::string m_DTTemplateHistoPU;
  TFile* FDTTemplates=0;
  TH1F* HDTTemplate_Sig=0;
  TH1F* HDTTemplate_PU=0;
  void fitJetDTJetCell(int idx);  
  void fitJetDTJetCore(int idx);  
  void fitJetDTJetCoreHGTrk(int idx);  
  void fitJetDT(int idx);  
  void fitJetDTFix(int idx); 
  void fitJetDTEw(int idx); 
  void fitJetDTEcw(int idx);

  //for the dR fitting
  int etastep=4;
  int minetaidx=24;
  int maxetaidx=40;
  int ptstep=20;
  int minptidx=30;
  int maxptidx=70;
  std::string m_dRTemplatesFilePath_Sig;
  std::string m_dRTemplatesFilePath_Bkg;
  TFile* FdRTemplates_Sig=0;
  TFile* FdRTemplates_Bkg=0;
  TH1F* HdRTemplate_Sig[50][100];
  TH1F* HdRTemplate_Bkg[50][100];
  TF1* FuncdR_Bkg[50][100];//for jet core bkg estimation
  TF1* FuncdR_BkgSig[50][100];//for dR fit

  TF1 * dRFitL;
  TF1 * dRFitM;
  TF1 * dRFitH;
  TH1F * dRFitArea;


  void fitJetDTBkgSub(int idx); 
  void fitJetDRDT(int idx); 

  void fitEleDT(int idx); 


  // Truth histograms
  TH1F* HVBFH_pT=0;
  TH1F* HVBFH_eta=0;
  TH1F* HVBFJ1_pT=0;
  TH1F* HVBFJ1_eta=0;
  TH1F* HVBFJ2_pT=0;
  TH1F* HVBFJ2_eta=0;
  TH1F* HVBFG_pT=0;
  TH1F* HVBFG_eta=0;

  TH1F* HZeeE1_pT=0;
  TH1F* HZeeE1_eta=0;
  TH1F* HZeeE2_pT=0;
  TH1F* HZeeE2_eta=0;
  


  ///cell hit distributions as a function of radius and layer
  TH1F* HCellHitVsT1[4][280];//inner cells : 0 - 280 / 1 
  TH1F* HCellHitVsT3[4][200];//outer cells : 0 - 600 / 3
  void fillCellHitDistributions(const DataHandle<LArHitContainer> p_collection, const HGTD_ID * ID);


  //// build the truth pu jet's
  //std::vector < std::vector < TLorentzVector > >  newjets;
  bool includeParticle(const xAOD::TruthParticle* part);
  StatusCode buildPUTruthJets();


  ///
  std::pair < Amg::Vector3D, Amg::Vector3D >  best_line_from_points(const std::vector<Amg::Vector3D> & c);

}; 

#endif //> !HGTDANALYSISTOOLS_HGTDANALYSISTOOLSALG_H

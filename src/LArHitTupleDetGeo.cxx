/// HGTDAnalysisTools includes
#include "HGTDAnalysisTools/LArHitTupleDetGeo.h"

#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>
#include <TH1F.h>
#include <TF1.h>
#include <TF2.h>
#include "TRandom.h"
#include <fstream>
#include <sstream>
#include <TCanvas.h>

#include "StoreGate/StoreGate.h"

//#include "xAODRootAccess/TEvent.h"
#include "xAODEventInfo/EventInfo.h"
#include "xAODJet/JetContainer.h"
#include "xAODJet/Jet.h"
#include "xAODJet/JetConstituentVector.h"
#include "xAODMissingET/MissingETComponentMap.h"
#include "xAODMissingET/MissingETContainer.h"
#include "xAODMissingET/MissingET.h"
#include "xAODTracking/Vertex.h"

#include "xAODTracking/TrackParticle.h"
#include "xAODTracking/TrackParticleContainer.h"

#include "xAODTruth/TruthVertexContainer.h"
#include "xAODEgamma/ElectronContainer.h"

#include "CaloDetDescr/CaloDetDescrElement.h"
#include "CaloDetDescr/CaloDetDescrManager.h"
#include "CaloIdentifier/CaloIdManager.h"

#include "GeoPrimitives/GeoPrimitives.h"
//#include "TrkTrackLink/ITrackLink.h"
//#include "TrkParticleBase/LinkToTrackParticleBase.h"
#include "TrkParameters/TrackParameters.h"
//#include "Particle/TrackParticle.h"
//#include "TrkTrack/TrackCollection.h"

////this is for the pu truth jets building
//#include "xAODJet/FastJetLinkBase.h"
//#include "xAODTruth/TruthEventContainer.h"
#include "xAODTruth/TruthPileupEventContainer.h"
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"



LArHitTupleDetGeo::LArHitTupleDetGeo( const std::string& name, ISvcLocator* pSvcLocator ) : 
  AthAnalysisAlgorithm( name, pSvcLocator ),
  m_extrapolator("Trk::Extrapolator/AtlasExtrapolator")
{

  declareProperty( "ASU",ASU);
  declareProperty( "HGTDHEIGHTIN",HGTDHEIGHTIN);
  declareProperty( "HGTDHEIGHTOUT",HGTDHEIGHTOUT);
  declareProperty( "deadZoneType",m_deadZoneType);

  declareProperty( "geometry",m_geometry);
  declareProperty( "VertexCollection",m_VertexCollection);
  declareProperty( "HGTDHitCollection", m_HGTDHitCollection ); 
  declareProperty( "JetCollection", m_JetCollection ); 
  declareProperty( "METCollection", m_METCollection ); 
  declareProperty( "METSoftTerm", m_METSoftTerm ); 
  declareProperty( "ElectronCollection", m_ElectronCollection ); 

  declareProperty( "CellSize1",m_CellSize1);
  declareProperty( "CellSize2",m_CellSize2);
  declareProperty( "CellMinEnergy",m_CellMinEnergy);
  declareProperty( "WriteAllCells",m_WriteAllCells);

  declareProperty( "selectJetEvents",m_selectJetEvents);
  declareProperty( "selectElectronEvents",m_selectElectronEvents);

  declareProperty( "JetPtCut", m_JetPtCut);
  declareProperty( "JetPtCutMax", m_JetPtCutMax);
  declareProperty( "JetMinEta", m_JetMinEta);
  declareProperty( "JetMinHitDR",m_JetMinHitDR);

  declareProperty( "EleMinHitDR", m_EleMinHitDR);
  declareProperty( "EleMinEta", m_EleMinEta); 

  declareProperty( "ClusterMinEta",m_ClusterMinEta);
  declareProperty( "ClusterMinHitDR",m_ClusterMinHitDR);

  declareProperty( "TrackMatchRadius",m_TrackMatchRadius);

  declareProperty( "HitTimeSmear",m_HitTimeSmear1);
  declareProperty( "HitTimeSmear2",m_HitTimeSmear2);
  declareProperty( "dTSigma",m_dTSigma);

  declareProperty( "dTVertex",m_dTVertex);

  declareProperty( "DTTemplatesFilePath", m_DTTemplatesFilePath);
  declareProperty( "DTTemplateHistoSig", m_DTTemplateHistoSig);
  declareProperty( "DTTemplateHistoPU", m_DTTemplateHistoPU);
  

  declareProperty( "dRTemplatesFilePath_Sig", m_dRTemplatesFilePath_Sig);
  declareProperty( "dRTemplatesFilePath_Bkg", m_dRTemplatesFilePath_Bkg);


  declareProperty( "VBFJetWPAlgo",m_VBFJetWPAlgo);
  declareProperty( "VBFJetWPInputFile",m_VBFJetWPInputFile);
  declareProperty( "VBFMinPt",m_VBFMinPt);
  declareProperty( "VBFMinMjj",m_VBFMinMjj);

  declareProperty( "dTFitCoreBkgSubRadius",m_dTFitCoreBkgSubRadius);
  declareProperty( "dTFitCoreBkgSubRadiusSideband",m_dTFitCoreBkgSubRadiusSideband);


  declareProperty( "TrigCut", m_TrigCut);


  declareProperty( "RunTrkExtraPol",m_RunTrkExtraPol);

}


LArHitTupleDetGeo::~LArHitTupleDetGeo() {}


StatusCode LArHitTupleDetGeo::initialize() {
  ATH_MSG_INFO ("Initializing " << name() << "...");

  for(int c=0;c<100;c++)
    cutflow[c]=0;


  FEtaVsR=new TF1("FEtaVsR","5.59809-0.0188504*x+6.66887e-05*x*x-1.48097e-07*x*x*x+1.75012e-10*x*x*x*x-8.38774e-14*x*x*x*x*x",100,650);


  //create the ouput file
  m_outfile = new TFile("tuple.root","RECREATE");
  if(!m_outfile || m_outfile->IsZombie()){
    ATH_MSG_ERROR ("Failed to create the output file" );
    return StatusCode::FAILURE;
  }

  ATH_MSG_DEBUG ("Output File has been created");

  //create the ntuple
  m_tree = new TTree("tuple","tuple");
  m_tree->SetDirectory(m_outfile);

  //Event info
  m_tree->Branch("event",&event,"event/l");
  m_tree->Branch("run",&run,"run/I");
  m_tree->Branch("mu",&mu,"mu/I");
  m_tree->Branch("bcid",&bcid,"bcid/I");


  if(m_WriteAllCells){
    //Cells
    m_tree->Branch("ncell",&ncell,"ncell/I");
    m_tree->Branch("cell_t",cell_t,"cell_t[ncell]/F");
    m_tree->Branch("cell_tr",cell_tr,"cell_tr[ncell]/F");
    m_tree->Branch("cell_dt",cell_dt,"cell_dt[ncell]/F");
    m_tree->Branch("cell_dtreco",cell_dtreco,"cell_dtreco[ncell]/F");
    m_tree->Branch("cell_dttrue",cell_dttrue,"cell_dttrue[ncell]/F");
    m_tree->Branch("cell_dtZ0",cell_dtZ0,"cell_dtZ0[ncell]/F");
    m_tree->Branch("cell_truedt",cell_truedt,"cell_truedt[ncell]/F");
    m_tree->Branch("cell_E",cell_E,"cell_E[ncell]/F");
    m_tree->Branch("cell_eta",cell_eta,"cell_eta[ncell]/F");
    m_tree->Branch("cell_phi",cell_phi,"cell_phi[ncell]/F");
    m_tree->Branch("cell_x",cell_x,"cell_x[ncell]/F");
    m_tree->Branch("cell_y",cell_y,"cell_y[ncell]/F");
    m_tree->Branch("cell_z",cell_z,"cell_z[ncell]/F");
    m_tree->Branch("cell_S",cell_S,"cell_S[ncell]/I");
    m_tree->Branch("cell_r",cell_r,"cell_r[ncell]/F");
    m_tree->Branch("cell_D",cell_D,"cell_D[ncell]/F");
    m_tree->Branch("cell_Dreco",cell_Dreco,"cell_Dreco[ncell]/F");
    m_tree->Branch("cell_trueD",cell_trueD,"cell_trueD[ncell]/F");
    m_tree->Branch("cell_DZ0",cell_DZ0,"cell_DZ0[ncell]/F");
    m_tree->Branch("cell_nhit",cell_nhit,"cell_nhit[ncell]/I");
    m_tree->Branch("cell_type",cell_type,"cell_type[ncell]/I");
    m_tree->Branch("cell_hx",cell_hx,"cell_hx[ncell]/I");
    m_tree->Branch("cell_hy",cell_hy,"cell_hy[ncell]/I");
    m_tree->Branch("cell_jet",cell_jet,"cell_jet[ncell]/I");
    m_tree->Branch("cell_jetdR",cell_jetdR,"cell_jetdR[ncell]/F");
    m_tree->Branch("cell_clus",cell_clus,"cell_clus[ncell]/I");
    m_tree->Branch("cell_clusdR",cell_clusdR,"cell_clusdR[ncell]/F");
    m_tree->Branch("cell_ele", cell_ele, "cell_ele[ncell]/I");
    m_tree->Branch("cell_eledR", cell_eledR, "cell_eledR[ncell]/F");
    m_tree->Branch("cell_elePU", cell_elePU, "cell_elePU[ncell]/I"); 
    m_tree->Branch("cell_trig",cell_trig,"cell_trig[ncell]/I");

    m_tree->Branch("cell_trk",cell_trk,"cell_trk[ncell]/I");
    m_tree->Branch("cell_trkdR",cell_trkdR,"cell_trkdR[ncell]/F");

    m_tree->Branch("ncellL0",&ncellL0,"ncellL0/I");
    m_tree->Branch("ncellL1",&ncellL1,"ncellL1/I");
    m_tree->Branch("ncellL2",&ncellL2,"ncellL2/I");
    m_tree->Branch("ncellL3",&ncellL3,"ncellL3/I");

  }

 
  m_tree->Branch("nhgtrk",&nhgtrk,"nhgtrk/I");
  m_tree->Branch("hgtrk_t",hgtrk_t,"hgtrk_t[nhgtrk]/F");
  m_tree->Branch("hgtrk_tr",hgtrk_tr,"hgtrk_tr[nhgtrk]/F");
  m_tree->Branch("hgtrk_dt",hgtrk_dt,"hgtrk_dt[nhgtrk]/F");
  m_tree->Branch("hgtrk_truedt",hgtrk_truedt,"hgtrk_truedt[nhgtrk]/F");
  m_tree->Branch("hgtrk_E",hgtrk_E,"hgtrk_E[nhgtrk]/F");
  m_tree->Branch("hgtrk_eta",hgtrk_eta,"hgtrk_eta[nhgtrk]/F");
  m_tree->Branch("hgtrk_phi",hgtrk_phi,"hgtrk_phi[nhgtrk]/F");
  m_tree->Branch("hgtrk_x",hgtrk_x,"hgtrk_x[nhgtrk]/F");
  m_tree->Branch("hgtrk_y",hgtrk_y,"hgtrk_y[nhgtrk]/F");
  m_tree->Branch("hgtrk_z",hgtrk_z,"hgtrk_z[nhgtrk]/F");
  m_tree->Branch("hgtrk_r",hgtrk_r,"hgtrk_r[nhgtrk]/F");
  m_tree->Branch("hgtrk_nhit",hgtrk_nhit,"hgtrk_nhit[nhgtrk]/I");
  m_tree->Branch("hgtrk_jet",hgtrk_jet,"hgtrk_jet[nhgtrk]/I");
  m_tree->Branch("hgtrk_jetdR",hgtrk_jetdR,"hgtrk_jetdR[nhgtrk]/F");

  m_tree->Branch("hgtrk_deta",hgtrk_deta,"hgtrk_deta[nhgtrk]/F");
  m_tree->Branch("hgtrk_dphi",hgtrk_dphi,"hgtrk_dphi[nhgtrk]/F");

  m_tree->Branch("hgtrk_type",hgtrk_type,"hgtrk_type[nhgtrk]/I");
  m_tree->Branch("hgtrk_trkidx",hgtrk_trkidx,"hgtrk_trkidx[nhgtrk]/I");
  m_tree->Branch("hgtrk_idx2",hgtrk_idx2,"hgtrk_idx2[nhgtrk]/I");
  m_tree->Branch("hgtrk_c1",hgtrk_c1,"hgtrk_c1[nhgtrk]/I");
  m_tree->Branch("hgtrk_c2",hgtrk_c2,"hgtrk_c2[nhgtrk]/I");

  // ///trigger
  // m_tree->Branch("ntrig",&ntrig,"ntrig/I");
  // m_tree->Branch("trig_x",trig_x,"trig_x[ntrig]/F");
  // m_tree->Branch("trig_y",trig_y,"trig_y[ntrig]/F");
  // m_tree->Branch("trig_z",trig_z,"trig_z[ntrig]/F");
  // //m_tree->Branch("trig_S",trig_S,"trig_S[ntrig]/I");
  // m_tree->Branch("trig_r",trig_r,"trig_r[ntrig]/F");
  // m_tree->Branch("trig_D",trig_D,"trig_D[ntrig]/F");
  // m_tree->Branch("trig_eta",trig_eta,"trig_eta[ntrig]/F");
  // m_tree->Branch("trig_phi",trig_phi,"trig_phi[ntrig]/F");
  // m_tree->Branch("trig_size",trig_size,"trig_size[ntrig]/F");
  // m_tree->Branch("trig_sizeeta",trig_sizeeta,"trig_sizeeta[ntrig]/F");
  // m_tree->Branch("trig_dt",trig_dt,"trig_dt[ntrig]/F");
  // m_tree->Branch("trig_truedt",trig_truedt,"trig_truedt[ntrig]/F");
  // m_tree->Branch("trig_nhit",trig_nhit,"trig_nhit[ntrig]/I");
  // m_tree->Branch("trig_nhit_peak",trig_nhit_peak,"trig_nhit_peak[ntrig]/I");
  // m_tree->Branch("trig_jet",trig_jet,"trig_jet[ntrig]/I");

  // m_tree->Branch("trig_E",trig_E,"trig_E[ntrig]/F");
  // m_tree->Branch("trig_E_peak",trig_E_peak,"trig_E_peak[ntrig]/F");

  // m_tree->Branch("ntrig_cut",&ntrig_cut,"ntrig_cut/I");

  // m_tree->Branch("ntrig_5",&ntrig_5,"ntrig_5/I");
  // m_tree->Branch("ntrig_10",&ntrig_10,"ntrig_10/I");
  // m_tree->Branch("ntrig_15",&ntrig_15,"ntrig_15/I");
  // m_tree->Branch("ntrig_20",&ntrig_20,"ntrig_20/I");
  // m_tree->Branch("ntrig_25",&ntrig_25,"ntrig_25/I");
  // m_tree->Branch("ntrig_30",&ntrig_30,"ntrig_30/I");
  // m_tree->Branch("ntrig_35",&ntrig_35,"ntrig_35/I");
  // m_tree->Branch("ntrig_40",&ntrig_40,"ntrig_40/I");
  // m_tree->Branch("ntrig_45",&ntrig_45,"ntrig_45/I");
  // m_tree->Branch("ntrig_50",&ntrig_50,"ntrig_50/I");


  // MC truth 
  m_tree->Branch("ntruth",&ntruth,"ntruth/I");
  m_tree->Branch("truth_m",truth_m,"truth_m[ntruth]/F");
  m_tree->Branch("truth_p",truth_p,"truth_p[ntruth]/F");
  m_tree->Branch("truth_pt",truth_pt,"truth_pt[ntruth]/F");
  m_tree->Branch("truth_eta",truth_eta,"truth_eta[ntruth]/F");
  m_tree->Branch("truth_phi",truth_phi,"truth_phi[ntruth]/F");
  m_tree->Branch("truth_status",truth_status,"truth_status[ntruth]/I");
  m_tree->Branch("truth_pdg",truth_pdg,"truth_pdg[ntruth]/I");

  m_tree->Branch("truth_VBFJ1",&truth_VBFJ1,"truth_VBFJ1/I");
  m_tree->Branch("truth_VBFJ2",&truth_VBFJ2,"truth_VBFJ2/I");
  m_tree->Branch("truth_Higgs",&truth_Higgs,"truth_Higgs/I");
  m_tree->Branch("truth_Gluon",&truth_Gluon,"truth_Gluon/I");
  m_tree->Branch("truth_Wp",&truth_Wp,"truth_Wp/I");
  m_tree->Branch("truth_Wm",&truth_Wm,"truth_Wm/I");
  m_tree->Branch("truth_Lp",&truth_Lp,"truth_Lp/I");
  m_tree->Branch("truth_Lm",&truth_Lm,"truth_Lm/I");

  m_tree->Branch("truth_HiggsB",&truth_HiggsB,"truth_HiggsB/I");
  m_tree->Branch("truth_HiggsBbar",&truth_HiggsBbar,"truth_HiggsBbar/I");

  m_tree->Branch("truth_Z", &truth_Z, "truth_Z/I"); 
  m_tree->Branch("truth_Z_Lp",&truth_Z_Lp, "truth_Z_Lp/I"); 
  m_tree->Branch("truth_Z_Lm",&truth_Z_Lm, "truth_Z_Lm/I");
  m_tree->Branch("truth_Z_e1",&truth_Z_e1, "truth_Z_e1/I");
  m_tree->Branch("truth_Z_e2",&truth_Z_e2, "truth_Z_e2/I");

  //Vertices
  m_tree->Branch("nvtx",&nvtx,"nvtx/I");
  m_tree->Branch("vtx_x",&vtx_x,"vtx_x/F");
  m_tree->Branch("vtx_y",&vtx_y,"vtx_y/F");
  m_tree->Branch("vtx_z",&vtx_z,"vtx_z/F");

  m_tree->Branch("truevtx_t",&truevtx_t,"truevtx_t/F");
  m_tree->Branch("truevtx_x",&truevtx_x,"truevtx_x/F");
  m_tree->Branch("truevtx_y",&truevtx_y,"truevtx_y/F");
  m_tree->Branch("truevtx_z",&truevtx_z,"truevtx_z/F");

  //clusters
  m_tree->Branch("nclus",&nclus,"nclus/I");
  m_tree->Branch("clus_pt", clus_pt,"clus_pt[nclus]/F");
  m_tree->Branch("clus_phi",clus_phi,"clus_phi[nclus]/F");
  m_tree->Branch("clus_eta",clus_eta,"clus_eta[nclus]/F");
  m_tree->Branch("clus_E",clus_E,"clus_E[nclus]/F");
  m_tree->Branch("clus_w",clus_w,"clus_w[nclus]/F");
  m_tree->Branch("clus_ncell",clus_ncell,"clus_ncell[nclus]/I");
  m_tree->Branch("clus_ncell_clean",clus_ncell_clean,"clus_ncell_clean[nclus]/I");
  m_tree->Branch("clus_jet",clus_jet,"clus_jet[nclus]/I");
  m_tree->Branch("clus_jetdR",clus_jetdR,"clus_jetdR[nclus]/F");
  m_tree->Branch("clus_lambda",clus_lambda,"clus_lambda[nclus]/F");
  m_tree->Branch("clus_emfrac",clus_emfrac,"clus_emfrac[nclus]/F");
  m_tree->Branch("clus_emprob",clus_emprob,"clus_emprob[nclus]/F");
  m_tree->Branch("clus_sig",clus_sig,"clus_sig[nclus]/F");

  for(long t=0;t<NTWINDOW;t++)
    m_tree->Branch(TString("clus_ncell_window")+(t+1)*TSTEP,clus_ncell_window[t],TString("clus_ncell_window")+(t+1)*TSTEP+"[nclus]/I");


  //truth jets
  m_tree->Branch("ntruthjet",&ntruthjet,"ntruthjet/I");
  m_tree->Branch("truthjet_vtx", truthjet_vtx,"truthjet_vtx[ntruthjet]/I");
  m_tree->Branch("truthjet_truth", truthjet_truth,"truthjet_truth[ntruthjet]/I");
  m_tree->Branch("truthjet_pt", truthjet_pt,"truthjet_pt[ntruthjet]/F");
  m_tree->Branch("truthjet_phi",truthjet_phi,"truthjet_phi[ntruthjet]/F");
  m_tree->Branch("truthjet_eta",truthjet_eta,"truthjet_eta[ntruthjet]/F");
  m_tree->Branch("truthjet_E",truthjet_E,"truthjet_E[ntruthjet]/F");


  //Jets
  m_tree->Branch("njet",&njet,"njet/I");
  m_tree->Branch("jet_truthJ", jet_truthJ,"jet_truthJ[njet]/I");
  m_tree->Branch("jet_pt", jet_pt,"jet_pt[njet]/F");
  m_tree->Branch("jet_phi",jet_phi,"jet_phi[njet]/F");
  m_tree->Branch("jet_eta",jet_eta,"jet_eta[njet]/F");
  m_tree->Branch("jet_m",jet_m,"jet_m[njet]/F");
  m_tree->Branch("jet_E",jet_E,"jet_E[njet]/F");
  m_tree->Branch("jet_ntrk",jet_ntrk,"jet_ntrk[njet]/I");
  m_tree->Branch("jet_ltrk",jet_ltrk,"jet_ltrk[njet]/I");
  m_tree->Branch("jet_dRtrk",jet_dRtrk,"jet_dRtrk[njet]/I");
  m_tree->Branch("jet_nhgtrk",jet_nhgtrk,"jet_nhgtrk[njet]/I");
  m_tree->Branch("jet_trig",jet_trig,"jet_trig[njet]/I");
  
  m_tree->Branch("jet_truthjhs_dr3pt10",jet_truthjhs_dr3pt10,"jet_truthjhs_dr3pt10[njet]/I");
  m_tree->Branch("jet_truthjhs_dr3pt4",jet_truthjhs_dr3pt4,"jet_truthjhs_dr3pt4[njet]/I");
  m_tree->Branch("jet_truthjpu_dr3pt10",jet_truthjpu_dr3pt10,"jet_truthjpu_dr3pt10[njet]/I");
  m_tree->Branch("jet_truthjpu_dr6pt10",jet_truthjpu_dr6pt10,"jet_truthjpu_dr6pt10[njet]/I");

  m_tree->Branch("jet_njetnear",jet_njetnear,"jet_njetnear[njet]/I");
  m_tree->Branch("jet_nearjet",jet_nearjet,"jet_nearjet[njet]/I");
  m_tree->Branch("jet_nearjetdR",jet_nearjetdR,"jet_nearjetdR[njet]/F");

  //jet cone cells
  m_tree->Branch("jet_ncell",jet_ncell,"jet_ncell[njet]/I");
  for(long t=0;t<NTWINDOW;t++)
    m_tree->Branch(TString("jet_ncell_window")+(t+1)*TSTEP,jet_ncell_window[t],TString("jet_ncell_window")+(t+1)*TSTEP+"[njet]/I");


  m_tree->Branch("jet_ncell_dr00",jet_ncell_dr00,"jet_ncell_dr00[njet]/I");
  m_tree->Branch("jet_ncell_dr05",jet_ncell_dr05,"jet_ncell_dr05[njet]/I");
  m_tree->Branch("jet_ncell_dr10",jet_ncell_dr10,"jet_ncell_dr10[njet]/I");
  m_tree->Branch("jet_ncell_dr15",jet_ncell_dr15,"jet_ncell_dr15[njet]/I");
  m_tree->Branch("jet_ncell_dr20",jet_ncell_dr20,"jet_ncell_dr20[njet]/I");
  m_tree->Branch("jet_ncell_dr25",jet_ncell_dr25,"jet_ncell_dr25[njet]/I");
  m_tree->Branch("jet_ncell_dr30",jet_ncell_dr30,"jet_ncell_dr30[njet]/I");
  m_tree->Branch("jet_ncell_dr35",jet_ncell_dr35,"jet_ncell_dr35[njet]/I");


  // m_tree->Branch("jet_clusE_dr00",jet_clusE_dr00,"jet_clusE_dr00[njet]/F");
  // m_tree->Branch("jet_clusE_dr05",jet_clusE_dr05,"jet_clusE_dr05[njet]/F");
  // m_tree->Branch("jet_clusE_dr10",jet_clusE_dr10,"jet_clusE_dr10[njet]/F");
  // m_tree->Branch("jet_clusE_dr15",jet_clusE_dr15,"jet_clusE_dr15[njet]/F");
  // m_tree->Branch("jet_clusE_dr20",jet_clusE_dr20,"jet_clusE_dr20[njet]/F");
  // m_tree->Branch("jet_clusE_dr25",jet_clusE_dr25,"jet_clusE_dr25[njet]/F");
  // m_tree->Branch("jet_clusE_dr30",jet_clusE_dr30,"jet_clusE_dr30[njet]/F");
  // m_tree->Branch("jet_clusE_dr35",jet_clusE_dr35,"jet_clusE_dr35[njet]/F");

  m_tree->Branch("jet_trk_dr00",jet_trk_dr00,"jet_trk_dr00[njet]/F");
  m_tree->Branch("jet_trk_dr05",jet_trk_dr05,"jet_trk_dr05[njet]/F");
  m_tree->Branch("jet_trk_dr10",jet_trk_dr10,"jet_trk_dr10[njet]/F");
  m_tree->Branch("jet_trk_dr15",jet_trk_dr15,"jet_trk_dr15[njet]/F");
  m_tree->Branch("jet_trk_dr20",jet_trk_dr20,"jet_trk_dr20[njet]/F");
  m_tree->Branch("jet_trk_dr25",jet_trk_dr25,"jet_trk_dr25[njet]/F");
  m_tree->Branch("jet_trk_dr30",jet_trk_dr30,"jet_trk_dr30[njet]/F");
  m_tree->Branch("jet_trk_dr35",jet_trk_dr35,"jet_trk_dr35[njet]/F");

  FJetShape=new TF1("FJetShape","[0]+[1]*x",0,0.4);
  HJetShape=new TH1F("HJetShape","",8,0.,0.4);
  // m_tree->Branch("jet_shape_p0",jet_shape_p0,"jet_shape_p0[njet]/F");
  // m_tree->Branch("jet_shape_p0er",jet_shape_p0er,"jet_shape_p0er[njet]/F");
  // m_tree->Branch("jet_shape_p1",jet_shape_p1,"jet_shape_p1[njet]/F");
  // m_tree->Branch("jet_shape_p1er",jet_shape_p1er,"jet_shape_p1er[njet]/F");
  // m_tree->Branch("jet_shape_p2",jet_shape_p2,"jet_shape_p2[njet]/F");
  // m_tree->Branch("jet_shape_p2er",jet_shape_p2er,"jet_shape_p2er[njet]/F");
  // m_tree->Branch("jet_shape_p3",jet_shape_p3,"jet_shape_p3[njet]/F");
  // m_tree->Branch("jet_shape_p3er",jet_shape_p3er,"jet_shape_p3er[njet]/F");


  FJetShapeKnee=new TF1("FJetShapeKnee","[0]+(x-0.2)*((x<0.2)*tan(3.14159/2+[1])+(x>0.2)*tan(3.14159/2+[2]))",0,0.4);
  FJetShapeKnee->SetParLimits(1,0,3.14159);
  FJetShapeKnee->SetParLimits(2,3.14159,2*3.14159);
  // m_tree->Branch("jet_shapeknee_chi2pndf",jet_shapeknee_chi2pndf,"jet_shapeknee_chi2pndf[njet]/F");
  // m_tree->Branch("jet_shapeknee_p0",jet_shapeknee_p0,"jet_shapeknee_p0[njet]/F");
  // m_tree->Branch("jet_shapeknee_p0er",jet_shapeknee_p0er,"jet_shapeknee_p0er[njet]/F");
  // m_tree->Branch("jet_shapeknee_p1",jet_shapeknee_p1,"jet_shapeknee_p1[njet]/F");
  // m_tree->Branch("jet_shapeknee_p1er",jet_shapeknee_p1er,"jet_shapeknee_p1er[njet]/F");
  // m_tree->Branch("jet_shapeknee_p2",jet_shapeknee_p2,"jet_shapeknee_p2[njet]/F");
  // m_tree->Branch("jet_shapeknee_p2er",jet_shapeknee_p2er,"jet_shapeknee_p2er[njet]/F");

  // for(long t=0;t<NTWINDOW;t++){
  //   m_tree->Branch(TString("jet_cellR1_window")+(t+1)*TSTEP,jet_cellR1_window[t],TString("jet_cellR1_window")+(t+1)*TSTEP+"[njet]/F");
  //   m_tree->Branch(TString("jet_cellR2_window")+(t+1)*TSTEP,jet_cellR2_window[t],TString("jet_cellR2_window")+(t+1)*TSTEP+"[njet]/F");
  //   m_tree->Branch(TString("jet_cellE_window")+(t+1)*TSTEP,jet_cellE_window[t],TString("jet_cellE_window")+(t+1)*TSTEP+"[njet]/F");
  //   m_tree->Branch(TString("jet_cellR1Ew_window")+(t+1)*TSTEP,jet_cellR1Ew_window[t],TString("jet_cellR1Ew_window")+(t+1)*TSTEP+"[njet]/F");
  //   m_tree->Branch(TString("jet_cellR2Ew_window")+(t+1)*TSTEP,jet_cellR2Ew_window[t],TString("jet_cellR2Ew_window")+(t+1)*TSTEP+"[njet]/F");
  // }

  //jet clusters
  m_tree->Branch("jet_nclus", jet_nclus,"jet_nclus[njet]/I");
  for(long t=0;t<NTWINDOW;t++)
    m_tree->Branch(TString("jet_nclus_window")+(t+1)*TSTEP,jet_nclus_window[t],TString("jet_nclus_window")+(t+1)*TSTEP+"[njet]/I");

  // //jet cluster cells
  // m_tree->Branch("jet_ncell_clus",jet_ncell_clus,"jet_ncell_clus[njet]/I");
  // for(long t=0;t<NTWINDOW;t++)
  //   m_tree->Branch(TString("jet_ncell_clus_window")+(t+1)*TSTEP,jet_ncell_clus_window[t],TString("jet_ncell_clus_window")+(t+1)*TSTEP+"[njet]/I");
    
  // ///results from fit with flexible mean
  // m_tree->Branch("jet_dTFitFix_sfrac",jet_dTFitFix_sfrac,"jet_dTFitFix_sfrac[njet]/F");
  // m_tree->Branch("jet_dTFitFix_sbratio",jet_dTFitFix_sbratio,"jet_dTFitFix_sbratio[njet]/F");
  // m_tree->Branch("jet_dTFitFix_minnll",jet_dTFitFix_minnll,"jet_dTFitFix_minnll[njet]/F");

  // ///results of peak finding using all cells
  // m_tree->Branch("jet_dTFitJetCell_signalmean",jet_dTFitJetCell_signalmean,"jet_dTFitJetCell_signalmean[njet]/F");
  // for(long t=0;t<NTWINDOW;t++)
  //    m_tree->Branch(TString("jet_dTFitJetCell_ncell_window")+(t+1)*TSTEP,jet_dTFitJetCell_ncell_window[t],TString("jet_dTFitJetCell_ncell_window")+(t+1)*TSTEP+"[njet]/I");


  // ///results from fit with flexible mean
  // m_tree->Branch("jet_dTFit_signalmean",jet_dTFit_signalmean,"jet_dTFit_signalmean[njet]/F");
  // m_tree->Branch("jet_dTFit_ncell_clus",jet_dTFit_ncell_clus,"jet_dTFit_ncell_clus[njet]/I");
  // for(long t=0;t<NTWINDOW;t++)
  //    m_tree->Branch(TString("jet_dTFit_ncell_clus_window")+(t+1)*TSTEP,jet_dTFit_ncell_clus_window[t],TString("jet_dTFit_ncell_clus_window")+(t+1)*TSTEP+"[njet]/I");
  // m_tree->Branch("jet_dTFit_wp70",jet_dTFit_wp70,"jet_dTFit_wp70[njet]/I");
  

  ///results from fit with flexible mean
  m_tree->Branch("jet_dTFitCoreBkgSub_signalmean",jet_dTFitCoreBkgSub_signalmean,"jet_dTFitCoreBkgSub_signalmean[njet]/F");
  m_tree->Branch("jet_dTFitCoreBkgSub_ncellsig",jet_dTFitCoreBkgSub_ncellsig,"jet_dTFitCoreBkgSub_ncellsig[njet]/F");
  m_tree->Branch("jet_dTFitCoreBkgSub_ncellbkg",jet_dTFitCoreBkgSub_ncellbkg,"jet_dTFitCoreBkgSub_ncellbkg[njet]/F");
  m_tree->Branch("jet_dTFitCoreBkgSub_sF",jet_dTFitCoreBkgSub_sF,"jet_dTFitCoreBkgSub_sF[njet]/F");
  m_tree->Branch("jet_dTFitCoreBkgSub_avgbkg",jet_dTFitCoreBkgSub_avgbkg,"jet_dTFitCoreBkgSub_avgbkg[njet]/F");
  m_tree->Branch("jet_dTFitCoreBkgSub_maxbkg",jet_dTFitCoreBkgSub_maxbkg,"jet_dTFitCoreBkgSub_maxbkg[njet]/F");

  m_tree->Branch("jet_dTFitCoreBkgSub_ncell",jet_dTFitCoreBkgSub_ncell,"jet_dTFitCoreBkgSub_ncell[njet]/I");
  for(long t=0;t<NTWINDOW;t++)
     m_tree->Branch(TString("jet_dTFitCoreBkgSub_ncell_window")+(t+1)*TSTEP,jet_dTFitCoreBkgSub_ncell_window[t],TString("jet_dTFitCoreBkgSub_ncell_window")+(t+1)*TSTEP+"[njet]/I");

  // //dR fit with sliding window
  // m_tree->Branch("jet_dRdTFit_signalmean",jet_dRdTFit_signalmean,"jet_dRdTFit_signalmean[njet]/F");
  // m_tree->Branch("jet_dRdTFit_sig",jet_dRdTFit_sig,"jet_dRdTFit_sig[njet]/F");
  // m_tree->Branch("jet_dRdTFit_siger",jet_dRdTFit_siger,"jet_dRdTFit_siger[njet]/F");
  // m_tree->Branch("jet_dRdTFit_chi2",jet_dRdTFit_chi2,"jet_dRdTFit_chi2[njet]/F");
  // m_tree->Branch("jet_dRdTFit_bkg",jet_dRdTFit_bkg,"jet_dRdTFit_bkg[njet]/F");
  // m_tree->Branch("jet_dRdTFit_bkger",jet_dRdTFit_bkger,"jet_dRdTFit_bkger[njet]/F");
  // m_tree->Branch("jet_dRdTFit_slp",jet_dRdTFit_slp,"jet_dRdTFit_slp[njet]/F");
  // m_tree->Branch("jet_dRdTFit_slper",jet_dRdTFit_slper,"jet_dRdTFit_slper[njet]/F");

  ///results from fit for Jet Core
  m_tree->Branch("jet_dTFitCore_ncell",jet_dTFitCore_ncell,"jet_dTFitCore_ncell[njet]/I");
  m_tree->Branch("jet_dTFitCore_truencell",jet_dTFitCore_truencell,"jet_dTFitCore_truencell[njet]/I");
  m_tree->Branch("jet_dTFitCore_signalmean",jet_dTFitCore_signalmean,"jet_dTFitCore_signalmean[njet]/F");
  for(long t=0;t<NTWINDOW;t++){
    m_tree->Branch(TString("jet_dTFitCore_meandt_window")+(t+1)*TSTEP,jet_dTFitCore_meandt_window[t],TString("jet_dTFitCore_meandt_window")+(t+1)*TSTEP+"[njet]/F");
    m_tree->Branch(TString("jet_dTFitCore_ncell_window")+(t+1)*TSTEP,jet_dTFitCore_ncell_window[t],TString("jet_dTFitCore_ncell_window")+(t+1)*TSTEP+"[njet]/I");
  }

  ///results from fit for Jet Core with tracklets
  m_tree->Branch("jet_dTFitCoreHGTrk_ntrktot",jet_dTFitCoreHGTrk_ntrktot,"jet_dTFitCoreHGTrk_ntrktot[njet]/I");
  m_tree->Branch("jet_dTFitCoreHGTrk_ntrk",jet_dTFitCoreHGTrk_ntrk,"jet_dTFitCoreHGTrk_ntrk[njet]/I");
  m_tree->Branch("jet_dTFitCoreHGTrk_dt",jet_dTFitCoreHGTrk_dt,"jet_dTFitCoreHGTrk_dt[njet]/F");

  // ///results from fit with flexible mean and weighted by cluster energy
  // m_tree->Branch("jet_dTFitEw_signalmean",jet_dTFitEw_signalmean,"jet_dTFitEw_signalmean[njet]/F");
  // m_tree->Branch("jet_dTFitEw_Etot_clus",jet_dTFitEw_Etot_clus,"jet_dTFitEw_Etot_clus[njet]/F");
  // for(long t=0;t<NTWINDOW;t++)
  //   m_tree->Branch(TString("jet_dTFitEw_Ecell_clus_window")+(t+1)*TSTEP,jet_dTFitEw_Ecell_clus_window[t],TString("jet_dTFitEw_Ecell_clus_window")+(t+1)*TSTEP+"[njet]/F");
 
  // ///results from fit with flexible mean and weighted by cluster energy
  // m_tree->Branch("jet_dTFitEcw_signalmean",jet_dTFitEcw_signalmean,"jet_dTFitEcw_signalmean[njet]/F");
  // m_tree->Branch("jet_dTFitEcw_Etot_clus",jet_dTFitEcw_Etot_clus,"jet_dTFitEcw_Etot_clus[njet]/F");
  // for(long t=0;t<NTWINDOW;t++){
  //   m_tree->Branch(TString("jet_dTFitEcw_Ecell_clus_window")+(t+1)*TSTEP,jet_dTFitEcw_Ecell_clus_window[t],TString("jet_dTFitEcw_Ecell_clus_window")+(t+1)*TSTEP+"[njet]/F");
  // }
  // m_tree->Branch("jet_dTFitEcw_wp70",jet_dTFitEcw_wp70,"jet_dTFitEcw_wp70[njet]/I");

  //results from multi-signal fit
  //m_tree->Branch("jet_dTFitMultiSignal_sfrac",jet_dTFitMultiSignal_sfrac,"jet_dTFitMultiSignal_sfrac[njet]/F");
  //m_tree->Branch("jet_dTFitMultiSignal_sbratio",jet_dTFitMultiSignal_sbratio,"jet_dTFitMultiSignal_sbratio[njet]/F");
  //m_tree->Branch("jet_dTFitMultiSignal_minnll",jet_dTFitMultiSignal_minnll,"jet_dTFitMultiSignal_minnll[njet]/F");

  


  ////uncalibrated
  m_tree->Branch("jet_uncalib_pt", jet_uncalib_pt,"jet_uncalib_pt[njet]/F");
  m_tree->Branch("jet_uncalib_phi",jet_uncalib_phi,"jet_uncalib_phi[njet]/F");
  m_tree->Branch("jet_uncalib_eta",jet_uncalib_eta,"jet_uncalib_eta[njet]/F");
  m_tree->Branch("jet_uncalib_m",jet_uncalib_m,"jet_uncalib_m[njet]/F");
  m_tree->Branch("jet_uncalib_E",jet_uncalib_E,"jet_uncalib_E[njet]/F");

  m_tree->Branch("jet_truedt", jet_truedt,"jet_truedt[njet]/F");

  /////jet counters
  m_tree->Branch("njet_hgtd",&njet_hgtd,"njet_hgtd/I");
  m_tree->Branch("njet_pt20_hgtd",&njet_pt20_hgtd,"njet_pt20_hgtd/I");

  /////VBF signal selection
  m_tree->Branch("vbf_mjj",&vbf_mjj,"vbf_mjj/F");
  m_tree->Branch("vbf_j1",&vbf_j1,"vbf_j1/I");
  m_tree->Branch("vbf_j2",&vbf_j2,"vbf_j2/I");

  m_tree->Branch("vbfdt1_mjj",&vbfdt1_mjj,"vbfdt1_mjj/F");
  m_tree->Branch("vbfdt1_j1",&vbfdt1_j1,"vbfdt1_j1/I");
  m_tree->Branch("vbfdt1_j2",&vbfdt1_j2,"vbfdt1_j2/I");

  m_tree->Branch("vbfdt2_mjj",&vbfdt2_mjj,"vbfdt2_mjj/F");
  m_tree->Branch("vbfdt2_j1",&vbfdt2_j1,"vbfdt2_j1/I");
  m_tree->Branch("vbfdt2_j2",&vbfdt2_j2,"vbfdt2_j2/I");

  m_tree->Branch("vbfcoin_mjj",&vbfcoin_mjj,"vbfcoin_mjj/F");
  m_tree->Branch("vbfcoin_j1",&vbfcoin_j1,"vbfcoin_j1/I");
  m_tree->Branch("vbfcoin_j2",&vbfcoin_j2,"vbfcoin_j2/I");

  m_tree->Branch("vbfcoinnowp_mjj",&vbfcoinnowp_mjj,"vbfcoinnowp_mjj/F");
  m_tree->Branch("vbfcoinnowp_j1",&vbfcoinnowp_j1,"vbfcoinnowp_j1/I");
  m_tree->Branch("vbfcoinnowp_j2",&vbfcoinnowp_j2,"vbfcoinnowp_j2/I");

  m_tree->Branch("vbfcoinnowpj1_mjj",&vbfcoinnowpj1_mjj,"vbfcoinnowpj1_mjj/F");
  m_tree->Branch("vbfcoinnowpj1_j1",&vbfcoinnowpj1_j1,"vbfcoinnowpj1_j1/I");
  m_tree->Branch("vbfcoinnowpj1_j2",&vbfcoinnowpj1_j2,"vbfcoinnowpj1_j2/I");

  m_tree->Branch("vbfcjv_mjj",&vbfcjv_mjj,"vbfcjv_mjj/F");
  m_tree->Branch("vbfcjv_j1",&vbfcjv_j1,"vbfcjv_j1/I");
  m_tree->Branch("vbfcjv_j2",&vbfcjv_j2,"vbfcjv_j2/I");

  m_tree->Branch("vbfdt2cjv_mjj",&vbfdt2cjv_mjj,"vbfdt2cjv_mjj/F");
  m_tree->Branch("vbfdt2cjv_j1",&vbfdt2cjv_j1,"vbfdt2cjv_j1/I");
  m_tree->Branch("vbfdt2cjv_j2",&vbfdt2cjv_j2,"vbfdt2cjv_j2/I");


  //cluster counters
  m_tree->Branch("nclus_hgtd",&nclus_hgtd,"nclus_hgtd/I");
  m_tree->Branch("nclus_hgtd_clean",&nclus_hgtd_clean,"nclus_hgtd_clean/I");


  //MET
  m_tree->Branch("sumet",&sumet,"sumet/F");
  m_tree->Branch("met",&met,"met/F");
  m_tree->Branch("met_phi",&met_phi,"met_phi/F");

  //Electrons
  m_tree->Branch("nele",&nele,"nele/I");
  m_tree->Branch("ele_truth", ele_truth,"ele_truth[nele]/I");
  m_tree->Branch("ele_pt", ele_pt,"ele_pt[nele]/F");
  m_tree->Branch("ele_phi",ele_phi,"ele_phi[nele]/F");
  m_tree->Branch("ele_eta",ele_eta,"ele_eta[nele]/F");
  m_tree->Branch("ele_m",ele_m,"ele_m[nele]/F");
  m_tree->Branch("ele_E",ele_E,"ele_E[nele]/F");
  m_tree->Branch("ele_ncell",ele_ncell,"ele_ncell[nele]/I");
  m_tree->Branch("ele_truedt", ele_truedt,"ele_truedt[nele]/F");
  m_tree->Branch("ele_dTFit_signalmean",ele_dTFit_signalmean,"ele_dTFit_signalmean[nele]/F"); 
  m_tree->Branch("ele_ncellPU", ele_ncellPU, "ele_ncellPU[nele]/I");
  m_tree->Branch("ele_cellEPU", ele_cellEPU, "ele_cellEPU[nele]/F");

  for(long t=0;t<NTWINDOW;t++){
    m_tree->Branch(TString("ele_ncell_window")+(t+1)*TSTEP,ele_ncell_window[t],TString("ele_ncell_window")+(t+1)*TSTEP+"[nele]/I");
    m_tree->Branch(TString("ele_cellE_window")+(t+1)*TSTEP,ele_cellE_window[t],TString("ele_cellE_window")+(t+1)*TSTEP+"[nele]/F");
  }

  //Electron clusters
  m_tree->Branch("ele_clus",&ele_clus,"ele_clus[nele]/I");
  m_tree->Branch("ele_clus_pt",ele_clus_pt,"ele_clus_pt[nele]/F");
  m_tree->Branch("ele_clus_phi",ele_clus_phi,"ele_clus_phi[nele]/F");
  m_tree->Branch("ele_clus_eta",ele_clus_eta,"ele_clus_eta[nele]/F");
  m_tree->Branch("ele_clus_E",ele_clus_E,"ele_clus_E[nele]/F");
  m_tree->Branch("ele_clus_lambda",ele_clus_lambda,"ele_clus_lambda[nele]/F");
  m_tree->Branch("ele_clus_emfrac",ele_clus_emfrac,"ele_clus_emfrac[nele]/F");
  m_tree->Branch("ele_clus_emprob",ele_clus_emprob,"ele_clus_emprob[nele]/F");
  m_tree->Branch("ele_clus_sig",ele_clus_sig,"ele_clus_sig[nclus]/F");

  //Electron track
  m_tree->Branch("ele_ntrk",ele_ntrk,"ele_ntrk[njet]/I");
  m_tree->Branch("ele_ntrkpv",ele_ntrkpv,"ele_ntrkpv[njet]/I");
  m_tree->Branch("ele_trk",ele_trk,"ele_trk[njet]/I");

  //Tracks
  m_tree->Branch("ntrk",&ntrk,"ntrk/I");
  m_tree->Branch("trk_e", trk_e,"trk_e[ntrk]/F");
  m_tree->Branch("trk_pt", trk_pt,"trk_pt[ntrk]/F");
  m_tree->Branch("trk_eta",trk_eta,"trk_eta[ntrk]/F");
  m_tree->Branch("trk_phi",trk_phi,"trk_phi[ntrk]/F");
  m_tree->Branch("trk_d0", trk_d0,"trk_d0[ntrk]/F");
  m_tree->Branch("trk_z0", trk_z0,"trk_z0[ntrk]/F");
  m_tree->Branch("trk_charge", trk_charge,"trk_charge[ntrk]/I");
  m_tree->Branch("trk_ele",trk_ele,"trk_ele[ntrk]/I");
  m_tree->Branch("trk_jet",trk_jet,"trk_jet[ntrk]/I");
  m_tree->Branch("trk_jetdR",trk_jetdR,"trk_jetdR[ntrk]/F");
  
  m_tree->Branch("trk_ext_eta",trk_ext_eta,"trk_ext_eta[ntrk]/F");
  m_tree->Branch("trk_ext_phi",trk_ext_phi,"trk_ext_phi[ntrk]/F");
  m_tree->Branch("trk_ext_x",trk_ext_x,"trk_ext_x[ntrk]/F");
  m_tree->Branch("trk_ext_y",trk_ext_y,"trk_ext_y[ntrk]/F");
  m_tree->Branch("trk_ext_z",trk_ext_z,"trk_ext_z[ntrk]/F");
  m_tree->Branch("trk_ext_peta",trk_ext_peta,"trk_ext_peta[ntrk]/F");
  m_tree->Branch("trk_ext_pphi",trk_ext_pphi,"trk_ext_pphi[ntrk]/F");

  m_tree->Branch("trk_ext_dx",trk_ext_dx,"trk_ext_dx[ntrk]/F");
  m_tree->Branch("trk_ext_dy",trk_ext_dy,"trk_ext_dy[ntrk]/F");
  m_tree->Branch("trk_ext_dtheta",trk_ext_dtheta,"trk_ext_dtheta[ntrk]/F");
  m_tree->Branch("trk_ext_dphi",trk_ext_dphi,"trk_ext_dphi[ntrk]/F");

  m_tree->Branch("trk_hgtd0_x",trk_hgtd_x[0],"trk_hgtd0_x[ntrk]/F");
  m_tree->Branch("trk_hgtd1_x",trk_hgtd_x[1],"trk_hgtd1_x[ntrk]/F");
  m_tree->Branch("trk_hgtd2_x",trk_hgtd_x[2],"trk_hgtd2_x[ntrk]/F");
  m_tree->Branch("trk_hgtd3_x",trk_hgtd_x[3],"trk_hgtd3_x[ntrk]/F");

  m_tree->Branch("trk_hgtd0_y",trk_hgtd_y[0],"trk_hgtd0_y[ntrk]/F");
  m_tree->Branch("trk_hgtd1_y",trk_hgtd_y[1],"trk_hgtd1_y[ntrk]/F");
  m_tree->Branch("trk_hgtd2_y",trk_hgtd_y[2],"trk_hgtd2_y[ntrk]/F");
  m_tree->Branch("trk_hgtd3_y",trk_hgtd_y[3],"trk_hgtd3_y[ntrk]/F");

  m_tree->Branch("trk_ncell",trk_ncell,"trk_ncell[ntrk]/I");
  m_tree->Branch("trk_ncellL0",trk_ncellL0,"trk_ncellL0[ntrk]/I");
  m_tree->Branch("trk_cell0",trk_cell0,"trk_cell0[ntrk]/I");
  m_tree->Branch("trk_cell1",trk_cell1,"trk_cell1[ntrk]/I");
  m_tree->Branch("trk_cell2",trk_cell2,"trk_cell2[ntrk]/I");
  m_tree->Branch("trk_cell3",trk_cell3,"trk_cell3[ntrk]/I");

  m_tree->Branch("trk_t",trk_t,"trk_t[ntrk]/F");
  m_tree->Branch("trk_dt",trk_dt,"trk_dt[ntrk]/F");
  m_tree->Branch("trk_truedt",trk_truedt,"trk_truedt[ntrk]/F");

  m_tree->Branch("trk_outlier",trk_outlier,"trk_outlier[ntrk]/I");


  m_tree->Branch("trk_tr",trk_tr,"trk_tr[ntrk]/F");
  m_tree->Branch("trk_truetr",trk_truetr,"trk_truetr[ntrk]/F");
  m_tree->Branch("trk_trtrue",trk_trtrue,"trk_trtrue[ntrk]/F");

  m_tree->Branch("trk_truth_pdg",trk_truth_pdg,"trk_truth_pdg[ntrk]/I");
  m_tree->Branch("trk_truth_pt",trk_truth_pt,"trk_truth_pt[ntrk]/F");
  m_tree->Branch("trk_truth_eta",trk_truth_eta,"trk_truth_eta[ntrk]/F");
  m_tree->Branch("trk_truth_phi",trk_truth_phi,"trk_truth_phi[ntrk]/F");

  m_tree->Branch("trk_truth_vtxt",trk_truth_vtxt,"trk_truth_vtxt[ntrk]/F");
  m_tree->Branch("trk_truth_vtxz",trk_truth_vtxz,"trk_truth_vtxz[ntrk]/F");


  m_tree->Branch("trk_hgtrk",trk_hgtrk,"trk_hgtrk[ntrk]/I");
  m_tree->Branch("trk_hgtrkdR",trk_hgtrkdR,"trk_hgtrkdR[ntrk]/F");

  

  /////////////////////////////////////////////
  /// define layer positions
  if(m_geometry.compare("Preshower")==0){
    ZPOSL[0]=ZPOSL0_Preshower;
    ZPOSL[1]=ZPOSL1_Preshower;
    ZPOSL[2]=ZPOSL2_Preshower;
    ZPOSL[3]=ZPOSL3_Preshower;
  }
  if(m_geometry.compare("Timing")==0){
    ZPOSL[0]=ZPOSL0_Timing;
    ZPOSL[1]=ZPOSL1_Timing;
    ZPOSL[2]=ZPOSL2_Timing;
    ZPOSL[3]=ZPOSL3_Timing;
  }



  ///////////////////////////////////////////////////////////////
  //Define Inner Cells
  if(m_CellSize1==0) {
    ATH_MSG_ERROR ("Wrong Inner CELLSIZE" );
    return StatusCode::FAILURE;
  }
  
  if(HGTDHEIGHTIN%m_CellSize1!=0) {
    ATH_MSG_ERROR ("HGTDHEIGHT: "<<HGTDHEIGHTIN<<", not an integer of Inner CELLSIZE: "<<m_CellSize1 );
    return StatusCode::FAILURE;
  }

  NCELLXY1=2*HGTDHEIGHTIN/m_CellSize1;
  ATH_MSG_INFO ("Number of Inner cells per X-Y dimension" << NCELLXY1 );

  NARRAYCELL1 = 2 * 4 * NCELLXY1 * NCELLXY1; 
  ATH_MSG_INFO ("Number of total Inner cells " << NARRAYCELL1 );
  if(NARRAYCELL1>MAXCELL1){
    ATH_MSG_ERROR ("Inner Ncells requested too large" );
    return StatusCode::FAILURE;
  }


  //Define Outer Cells
  if(m_CellSize2==0) {
    ATH_MSG_ERROR ("Wrong Outer CELLSIZE" );
    return StatusCode::FAILURE;
  }
  
  if(HGTDHEIGHTOUT%m_CellSize2!=0) {
    ATH_MSG_ERROR ("HGTDHEIGHT not an integer of Outer CELLSIZE" );
    return StatusCode::FAILURE;
  }

  NCELLXY2=2*HGTDHEIGHTOUT/m_CellSize2;
  ATH_MSG_INFO ("Numbe of Outer cells per X-Y dimension" << NCELLXY2 );

  NARRAYCELL2 = 2 * 4 * NCELLXY2 * NCELLXY2; 
  ATH_MSG_INFO ("Number of total Outer cells " << NARRAYCELL2 );
  if(NARRAYCELL2>MAXCELL2){
    ATH_MSG_ERROR ("Inner Ncells requested too large" );
    return StatusCode::FAILURE;
  }
  ///////////////////////////////////////////////////////////////




  //Init the jet calibration tool
  m_jetCalibration.setTypeAndName("JetCalibrationTool/myJESTool");
  CHECK( m_jetCalibration.retrieve() ); //optional, just forces initializing the tool here instead of at first use


  ///Init the deltaT fitting tools
  FDTTemplates = new TFile(m_DTTemplatesFilePath.c_str(),"read");
  if(FDTTemplates->IsZombie())  return StatusCode::FAILURE;
  HDTTemplate_Sig = (TH1F*)FDTTemplates->Get(m_DTTemplateHistoSig.c_str());
  HDTTemplate_PU = (TH1F*)FDTTemplates->Get(m_DTTemplateHistoPU.c_str());
  if(!HDTTemplate_Sig) return StatusCode::FAILURE;
  if(!HDTTemplate_PU) return StatusCode::FAILURE;
  if(fabs(HDTTemplate_Sig->Integral()-1.0)>0.01) return StatusCode::FAILURE;
  if(fabs(HDTTemplate_PU->Integral()-1.0)>0.01) return StatusCode::FAILURE;



  // //////////////////////////////////////////
  // //Load the dR templates
  // /////////////////////////////////////////
  // FdRTemplates_Sig = new TFile(m_dRTemplatesFilePath_Sig.c_str(),"read");
  // if(FdRTemplates_Sig->IsZombie())  return StatusCode::FAILURE;

  // FdRTemplates_Bkg = new TFile(m_dRTemplatesFilePath_Bkg.c_str(),"read");
  // if(FdRTemplates_Bkg->IsZombie())  return StatusCode::FAILURE;
  // //gROOT->cd();

  // for(long pt=minptidx;pt<=maxptidx;pt+=ptstep){
  //   for(long eta=minetaidx;eta<=maxetaidx;eta+=etastep){
  //     TString name=TString("HdR_eta")+eta+"_pt"+pt;
  //     HdRTemplate_Sig[eta][pt]=(TH1F*)FdRTemplates_Sig->Get(name);
  //     HdRTemplate_Bkg[eta][pt]=(TH1F*)FdRTemplates_Bkg->Get(name);

  //     if(!HdRTemplate_Sig[eta][pt] || !HdRTemplate_Bkg[eta][pt]) continue;

  //     ///rebin
  //     if(HdRTemplate_Sig[eta][pt]->GetNbinsX()%dRFit_nbin != 0)
  // 	ATH_MSG_ERROR("dR template rebinning problem: hist hits="<<HdRTemplate_Sig[eta][pt]->GetNbinsX()<<" ,  new binning="<<dRFit_nbin);
      
  //     HdRTemplate_Sig[eta][pt]=(TH1F*)( HdRTemplate_Sig[eta][pt]->Rebin(HdRTemplate_Sig[eta][pt]->GetNbinsX()/dRFit_nbin,name+"_Sig_Rebin"));
  //     HdRTemplate_Bkg[eta][pt]=(TH1F*)( HdRTemplate_Bkg[eta][pt]->Rebin(HdRTemplate_Bkg[eta][pt]->GetNbinsX()/dRFit_nbin,name+"_Bkg_Rebin"));

  //     //continous functions 
  //     TString formula="(";
  //     char term[100];
  //     for(int b=1;b<=HdRTemplate_Bkg[eta][pt]->GetNbinsX();b++){
  // 	sprintf(term,"(%.3f<x&&x<=%.3f)*%.4f",
  // 		HdRTemplate_Bkg[eta][pt]->GetBinLowEdge(b),
  // 		HdRTemplate_Bkg[eta][pt]->GetBinLowEdge(b)+HdRTemplate_Bkg[eta][pt]->GetBinWidth(b),
  // 		HdRTemplate_Bkg[eta][pt]->GetBinContent(b));
  // 	formula+=term;
  // 	if(b<HdRTemplate_Bkg[eta][pt]->GetNbinsX())
  // 	  formula+="+";
  //     }
  //     formula+=")";
  //     FuncdR_Bkg[eta][pt] = new TF1(name+"Bkg",formula,
  // 				    HdRTemplate_Bkg[eta][pt]->GetXaxis()->GetXmin(),
  // 				    HdRTemplate_Bkg[eta][pt]->GetXaxis()->GetXmax());

  //     ///create a bkg + signal 
  //     TString formulaSig="(";
  //     for(int b=1;b<=HdRTemplate_Sig[eta][pt]->GetNbinsX();b++){
  // 	sprintf(term,"(%.3f<x&&x<=%.3f)*%.4f",
  // 		HdRTemplate_Sig[eta][pt]->GetBinLowEdge(b),
  // 		HdRTemplate_Sig[eta][pt]->GetBinLowEdge(b)+HdRTemplate_Sig[eta][pt]->GetBinWidth(b),
  // 		HdRTemplate_Sig[eta][pt]->GetBinContent(b));
  // 	formulaSig+=term;
  // 	if(b<HdRTemplate_Sig[eta][pt]->GetNbinsX())
  // 	  formulaSig+="+";
  //     }
  //     formulaSig+=")";
  //     FuncdR_BkgSig[eta][pt] = new TF1(name+"BkgSig",TString("[0]*((1-[1])*")+formula+"+[1]*"+formulaSig+")",
  // 				       HdRTemplate_Bkg[eta][pt]->GetXaxis()->GetXmin(),
  // 				       HdRTemplate_Bkg[eta][pt]->GetXaxis()->GetXmax());
  //     FuncdR_BkgSig[eta][pt]->SetParLimits(0,0,10000);
  //     FuncdR_BkgSig[eta][pt]->SetParLimits(1,0,1);

	
  //     //ATH_MSG_INFO("Loaded dRTemplate: eta="<<eta<<", pt="<<pt<<"  , "<<formula.Data());
     
  //   }
  // }



  ///functions for dRdTFit
  dRFitL=new TF1("dRFitL","[0]+[1]*3809.38*exp(-[2]*6.09719*(x^0.5))",0,0.4);
  dRFitM=new TF1("dRFitM","[0]+[1]*7065.16*exp(-[2]*6.53396*(x^0.5))",0,0.4);
  dRFitH=new TF1("dRFitH","[0]+[1]*8413.39*exp(-[2]*6.63098*(x^0.5))",0,0.4);
  dRFitArea=new TH1F("dRFitArea","",4,0.,0.4);
  for(int b=1; b<=dRFitArea->GetNbinsX(); b++){
    float area = TMath::Pi()*( pow(dRFitArea->GetBinLowEdge(b)+dRFitArea->GetBinWidth(b),2) - pow(dRFitArea->GetBinLowEdge(b),2) );
    dRFitArea->SetBinContent(b,area);
    dRFitArea->SetBinError(b,0.);
  }

  

  ////Create the truth histograms
  HVBFH_pT=new TH1F("HVBFH_pT","",100,0,500);
  HVBFH_eta=new TH1F("HVBFH_eta","",80,-8,8);
  HVBFJ1_pT=new TH1F("HVBFJ1_pT","",100,0,500);
  HVBFJ1_eta=new TH1F("HVBFJ1_eta","",80,-8,8);
  HVBFJ2_pT=new TH1F("HVBFJ2_pT","",100,0,500);
  HVBFJ2_eta=new TH1F("HVBFJ2_eta","",80,-8,8);
  HVBFG_pT=new TH1F("HVBFG_pT","",100,0,500);
  HVBFG_eta=new TH1F("HVBFG_eta","",80,-8,8);

  HZeeE1_pT=new TH1F("HZeeE1_pT","",100,0,500);
  HZeeE1_eta=new TH1F("HZeeE1_eta","",80,-8,8);
  HZeeE2_pT=new TH1F("HZeeE2_pT","",100,0,500);
  HZeeE2_eta=new TH1F("HZeeE2_eta","",80,-8,8);


  ///create the cell hit histograms
  for(long l=0;l<4;l++)
    for(long r=0;r<280;r++)
      HCellHitVsT1[l][r]=new TH1F(TString("HCellHitVsT1_")+l+"_"+r,"",100,0,1000);

  for(long l=0;l<4;l++)
    for(long r=0;r<200;r++)
      HCellHitVsT3[l][r]=new TH1F(TString("HCellHitVsT3_")+l+"_"+r,"",100,0,1000);




  ///////////////////////////////////////////
  ///Load the jet rejection WP's
  ///////////////////////////////////////////
  std::string line;
  float value;
  float ptmin;
  float ptmax;
  float etamin;
  float etamax;
  std::ifstream infile(m_VBFJetWPInputFile.c_str());
  while(std::getline(infile,line)){
    std::istringstream iss(line);
    iss>>ptmin>>ptmax>>etamin>>etamax>>value;
    int etabin=getJetWPEtaBin((etamin+etamax)/2);
    int ptbin=getJetWPPtBin(1000*(ptmin+ptmax)/2);
    VBFJetWP[etabin][ptbin]=value;
  }
    
  //print the values
  for(int p=0;p<3;p++){
    for(int e=0;e<6;e++){
      ATH_MSG_INFO ("WP pt="<<p<<" eta="<<e<<" : "<<VBFJetWP[e][p]);
    }
  }
  ATH_MSG_INFO ("WP cuts were loaded.");
  
  //set the time window index
  WPWindowIdx=(int(1000*0.06+0.5)/5 - 1);///NEEDS FIXING
  ATH_MSG_INFO ("WP Time window index is: "<<WPWindowIdx);




  ///////////////////////////////////////////////////////////////
  /////////Define the trigger arrays
  ///////////////////////////////////////

  ///initizalize the arrays
  if(StatusCode::FAILURE == initTrigger()) 
    return StatusCode::FAILURE;



  //////////////////////////////////////////////
  ///// Track extrapolation
  //////////////////////////////////////////////
  if(m_RunTrkExtraPol){
    m_CALO_INNER_Z = ZPOSL[0];
    
    Amg::Transform3D* translateAlongPositiveZ = new Amg::Transform3D(Amg::Vector3D(0.,0.,m_CALO_INNER_Z));
    m_discSurface_atCaloEntrance_positiveZ = new Trk::DiscSurface(translateAlongPositiveZ, 0., 10000.);
    
    Amg::Transform3D* translateAlongNegativeZ = new Amg::Transform3D(Amg::Vector3D(0.,0.,-m_CALO_INNER_Z)); 
    m_discSurface_atCaloEntrance_negativeZ = new Trk::DiscSurface(translateAlongNegativeZ, 0., 10000.);
    
    if ( m_extrapolator.retrieve().isFailure() ) {
      msg(MSG::FATAL) << "Failed to retrieve tool " << m_extrapolator << endreq;
      return StatusCode::FAILURE;
    }
  }

  FTrkMatch1mm=new TF1("FTrkMatch1mm","5.19+3.08e-4*x+0.91*exp(-8.43e-1*x)",0,1000);//x in GeV  
  FTrkMatch3mm=new TF1("FTrkMatch3mm","5.11+4.63e-3*x+1.92*exp(-7.10e-1*x)",0,1000);//x in GeV  
  

  FTrackZ0Cut = new TF1("FTrackZ0Cut","((-842.868+1348.4*x-858.564*x*x+271.583*x*x*x-42.5947*x*x*x*x+2.64988*x*x*x*x*x)>1.0?(-842.868+1348.4*x-858.564*x*x+271.583*x*x*x-42.5947*x*x*x*x+2.64988*x*x*x*x*x):1.0)",2.0,5.0);//must use |track eta|

 
 return StatusCode::SUCCESS;
}

float LArHitTupleDetGeo::getZPosition(int S){

  // float zpos=0.;

  // if(m_geometry.compare("Preshower")==0){//absorber
  //   if(S==-4) zpos = -ZPOSL3_Preshower;
  //   if(S==-3) zpos = -ZPOSL2_Preshower;
  //   if(S==-2) zpos = -ZPOSL1_Preshower;
  //   if(S==-1) zpos = -ZPOSL0_Preshower;
  //   if(S==1) zpos = ZPOSL0_Preshower;
  //   if(S==2) zpos = ZPOSL1_Preshower;
  //   if(S==3) zpos = ZPOSL2_Preshower;
  //   if(S==4) zpos = ZPOSL3_Preshower;
  // }
		
  // if(m_geometry.compare("Timing")==0){//no absorber
  //   if(S==-4) zpos = -ZPOSL3_Timing;
  //   if(S==-3) zpos = -ZPOSL2_Timing;
  //   if(S==-2) zpos = -ZPOSL1_Timing;
  //   if(S==-1) zpos = -ZPOSL0_Timing;
  //   if(S==1) zpos = ZPOSL0_Timing;
  //   if(S==2) zpos = ZPOSL1_Timing;
  //   if(S==3) zpos = ZPOSL2_Timing;
  //   if(S==4) zpos = ZPOSL3_Timing;
  // }

  if(S==0) return 0.;
  return (S<0 ? -ZPOSL[abs(S)-1]  :  ZPOSL[abs(S)-1]);
}


void LArHitTupleDetGeo::resetCells(){
  //this method must be called for each event because at the end of execute() the cells are compressed
  
  ///Inner Cells
  for(int S=-4;S<=4;S++){
    if(S==0) continue;
    for(int X=0; X < NCELLXY1; X++ ){
      for(int Y=0; Y < NCELLXY1; Y++ ){
	int cellidx = ( (S  + (S<0 ? 4 : 3))  * NCELLXY1 * NCELLXY1 ) + (X * NCELLXY1) + Y;//from S,X,Y to cell index

	cell1_x[cellidx] = X*m_CellSize1-HGTDHEIGHTIN+m_CellSize1/2.;//assign center of cell as (x,y)
	cell1_y[cellidx] = Y*m_CellSize1-HGTDHEIGHTIN+m_CellSize1/2.;
	cell1_S[cellidx] = S;
	cell1_r[cellidx] = sqrt(cell1_x[cellidx]*cell1_x[cellidx] + cell1_y[cellidx]*cell1_y[cellidx]);
	cell1_z[cellidx] = getZPosition(S);

	TLorentzVector P4;
	P4.SetXYZT(cell1_x[cellidx],cell1_y[cellidx],cell1_z[cellidx],0.);
	cell1_eta[cellidx]= P4.Eta();
	cell1_phi[cellidx]= P4.Phi();

	cell1_E[cellidx] = 0.;
	cell1_t[cellidx] = 0.;
	cell1_nhit[cellidx] = 0;
	cell1_jet[cellidx] = -1;
	cell1_jetdR[cellidx] = 100;
	cell1_clus[cellidx] = -1;
	cell1_clusdR[cellidx] = 100;
	cell1_ele[cellidx] = -1;
	cell1_eledR[cellidx] = 100;
	cell1_elePU[cellidx] = -1;

	cell1_hx[cellidx] = X;
	cell1_hy[cellidx] = Y;
	cell1_trkidx[cellidx] = (S<0 ? 0 : 1)  * NCELLXY1 * NCELLXY1 + (X * NCELLXY1) + Y;
      }
    }
  }




  /////Outer cells
  for(int S=-4;S<=4;S++){
    if(S==0) continue;
    for(int X=0; X < NCELLXY2; X++ ){
      for(int Y=0; Y < NCELLXY2; Y++ ){
	int cellidx = ( (S  + (S<0 ? 4 : 3))  * NCELLXY2 * NCELLXY2 ) + (X * NCELLXY2) + Y;//from S,X,Y to cell index

	cell2_x[cellidx] = X*m_CellSize2-HGTDHEIGHTOUT+m_CellSize2/2.;//assign center of cell as (x,y)
	cell2_y[cellidx] = Y*m_CellSize2-HGTDHEIGHTOUT+m_CellSize2/2.;
	cell2_S[cellidx] = S;
	cell2_r[cellidx] = sqrt(cell2_x[cellidx]*cell2_x[cellidx] + cell2_y[cellidx]*cell2_y[cellidx]);
	cell2_z[cellidx] = getZPosition(S);

	TLorentzVector P4;
	P4.SetXYZT(cell2_x[cellidx],cell2_y[cellidx],cell2_z[cellidx],0.);
	cell2_eta[cellidx]= P4.Eta();
	cell2_phi[cellidx]= P4.Phi();

	cell2_E[cellidx] = 0.;
	cell2_t[cellidx] = 0.;
	cell2_nhit[cellidx] = 0;
	cell2_jet[cellidx] = -1;
	cell2_jetdR[cellidx] = 100;
	cell2_clus[cellidx] = -1;
	cell2_clusdR[cellidx] = 100;
	cell2_ele[cellidx] = -1;
	cell2_eledR[cellidx] = 100;
	cell2_elePU[cellidx] = -1;

	cell2_hx[cellidx] = X;
	cell2_hy[cellidx] = Y;
	cell2_trkidx[cellidx] = (S<0 ? 0 : 1)  * NCELLXY2 * NCELLXY2 + (X * NCELLXY2) + Y;
      }
    }
  }


}



StatusCode LArHitTupleDetGeo::initTrigger(){

  ////////////////////////////////////////////////
  NTRIGGERXY1=int(2*TRIGBOUNDARY1/TRIGSIZE1);
  ATH_MSG_INFO ("Number of trigger tiles per X-Y dimension" <<NTRIGGERXY1);

  NARRAYTRIGGER1 = 2 * NTRIGGERXY1 * NTRIGGERXY1; 
  ATH_MSG_INFO ("Number of total trigger tiles " << NARRAYTRIGGER1);
  if(NARRAYTRIGGER1>MAXTRIG1){
    ATH_MSG_ERROR ("Number of Trigger tiles requested too large: "<<NARRAYTRIGGER1<<" , max is "<<MAXTRIG1);
    return StatusCode::FAILURE;
  }

  for(int S=-1;S<=1;S++){//merge layers, for all layers need to change 1 : 0 -> 4 : 3
    if(S==0) continue;

    ////Inner tiles
    for(int X=0; X < NTRIGGERXY1; X++ ){
      for(int Y=0; Y < NTRIGGERXY1; Y++ ){
	int trigidx = ( (S  + (S<0 ? 1 : 0))  * NTRIGGERXY1 * NTRIGGERXY1 ) + (X * NTRIGGERXY1) + Y;//from S,X,Y to trig index

	trig1_x[trigidx] = X*TRIGSIZE1-TRIGBOUNDARY1+TRIGSIZE1/2.;//assign center of trig as (x,y)
	trig1_y[trigidx] = Y*TRIGSIZE1-TRIGBOUNDARY1+TRIGSIZE1/2.;
	//trig1_S[trigidx] = S;
	trig1_z[trigidx] = getZPosition(S);
      }
    }
  }


  ////////////////////////////////////////////////
  NTRIGGERXY2=int(2*TRIGBOUNDARY2/TRIGSIZE2);
  ATH_MSG_INFO ("Number of trigger tiles per X-Y dimension" <<NTRIGGERXY2);

  NARRAYTRIGGER2 = 2 * NTRIGGERXY2 * NTRIGGERXY2; 
  ATH_MSG_INFO ("Number of total trigger tiles " << NARRAYTRIGGER2);
  if(NARRAYTRIGGER2>MAXTRIG2){
    ATH_MSG_ERROR ("Number of Trigger tiles requested too large: "<<NARRAYTRIGGER2<<" , max is "<<MAXTRIG2);
    return StatusCode::FAILURE;
  }

  for(int S=-1;S<=1;S++){//merge layers, for all layers need to change 1 : 0 -> 4 : 3
    if(S==0) continue;

    for(int X=0; X < NTRIGGERXY2; X++ ){
      for(int Y=0; Y < NTRIGGERXY2; Y++ ){
	int trigidx = ( (S  + (S<0 ? 1 : 0))  * NTRIGGERXY2 * NTRIGGERXY2 ) + (X * NTRIGGERXY2) + Y;//from S,X,Y to trig index

	trig2_x[trigidx] = X*TRIGSIZE2-TRIGBOUNDARY2+TRIGSIZE2/2.;//assign center of trig as (x,y)
	trig2_y[trigidx] = Y*TRIGSIZE2-TRIGBOUNDARY2+TRIGSIZE2/2.;
	//trig2_S[trigidx] = S;
	trig2_z[trigidx] = getZPosition(S);
	//ATH_MSG_INFO ("TRIG2: "<<S<<"  X="<<X<<"  Y="<<Y<<" idx="<<trigidx<<" Size="<<TRIGSIZE2<<" Boundary="<<TRIGBOUNDARY2<<"   x="<<trig2_x[trigidx]<<"  y="<<trig2_y[trigidx]);
      }
    }

  }

  ////////////////////////////////////////////////
  NTRIGGERXY3=int(2*TRIGBOUNDARY3/TRIGSIZE3);
  ATH_MSG_INFO ("Number of trigger tiles per X-Y dimension" <<NTRIGGERXY3);

  NARRAYTRIGGER3 = 2 * NTRIGGERXY3 * NTRIGGERXY3; 
  ATH_MSG_INFO ("Number of total trigger tiles " << NARRAYTRIGGER3);
  if(NARRAYTRIGGER3>MAXTRIG3){
    ATH_MSG_ERROR ("Number of Trigger tiles requested too large: "<<NARRAYTRIGGER3<<" , max is "<<MAXTRIG3);
    return StatusCode::FAILURE;
  }
  
  for(int S=-1;S<=1;S++){//merge layers, for all layers need to change 1 : 0 -> 4 : 3
    if(S==0) continue;

    for(int X=0; X < NTRIGGERXY3; X++ ){
      for(int Y=0; Y < NTRIGGERXY3; Y++ ){
	int trigidx = ( (S  + (S<0 ? 1 : 0))  * NTRIGGERXY3 * NTRIGGERXY3 ) + (X * NTRIGGERXY3) + Y;//from S,X,Y to trig index
	
	trig3_x[trigidx] = X*TRIGSIZE3-TRIGBOUNDARY3+TRIGSIZE3/2.;//assign center of trig as (x,y)
	trig3_y[trigidx] = Y*TRIGSIZE3-TRIGBOUNDARY3+TRIGSIZE3/2.;
	//trig3_S[trigidx] = S;
	trig3_z[trigidx] = getZPosition(S);
      }
    }
  } 



  ////////////////////////////////////////////////
  NTRIGGERXY4=int(2*TRIGBOUNDARY4/TRIGSIZE4);
  ATH_MSG_INFO ("Number of trigger tiles per X-Y dimension" <<NTRIGGERXY4);

  NARRAYTRIGGER4 = 2 * NTRIGGERXY4 * NTRIGGERXY4; 
  ATH_MSG_INFO ("Number of total trigger tiles " << NARRAYTRIGGER4);
  if(NARRAYTRIGGER4>MAXTRIG4){
    ATH_MSG_ERROR ("Number of Trigger tiles requested too large: "<<NARRAYTRIGGER4<<" , max is "<<MAXTRIG4);
    return StatusCode::FAILURE;
  }
  
  for(int S=-1;S<=1;S++){//merge layers, for all layers need to change 1 : 0 -> 4 : 3
    if(S==0) continue;

    for(int X=0; X < NTRIGGERXY4; X++ ){
      for(int Y=0; Y < NTRIGGERXY4; Y++ ){
	int trigidx = ( (S  + (S<0 ? 1 : 0))  * NTRIGGERXY4 * NTRIGGERXY4 ) + (X * NTRIGGERXY4) + Y;//from S,X,Y to trig index
	
	trig4_x[trigidx] = X*TRIGSIZE4-TRIGBOUNDARY4+TRIGSIZE4/2.;//assign center of trig as (x,y)
	trig4_y[trigidx] = Y*TRIGSIZE4-TRIGBOUNDARY4+TRIGSIZE4/2.;
	//trig4_S[trigidx] = S;
	trig4_z[trigidx] = getZPosition(S);
      }
    }
  } 


  /////////////////////////////////////////////
  /////Merge the tile arrays into one
  /////////////////////////////////////////////
  ntrig=0;
  for(int idx=0;idx<NARRAYTRIGGER1;idx++){//these are 1/3 x 1/3 tiles
    if(fabs(trig1_x[idx])<ASU/2 && fabs(trig1_y[idx])<ASU/2) continue; //remove inner hole
    if(fabs(trig1_x[idx])>ASU/2 && fabs(trig1_y[idx])>ASU/2) continue; //remove corner sensor which is 1/2 x 1/2
    
    trig_x[ntrig] = trig1_x[idx];
    trig_y[ntrig] = trig1_y[idx];
    //trig_S[ntrig] = trig1_S[idx];
    trig_z[ntrig] = trig1_z[idx];
    trig_size[ntrig] = TRIGSIZE1;
    ntrig++;
  }

  for(int idx=0;idx<NARRAYTRIGGER2;idx++){//these are 1/2 x 1/2 tiles
    if(fabs(trig2_x[idx])<ASU/2 && fabs(trig2_y[idx])<ASU) continue; //remove inner 
    if(fabs(trig2_x[idx])<ASU && fabs(trig2_y[idx])<ASU/2) continue; //remove inner 
    if(fabs(trig2_x[idx])>ASU && fabs(trig2_y[idx])>ASU) continue; //remove corner sensor which is 1 x 1

    trig_x[ntrig] = trig2_x[idx];
    trig_y[ntrig] = trig2_y[idx];
    //trig_S[ntrig] = trig2_S[idx];
    trig_z[ntrig] = trig2_z[idx];
    trig_size[ntrig] = TRIGSIZE2;
    ntrig++;
  }

  for(int idx=0;idx<NARRAYTRIGGER3;idx++){//these are 1 x 1 tiles
    if(fabs(trig3_x[idx])<ASU && fabs(trig3_y[idx])<1.5*ASU) continue; //remove inner
    if(fabs(trig3_x[idx])<1.5*ASU && fabs(trig3_y[idx])<ASU) continue; //remove inner
    
    trig_x[ntrig] = trig3_x[idx];
    trig_y[ntrig] = trig3_y[idx];
    //trig_S[ntrig] = trig3_S[idx];
    trig_z[ntrig] = trig3_z[idx];
    trig_size[ntrig] = TRIGSIZE3;
    ntrig++;
  }

  for(int idx=0;idx<NARRAYTRIGGER4;idx++){////these are 2 x 2 tiles
    if(fabs(trig4_x[idx])<2*ASU && fabs(trig4_y[idx])<2*ASU) continue; //remove inner tiles
    
    trig_x[ntrig] = trig4_x[idx];
    trig_y[ntrig] = trig4_y[idx];
    //trig_S[ntrig] = trig4_S[idx];
    trig_z[ntrig] = trig4_z[idx];
    trig_size[ntrig] = TRIGSIZE4;
    ntrig++;
  }


  /// set derived quantities
  TLorentzVector P4;
  for(int idx=0;idx<ntrig;idx++){
    trig_r[idx] = sqrt(trig_x[idx]*trig_x[idx] + trig_y[idx]*trig_y[idx]); 
    trig_D[idx] = sqrt(trig_x[idx]*trig_x[idx] + trig_y[idx]*trig_y[idx] + trig_z[idx]*trig_z[idx]);    
    trig_sizeeta[idx]= FEtaVsR->Eval(trig_r[idx]-trig_size[idx]/2) - FEtaVsR->Eval(trig_r[idx]+trig_size[idx]/2) ; 

    P4.SetXYZT(trig_x[idx],trig_y[idx],trig_z[idx],0);
    trig_eta[idx] = P4.Eta();
    trig_phi[idx] = P4.Phi();
  }



  /////////////////////
  /// set predefined trigger cut
  //////////////
  TrigCut=new TF2("TrigCut",m_TrigCut.c_str(),0,200,0,1000);//x: tile size in mm, y tile radial position
  ATH_MSG_INFO (" TrigCut = " << m_TrigCut.c_str());

  return StatusCode::SUCCESS;
}



StatusCode LArHitTupleDetGeo::finalize() {
  ATH_MSG_INFO ("Finalizing " << name() );
  ATH_MSG_INFO (" number of events in tuple = " << m_tree->GetEntries());

  for(int c=0;c<100;c++)
    if(cutflow[c]>0)
      ATH_MSG_INFO (" cut flow "<<c<<" : "<<cutflow[c]);


  //////////////////////////////////////////////
  //Write histograms with the cell geometry information
  //////////////////////////////////////////////
  resetCells();
  int rcellstep=10;//mm
  int rcellmin=50;

  ////////////////////////////////////
  //number of cells in one HGTD Layer
  ///////////////////////////////////
  TH1F HNCell("HNCell","",HGTDHEIGHTOUT/rcellstep,0,HGTDHEIGHTOUT);//
  for(int X=0; X < NCELLXY1; X++ ){
    for(int Y=0; Y < NCELLXY1; Y++ ){
      int cellidx = ( (1  + (1<0 ? 4 : 3))  * NCELLXY1 * NCELLXY1 ) + (X * NCELLXY1) + Y;
      if(rcellmin<cell1_r[cellidx] && cell1_r[cellidx]<HGTDHEIGHTIN) 
	HNCell.Fill(cell1_r[cellidx]);
    }
  }
  for(int X=0; X < NCELLXY2; X++ ){
    for(int Y=0; Y < NCELLXY2; Y++ ){
      int cellidx = ( (1  + (1<0 ? 4 : 3))  * NCELLXY2 * NCELLXY2 ) + (X * NCELLXY2) + Y;
      if(HGTDHEIGHTIN<cell2_r[cellidx] && cell2_r[cellidx]<HGTDHEIGHTOUT) 
	HNCell.Fill(cell2_r[cellidx]);
    }
  }
  m_outfile->cd();
  HNCell.Write();


  //////////////////////////////////////////////
  ////write tuple
  //////////////////////////////////////////////
  m_outfile->cd();
  m_tree->Write();


  //////////////////////////////////////////////
  ///Write the truth histograms
  //////////////////////////////////////////////
  m_outfile->cd();

  HVBFH_pT->Write();
  HVBFH_eta->Write();
  HVBFJ1_pT->Write();
  HVBFJ1_eta->Write();
  HVBFJ2_pT->Write();
  HVBFJ2_eta->Write();
  HVBFG_pT->Write();
  HVBFG_eta->Write();

  HZeeE1_pT->Write();
  HZeeE1_eta->Write();
  HZeeE2_pT->Write();
  HZeeE2_eta->Write();


  // ////////////////////////////
  // // Write the hit distributions
  // /////////////////////////////
  // m_outfile->cd();

  // for(long l=0;l<4;l++)
  //   for(long r=0;r<280;r++)
  //     HCellHitVsT1[l][r]->Write();

  // for(long l=0;l<4;l++)
  //   for(long r=0;r<200;r++)
  //     HCellHitVsT3[l][r]->Write();
	

  //////////////////////////////////////////////
  //Close
  //m_outfile->ls();
  m_outfile->Close();
  delete m_outfile;
  //delete m_tree;

  delete FDTTemplates;

  return StatusCode::SUCCESS;
}


void LArHitTupleDetGeo::printTruthTree(unsigned int idx){
  if(idx >= Truth->size()) return;

  ATH_MSG_INFO (" TRUTH TREE: "<<Truth->at(idx)->pdgId()<<" status="<<Truth->at(idx)->status()<<" nChildren="<<Truth->at(idx)->nChildren());
  for(unsigned int i=0;i< Truth->at(idx)->nChildren();i++){
    const xAOD::TruthParticle* part1 = Truth->at(idx)->child(i); 
    ///First generation
    ATH_MSG_INFO ("    id="<<part1->pdgId()<<"  status="<<part1->status()<<"  (pt,eta)="<<part1->p4().Pt()<<","<<part1->p4().Eta()<<")");
    for(unsigned int j=0;j< part1->nChildren();j++){
      const xAOD::TruthParticle* part2 = part1->child(j);
      ///Second generation
      ATH_MSG_INFO ("        id="<<part2->pdgId()<<"  status="<<part2->status()<<"  (pt,eta)="<<part2->p4().Pt()<<","<<part2->p4().Eta()<<")");
      for(unsigned int k=0;k< part2->nChildren();k++){
	const xAOD::TruthParticle* part3 = part2->child(j);
	///Third generation
	ATH_MSG_INFO ("            id="<<part3->pdgId()<<"  status="<<part3->status()<<"  (pt,eta)="<<part3->p4().Pt()<<","<<part3->p4().Eta()<<")");
      }
    }
  }

}

StatusCode LArHitTupleDetGeo::processTruth(){

  if( ! evtStore()->retrieve( Truth , "TruthParticles" ).isSuccess() ){
    ATH_MSG_ERROR ("Failed to retrieve  TruthParticle collection. Exiting." );
    return StatusCode::FAILURE;
  }
  

  ///Print hard scatter
  //printTruthTree(2);


  truth_Higgs = -1;

  truth_Wp = -1;
  truth_Wm = -1;
  truth_Lp = -1;
  truth_Lm = -1;

  truth_VBFJ1 = -1;
  truth_VBFJ2 = -1;
  truth_Gluon = -1;

  truth_HiggsB = -1;
  truth_HiggsBbar = -1;

  truth_Z = -1; 
  truth_Z_Lp = -1; 
  truth_Z_Lm = -1;
  truth_Z_e1 = -1; 
  truth_Z_e2 = -1; 

  for (int i = 0 ; i < int(Truth->size()); ++i) {
     const xAOD::TruthParticle* part = Truth->at(i);

    //ATH_MSG_DEBUG (" PDGID="<<part->pdgId()<<" status= "<< part->status()<<"  pT="<<part->p4().Pt()<<" eta="<<part->p4().Eta()<<" phi="<<part->p4().Phi());

    //find the higgs
    if(truth_Higgs == -1 && part->status() == 22  && part->pdgId()==25 ) truth_Higgs = i; //Higgs

    //find the higgs -> bb
    if(truth_Higgs != -1 && part->pdgId() == 5  )
      if( part->nParents()>0 && part->parent(0)->pdgId()==25 ) truth_HiggsB = i; //b
    if(truth_Higgs != -1 && part->pdgId() == -5  )
      if( part->nParents()>0 && part->parent(0)->pdgId()==25 ) truth_HiggsBbar = i; //bbar
    

    //find VBF jets 
    if(truth_VBFJ1 == -1 && part->status() == 23  && (1<= abs(part->pdgId()) && abs(part->pdgId())<= 5) ) truth_VBFJ1 = i; //find the first VBF jet
    if(truth_VBFJ1 != -1 && i != truth_VBFJ1 && 
       truth_VBFJ2 == -1 && part->status() == 23  && (1<= abs(part->pdgId()) && abs(part->pdgId())<= 5) ) truth_VBFJ2 = i; //find the second one
    
    if(truth_Gluon == -1 && part->status() == 23  && part->pdgId()==21 ) truth_Gluon = i; //gluon radiated jet

    //find W+W-
    if(truth_Wp == -1 && part->pdgId()==24 ) truth_Wp = i; //W+
    if(truth_Wm == -1 && part->pdgId()==-24 ) truth_Wm = i; //W-

    //find the W leptons
    if(truth_Lp ==-1  && (part->pdgId()==-11 || part->pdgId()==-13)){ //&& part->status()==1
      for(unsigned int P1=0;P1< part->nParents();P1++){
	if( part->parent(P1)->pdgId()==24 ) truth_Lp = i; 
	for(unsigned int P2=0;P2< part->parent(P1)->nParents();P2++){
	  if( part->parent(P1)->parent(P2)->pdgId()==24 ) truth_Lp = i; 
	  for(unsigned int P3=0;P3< part->parent(P1)->parent(P2)->nParents();P3++)
	    if( part->parent(P1)->parent(P2)->parent(P3)->pdgId()==24 ) truth_Lp = i; 
	}
      }
    }

    if(truth_Lm ==-1  && (part->pdgId()==11 || part->pdgId()==13)){ //&& part->status()==1
      for(unsigned int P1=0;P1< part->nParents();P1++){
	if( part->parent(P1)->pdgId()==-24 ) truth_Lm = i; 
	for(unsigned int P2=0;P2< part->parent(P1)->nParents();P2++){
	  if( part->parent(P1)->parent(P2)->pdgId()==-24 ) truth_Lm = i; 
	  for(unsigned int P3=0;P3< part->parent(P1)->parent(P2)->nParents();P3++)
	    if( part->parent(P1)->parent(P2)->parent(P3)->pdgId()==-24 ) truth_Lm = i; 
	}
      }
    }



    //find Z
    if(truth_Z == -1 && part->pdgId()==23 ){
      truth_Z = i; //Z
    }
    
    //find the Z electrons
    if(truth_Z_Lp ==-1  && part->pdgId()==-11){
      for(unsigned int P1=0;P1< part->nParents();P1++){
	if( part->parent(P1)->pdgId()==23 ) truth_Z_Lp = i; 
	for(unsigned int P2=0;P2< part->parent(P1)->nParents();P2++){
	  if( part->parent(P1)->parent(P2)->pdgId()==23 )  truth_Z_Lp = i; 
	}
      }
    }
    if(truth_Z_Lm ==-1  && part->pdgId()== 11){
      for(unsigned int P1=0;P1< part->nParents();P1++){
	if( part->parent(P1)->pdgId()== 23) truth_Z_Lm = i;
	for(unsigned int P2=0;P2< part->parent(P1)->nParents();P2++){
	  if( part->parent(P1)->parent(P2)->pdgId()== 23) truth_Z_Lm = i; 
	}  
      }
    }

  }  

  

  // ////Swap the jets to have J1 in the forward
  // if(truth_VBFJ1 != -1 && truth_VBFJ2 != -1){
  //   if(fabs(Truth->at(truth_VBFJ1)->p4().Eta()) < fabs(Truth->at(truth_VBFJ2)->p4().Eta())){
  //     int tmp=truth_VBFJ1;
  //     truth_VBFJ1 = truth_VBFJ2; 
  //     truth_VBFJ2 = tmp; 
  //   }
  // }


  ///Determine which electron is in the forward region
  if(truth_Z_Lm != -1 && truth_Z_Lp != -1){
    if(fabs(Truth->at(truth_Z_Lm)->p4().Eta()) > fabs(Truth->at(truth_Z_Lp)->p4().Eta())){
      truth_Z_e1 = truth_Z_Lm; 
      truth_Z_e2 = truth_Z_Lp; 
    }
    if(fabs(Truth->at(truth_Z_Lm)->p4().Eta()) < fabs(Truth->at(truth_Z_Lp)->p4().Eta())){
      truth_Z_e1 = truth_Z_Lp; 
      truth_Z_e2 = truth_Z_Lm; 
    } 
  }
  
 

  ///////////////////////////////////////////////
  //////// Fill the truth block
  /////////////////////////////////////////////
  ntruth=0;
  for (int i = 0 ; i < int(Truth->size()); ++i) {
    if( ntruth == MAXTRUTH){
      ATH_MSG_DEBUG ("Truth block is at max "<<MAXTRUTH);
      break;
    }
    const xAOD::TruthParticle* part = Truth->at(i);
    
    ///save only the particles we are interested in 
    if(i == truth_Higgs) truth_Higgs=ntruth;
    else if(i == truth_VBFJ1) truth_VBFJ1=ntruth;
    else if(i == truth_VBFJ2) truth_VBFJ2=ntruth;
    else if(i == truth_Gluon) truth_Gluon=ntruth;
    else if(i == truth_HiggsB) truth_HiggsB=ntruth;
    else if(i == truth_HiggsBbar) truth_HiggsBbar=ntruth;
    else if(i == truth_Wp) truth_Wp=ntruth;
    else if(i == truth_Wm) truth_Wm=ntruth;
    else if(i == truth_Lp) truth_Lp=ntruth;
    else if(i == truth_Lm) truth_Lm=ntruth;
    else if(i == truth_Z) truth_Z=ntruth;
    else if(i == truth_Z_Lp) truth_Z_Lp=ntruth;
    else if(i == truth_Z_Lm) truth_Z_Lm=ntruth;
    else if(i == truth_Z_e1) truth_Z_e1=ntruth;
    else if(i == truth_Z_e2) truth_Z_e2=ntruth;
    else continue;

    truth_m[ntruth] = part->p4().M();
    truth_p[ntruth] = part->p4().P();
    truth_pt[ntruth] = part->p4().Pt();
    if(truth_pt[ntruth] > 0.0001 ) truth_eta[ntruth] = part->p4().Eta();
    else truth_eta[ntruth] = -9999;
    truth_phi[ntruth] = part->p4().Phi();
    truth_status[ntruth] = part->status();
    truth_pdg[ntruth] = part->pdgId();

    ntruth++;    

  }



  /////////////////////////////DEBUG info
  ATH_MSG_DEBUG (" Higgs="<<truth_Higgs<<" "<<truth_pt[truth_Higgs]<<" "<<truth_eta[truth_Higgs]<<" "<<truth_phi[truth_Higgs]);
  ATH_MSG_DEBUG (" VBFJ1="<<truth_VBFJ1<<" "<<truth_pt[truth_VBFJ1]<<" "<<truth_eta[truth_VBFJ1]<<" "<<truth_phi[truth_VBFJ1]);
  ATH_MSG_DEBUG (" VBFJ2="<<truth_VBFJ2<<" "<<truth_pt[truth_VBFJ2]<<" "<<truth_eta[truth_VBFJ2]<<" "<<truth_phi[truth_VBFJ2]);
  ATH_MSG_DEBUG (" Gluon="<<truth_Gluon<<" "<<truth_pt[truth_Gluon]<<" "<<truth_eta[truth_Gluon]<<" "<<truth_phi[truth_Gluon]);
  ATH_MSG_DEBUG (" Hbb b1="<<truth_HiggsB);
  ATH_MSG_DEBUG (" Hbb b2="<<truth_HiggsBbar);
  ATH_MSG_DEBUG (" Lp="<<truth_Lp<<", Lm="<<truth_Lm); 
  ATH_MSG_DEBUG (" truth_Z="<<truth_Z<<" truth_Z_Lm = "<<truth_Z_Lm<<", truth_Z_Lp = "<<truth_Z_Lp); 

  //if(truth_Lm ==-1) printTruthTree(truth_Wm);


  ////Fill the truth histograms for VBF sample
  HVBFH_pT->Fill(truth_pt[truth_Higgs]/1000.);
  HVBFH_eta->Fill(truth_eta[truth_Higgs]);
  HVBFJ1_pT->Fill(truth_pt[truth_VBFJ1]/1000.);
  HVBFJ1_eta->Fill(truth_eta[truth_VBFJ1]);
  HVBFJ2_pT->Fill(truth_pt[truth_VBFJ2]/1000.);
  HVBFJ2_eta->Fill(truth_eta[truth_VBFJ2]);
  if(truth_Gluon!=-1) HVBFG_pT->Fill(truth_pt[truth_Gluon]/1000.);
  if(truth_Gluon!=-1) HVBFG_eta->Fill(truth_eta[truth_Gluon]);
  
  ////Fill the truth histograms for Zee sample
  HZeeE1_pT->Fill(truth_pt[truth_Z_e1]/1000.);
  HZeeE1_eta->Fill(truth_eta[truth_Z_e1]);
  HZeeE2_pT->Fill(truth_pt[truth_Z_e2]/1000.);
  HZeeE2_eta->Fill(truth_eta[truth_Z_e2]);



  return StatusCode::SUCCESS;
}






bool LArHitTupleDetGeo::includeParticle(const xAOD::TruthParticle* part){
  if (part->status()!=1) return false;

  if (part->pdgId()==21 && part->e()==0) return false; //Work around for an old generator bug

  if (abs(part->pdgId())==13) return false; //muons

  if ((part->barcode())>2e5) return false; //from G4

  if (abs(part->pdgId())==12) return false; //neutrinos

  if (abs(part->pdgId())==14) return false; //neutrinos

  if (abs(part->pdgId())==16) return false; //neutrinos

  if (fabs(part->eta())>5) return false; //outside detector range

  //Assuming we are including W, Z, and taus

  return true;

  
}





StatusCode LArHitTupleDetGeo::processEventInfo(){

  const xAOD::EventInfo * m_eventInfo;
  if( ! evtStore()->retrieve( m_eventInfo, "EventInfo").isSuccess() ) {
    ATH_MSG_ERROR ("Failed to retrieve event info collection. Exiting." << name() );
    return StatusCode::FAILURE;
  }

  ///Print Event Information:

  
  //Fill tuple
  run=m_eventInfo->runNumber();
  event=m_eventInfo->eventNumber();
  mu=m_eventInfo->averageInteractionsPerCrossing();
  bcid=m_eventInfo->bcid();


  ATH_MSG_DEBUG ("EventInfo: event="<<m_eventInfo->eventNumber()<<" run="<<m_eventInfo->runNumber()<<" mu="<<m_eventInfo->averageInteractionsPerCrossing()<<"  bcid="<<bcid);

  ///////////////////////////////////////////
  ////Retrieve the Vertex 
  /////////////////////////////////////////////
  const xAOD::VertexContainer * primVtxCont = 0;
  nvtx = 0;
  vtx_x=0.;
  vtx_y=0.;
  vtx_z=0.;

  if ( ! evtStore()->retrieve(primVtxCont, m_VertexCollection).isSuccess() ) {
    ATH_MSG_ERROR ("Failed to retrieve primary vertices collection. ");
    //return StatusCode::SUCCESS;//just skip event
  }else{
    const xAOD::Vertex * primVtx = 0;
    for (const xAOD::Vertex * vtx : *primVtxCont) {
      if ( vtx->nTrackParticles() >= 2 ) nvtx++;
      if ( vtx->vertexType() == xAOD::VxType::PriVtx ) primVtx = vtx;
    }
    if ( ! primVtx ) {
      ATH_MSG_ERROR ("Failed to retrieve primary vertex ");
      //return StatusCode::SUCCESS;
    }else {
      //Print the Vertex info
      ATH_MSG_DEBUG ("Vertex: "<< nvtx << " "<<primVtx->x()<<" "<<primVtx->y()<<" "<<primVtx->z());
      
      ///Fill tuple
      vtx_x=primVtx->x();
      vtx_y=primVtx->y();
      vtx_z=primVtx->z();
    }
  }

  ///////////  The truth vertex
  const xAOD::TruthVertexContainer * trueVtxCont = 0;
  if ( ! evtStore()->retrieve(trueVtxCont,"TruthVertices").isSuccess() ) {
    ATH_MSG_ERROR ("Failed to retrieve TruthVertices collection. ");
    return StatusCode::SUCCESS;
  } 
  if(trueVtxCont->size()>0){
    truevtx_t=(*trueVtxCont)[0]->t()/speed_light;
    truevtx_x=(*trueVtxCont)[0]->x();
    truevtx_y=(*trueVtxCont)[0]->y();
    truevtx_z=(*trueVtxCont)[0]->z();
  }



  return StatusCode::SUCCESS;
}




StatusCode LArHitTupleDetGeo::processTracks(){
  
  const xAOD::TrackParticleContainer * trackCont = 0;
  if ( ! evtStore()->retrieve(trackCont,"InDetTrackParticles").isSuccess() ) {
    ATH_MSG_ERROR ("Failed to retrieve InDetTrackParticles collection. ");
    return StatusCode::SUCCESS;
  } 

  ATH_MSG_DEBUG("Track collection: "<<trackCont->size());
  ntrk=0;
  for( xAOD::TrackParticleContainer::const_iterator iter = trackCont->begin(); iter!= trackCont->end() && ntrk < MAXTRKS; ++iter ) {
    if(fabs((*iter)->eta())<2.3) continue;
    if((*iter)->pt()<1000) continue;    ///pt>400MeV, d0<10mm , z0<25cm already applied on xAOD tracks
    //if(fabs((*iter)->z0()-truevtx_z)>3) continue; //vertex association
    if(fabs((*iter)->z0()-truevtx_z)>FTrackZ0Cut->Eval(fabs((*iter)->eta()))) continue; //vertex association , eta dependent 
    
    trk_charge[ntrk]=(*iter)->charge();
    trk_e[ntrk]=(*iter)->e();
    trk_pt[ntrk]=(*iter)->pt();
    trk_eta[ntrk]=(*iter)->eta();
    trk_phi[ntrk]=(*iter)->phi();
    trk_d0[ntrk]=(*iter)->d0();
    trk_z0[ntrk]=(*iter)->z0();


    /////////////////////////////////////////////////////
    ////////////Track extrapolation to HGTD
    /////////////////////////////////////////////////////
    trk_ext_eta[ntrk]=0;
    trk_ext_phi[ntrk]=0;
    trk_ext_x[ntrk]=0;
    trk_ext_y[ntrk]=0;
    trk_ext_z[ntrk]=0;    
    trk_ext_peta[ntrk]=0;
    trk_ext_pphi[ntrk]=0;

    trk_ext_dx[ntrk]=0;
    trk_ext_dy[ntrk]=0;
    trk_ext_dtheta[ntrk]=0;
    trk_ext_dphi[ntrk]=0;

    for(int l=0;l<4;l++){
      trk_hgtd_x[l][ntrk]=0;
      trk_hgtd_y[l][ntrk]=0;
    }

    const Trk::TrackParameters* trackParameters_atCaloEntrance(0);

    if(m_RunTrkExtraPol==1){
      ///>extrapolate(Trk::Track,... uses the closest parameters to destination
      const Trk::Track* TRK=(*iter)->track();
      if ((*iter)->eta() > 1.35 && TRK)
      	trackParameters_atCaloEntrance = dynamic_cast< const Trk::TrackParameters * >(m_extrapolator->extrapolate(*TRK,*m_discSurface_atCaloEntrance_positiveZ,Trk::alongMomentum, true, Trk::pion));
      else if ((*iter)->eta() < -1.35 && TRK)
      	trackParameters_atCaloEntrance = dynamic_cast< const Trk::TrackParameters * >(m_extrapolator->extrapolate(*TRK,*m_discSurface_atCaloEntrance_negativeZ,Trk::alongMomentum, true, Trk::pion));
    }
      
    if(m_RunTrkExtraPol==2){
      //xAODTrackParticle only has track parameters at beam-spot
      if ((*iter)->eta() > 1.35)
	trackParameters_atCaloEntrance = dynamic_cast< const Trk::TrackParameters * >(m_extrapolator->extrapolate(*(*iter),*m_discSurface_atCaloEntrance_positiveZ,Trk::alongMomentum, true, Trk::pion));
      else if ((*iter)->eta() < -1.35)
	trackParameters_atCaloEntrance = dynamic_cast< const Trk::TrackParameters * >(m_extrapolator->extrapolate(*(*iter),*m_discSurface_atCaloEntrance_negativeZ,Trk::alongMomentum, true, Trk::pion));
    }

    if (trackParameters_atCaloEntrance!=0) {
      ///See documentation: Tracking/TrkEvent/TrkParametersBase/TrkParametersBase/ParametersBase.h
      trk_ext_eta[ntrk]=trackParameters_atCaloEntrance->position().eta();
      trk_ext_phi[ntrk]=trackParameters_atCaloEntrance->position().phi();
      trk_ext_x[ntrk]=trackParameters_atCaloEntrance->position().x();
      trk_ext_y[ntrk]=trackParameters_atCaloEntrance->position().y();
      trk_ext_z[ntrk]=trackParameters_atCaloEntrance->position().z();
      trk_ext_peta[ntrk]=trackParameters_atCaloEntrance->momentum().eta();
      trk_ext_pphi[ntrk]=trackParameters_atCaloEntrance->momentum().phi();
	
      ////uncertainties
      const AmgSymMatrix(5)* cov=trackParameters_atCaloEntrance->covariance();
      trk_ext_dx[ntrk]=sqrt((*cov)(1,1));
      trk_ext_dy[ntrk]=sqrt((*cov)(2,2));
      trk_ext_dtheta[ntrk]=sqrt((*cov)(3,3));
      trk_ext_dphi[ntrk]=sqrt((*cov)(4,4));
      //ATH_MSG_INFO (" extrapolation dx= "<<trk_ext_dx[ntrk]<<", dy="<<trk_ext_dy[ntrk]);

	
      /// Calculate positions on all layers
      Amg::Vector3D POS(trackParameters_atCaloEntrance->position());
      Amg::Vector3D DIR(trackParameters_atCaloEntrance->momentum()/trackParameters_atCaloEntrance->momentum().mag());

      float d1 = 0.;
      float d2 = 0.;
      float d3 = 0.;
      // if(m_geometry.compare("Preshower")==0){
      // 	d1 = (ZPOSL1_Preshower - ZPOSL0_Preshower) / DIR.z();
      // 	d2 = (ZPOSL2_Preshower - ZPOSL0_Preshower) / DIR.z();
      // 	d3 = (ZPOSL3_Preshower - ZPOSL0_Preshower) / DIR.z();
      // }
      // if(m_geometry.compare("Timing")==0){
      // 	d1 = (ZPOSL1_Timing - ZPOSL0_Timing) / DIR.z();
      // 	d2 = (ZPOSL2_Timing - ZPOSL0_Timing) / DIR.z();
      // 	d3 = (ZPOSL3_Timing - ZPOSL0_Timing) / DIR.z();
      // }

      d1 = (ZPOSL[1] - ZPOSL[0]) / DIR.z();
      d2 = (ZPOSL[2] - ZPOSL[0]) / DIR.z();
      d3 = (ZPOSL[3] - ZPOSL[0]) / DIR.z();
	
      trk_hgtd_x[0][ntrk]=POS.x();
      trk_hgtd_y[0][ntrk]=POS.y();

      trk_hgtd_x[1][ntrk]=(POS+d1*DIR).x();
      trk_hgtd_y[1][ntrk]=(POS+d1*DIR).y();

      trk_hgtd_x[2][ntrk]=(POS+d2*DIR).x();
      trk_hgtd_y[2][ntrk]=(POS+d2*DIR).y();

      trk_hgtd_x[3][ntrk]=(POS+d3*DIR).x();
      trk_hgtd_y[3][ntrk]=(POS+d3*DIR).y();

      delete trackParameters_atCaloEntrance;
    }    
 


    //////////////////////////////
    ///Truth info
    ////////////////////////////
    // code from here: https://svnweb.cern.ch/trac/atlasoff/browser/InnerDetector/InDetValidation/InDetPhysValMonitoring/trunk/src/CachedGetAssocTruth.cxx
    trk_truth_pdg[ntrk]=0;
    trk_truth_pt[ntrk]=0;
    trk_truth_eta[ntrk]=0;
    trk_truth_phi[ntrk]=0;

    trk_truth_vtxt[ntrk]=0;
    trk_truth_vtxz[ntrk]=0;

    typedef ElementLink<xAOD::TruthParticleContainer> ElementTruthLink_t;
    const xAOD::TruthParticle* result(nullptr);
    if ((*iter)->isAvailable<ElementTruthLink_t>("truthParticleLink")) {    // 0. is there any truth?
      const ElementTruthLink_t ptruthContainer = (*iter)->auxdata<ElementTruthLink_t>("truthParticleLink");      // 1. ..then get link
      if (ptruthContainer.isValid()){
	result = *ptruthContainer;

	trk_truth_pdg[ntrk]=result->pdgId();
	trk_truth_pt[ntrk]=result->pt();
	trk_truth_eta[ntrk]=result->eta();
	trk_truth_phi[ntrk]=result->phi();

	//For vertex look for the status 3 parent, 
	// otherwise getting truth match to electrons
	const xAOD::TruthParticle* parent(result);
	if(parent->status()!=3&&parent->nParents()>0){
	  parent=parent->parent(0);//try 1 up
	  if(parent->status()!=3&&parent->nParents()>0)
	    parent=parent->parent(0);//try 2 up
	}

	if(parent){
	  if(parent->hasProdVtx()){
	    //for some tracks the vertex is not found ?? (need to be removed later)
	    trk_truth_vtxt[ntrk]=parent->prodVtx()->t()/speed_light;
	    trk_truth_vtxz[ntrk]=parent->prodVtx()->z();

	    trk_truetr[ntrk] = trk_truth_vtxt[ntrk] + sqrt(trk_ext_x[ntrk]*trk_ext_x[ntrk] + trk_ext_y[ntrk]*trk_ext_y[ntrk] + pow(trk_ext_z[ntrk]-trk_truth_vtxz[ntrk],2)) / speed_light  ;

	  }
	}
	
      }
    }




    ntrk++;
  }


  return StatusCode::SUCCESS;
}


StatusCode LArHitTupleDetGeo::processTruthJets(){
  //must be called after processTruth as some jets are labeled below

  ntruthjet=0;
 
  const xAOD::JetContainer * m_TruthJets = 0;
  if( evtStore()->retrieve( m_TruthJets, "AntiKt4TruthJets" ).isSuccess() ) {
    //ATH_MSG_ERROR ("Failed to retrieve truth jet container. Exiting.");

    ATH_MSG_DEBUG ("Saving Truth Jets =  "<<m_TruthJets->size()); 
    for( xAOD::JetContainer::const_iterator truth_iter = m_TruthJets->begin(); truth_iter != m_TruthJets->end() ; ++truth_iter ){
      if((*truth_iter)->pt()<4000) continue;
      if(fabs((*truth_iter)->eta())<2.0 || fabs((*truth_iter)->eta())>5.0) continue;
      // ATH_MSG_DEBUG (" truth jet: "<<(*truth_iter)->pt()<<" "<<(*truth_iter)->eta()<<" "<<(*truth_iter)->phi()); 
    
      truthjet_vtx[ntruthjet]=0;

      truthjet_pt[ntruthjet]=(*truth_iter)->pt();
      truthjet_eta[ntruthjet]=(*truth_iter)->eta();
      truthjet_phi[ntruthjet]=(*truth_iter)->phi();
      truthjet_E[ntruthjet]=(*truth_iter)->e();

      ntruthjet++;
    }

  }


  //////////////////////////////////////////////////////////
  /// Now build the truth jets from pu vertices
  ////////////////////////////////////////////////////////
  fastjet::JetDefinition jet_def(fastjet::antikt_algorithm,0.4);
  const xAOD::TruthPileupEventContainer* tpec = NULL;
  if (evtStore()->retrieve( tpec , "TruthPileupEvents" ).isSuccess() ) {
    ATH_MSG_DEBUG ("Saving Truth Jets =  "<<m_TruthJets->size()); 
	
    int puvtx=1;//note defined from 1 to leave 0 for h.s.
    for(const auto& tpe : *tpec) {

      std::vector<fastjet::PseudoJet> input_particles;
      for (unsigned int i = 0; i < tpe->nTruthParticles(); i++) {
	const xAOD::TruthParticle *part = static_cast<const xAOD::TruthParticle*>(tpe->truthParticle(i));
	if (!includeParticle(part)) continue;
	input_particles.push_back(fastjet::PseudoJet(part->p4().Px(),part->p4().Py(),part->p4().Pz(),part->e()));
      }
      fastjet::ClusterSequence clust_seq(input_particles,jet_def);
      std::vector<fastjet::PseudoJet> inclusive_jets = sorted_by_pt(clust_seq.inclusive_jets(0));

      for (unsigned int i = 0; i < inclusive_jets.size(); i++) {
	if(inclusive_jets[i].perp()<10000) continue; //only save jets above some pT (number of truth jets is large at low pT)
	if(fabs(inclusive_jets[i].rap())<2.0 || fabs(inclusive_jets[i].rap())>5.0) continue;

	truthjet_vtx[ntruthjet]=puvtx;
	truthjet_pt[ntruthjet]=inclusive_jets[i].perp();
	truthjet_eta[ntruthjet]=inclusive_jets[i].rap();
	truthjet_phi[ntruthjet]=inclusive_jets[i].phi();
	truthjet_E[ntruthjet]=inclusive_jets[i].e();

	ntruthjet++;
      }

      puvtx++;
    }

  }

  
  ///////////////////////////////////////////
  /// match to the h.s. partons
  ///////////////////////////////////////////
  for(int j=0;j<ntruthjet;j++){
    truthjet_truth[j]=-1;

    ///VBF
    if(truth_VBFJ1 !=-1 && deltaR(truthjet_eta[j],truthjet_phi[j],truth_eta[truth_VBFJ1],truth_phi[truth_VBFJ1])<0.4)
      truthjet_truth[j]=truth_VBFJ1;
    if(truth_VBFJ2 !=-1 && deltaR(truthjet_eta[j],truthjet_phi[j],truth_eta[truth_VBFJ2],truth_phi[truth_VBFJ2])<0.4)
      truthjet_truth[j]=truth_VBFJ2;
    if(truth_Gluon !=-1 && deltaR(truthjet_eta[j],truthjet_phi[j],truth_eta[truth_Gluon],truth_phi[truth_Gluon])<0.4)
      truthjet_truth[j]=truth_Gluon;

    ///H->bb
    if(truth_HiggsB !=-1 && deltaR(truthjet_eta[j],truthjet_phi[j],truth_eta[truth_HiggsB],truth_phi[truth_HiggsB])<0.4)
      truthjet_truth[j]=truth_HiggsB;
    if(truth_HiggsBbar !=-1 && deltaR(truthjet_eta[j],truthjet_phi[j],truth_eta[truth_HiggsBbar],truth_phi[truth_HiggsBbar])<0.4)
      truthjet_truth[j]=truth_HiggsBbar;

    //Z->ee
    if(truth_Lp !=-1 && deltaR(truthjet_eta[j],truthjet_phi[j],truth_eta[truth_Lp],truth_phi[truth_Lp])<0.2)
      truthjet_truth[j]=truth_Lp;
    if(truth_Lm !=-1 && deltaR(truthjet_eta[j],truthjet_phi[j],truth_eta[truth_Lm],truth_phi[truth_Lm])<0.2)
      truthjet_truth[j]=truth_Lm;
    

  }


  return StatusCode::SUCCESS;
}

StatusCode LArHitTupleDetGeo::processJets(){

  const xAOD::JetContainer * m_Jets = 0;
  if( !evtStore()->retrieve( m_Jets,  m_JetCollection ).isSuccess() ) {
    ATH_MSG_ERROR ("Failed to retrieve jet container. Exiting.");
    return StatusCode::FAILURE;
  }

  njet = 0;
  nclus = 0;
  for( xAOD::JetContainer::const_iterator iter = m_Jets->begin();iter!= m_Jets->end() && njet < MAXJETS; ++iter ) {
    
    ///Make a calibrated jet
    xAOD::Jet * jet = 0; //*iter ;
    m_jetCalibration->calibratedCopy(*(*iter),jet); //make a calibrated copy, assuming a copy hasn't been made already, alternative is:

    int OriginCorrected = 0; 
    if ( !(*iter)->getAttribute<int>("OriginCorrected",OriginCorrected) )
      OriginCorrected=0;

    int PileupCorrected = 0;
    if ( !(*iter)->getAttribute<int>("PileupCorrected",PileupCorrected) )
      PileupCorrected=0;


    //Apply a jet pT cut
    if(fabs(jet->eta()) < m_JetMinEta) continue;
    if(jet->pt() < m_JetPtCut) continue;
    if(jet->pt() > m_JetPtCutMax) continue;


    ATH_MSG_DEBUG ("Uncalib flags: OriginCorrected="<<OriginCorrected<<"  PileupCorrected="<<PileupCorrected);    
    ATH_MSG_DEBUG ("Uncalib Jet: "<<(*iter)->pt()<<" "<<(*iter)->eta()<<" "<<(*iter)->phi());    
    ATH_MSG_DEBUG ("Calib   Jet: "<<jet->pt()<<" "<<jet->eta()<<" "<<jet->phi());    


    /////////////////////////
    //////save the jet clusters 
    //////////////////////////
    static SG::AuxElement::Accessor< std::vector< float> > constituentWeightAcc( "constituentWeights" );
    xAOD::JetConstituentVector clusters=jet->getConstituents();
    //ATH_MSG_DEBUG (" jet constituents [ "<< clusters.size()<<" ]: ");
    xAOD::JetConstituentVector::iterator cluster = clusters.begin();
    xAOD::JetConstituentVector::iterator clusters_e = clusters.end();

    int jetnclus=0;
    for(;cluster != clusters_e ; cluster++){
      if(nclus>=MAXCLUS){
	ATH_MSG_DEBUG (" number of clusters is at max "<<MAXCLUS);
	break;
      }
      //ATH_MSG_DEBUG ("  "<<cluster->e()<<" , "<<cluster->eta() <<" , "<<cluster->phi());
      clus_pt[nclus]=cluster->pt();
      clus_eta[nclus]=cluster->eta();
      clus_phi[nclus]=cluster->phi();
      clus_E[nclus]=cluster->e();
      clus_w[nclus]=constituentWeightAcc(*jet)[jetnclus];
      clus_jet[nclus]=njet;
      clus_jetdR[nclus]=deltaR(cluster->eta(),cluster->phi(),jet->eta(),jet->phi());
      
      const xAOD::CaloCluster * CaloCluster = dynamic_cast<const xAOD::CaloCluster*>( cluster->rawConstituent() );
      clus_lambda[nclus]=CaloCluster->getMomentValue(xAOD::CaloCluster::MomentType::CENTER_LAMBDA);
      clus_emprob[nele] = CaloCluster->getMomentValue(xAOD::CaloCluster::MomentType::EM_PROBABILITY);
      clus_emfrac[nele] = CaloCluster->getMomentValue(xAOD::CaloCluster::MomentType::ENG_FRAC_EM);
      clus_sig[nele] = CaloCluster->getMomentValue(xAOD::CaloCluster::MomentType::SIGNIFICANCE);

      nclus++;
      jetnclus++;
    }
    

    
    ///////////////////
    //fill the nominal info
    ///////////////////
    jet_pt[njet]=jet->pt();
    jet_eta[njet]=jet->eta();
    jet_phi[njet]=jet->phi();
    jet_m[njet]=jet->m();
    jet_E[njet]=jet->e();
    jet_nclus[njet]=jetnclus;

    ///////////////////
    //fill uncalibrated info
    ///////////////////
    jet_uncalib_pt[njet]=(*iter)->pt();
    jet_uncalib_eta[njet]=(*iter)->eta();
    jet_uncalib_phi[njet]=(*iter)->phi();
    jet_uncalib_m[njet]=(*iter)->m();
    jet_uncalib_E[njet]=(*iter)->e();
    
    /////////////////////////////////
    ///fill the truth info
    ///////////////////////////////
    jet_truthJ[njet]=-1;
    jet_truthjhs_dr3pt10[njet]=-1;
    jet_truthjhs_dr3pt4[njet]=-1;
    jet_truthjpu_dr3pt10[njet]=-1;
    jet_truthjpu_dr6pt10[njet]=-1;
    for(int jt=0;jt<ntruthjet;jt++){
      if( truthjet_vtx[jt]==0 && truthjet_pt[jt] > 10000 && deltaR(jet_eta[njet],jet_phi[njet],truthjet_eta[jt],truthjet_phi[jt]) < JETMATCHDR )
	if(jet_truthjhs_dr3pt10[njet]==-1 || truthjet_pt[jet_truthjhs_dr3pt10[njet]]<truthjet_pt[jt] ) 
	  jet_truthjhs_dr3pt10[njet]=jt;
      if( truthjet_vtx[jt]==0 && truthjet_pt[jt] > 4000 && deltaR(jet_eta[njet],jet_phi[njet],truthjet_eta[jt],truthjet_phi[jt]) < JETMATCHDR )
	if(jet_truthjhs_dr3pt4[njet]==-1 || truthjet_pt[jet_truthjhs_dr3pt4[njet]]<truthjet_pt[jt] ) 
	  jet_truthjhs_dr3pt4[njet]=jt;
      if( truthjet_vtx[jt]>0 && truthjet_pt[jt] > 10000 && deltaR(jet_eta[njet],jet_phi[njet],truthjet_eta[jt],truthjet_phi[jt]) < JETMATCHDR )
	if(jet_truthjpu_dr3pt10[njet]==-1 || truthjet_pt[jet_truthjpu_dr3pt10[njet]]<truthjet_pt[jt] ) 
	  jet_truthjpu_dr3pt10[njet]=jt;
      if( truthjet_vtx[jt]>0 && truthjet_pt[jt] > 10000 && deltaR(jet_eta[njet],jet_phi[njet],truthjet_eta[jt],truthjet_phi[jt]) < 0.6 )
	if(jet_truthjpu_dr6pt10[njet]==-1 || truthjet_pt[jet_truthjpu_dr6pt10[njet]]<truthjet_pt[jt] ) 
	  jet_truthjpu_dr6pt10[njet]=jt;
      
    }




    delete jet;
    njet++;
  }


  ////////////////////////////
  //////Counter nearby jets
  ////////////////////////////
  for(int j=0;j<njet;j++){
    jet_njetnear[j]=0;
    jet_nearjetdR[j]=0;
    jet_nearjet[j]=-1;

    for(int j2=0;j2<njet;j2++){
      if(j2==j) continue;

      float dr=deltaR(jet_eta[j2],jet_phi[j2],jet_eta[j],jet_phi[j]);

      if(dr<0.8) jet_njetnear[j]++;
      
      if(jet_nearjet[j]==-1 || dr<jet_nearjetdR[j]){
	jet_nearjet[j]=j2;
	jet_nearjetdR[j]=dr;
      }

    }

  }


  return StatusCode::SUCCESS;
}



StatusCode LArHitTupleDetGeo::processMET(){

  const xAOD::MissingETContainer* m_MissingET = 0;
  if ( !evtStore()->retrieve(m_MissingET,m_METCollection).isSuccess() ) {
    ATH_MSG_ERROR ("Failed to retrieve missing ET container . Exiting."); 
    return StatusCode::FAILURE;
  }    
  
  xAOD::MissingETContainer::const_iterator met_it = m_MissingET->find(m_METSoftTerm);  
  if(met_it == m_MissingET->end()){
    ATH_MSG_DEBUG ("Failed to retrieve missing ET.");
    ATH_MSG_DEBUG ("Size of MET collection = "<<m_MissingET->size()); 

    //ATH_MSG_ERROR ("Exiting."); 
    //return StatusCode::FAILURE;
  }else{
    //fill tuple
    sumet = (*met_it)->sumet();
    met = (*met_it)->met();
    met_phi = (*met_it)->phi();
    ATH_MSG_DEBUG ("MET "<<sumet<<" "<<met<<" "<<met_phi);
  }

  return StatusCode::SUCCESS;
}



StatusCode LArHitTupleDetGeo::processElectrons(){

  const xAOD::ElectronContainer * m_Electrons = 0;
  if( !evtStore()->retrieve( m_Electrons,  m_ElectronCollection ).isSuccess() ) {
    ATH_MSG_ERROR ("Failed to retrieve  electron container. Exiting.");
    return StatusCode::FAILURE;
  }



  int maxLm = -1; 
  int maxLp = -1; 
  float drm = 10000;
  float drp = 10000;
  float maxdrm=10000;
  float maxdrp=10000;

  nele = 0;
  for( xAOD::ElectronContainer::const_iterator iter = m_Electrons->begin(); iter!= m_Electrons->end() && nele < MAXELES; ++iter ) {
    
    /// Make a calibrated ele
    const xAOD::Electron * ele = *iter ;

    /// apply basic selections 
    if(fabs(ele->eta()) < m_EleMinEta) continue;


    ///////////////////
    //fill the nominal info
    ///////////////////
    ele_pt[nele]=ele->pt();
    ele_eta[nele]=ele->eta();
    ele_phi[nele]=ele->phi();
    ele_m[nele]=ele->m();
    ele_E[nele]=ele->e();
    ele_truth[nele] = -1;    
   
    
    // ///truth match
    // if(ele_truth[nele] ==-1 && truth_Z_Lm != -1 
    //    && deltaR(ele_eta[nele],ele_phi[nele],truth_eta[truth_Z_Lm],truth_phi[truth_Z_Lm])<ELEMATCHDR){
    //   ele_truth[nele] = truth_Z_Lm; 
    // }
    // if(ele_truth[nele] ==-1 && truth_Z_Lp != -1 
    //    && deltaR(ele_eta[nele],ele_phi[nele],truth_eta[truth_Z_Lp],truth_phi[truth_Z_Lp])<ELEMATCHDR){
    //   ele_truth[nele] = truth_Z_Lp;     
    // }
    
    
    ///truth match
    drm = deltaR(truth_eta[truth_Z_Lm], truth_phi[truth_Z_Lm], ele_eta[nele], ele_phi[nele]);
    if( drm < maxdrm ){
      maxdrm = drm;
      maxLm = nele; 
    }
    drp = deltaR(truth_eta[truth_Z_Lp], truth_phi[truth_Z_Lp], ele_eta[nele], ele_phi[nele]);
    if( drp < maxdrp ){
      maxdrp = drp;
      maxLp = nele;  
    }



    //cluster info
    ele_clus_pt[nele] = -1;
    ele_clus_phi[nele] = -999;
    ele_clus_eta[nele] = -999;
    ele_clus_E[nele] = -1;
    const xAOD::CaloCluster* ele_cluster = ele->caloCluster();
    if (ele_cluster) {
      ele_clus_pt[nele] = ele_cluster->pt();
      ele_clus_eta[nele] = ele_cluster->eta();
      ele_clus_phi[nele] = ele_cluster->phi();
      ele_clus_E[nele] = ele_cluster->e(); 
      ele_clus_lambda[nele] = ele_cluster->getMomentValue(xAOD::CaloCluster::MomentType::CENTER_LAMBDA);
      ele_clus_emprob[nele] = ele_cluster->getMomentValue(xAOD::CaloCluster::MomentType::EM_PROBABILITY);
      ele_clus_emfrac[nele] = ele_cluster->getMomentValue(xAOD::CaloCluster::MomentType::ENG_FRAC_EM);
    }else {
      ATH_MSG_ERROR("caloCluster for electron not found"); 
      continue; 
    }
    
    
    nele++;
  }


  if( maxdrm < ELEMATCHDR ){
    ele_truth[maxLm]=truth_Z_Lm;
  }
  if( maxdrp < ELEMATCHDR ){
    ele_truth[maxLp]=truth_Z_Lp;
  }
    

  return StatusCode::SUCCESS;
}



StatusCode LArHitTupleDetGeo::processCells(){

  ///cells must be reset every time
  resetCells();

  ///get the hits
  const DataHandle<LArHitContainer> p_collection ;
  StatusCode sc =  StoreGate::instance().retrieve( p_collection,m_HGTDHitCollection.c_str()) ;
  if (sc.isFailure() || !p_collection) {
    ATH_MSG_DEBUG ("Failed to retrieve collection : "<<m_HGTDHitCollection.c_str());
    return StatusCode::FAILURE;
  } 

  ///Get the Identifier
  StoreGateSvc *detStore = StoreGate::pointer("DetectorStore");
  const DataHandle<CaloIdManager> caloIdMgr;
  if ((detStore->retrieve(caloIdMgr)).isFailure()) {
    ATH_MSG_ERROR(" Unable to retrieve CaloIdManager from DetectoreStore");
    return StatusCode::FAILURE;
  }   
  const HGTD_ID *    ID       = caloIdMgr->getHGTD_ID();//CaloCell->hgtd_idHelper();


  ///fill the cells
  for (LArHitContainer::const_iterator hi= p_collection->begin(); hi!=p_collection->end() ; hi++){
    //ATH_MSG_DEBUG ("LArHitRead::execute  cellID:" << (*hi)->cellID()  << "  energy:" << (*hi)->energy() << " time:" << (*hi)->time());

    int cellS=ID->barrel_ec((*hi)->cellID()) * ( ID->sampling((*hi)->cellID())  + 1);
    if( cellS==0 || abs(cellS) > 4 ){
      ATH_MSG_ERROR ("Failed to determine HGTD layer for hit "); 
      continue;
    }

    float hitE=(*hi)->energy();
    float hitt=(*hi)->time()+11.8;
    float hity=0.5*ID->y_index((*hi)->cellID());


    ///remove hits which are set to the overflow
    if(hitt>14.4) continue;

    ///hits are saved in local coordinates ( negative z hits have a rotation around Y-axis), so x -> -x
    float hitx=0.5*ID->x_index((*hi)->cellID())*(cellS<0?-1:1);     
    
    //remove the dead zones
    if( m_deadZoneType==1){ if(!passDeadZone(hitx,hity)) continue;}
    else if( m_deadZoneType==2){ if(!passDeadZoneStripStandard(hitx,hity)) continue;}
    else  ATH_MSG_ERROR ("WRONG deadZoneType"); 

    ///fill cells
    if(fabs(hitx)<HGTDHEIGHTIN&&fabs(hity)<HGTDHEIGHTIN){//inner cells
      int cellidx =  ((cellS +  ( cellS<0 ? 4 : 3 ) ) * NCELLXY1 * NCELLXY1) + int((hitx+HGTDHEIGHTIN)/m_CellSize1) * NCELLXY1 + int((hity+HGTDHEIGHTIN)/m_CellSize1);
      cell1_nhit[cellidx]++;
      cell1_E[cellidx] += hitE;
      cell1_t[cellidx] += hitE*hitt; 
    }else{//outer cells
      int cellidx =  ((cellS +  ( cellS<0 ? 4 : 3 ) ) * NCELLXY2 * NCELLXY2) + int((hitx+HGTDHEIGHTOUT)/m_CellSize2) * NCELLXY2 + int((hity+HGTDHEIGHTOUT)/m_CellSize2);
      cell2_nhit[cellidx]++;
      cell2_E[cellidx] += hitE;
      cell2_t[cellidx] += hitE*hitt;
    }
  }




  /////////////////////////////////////////////////////////
  ///compress cell collection because it takes a lot of memory
  /////////////////////////////////////////////////////////
  ncell=0;

  ///fill the inner cells
  for(int c=0;c<NARRAYCELL1;c++){    
    if(cell1_E[c]>m_CellMinEnergy){
      cell_type[ncell] = 1;
      cell_x[ncell] = cell1_x[c];
      cell_y[ncell] = cell1_y[c];
      cell_z[ncell] = cell1_z[c];
      cell_S[ncell] = cell1_S[c];
      cell_r[ncell] = cell1_r[c];
      cell_eta[ncell] = cell1_eta[c];
      cell_phi[ncell] = cell1_phi[c];
      cell_E[ncell] = cell1_E[c];
      cell_nhit[ncell] = cell1_nhit[c];
      cell_tr[ncell] = cell1_t[c]/cell1_E[c];
      cell_t[ncell] = cell1_t[c]/cell1_E[c] + gRandom->Gaus(0,m_HitTimeSmear1); 

      cell_trkidx[ncell] = cell1_trkidx[c] ;
      cell_hx[ncell] = cell1_hx[c];
      cell_hy[ncell] = cell1_hy[c];

      ncell++;
    }
  }


  //fill the outer cells
  for(int c=0;c<NARRAYCELL2;c++){    
    if(cell2_E[c]>m_CellMinEnergy){
      cell_type[ncell] = 2;
      cell_x[ncell] = cell2_x[c];
      cell_y[ncell] = cell2_y[c];
      cell_z[ncell] = cell2_z[c];
      cell_S[ncell] = cell2_S[c];
      cell_r[ncell] = cell2_r[c];
      cell_eta[ncell] = cell2_eta[c];
      cell_phi[ncell] = cell2_phi[c];
      cell_E[ncell] = cell2_E[c];
      cell_nhit[ncell] = cell2_nhit[c];
      cell_tr[ncell] = cell2_t[c]/cell2_E[c];
      cell_t[ncell] = cell2_t[c]/cell2_E[c] + gRandom->Gaus(0,m_HitTimeSmear2); 

      cell_trkidx[ncell] = cell2_trkidx[c] ;
      cell_hx[ncell] = cell2_hx[c];
      cell_hy[ncell] = cell2_hy[c];

      ncell++;
    }
  }


  //here fill the quantities which are computed from the cell own variables
  ncellL0=0;
  ncellL1=0;
  ncellL2=0;
  ncellL3=0;
  for(int c=0;c<ncell;c++){
    ///choose the distance from the vertex to the cell
    cell_DZ0[c] =   sqrt(pow(cell_x[c],2)+pow(cell_y[c],2)+pow(cell_z[c],2)); //distance from 0,0,0
    cell_Dreco[c] = sqrt(pow(cell_x[c]-vtx_x,2)+pow(cell_y[c]-vtx_y,2)+pow(cell_z[c]-vtx_z,2)); //use reco vertex
    cell_trueD[c] = sqrt(pow(cell_x[c]-truevtx_x,2)+pow(cell_y[c]-truevtx_y,2)+pow(cell_z[c]-truevtx_z,2)); //use true vertex
    
    if(m_dTVertex==0)     cell_D[c]=cell_trueD[c];
    else if(m_dTVertex==1)cell_D[c]=cell_DZ0[c];
    else if(m_dTVertex==2)cell_D[c]=cell_Dreco[c];
    else cell_D[c]=0.;
    
    //this is the best place to define deltaT (after smearing)	
    cell_dt[c] = cell_t[c] - cell_D[c]/speed_light;   
    cell_truedt[c] = truevtx_t + (cell_trueD[c]-cell_D[c])/speed_light;   
    cell_dtreco[c] = cell_t[c] - cell_Dreco[c]/speed_light;      //this uses the reconstructed vertex
    cell_dttrue[c] = cell_t[c] - cell_trueD[c]/speed_light;  //this uses the true primary vertex
    cell_dtZ0[c] = cell_t[c] - cell_DZ0[c]/speed_light;      //this uses 0,0,0 as the primary vertex


    ///
    if(abs(cell_S[c])==1) ncellL0++;
    if(abs(cell_S[c])==2) ncellL1++;
    if(abs(cell_S[c])==3) ncellL2++;
    if(abs(cell_S[c])==4) ncellL3++;

  }
  
  return StatusCode::SUCCESS;
}


// void LArHitTupleDetGeo::fillHGTracks(){

//   for(int t=0;t<MAXTRK1;t++){
//     trk1_nhit[t] =0;
//     trk1_t[t]    =0;
//     trk1_tr[t]    =0;
//     trk1_dt[t]   =0;
//     trk1_truedt[t]=0;
//     trk1_E[t]    =0;
//   }
//   for(int t=0;t<MAXTRK2;t++){
//     trk2_nhit[t] =0;
//     trk2_t[t]    =0;
//     trk2_tr[t]    =0;
//     trk2_dt[t]   =0;
//     trk2_truedt[t]=0;
//     trk2_E[t]    =0;
//   }


//   /////Fill the tracks
//   for(int c=0;c<ncell;c++){
//     int trkidx = cell_trkidx[c];

//     ////tracks must be separated by type (inner vs outer) because the trkidx refers to different arrays
//     if(cell_type[c]==1){
//       trk1_nhit[trkidx]++;
//       trk1_t[trkidx] += cell_t[c] ; //avg the time
//       trk1_tr[trkidx] += cell_tr[c] ; //avg the time
//       trk1_dt[trkidx] += cell_dt[c] ; //avg the time
//       trk1_truedt[trkidx] += cell_truedt[c] ; //avg the time
//       trk1_E[trkidx] += cell_E[c] ; //sum the cell E

//       if(trk1_nhit[trkidx]==1){//spatial coordinates use first hit
// 	trk1_x[trkidx] = cell_x[c] ;
// 	trk1_y[trkidx] = cell_y[c] ;
// 	trk1_z[trkidx] = cell_z[c] ;
// 	trk1_r[trkidx] = cell_r[c] ;
// 	trk1_eta[trkidx] = cell_eta[c] ;
// 	trk1_phi[trkidx] = cell_phi[c] ;
//       }
//     }
    

//     if(cell_type[c]==2){
//       trk2_nhit[trkidx]++;
//       trk2_t[trkidx] += cell_t[c] ; //avg the time
//       trk2_tr[trkidx] += cell_tr[c] ; //avg the time
//       trk2_dt[trkidx] += cell_dt[c] ; //avg the time
//       trk2_truedt[trkidx] += cell_truedt[c] ; //avg the time
//       trk2_E[trkidx] += cell_E[c] ; //sum the cell E

//       if(trk2_nhit[trkidx]==1){//spatial coordinates use first hit
// 	trk2_x[trkidx] = cell_x[c] ;
// 	trk2_y[trkidx] = cell_y[c] ;
// 	trk2_z[trkidx] = cell_z[c] ;
// 	trk2_r[trkidx] = cell_r[c] ;
// 	trk2_eta[trkidx] = cell_eta[c] ;
// 	trk2_phi[trkidx] = cell_phi[c] ;
//       }
//     }
//   }

//   //////////////Fill final track collection
//   nhgtrk=0;
//   for(int t=0;t<MAXTRK1;t++){
//     if(trk1_nhit[t]<3)continue;
//     hgtrk_type[nhgtrk]=1;
//     hgtrk_nhit[nhgtrk]=trk1_nhit[t];
//     hgtrk_t[nhgtrk]=trk1_t[t]/trk1_nhit[t];
//     hgtrk_tr[nhgtrk]=trk1_tr[t]/trk1_nhit[t];
//     hgtrk_dt[nhgtrk]=trk1_dt[t]/trk1_nhit[t];
//     hgtrk_truedt[nhgtrk]=trk1_truedt[t]/trk1_nhit[t];
//     hgtrk_E[nhgtrk]=trk1_E[t];
//     hgtrk_x[nhgtrk] = trk1_x[t];
//     hgtrk_y[nhgtrk] = trk1_y[t];
//     hgtrk_z[nhgtrk] = trk1_z[t];
//     hgtrk_r[nhgtrk] = trk1_r[t];
//     hgtrk_eta[nhgtrk] = trk1_eta[t];
//     hgtrk_phi[nhgtrk] = trk1_phi[t];

//     nhgtrk++;
//   }
//   for(int t=0;t<MAXTRK2;t++){
//     if(trk2_nhit[t]<3)continue;
//     hgtrk_type[nhgtrk]=2;
//     hgtrk_nhit[nhgtrk]=trk2_nhit[t];
//     hgtrk_t[nhgtrk]=trk2_t[t]/trk2_nhit[t];
//     hgtrk_tr[nhgtrk]=trk2_tr[t]/trk2_nhit[t];
//     hgtrk_dt[nhgtrk]=trk2_dt[t]/trk2_nhit[t];
//     hgtrk_truedt[nhgtrk]=trk2_truedt[t]/trk2_nhit[t];
//     hgtrk_E[nhgtrk]=trk2_E[t];
//     hgtrk_x[nhgtrk] = trk2_x[t];
//     hgtrk_y[nhgtrk] = trk2_y[t];
//     hgtrk_z[nhgtrk] = trk2_z[t];
//     hgtrk_r[nhgtrk] = trk2_r[t];
//     hgtrk_eta[nhgtrk] = trk2_eta[t];
//     hgtrk_phi[nhgtrk] = trk2_phi[t];

//     nhgtrk++;
//   }

// }




void LArHitTupleDetGeo::fillHGTDTracks(){

  ///any quantity being summed needs reset
  for(int t=0;t<MAXTRK1;t++){//1 mm region
    for(int idx2=0;idx2<9;idx2++){
      hgtrk1_nhit[t][idx2]=0;
      hgtrk1_t[t][idx2]   =0;
      hgtrk1_tr[t][idx2]   =0;
      hgtrk1_dt[t][idx2]  =0;
      hgtrk1_truedt[t][idx2]   =0;
      hgtrk1_E[t][idx2]   =0;
    }
  }


  for(int t=0;t<MAXTRK2;t++){//3mm region
    for(int idx2=0;idx2<9;idx2++){
      hgtrk2_nhit[t][idx2]=0;
      hgtrk2_t[t][idx2]   =0;
      hgtrk2_tr[t][idx2]   =0;
      hgtrk2_dt[t][idx2]  =0;
      hgtrk2_truedt[t][idx2]   =0;
      hgtrk2_E[t][idx2]   =0;
    }
  }


  int ncellxy=0;//depends on cell type
  int trkidx[9][9];//array selecting the tracks to be incremented
  // the primary index values corresponds to the first layer cell id and its neighbors
  // the secondary indices correspond to the 9 tracks for each layer0 cell, values are 0-8

  
  ////loop over hit cells
  for(int c=0;c<ncell;c++){
    if(abs(cell_S[c])>2)  continue; //3rd and 4th layers will used later.

    if(cell_type[c]==1) ncellxy=NCELLXY1;
    if(cell_type[c]==2) ncellxy=NCELLXY2;
      

    //need to determine which tracks will be incremented by this hit     
    if(abs(cell_S[c])==1) { 
      
      for(int idx1=0;idx1<9;idx1++)
	for(int idx2=0;idx2<9;idx2++)
	  if(idx1==4)trkidx[idx1][idx2]=cell_trkidx[c]; //enable all tracks of this cell
	  else trkidx[idx1][idx2]=-1; //all tracks of  neighbor cells disabled 
	

    }else if(abs(cell_S[c])==2){ ///hits on second layer

      /////////////////////
      ///this is the simple algorithm selecting straight tracks
      /////////////////////
      // for(int idx1=0;idx1<9;idx1++)
      // 	for(int idx2=0;idx2<9;idx2++)
      // 	  if(idx1==4&&idx2==4) trkidx[idx1][idx2]=cell_trkidx[c];
      // 	  else trkidx[idx1][idx2]=-1; 


      /////////////////////
      /// this is the algorithm allowing hits on neighboring pads in second layer 
      /////////////////////

      /// basic arrays for layer0 and layer1 and numbering
      // [6][7][8]      [ ][ ][ ]
      // [3][4][5]      [ ][x][ ]
      // [0][1][2]      [ ][ ][ ]

      for(int idx1=0;idx1<9;idx1++)
	for(int idx2=0;idx2<9;idx2++)
	  trkidx[idx1][idx2]=-1; //disable all tracks by default

      ////// example showing how the [0][8] element is determined
      /// first index [0] taken from first matrix, 
      /// second index [8] taken from last matrix
      // [6][7][8]      [ ][ ][ ]     [ ][ ][ ]      [ ][ ][ ]
      // [3][4][5]      [ ][x][ ] ->  [7][8][ ]      [7][8][ ]
      // [0][1][2]      [ ][ ][ ]     [4][5][ ]      [4][5][ ]

      ///track id's need to be recomputed for the neighbor cells (cell_hx +/- 1) 

      if(cell_hx[c]>0&&cell_hy[c]>0) 
	trkidx[0][8]= (cell_S[c]<0 ? 0 : 1 ) * ncellxy * ncellxy +  (cell_hx[c]-1) * ncellxy  +  cell_hy[c]-1;
      if(cell_hy[c]>0) 
	trkidx[1][7]= (cell_S[c]<0 ? 0 : 1 ) * ncellxy * ncellxy +  (cell_hx[c]) * ncellxy  +  cell_hy[c]-1;
      if(cell_hx[c]<ncellxy-1&&cell_hy[c]>0) 
	trkidx[2][6]= (cell_S[c]<0 ? 0 : 1 ) * ncellxy * ncellxy +  (cell_hx[c]+1) * ncellxy  +  cell_hy[c]-1;

      if(cell_hx[c]>0) 
	trkidx[3][5]= (cell_S[c]<0 ? 0 : 1 ) * ncellxy * ncellxy +  (cell_hx[c]-1) * ncellxy  +  cell_hy[c];
      
      trkidx[4][4]= cell_trkidx[c];
      
      if(cell_hx[c]<ncellxy-1) 
	trkidx[5][3]= (cell_S[c]<0 ? 0 : 1 ) * ncellxy * ncellxy +  (cell_hx[c]+1) * ncellxy  +  cell_hy[c];

      if(cell_hx[c]>0&&cell_hy[c]<ncellxy-1) 	
	trkidx[6][2]= (cell_S[c]<0 ? 0 : 1 ) * ncellxy * ncellxy +  (cell_hx[c]-1) * ncellxy  +  cell_hy[c]+1;
      if(cell_hy[c]<ncellxy-1) 	
	trkidx[7][1]= (cell_S[c]<0 ? 0 : 1 ) * ncellxy * ncellxy +  (cell_hx[c]) * ncellxy  +  cell_hy[c]+1;
      if(cell_hx[c]<ncellxy-1&&cell_hy[c]<ncellxy-1) 	
	trkidx[8][0]= (cell_S[c]<0 ? 0 : 1 ) * ncellxy * ncellxy +  (cell_hx[c]+1) * ncellxy  +  cell_hy[c]+1;
      
      
    }


    ////fill the tracks
    for(int idx1=0;idx1<9;idx1++)
      for(int idx2=0;idx2<9;idx2++){
	if(trkidx[idx1][idx2]<0) continue; //skip disabled tracks
	  
	///fill inner tracks
	if(cell_type[c]==1){
      	  hgtrk1_nhit[trkidx[idx1][idx2]][idx2]++;   //increment number of hits

	  hgtrk1_dt[trkidx[idx1][idx2]][idx2] += cell_dt[c] ; //avg the time
	  hgtrk1_truedt[trkidx[idx1][idx2]][idx2] += cell_truedt[c] ; //avg the time
	  hgtrk1_E[trkidx[idx1][idx2]][idx2]  += cell_E[c] ;   //sum the cell E
	  hgtrk1_t[trkidx[idx1][idx2]][idx2]  += cell_t[c] - (ZPOSL[abs(cell_S[c])-1]-ZPOSL[0])/speed_light ;   //must correct for the layer 
	  hgtrk1_tr[trkidx[idx1][idx2]][idx2]  += cell_tr[c] - (ZPOSL[abs(cell_S[c])-1]-ZPOSL[0])/speed_light ;   //must correct for the layer 


	  if(abs(cell_S[c])==1){
	    //spatial coordinates use hit on first layer
	    hgtrk1_x[trkidx[idx1][idx2]][idx2] = cell_x[c] ;
	    hgtrk1_y[trkidx[idx1][idx2]][idx2] = cell_y[c] ;
	    hgtrk1_z[trkidx[idx1][idx2]][idx2] = cell_z[c] ;
	    hgtrk1_r[trkidx[idx1][idx2]][idx2] = cell_r[c] ;
	    hgtrk1_eta[trkidx[idx1][idx2]][idx2] = cell_eta[c] ;
	    hgtrk1_phi[trkidx[idx1][idx2]][idx2] = cell_phi[c] ;

	    hgtrk1_c1[trkidx[idx1][idx2]][idx2]=c;  
	  }    

	  if(abs(cell_S[c])==2){
	    hgtrk1_c2[trkidx[idx1][idx2]][idx2]=c;   
	  }
	    
	  
	}


	////fill outer tracks
	if(cell_type[c]==2){
      	  hgtrk2_nhit[trkidx[idx1][idx2]][idx2]++;            //increment number of hits
	  hgtrk2_dt[trkidx[idx1][idx2]][idx2] += cell_dt[c] ; //avg the time
	  hgtrk2_truedt[trkidx[idx1][idx2]][idx2] += cell_truedt[c] ; //avg the time
	  hgtrk2_E[trkidx[idx1][idx2]][idx2]  += cell_E[c] ;   //sum the cell E
	  hgtrk2_t[trkidx[idx1][idx2]][idx2]  += cell_t[c] - (ZPOSL[abs(cell_S[c])-1]-ZPOSL[0])/speed_light;  
	  hgtrk2_tr[trkidx[idx1][idx2]][idx2]  += cell_tr[c] - (ZPOSL[abs(cell_S[c])-1]-ZPOSL[0])/speed_light;   //must correct for the layer 

	  if(abs(cell_S[c])==1){
	    //spatial coordinates use hit on first layer
	    hgtrk2_x[trkidx[idx1][idx2]][idx2] = cell_x[c] ;
	    hgtrk2_y[trkidx[idx1][idx2]][idx2] = cell_y[c] ;
	    hgtrk2_z[trkidx[idx1][idx2]][idx2] = cell_z[c] ;
	    hgtrk2_r[trkidx[idx1][idx2]][idx2] = cell_r[c] ;
	    hgtrk2_eta[trkidx[idx1][idx2]][idx2] = cell_eta[c] ;
	    hgtrk2_phi[trkidx[idx1][idx2]][idx2] = cell_phi[c] ;

	    hgtrk2_c1[trkidx[idx1][idx2]][idx2]=c;  
	  }   

	  if(abs(cell_S[c])==2){
	    hgtrk2_c2[trkidx[idx1][idx2]][idx2]=c;   
	  } 
	  
	}

      }
    
  }



  /////////////////////////////////
  /// Select only real tracks and fill final collection
  /////////////////////////////////
  nhgtrk=0;
  for(int t=0;t<MAXTRK1;t++){///Inner tracks

    int selidx2=-1;
    for(int idx2=0;idx2<9;idx2++){
      if(hgtrk1_nhit[t][idx2]<2) continue; //remove empty tracks 
      if(fabs(cell_t[hgtrk1_c2[t][idx2]]-cell_t[hgtrk1_c1[t][idx2]]) > 3*m_HitTimeSmear1 ) continue; // remove tracks with inconsistent times

      if(selidx2==-1)
	selidx2=idx2;
      else if(hgtrk1_E[t][idx2]>hgtrk1_E[t][selidx2]) //select track with highest cell energy
	selidx2=idx2;

    }
    if(selidx2==-1) continue;

    hgtrk_type[nhgtrk]   = 1;
    hgtrk_trkidx[nhgtrk] = t;
    hgtrk_idx2[nhgtrk]   = selidx2;
    hgtrk_c1[nhgtrk]     = hgtrk1_c1[t][selidx2];
    hgtrk_c2[nhgtrk]     = hgtrk1_c2[t][selidx2];

    hgtrk_nhit[nhgtrk]   = hgtrk1_nhit[t][selidx2];
    hgtrk_t[nhgtrk]      = hgtrk1_t[t][selidx2];
    hgtrk_tr[nhgtrk]     = hgtrk1_tr[t][selidx2];
    hgtrk_dt[nhgtrk]     = hgtrk1_dt[t][selidx2];
    hgtrk_truedt[nhgtrk] = hgtrk1_truedt[t][selidx2];
    hgtrk_E[nhgtrk]      = hgtrk1_E[t][selidx2];
    hgtrk_x[nhgtrk]      = hgtrk1_x[t][selidx2];
    hgtrk_y[nhgtrk]      = hgtrk1_y[t][selidx2];
    hgtrk_z[nhgtrk]      = hgtrk1_z[t][selidx2];
    hgtrk_r[nhgtrk]      = hgtrk1_r[t][selidx2];
    hgtrk_eta[nhgtrk]    = hgtrk1_eta[t][selidx2];
    hgtrk_phi[nhgtrk]    = hgtrk1_phi[t][selidx2];

    nhgtrk++;
  }


  for(int t=0;t<MAXTRK2;t++){///outer tracks
    
    int selidx2=-1;
    for(int idx2=0;idx2<9;idx2++){
      if(hgtrk2_nhit[t][idx2]<2)continue;
      if(fabs(cell_t[hgtrk2_c2[t][idx2]]-cell_t[hgtrk2_c1[t][idx2]])>3*m_HitTimeSmear2 )continue;
      
      if(selidx2==-1)
	selidx2=idx2;
      else if(hgtrk2_E[t][idx2]>hgtrk2_E[t][selidx2])
	selidx2=idx2;

    }
    if(selidx2==-1) continue;


    hgtrk_type[nhgtrk]  = 2;
    hgtrk_trkidx[nhgtrk]= t;
    hgtrk_idx2[nhgtrk]  = selidx2;
    hgtrk_c1[nhgtrk]    = hgtrk2_c1[t][selidx2];
    hgtrk_c2[nhgtrk]    = hgtrk2_c2[t][selidx2];

    hgtrk_nhit[nhgtrk]  = hgtrk2_nhit[t][selidx2];
    hgtrk_t[nhgtrk]     = hgtrk2_t[t][selidx2];
    hgtrk_tr[nhgtrk]    = hgtrk2_tr[t][selidx2];
    hgtrk_dt[nhgtrk]    = hgtrk2_dt[t][selidx2];
    hgtrk_truedt[nhgtrk]= hgtrk2_truedt[t][selidx2];
    hgtrk_E[nhgtrk]     = hgtrk2_E[t][selidx2];
    hgtrk_x[nhgtrk]     = hgtrk2_x[t][selidx2];
    hgtrk_y[nhgtrk]     = hgtrk2_y[t][selidx2];
    hgtrk_z[nhgtrk]     = hgtrk2_z[t][selidx2];
    hgtrk_r[nhgtrk]     = hgtrk2_r[t][selidx2];
    hgtrk_eta[nhgtrk]   = hgtrk2_eta[t][selidx2];
    hgtrk_phi[nhgtrk]   = hgtrk2_phi[t][selidx2];

    nhgtrk++;
  }




  ///////////////////////////////
  /// track derived quantities
  //////////////////////////////
  for(int t=0;t<nhgtrk;t++){
    hgtrk_deta[t]= 0.;
    hgtrk_dphi[t]= 0.;

        
    //////////////////////////////
    // now count hits on 3rd and 4th layer
    ////////////////////////////

    //determine the offsets in the hx,hy indices for layer3
    int dhx=0;
    int dhy=0;
    if(hgtrk_idx2[t]==0){ dhx=-1;dhy=-1;}
    if(hgtrk_idx2[t]==1){ dhx=0;dhy=-1;}
    if(hgtrk_idx2[t]==2){ dhx=+1;dhy=-1;}
    if(hgtrk_idx2[t]==3){ dhx=-1;dhy=0;}
    if(hgtrk_idx2[t]==4){ dhx=0;dhy=0;}
    if(hgtrk_idx2[t]==5){ dhx=+1;dhy=0;}
    if(hgtrk_idx2[t]==6){ dhx=-1;dhy=+1;}
    if(hgtrk_idx2[t]==7){ dhx=0;dhy=+1;}
    if(hgtrk_idx2[t]==8){ dhx=+1;dhy=+1;}


    int nhitl3=0;
    float cellxl3=0.;
    float cellyl3=0.;
    float cellzl3=0.;

    int nhitl4=0;
    float cellxl4=0.;
    float cellyl4=0.;
    float cellzl4=0.;

    for(int c=0;c<ncell;c++){
      if(abs(cell_S[c])<=2)  continue; //first and second layers already used
      if(cell_z[c]*hgtrk_z[t]<0)  continue;//cell is on other sided 
      if(cell_type[c]!=hgtrk_type[t])  continue;//cell array is of different kind

      if(cell_hx[c]<cell_hx[hgtrk_c1[t]]+(abs(cell_S[c])-1)*dhx-1 || cell_hx[hgtrk_c1[t]]+(abs(cell_S[c])-1)*dhx+1 < cell_hx[c]) continue;
      if(cell_hy[c]<cell_hy[hgtrk_c1[t]]+(abs(cell_S[c])-1)*dhy-1 || cell_hy[hgtrk_c1[t]]+(abs(cell_S[c])-1)*dhy+1 < cell_hy[c]) continue;

      hgtrk_nhit[t]++;      

      if(abs(cell_S[c])<=3){
    	nhitl3++;
    	cellxl3+=cell_x[c];
    	cellyl3+=cell_y[c];
    	cellzl3+=cell_z[c];
      }

      if(abs(cell_S[c])<=4){
    	nhitl4++;
    	cellxl4+=cell_x[c];
    	cellyl4+=cell_y[c];
    	cellzl4+=cell_z[c];
      }
 
      hgtrk_t[t] +=cell_t[c] - (ZPOSL[abs(cell_S[c])-1]-ZPOSL[0])/speed_light;//time needs to be corrected
      hgtrk_tr[t] +=cell_tr[c] - (ZPOSL[abs(cell_S[c])-1]-ZPOSL[0])/speed_light;//time needs to be corrected      
      hgtrk_dt[t]+=cell_dt[c];
      hgtrk_truedt[t]+=cell_truedt[c];
      hgtrk_E[t] +=cell_E[c];
    }

    ///// set track time
    hgtrk_t[t]     =hgtrk_t[t]/hgtrk_nhit[t];
    hgtrk_tr[t]    =hgtrk_tr[t]/hgtrk_nhit[t];
    hgtrk_dt[t]    =hgtrk_dt[t]/hgtrk_nhit[t];
    hgtrk_truedt[t]=hgtrk_truedt[t]/hgtrk_nhit[t];

    /////////////////////////////
    ////track direction fit
    std::vector<Amg::Vector3D>  trkpoints;
    trkpoints.push_back(Amg::Vector3D(cell_x[hgtrk_c1[t]],cell_y[hgtrk_c1[t]],cell_z[hgtrk_c1[t]]));
    trkpoints.push_back(Amg::Vector3D(cell_x[hgtrk_c2[t]],cell_y[hgtrk_c2[t]],cell_z[hgtrk_c2[t]]));

    if(nhitl3>1){
      cellxl3=cellxl3/nhitl3;
      cellyl3=cellyl3/nhitl3;
      cellzl3=cellzl3/nhitl3;
    }
    trkpoints.push_back(Amg::Vector3D(cellxl3,cellyl3,cellzl3));

    if(nhitl4>1){
      cellxl4=cellxl4/nhitl4;
      cellyl4=cellyl4/nhitl4;
      cellzl4=cellzl4/nhitl4;
    }
    trkpoints.push_back(Amg::Vector3D(cellxl4,cellyl4,cellzl4));
    
    
    ////////////skip tracks whose cells are behind each other (will give undefined direction)
    if(nhitl3==0 && nhitl4==0 && cell_trkidx[hgtrk_c2[t]]==cell_trkidx[hgtrk_c1[t]] ) continue; //only two cells behind each other (L0,L1)
    if(nhitl3==1 && nhitl4==0 && cell_trkidx[hgtrk_c2[t]]==cell_trkidx[hgtrk_c1[t]] 
       && cellxl3==cell_x[hgtrk_c1[t]] && cellyl3==cell_y[hgtrk_c1[t]] ) continue; //only 3 cells behind each other (L0,L1,L2)
    if(nhitl3==0 && nhitl4==1 && cell_trkidx[hgtrk_c2[t]]==cell_trkidx[hgtrk_c1[t]] 
       && cellxl4==cell_x[hgtrk_c1[t]] && cellyl4==cell_y[hgtrk_c1[t]] ) continue; //only 3 cells behind each other (L0,L1,L3)
    if(nhitl3==1 && nhitl4==1 && cell_trkidx[hgtrk_c2[t]]==cell_trkidx[hgtrk_c1[t]] 
       && cellxl3==cell_x[hgtrk_c1[t]] && cellyl3==cell_y[hgtrk_c1[t]] 
       && cellxl4==cell_x[hgtrk_c1[t]] && cellyl4==cell_y[hgtrk_c1[t]] ) continue; //only 4 cells behind each other (L0,L1,L3,L4)

    
    //perform fit with available points
    std::pair < Amg::Vector3D, Amg::Vector3D > fit = best_line_from_points(trkpoints);
    hgtrk_deta[t] = fit.second.eta();
    hgtrk_dphi[t] = fit.second.phi();
    
    // ////crosscheck using two hits
    // if(hgtrk_nhit[t]==2){
    //   TLorentzVector P4;
    //   P4.SetXYZT(cell_x[hgtrk_c2[t]]-cell_x[hgtrk_c1[t]],
    // 		 cell_y[hgtrk_c2[t]]-cell_y[hgtrk_c1[t]],
    // 		 cell_z[hgtrk_c2[t]]-cell_z[hgtrk_c1[t]],
    // 		 0.);
    //   ATH_MSG_INFO ("fillHGTDTracks: check fit : "<< P4.Eta()<<","<<P4.Phi()<<" : "<<fit.second.eta()<<","<<fit.second.phi());
    // }

  }



  // ///for debugging , print list of L0 cells and list of hgtrks
  // for(int c=0;c<ncell;c++){
  //   if(abs(cell_S[c])==1)
  //     ATH_MSG_INFO ("fillHGTDTracks: c="<<c<<" : "<<cell_x[c]<<" , "<<cell_y[c]<<" , "<<cell_dt[c]);
  // }
  // for(int t=0;t<nhgtrk;t++){
  //   ATH_MSG_INFO ("fillHGTDTracks: t="<<t<<" : "<<hgtrk_x[t]<<" , "<<hgtrk_y[t]<<" , "<<hgtrk_dt[t]<<" , deta="<<hgtrk_deta[t]<<" , dphi="<<hgtrk_dphi[t]<<" , c1=("<<cell_x[hgtrk_c1[t]]<<","<<cell_y[hgtrk_c1[t]]<<","<<cell_z[hgtrk_c1[t]]<<")  c2=("<<cell_x[hgtrk_c2[t]]<<","<<cell_y[hgtrk_c2[t]]<<","<<cell_z[hgtrk_c2[t]]<<")");
  // }


}


std::pair < Amg::Vector3D, Amg::Vector3D > LArHitTupleDetGeo::best_line_from_points(const std::vector<Amg::Vector3D> & c)
{
  /// algorithm from here
  /// https://gist.github.com/ialhashim/0a2554076a6cf32831ca 

  ///To do: check here that all cells are not behind each other, eta() is used below

  size_t num_atoms = c.size();
  Eigen::Matrix< Amg::Vector3D::Scalar, Eigen::Dynamic, Eigen::Dynamic > centers(num_atoms, 3);
  for (size_t i = 0; i < num_atoms; ++i) 
    centers.row(i) = c[i];
  
  Amg::Vector3D origin = centers.colwise().mean();
  Eigen::MatrixXd centered = centers.rowwise() - origin.transpose();
  Eigen::MatrixXd cov = centered.adjoint() * centered;
  Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> eig(cov);
  Amg::Vector3D axis = eig.eigenvectors().col(2).normalized();

  // ////fix the direction, note first point in list needs to be in layer 0
  Amg::Vector3D new_axis(axis.x(),axis.y(),axis.z());
  if( ( c[num_atoms-1].z()>c[0].z() && axis.eta()<0 ) || 
      ( c[num_atoms-1].z()<c[0].z() && axis.eta()>0 ) ) 
    new_axis=Amg::Vector3D(-axis.x(),-axis.y(),-axis.z()); 
  
  
  return std::make_pair(origin, new_axis);
}



void LArHitTupleDetGeo::fillCellHitDistributions(const DataHandle<LArHitContainer> p_collection, const HGTD_ID * ID){
  
  //Fill the 1x1mm histograms below 280mm
  for (LArHitContainer::const_iterator hi= p_collection->begin(); hi!=p_collection->end() ; hi++){
    if(abs(ID->x_index((*hi)->cellID()))>1) 
      continue; // select cells along the x-axis
    int y= int(0.5*ID->y_index((*hi)->cellID()));// y is the radius
    if(y<=0 || y>= 280) 
      continue; // select hits 0<r<280mm
    if(ID->barrel_ec((*hi)->cellID())<0) 
      continue; // only use hits on positive side
    int cellS = ID->sampling((*hi)->cellID());

    if(HCellHitVsT1[cellS][y]->GetBinContent(0)==0.) 
      HCellHitVsT1[cellS][y]->SetBinContent(0,(*hi)->time()*1000); //save the time of the fiyst hit

    HCellHitVsT1[cellS][y]->Fill((*hi)->time()*1000-HCellHitVsT1[cellS][y]->GetBinContent(0));
  }

  //reset the arrival time
  for(long l=0;l<4;l++)
    for(long r=0;r<280;r++)
      HCellHitVsT1[l][r]->SetBinContent(0,0.);
  


  //Fill the 3x3mm histograms above 280mm
  for (LArHitContainer::const_iterator hi= p_collection->begin(); hi!=p_collection->end() ; hi++){
    if(abs(ID->x_index((*hi)->cellID()))>3) // in units of 0.5mm
      continue; //select cells along the x-axis

    int y= int(0.5*ID->y_index((*hi)->cellID())); // y is the radius
    if( y<280 ) 
      continue;
    
    if(ID->barrel_ec((*hi)->cellID())<0) 
      continue; // only use hits on positive side
    
    int cellS=ID->sampling((*hi)->cellID());
    
    int i=int(y/3.);
    
    if(HCellHitVsT3[cellS][i]->GetBinContent(0)==0.) 
      HCellHitVsT3[cellS][i]->SetBinContent(0,(*hi)->time()*1000);//save the time of the first hit
    
    HCellHitVsT3[cellS][i]->Fill((*hi)->time()*1000-HCellHitVsT3[cellS][i]->GetBinContent(0));
  }
  
  //reset the arrival time
  for(long l=0;l<4;l++)
    for(long r=0;r<200;r++)
      HCellHitVsT3[l][r]->SetBinContent(0,0.);


}





void LArHitTupleDetGeo::associateTriggerCells(){

  for(int c=0;c<ncell;c++)
    cell_trig[c]=-1;

  for(int c=0;c<ncell;c++){
    int trig=-1;
    for(int idx=0;idx<ntrig;idx++){
      //if(trig_S[idx] != cell_S[c]) continue;
      if(trig_z[idx]*cell_z[c]<0) continue;//must be on same side

      if( cell_x[c] < (trig_x[idx] - trig_size[idx]/2) ) continue;
      if( cell_x[c] >= (trig_x[idx] + trig_size[idx]/2) ) continue;
      if( cell_y[c] < (trig_y[idx] - trig_size[idx]/2) ) continue;
      if( cell_y[c] >= (trig_y[idx] + trig_size[idx]/2) ) continue;
      trig=idx;
      break;
    }

    cell_trig[c]=trig;
  }
  

  // //print the cells 
  // for(int c=0;c<ncell;c++){
  //   if(cell_trig[c]==0)
  //     ATH_MSG_ERROR ("cell: x="<<cell_x[c]<<" y="<<cell_y[c]<<" z="<<cell_z[c]);
  // }

}


void LArHitTupleDetGeo::processTrigger(){
  
  ///reset previous event:
  ntrig_cut=0;
  ntrig_5=0;
  ntrig_10=0;
  ntrig_15=0;
  ntrig_20=0;
  ntrig_25=0;
  ntrig_30=0;
  ntrig_35=0;
  ntrig_40=0;
  ntrig_45=0;
  ntrig_50=0;

  for(int idx=0;idx<ntrig;idx++){
    trig_nhit[idx]=0;
    trig_truedt[idx]=0.;
    trig_jet[idx]=-1;

    trig_E[idx]=0;
    trig_E_peak[idx]=0;
  }


  ////////Fill this event
  for(int idx=0;idx<ntrig;idx++){
    
    ////////////////basic cell quantities
    for(int c=0;c<ncell;c++){
      if(cell_trig[c] != idx) continue;      

      trig_nhit[idx]++;   ///count total number of hits on this tile
      trig_truedt[idx] += cell_truedt[c];///get an average of true dt for this tile

      trig_E[idx]+=cell_E[c]; 
    }

    if(trig_nhit[idx]>0)
      trig_truedt[idx]  = trig_truedt[idx]/trig_nhit[idx];


    /////////match to jet
    int mintrig=-1;
    float mintrigdr=100000;
    for(int j=0;j<njet;j++){
      float dr=deltaR(trig_eta[idx],trig_phi[idx],jet_eta[j],jet_phi[j]);
      if(dr<trig_sizeeta[idx]/1.1414&&dr<mintrigdr){
	mintrigdr=dr;
	mintrig=j;
      }
    }
    trig_jet[idx]=mintrig;

    //////////////////
    fitTriggerTileDT(idx);
    if(TrigCut)      if(trig_nhit_peak[idx]>TrigCut->Eval(trig_size[idx],trig_r[idx])) ntrig_cut++;
    if(trig_nhit_peak[idx]>5) ntrig_5++;
    if(trig_nhit_peak[idx]>10) ntrig_10++;
    if(trig_nhit_peak[idx]>15) ntrig_15++;
    if(trig_nhit_peak[idx]>20) ntrig_20++;
    if(trig_nhit_peak[idx]>25) ntrig_25++;
    if(trig_nhit_peak[idx]>30) ntrig_30++;
    if(trig_nhit_peak[idx]>35) ntrig_35++;
    if(trig_nhit_peak[idx]>40) ntrig_40++;
    if(trig_nhit_peak[idx]>45) ntrig_45++;
    if(trig_nhit_peak[idx]>50) ntrig_50++;
  }
  

  
  /////////////////////////////////
  // //match trigger to jets
  ////////////////////////////////
  for(int j=0; j<njet; j++)
    jet_trig[j]=-1;//init jets
 
  for(int t=0;t<ntrig;t++)
    if(trig_jet[t]>=0)
      jet_trig[trig_jet[t]]=t;

}

void LArHitTupleDetGeo::fitTriggerTileDT(int idx){

  trig_dt[idx]=0.;
  trig_nhit_peak[idx]=0;
  trig_E_peak[idx]=0.; 

  float bsize=2*m_HitTimeSmear1;//2 sigma of inner cell resolution
  if(fabs(trig_x[idx])>HGTDHEIGHTIN || fabs(trig_y[idx])>HGTDHEIGHTIN ) 
    bsize=2*m_HitTimeSmear2;//2 sigma of outer cell resolution


  ///fill the bin counts 
  float binE[200];
  for(int b=0;b<NMAXTBINS;b++){
    bincount[b]=0;
    binmean[b]=0.;
    binE[b]=0.;
  }
  for(int c=0;c<ncell;c++){
    if(cell_trig[c] != idx) continue;
    int bin=(cell_dt[c]+scanwindow)/(bsize);
    if(bin<0 || bin>=NMAXTBINS) continue;
    
    bincount[bin]++;
    binmean[bin] += cell_dt[c];
    binE[bin] += cell_E[c];
  }
    
  ////find the maximum bin
  int maxbin=-1; 
  for(int b=0;b<NMAXTBINS;b++){
    if(maxbin==-1 || bincount[b]>bincount[maxbin])
      maxbin=b; 
    
    //also normalize the mean time
    if(bincount[b]>1)
      binmean[b]=binmean[b]/bincount[b];
  }

  if(maxbin!=-1){  
    trig_dt[idx]        = binmean[maxbin];
    trig_nhit_peak[idx] = bincount[maxbin];
    trig_E_peak[idx]    = binE[maxbin]; 
  }

}


void LArHitTupleDetGeo::associateElectronCells(){

  for(int c=0;c<ncell;c++){
    cell_ele[c]  =-1;
    cell_eledR[c]=10000;

    float mindr=10000;
    int mini=-1;
    for(int i=0;i<nele;i++){
      float dr= deltaR(cell_eta[c],cell_phi[c],ele_eta[i],ele_phi[i]);
      if( dr < mindr ){
	mindr=dr; 
	mini=i;
      }
    }
    
    if( mindr < m_EleMinHitDR ){
      cell_ele[c]  =mini;
      cell_eledR[c]=mindr;
    }

  }

}


void  LArHitTupleDetGeo::setElectronCells(){
  
  ///count how many cells are associated to each electron
  for(int e=0; e<nele; e++){  
    ele_ncell[e]=0;
    ele_truedt[e]=0.;

    for(long t=0;t<NTWINDOW;t++){
      ele_ncell_window[t][e]=0;
      ele_cellE_window[t][e]=0; 
    }

    for(int c=0;c<ncell;c++){
      if(cell_ele[c]!=e)continue; 

      ele_ncell[e]++;

      ele_truedt[e]+=cell_truedt[c];

      for(long t=0;t<NTWINDOW;t++) {
	if(fabs(1000*cell_dt[c]) < (t+1)*TSTEP){
	  ele_ncell_window[t][e]++;
	  ele_cellE_window[t][e] += cell_E[c];
	}
      }
      
    }   
    
    ele_truedt[e] = ele_truedt[e]/ele_ncell[e];
    
    fitEleDT(e); 
 }
  
}



void LArHitTupleDetGeo::associateClusterCells(){

  for(int c=0;c<ncell;c++){
    cell_clus[c]  =-1;
    cell_clusdR[c]=10000;

    float mindr=10000;
    int mini=-1;
    for(int i=0;i<nclus;i++){
      float dr= deltaR(cell_eta[c],cell_phi[c],clus_eta[i],clus_phi[i]);
      if( dr < mindr ){
	mindr=dr; 
	mini=i;
      }
    }
    
    if( mindr < m_ClusterMinHitDR ){
      cell_clus[c]  =mini;
      cell_clusdR[c]=mindr;
    }

  }

}

void LArHitTupleDetGeo::setClusterCells(){

  ///count how many cells are associated to each cluster
  for(int i=0;i<nclus;i++){  
    clus_ncell[i]=0;
    clus_ncell_clean[i]=0;

    for(long t=0;t<NTWINDOW;t++)
      clus_ncell_window[t][i]=0;//to be defined

    for(int c=0;c<ncell;c++){
      if(cell_clus[c]==i) clus_ncell[i]++;
    }
  }
  
}


void LArHitTupleDetGeo::associateJetCells(){

  for(int c=0;c<ncell;c++){
    cell_jet[c]  =-1;
    cell_jetdR[c]=10000;
      
    float mindr=10000;
    int mini=-1;
    for(int i=0;i<njet;i++){
      float dr= deltaR(cell_eta[c],cell_phi[c],jet_eta[i],jet_phi[i]);
      if( dr < mindr ){
	mindr=dr; 
	mini=i;
      }
    }
    
    if( mindr < m_JetMinHitDR ){
      cell_jet[c]  =mini;
      cell_jetdR[c]=mindr;
    }
    
  }



  ///tracks
  for(int t=0;t<nhgtrk;t++){
    hgtrk_jet[t]  =-1;
    hgtrk_jetdR[t]=10000;
      
    float mindr=10000;
    int mini=-1;
    for(int i=0;i<njet;i++){
      float dr= deltaR(hgtrk_eta[t],hgtrk_phi[t],jet_eta[i],jet_phi[i]);
      if( dr < mindr ){
	mindr=dr; 
	mini=i;
      }
    }
    
    if( mindr < m_JetMinHitDR ){
      hgtrk_jet[t]  =mini;
      hgtrk_jetdR[t]=mindr;
    }
    
  }

  /// set the jet properties 
  for(int e=0; e<njet; e++){  
    jet_nhgtrk[e]=0;
    for(int t=0;t<nhgtrk;t++){
      if(hgtrk_jet[t]!=e) continue;
      jet_nhgtrk[e]++;
    }
  }



}


void LArHitTupleDetGeo::setJetCells(){
  
  ///count how many cells are associated to each jet
  for(int j=0;j<njet;j++){  


    //////////////////////////
    ///cluster shape info
    ////////////////////////
    jet_clusE_dr00[j]=0;
    jet_clusE_dr05[j]=0;
    jet_clusE_dr10[j]=0;
    jet_clusE_dr15[j]=0;
    jet_clusE_dr20[j]=0;
    jet_clusE_dr25[j]=0;
    jet_clusE_dr30[j]=0;
    jet_clusE_dr35[j]=0;
    for(int c=0;c<nclus;c++){
      if(clus_jet[c]!=j) continue;

      if(clus_jetdR[c]<0.05)     jet_clusE_dr00[j]+=clus_E[c];
      else if(clus_jetdR[c]<0.10)jet_clusE_dr05[j]+=clus_E[c];
      else if(clus_jetdR[c]<0.15)jet_clusE_dr10[j]+=clus_E[c];
      else if(clus_jetdR[c]<0.20)jet_clusE_dr15[j]+=clus_E[c];
      else if(clus_jetdR[c]<0.25)jet_clusE_dr20[j]+=clus_E[c];
      else if(clus_jetdR[c]<0.30)jet_clusE_dr25[j]+=clus_E[c];
      else if(clus_jetdR[c]<0.35)jet_clusE_dr30[j]+=clus_E[c];
      else if(clus_jetdR[c]<0.40)jet_clusE_dr35[j]+=clus_E[c];

    }



    //////////////////////////////
    ////Jet  shape info 
    /////////////////////////////
    jet_ncell_dr00[j]=0;
    jet_ncell_dr05[j]=0;
    jet_ncell_dr10[j]=0;
    jet_ncell_dr15[j]=0;
    jet_ncell_dr20[j]=0;
    jet_ncell_dr25[j]=0;
    jet_ncell_dr30[j]=0;
    jet_ncell_dr35[j]=0;
    for(int c=0;c<ncell;c++){
      //if(cell_jet[c]!=j) continue;//this may cause edge effect 

      if(fabs(cell_eta[c]-jet_eta[j])>0.4) continue; ///cells in other side
      
      float dr = deltaR(cell_eta[c],cell_phi[c],jet_eta[j],jet_phi[j]);     
      if(dr<0.05)     jet_ncell_dr00[j]++;
      else if(dr<0.10)jet_ncell_dr05[j]++;
      else if(dr<0.15)jet_ncell_dr10[j]++;
      else if(dr<0.20)jet_ncell_dr15[j]++;
      else if(dr<0.25)jet_ncell_dr20[j]++;
      else if(dr<0.30)jet_ncell_dr25[j]++;
      else if(dr<0.35)jet_ncell_dr30[j]++;
      else if(dr<0.40)jet_ncell_dr35[j]++;
    }

    /////////////////////////////////
    ////Cell counting without overlaps
    ////////////////////////////////
    jet_ncell[j]=0;
    jet_ncell_clus[j]=0;
    jet_truedt[j]=0.;
    for(int c=0;c<ncell;c++){
      if(cell_jet[c]!=j) continue;
      jet_ncell[j]++;
      jet_truedt[j] += cell_truedt[c];
    }
    jet_truedt[j]=jet_truedt[j]/jet_ncell[j];


    //fit the jet deltaT 
    fitJetDTJetCell(j);
    fitJetDTJetCore(j);
    fitJetDTJetCoreHGTrk(j);
    //fitJetDTBkgSub(j);
    //fitJetDTFix(j);
    //fitJetDT(j);
    //fitJetDTEw(j);
    //fitJetDTEcw(j);
    //fitJetDRDT(j);
    //fitJetShape(j);

  }

}


void  LArHitTupleDetGeo::setObjectTracks(){
  
  //first assign tracks to electrons
  for(int t=0;t<ntrk;t++){
    int bestele=-1;
    for(int e=0; e<nele; e++){  
      float dr=deltaR(trk_eta[t],trk_phi[t],ele_eta[e],ele_phi[e]);
      if(dr>0.1) continue;
      
      if(bestele==-1 || dr<deltaR(trk_eta[t],trk_phi[t],ele_eta[bestele],ele_phi[bestele]))
	bestele=e;
    }
    trk_ele[t]=bestele;
  }

  ///now set electron track properties
  for(int e=0; e<nele; e++){  
    ele_trk[e]=-1;
    ele_ntrk[e]=0;
    ele_ntrkpv[e]=0;
    
    int besttrk=-1;
    for(int t=0;t<ntrk;t++){
      if(trk_ele[t]!=e) continue;
      ele_ntrk[e]++;

      //select the track from the pv
      if(fabs(trk_d0[t])>1.) continue;
      if(trk_pt[t]<1000) continue;
      //if(fabs(trk_z0[t] - vtx_z)>1.5) continue;
      ele_ntrkpv[e]++;
      
      //if(besttrk==-1 || trk_pt[t]>trk_pt[besttrk] )
      if(besttrk==-1 || fabs(trk_z0[t] - vtx_z)<fabs(trk_z0[besttrk] - vtx_z) )
	besttrk=t;
    }

    ele_trk[e]=besttrk;
  }


  
  //assign tracks to jets
  for(int t=0;t<ntrk;t++){
    int bestjet=-1;
    float mindr=10000;
    for(int e=0; e<njet; e++){  
      float dr=deltaR(trk_eta[t],trk_phi[t],jet_eta[e],jet_phi[e]);
      if(dr>0.4) continue;
      
      if(bestjet==-1 || dr<mindr){
	bestjet=e;
	mindr=dr;
      }
    }

    trk_jet[t]=bestjet;
    trk_jetdR[t]=mindr;
  }

  //set jet track properties
  for(int j=0; j<njet; j++){  
    jet_ntrk[j]=0;
    jet_ltrk[j]=-1;
    jet_dRtrk[j]=-1;
    for(int t=0;t<ntrk;t++){
      if(trk_jet[t]!=j) continue;
      jet_ntrk[j]++;

      //leading track
      if(jet_ltrk[j]==-1 || trk_e[t]>trk_e[jet_ltrk[j]])
	jet_ltrk[j]=t;
	
      //trk closest to core
      if(jet_dRtrk[j]==-1 || 
	 deltaR(trk_eta[t],trk_phi[t],jet_eta[j],jet_phi[j])
	 <deltaR(trk_eta[jet_dRtrk[j]],trk_phi[jet_dRtrk[j]],jet_eta[j],jet_phi[j])
	 )
	jet_dRtrk[j]=t;

    }
  }


  ///track shape info
  for(int j=0; j<njet; j++){  
    jet_trk_dr00[j]=0;
    jet_trk_dr05[j]=0;
    jet_trk_dr10[j]=0;
    jet_trk_dr15[j]=0;
    jet_trk_dr20[j]=0;
    jet_trk_dr25[j]=0;
    jet_trk_dr30[j]=0;
    jet_trk_dr35[j]=0;
    for(int t=0;t<ntrk;t++){
      if(trk_jet[t]!=j) continue;

      if(trk_jetdR[t]<0.05)     jet_trk_dr00[j]+=trk_e[t];
      else if(trk_jetdR[t]<0.10)jet_trk_dr05[j]+=trk_e[t];
      else if(trk_jetdR[t]<0.15)jet_trk_dr10[j]+=trk_e[t];
      else if(trk_jetdR[t]<0.20)jet_trk_dr15[j]+=trk_e[t];
      else if(trk_jetdR[t]<0.25)jet_trk_dr20[j]+=trk_e[t];
      else if(trk_jetdR[t]<0.30)jet_trk_dr25[j]+=trk_e[t];
      else if(trk_jetdR[t]<0.35)jet_trk_dr30[j]+=trk_e[t];
      else if(trk_jetdR[t]<0.40)jet_trk_dr35[j]+=trk_e[t];
    
    }
  }



}



void LArHitTupleDetGeo::associateTrackCells(){

  // for(int c=0;c<ncell;c++){
  //   cell_trk[c]  =-1;
  //   cell_trkdR[c]=10000;
      
  //   float mindr=10000;
  //   int mini=-1;
  //   float trkpt=-1;
  //   for(int i=0;i<ntrk;i++){
  //     if(fabs(trk_ext_eta[i])<1.) continue;//remove bad tracks
  //     if(cell_eta[c]*trk_ext_eta[i] < 0) continue;//tracks on the other side
  //     float dr= sqrt( (cell_x[c]-trk_ext_x[i])*(cell_x[c]-trk_ext_x[i])+(cell_y[c]-trk_ext_y[i])*(cell_y[c]-trk_ext_y[i]));
      
  //     if( dr < mindr ){
  // 	mindr=dr; 
  // 	mini=i;
  // 	trkpt=trk_pt[i];
  //     }
      
  //   }
    
  //   if(mini==-1)continue;
  //   if(mindr>10.0) continue;

  //   // if(cell_type[c]==1&&trkpt<5000&&mindr>3.0) continue;
  //   // if(cell_type[c]==1&&trkpt>5000&&mindr>1.5) continue;
  //   // if(cell_type[c]==2&&trkpt<5000&&mindr>5.0) continue;
  //   // if(cell_type[c]==2&&trkpt>5000&&mindr>3.0) continue;

  //   cell_trk[c]  =mini;
  //   cell_trkdR[c]=mindr;    
  // }
  

  for(int c=0;c<ncell;c++){
    cell_trk[c]  = -1;
    cell_trkdR[c]= 100000;  
  }


  for(int i=0;i<ntrk;i++){
    if(fabs(trk_ext_eta[i])<1.)           continue; //remove bad tracks
    
    trk_outlier[i]=0;


    float mindr[4];
    int match[4]={-1,-1,-1,-1};
    for(int c=0;c<ncell;c++){
      if(cell_eta[c]*trk_ext_eta[i] < 0)    continue; //on the other side

      //if(fabs(cell_x[c]-trk_ext_x[i]) > 10) continue;
      //if(fabs(cell_y[c]-trk_ext_y[i]) > 10) continue;

      // float dr= sqrt(  (cell_x[c]-trk_ext_x[i])*(cell_x[c]-trk_ext_x[i])
      // 		      +(cell_y[c]-trk_ext_y[i])*(cell_y[c]-trk_ext_y[i]));
      

      //use layer dependent trk positions
      float dr= sqrt((cell_x[c]-trk_hgtd_x[abs(cell_S[c])-1][i])
		     *(cell_x[c]-trk_hgtd_x[abs(cell_S[c])-1][i])
		     +(cell_y[c]-trk_hgtd_y[abs(cell_S[c])-1][i])
		     *(cell_y[c]-trk_hgtd_y[abs(cell_S[c])-1][i]));


      if(dr>m_TrackMatchRadius) continue;

      if(match[abs(cell_S[c])-1]==-1 || dr < mindr[abs(cell_S[c])-1] ){
	mindr[abs(cell_S[c])-1]=dr; 
	match[abs(cell_S[c])-1]=c;
      }
      
    }
    
    //calculations below require at least one hit (do not remove this line)
    if(match[0]==-1&&match[1]==-1&&match[2]==-1&&match[3]==-1)continue;

    
    /////////////////////////////////////////////////
    ///evaluate average dt based on at most 3 cells
    ///combinations are cyclic: 012,123,230,301
    ///later remove at most one outlier
    ///////////////////////////////////////////////
    float meandt[4];
    for(int c=0;c<4;c++){
      meandt[c]=0.;
      int nmatch=0;
      for(int c2=c;c2<c+3;c2++){
	int c3=c2%4;
	if(match[c3]==-1)continue;
	meandt[c]+=cell_dt[match[c3]];
	nmatch++;
      }
      meandt[c]=meandt[c]/nmatch;
    }
    
    //find set which has smallest rms
    float minrms=100000;
    float minmeandt=0.;
    for(int c=0;c<4;c++){
      float rms=0.;
      for(int c2=c;c2<c+3;c2++){
	int c3=c2%4;
	if(match[c3]==-1)continue;
	rms += (cell_dt[match[c3]]-meandt[c])*(cell_dt[match[c3]]-meandt[c]);
      }
      if(rms<minrms){
	minrms=rms;
	minmeandt=meandt[c];
      }
    }
    if(minmeandt==0.) continue; // there is a problem here this should not happen.
    
    ///remove outliers based on set with minimum rms value
    for(int c=0;c<4;c++){
      if(match[c]==-1)continue;
      if( fabs(cell_dt[match[c]] - minmeandt) 
	  > 3*( (cell_type[match[c]]==1)*m_HitTimeSmear1 
		+ (cell_type[match[c]]==2)*m_HitTimeSmear2) ){//choose the resolution
	
	// ATH_MSG_INFO ("associateTrackCells:: outlier="<<cell_dt[match[c]]
	// 	      <<"  : "
	// 	      <<(match[0]>-1?cell_dt[match[0]]:0)<<","
	// 	      <<(match[1]>-1?cell_dt[match[1]]:0)<<","
	// 	      <<(match[2]>-1?cell_dt[match[2]]:0)<<","
	// 	      <<(match[3]>-1?cell_dt[match[3]]:0));

	match[c]=-1;
	trk_outlier[i]++;
      }
    }


    //////////////////////////////////////////
    ///set cell links to track
    /////////////////////////////////////
    for(int c=0;c<4;c++){
      if(match[c]==-1)continue;
   
      // if(mindr[c]>10.0) continue;

      // if(cell_type[c]==1&&trkpt<5000&&mindr>3.0) continue;
      // if(cell_type[c]==1&&trkpt>5000&&mindr>1.5) continue;
      // if(cell_type[c]==2&&trkpt<5000&&mindr>5.0) continue;
      // if(cell_type[c]==2&&trkpt>5000&&mindr>3.0) continue;
      
      //////
      //if(cell_type[c]==1 && mindr[c]>FTrkMatch1mm->Eval(trk_pt[i]/1000.)) continue;
      //if(cell_type[c]==2 && mindr[c]>FTrkMatch3mm->Eval(trk_pt[i]/1000.)) continue;

      cell_trk[match[c]]  = i;
      cell_trkdR[match[c]]= mindr[c];  
    }


  }
  



}




void LArHitTupleDetGeo::setTrackHGTDTrks(){


  for(int i=0;i<ntrk;i++){
    trk_hgtrk[i]=-1;///initialize
    
    if(fabs(trk_ext_eta[i])<1.)           continue; // do not consider  bad tracks
  
    float mindr=1000000;
    int match=-1;
    for(int c=0;c<nhgtrk;c++){
      if(hgtrk_eta[c]*trk_ext_eta[i] < 0)    continue; //on the other side

   
      //use layer dependent trk positions
      float dr= sqrt((hgtrk_x[c]-trk_hgtd_x[0][i])
		     *(hgtrk_x[c]-trk_hgtd_x[0][i])
		     +(hgtrk_y[c]-trk_hgtd_y[0][i])
		     *(hgtrk_y[c]-trk_hgtd_y[0][i]));
      

      if(match==-1 || dr < mindr ){
	mindr=dr; 
	match=c;
      }
      
    }

    trk_hgtrk[i]=match;
    trk_hgtrkdR[i]=mindr;
 
  }
  



}


void LArHitTupleDetGeo::setTrackCells(){

  ////count cells per track
  for(int i=0;i<ntrk;i++){
    trk_ncell[i]=0;
    trk_ncellL0[i]=0;
    trk_cell0[i]=-1;
    trk_cell1[i]=-1;
    trk_cell2[i]=-1;
    trk_cell3[i]=-1;



    trk_t[i]=0;
    trk_dt[i]=0;
    trk_truedt[i]=0;

    trk_tr[i]=0;
    //trk_truetr[i]=0;
    trk_trtrue[i]=0;

    for(int c=0;c<ncell;c++){
      if(cell_trk[c]==i){
	trk_ncell[i]++;
	if(abs(cell_S[c])==1)  trk_ncellL0[i]++;

	if(abs(cell_S[c])==1)	  trk_cell0[i]=c;
	if(abs(cell_S[c])==2)	  trk_cell1[i]=c;
	if(abs(cell_S[c])==3)	  trk_cell2[i]=c;
	if(abs(cell_S[c])==4)	  trk_cell3[i]=c;


	trk_dt[i]+=cell_dt[c];            

	trk_t[i]+=cell_t[c]- (ZPOSL[abs(cell_S[c])-1]-ZPOSL[0])/speed_light;            	

	///need to apply correction if it is a pile-up track , //ignore vtx x,y
	trk_truedt[i] +=  trk_truth_vtxt[i]
	  + sqrt(cell_x[c]*cell_x[c]+cell_y[c]*cell_y[c] + pow(cell_z[c]-trk_truth_vtxz[i],2)) / speed_light 
	  - cell_D[c]/speed_light; 
	


	//////////////////////
	///better to use the unsmeared time for checking the correct time assigment
	//////////////////////
	trk_tr[i] += cell_tr[c] - (ZPOSL[abs(cell_S[c])-1]-ZPOSL[0])/speed_light ; 

       	//trk_truetr[i] +=  trk_truth_vtxt[i] + sqrt(cell_x[c]*cell_x[c] + cell_y[c]*cell_y[c] + pow(cell_z[c]-trk_truth_vtxz[i],2)) / speed_light  ;
	
	trk_trtrue[i] += sqrt(cell_x[c]*cell_x[c] + cell_y[c]*cell_y[c] + pow(cell_z[c]-trk_truth_vtxz[i],2)) / speed_light; 

      }
    }
      
    if(trk_ncell[i]>0){ 
      trk_t[i]=trk_t[i]/trk_ncell[i];
      trk_dt[i]=trk_dt[i]/trk_ncell[i];
      trk_truedt[i]=trk_truedt[i]/trk_ncell[i];

      trk_tr[i]=trk_tr[i]/trk_ncell[i];
      //trk_truetr[i]=trk_truetr[i]/trk_ncell[i];
      trk_trtrue[i]=trk_trtrue[i]/trk_ncell[i];
    }

  }  

}




void LArHitTupleDetGeo::dividedRDensityHisto(TH1F* h){
  if(!h) return;
  for(int b=1; b<=h->GetNbinsX(); b++){
    float area = TMath::Pi()*( pow(h->GetBinLowEdge(b)+h->GetBinWidth(b),2) - pow(h->GetBinLowEdge(b),2) );
    h->SetBinError(b,h->GetBinError(b)/area);
    h->SetBinContent(b,h->GetBinContent(b)/area);
  }
}
  
void LArHitTupleDetGeo::fitJetShape(int idx){

  ///
  jet_shape_p0[idx]=0.;
  jet_shape_p0er[idx]=0.;
  jet_shape_p1[idx]=0;
  jet_shape_p1er[idx]=0;
  jet_shape_p2[idx]=0.;
  jet_shape_p2er[idx]=0.;
  jet_shape_p3[idx]=0;
  jet_shape_p3er[idx]=0;


  ///
  jet_shapeknee_chi2pndf[idx]=0.;
  jet_shapeknee_p0[idx]=0.;
  jet_shapeknee_p0er[idx]=0.;
  jet_shapeknee_p1[idx]=0;
  jet_shapeknee_p1er[idx]=0;
  jet_shapeknee_p2[idx]=0.;
  jet_shapeknee_p2er[idx]=0.;




  HJetShape->SetBinContent(1,jet_ncell_dr00[idx]);
  HJetShape->SetBinError(1,sqrt(jet_ncell_dr00[idx]));
  HJetShape->SetBinContent(2,jet_ncell_dr05[idx]);
  HJetShape->SetBinError(2,sqrt(jet_ncell_dr05[idx]));
  HJetShape->SetBinContent(3,jet_ncell_dr10[idx]);
  HJetShape->SetBinError(3,sqrt(jet_ncell_dr10[idx]));
  HJetShape->SetBinContent(4,jet_ncell_dr15[idx]);
  HJetShape->SetBinError(4,sqrt(jet_ncell_dr15[idx]));
  HJetShape->SetBinContent(5,jet_ncell_dr20[idx]);
  HJetShape->SetBinError(5,sqrt(jet_ncell_dr20[idx]));
  HJetShape->SetBinContent(6,jet_ncell_dr25[idx]);
  HJetShape->SetBinError(6,sqrt(jet_ncell_dr25[idx]));
  HJetShape->SetBinContent(7,jet_ncell_dr30[idx]);
  HJetShape->SetBinError(7,sqrt(jet_ncell_dr30[idx]));
  HJetShape->SetBinContent(8,jet_ncell_dr35[idx]);
  HJetShape->SetBinError(8,sqrt(jet_ncell_dr35[idx]));

  if(HJetShape->Integral()<=0) return;

  dividedRDensityHisto(HJetShape);

  ///Fit to the core
  HJetShape->Fit(FJetShape,"QN","",0.,0.2);
  jet_shape_p0[idx]=FJetShape->GetParameter(0);
  jet_shape_p1[idx]=FJetShape->GetParameter(1);
  jet_shape_p0er[idx]=FJetShape->GetParError(0);
  jet_shape_p1er[idx]=FJetShape->GetParError(1);
  ///fit to the edge
  HJetShape->Fit(FJetShape,"QN","",0.2,0.4);
  jet_shape_p2[idx]=FJetShape->GetParameter(0);
  jet_shape_p3[idx]=FJetShape->GetParameter(1);
  jet_shape_p2er[idx]=FJetShape->GetParError(0);
  jet_shape_p3er[idx]=FJetShape->GetParError(1);


  ////Fit full fit with knee function
  HJetShape->Fit(FJetShapeKnee,"QN","",0.,0.4);
  jet_shapeknee_chi2pndf[idx]=FJetShapeKnee->GetChisquare()/FJetShapeKnee->GetNDF();
  jet_shapeknee_p0[idx]=FJetShapeKnee->GetParameter(0);
  jet_shapeknee_p0er[idx]=FJetShapeKnee->GetParError(0);
  jet_shapeknee_p1[idx]=FJetShapeKnee->GetParameter(1);
  jet_shapeknee_p1er[idx]=FJetShapeKnee->GetParError(1);
  jet_shapeknee_p2[idx]=FJetShapeKnee->GetParameter(2);
  jet_shapeknee_p2er[idx]=FJetShapeKnee->GetParError(2);
  
}




void LArHitTupleDetGeo::fitJetDT(int idx){
  //algorithm using cells on jet clusters

  jet_dTFit_signalmean[idx]=0.;
  jet_dTFit_ncell_clus[idx]=0;
  for(long t=0;t<NTWINDOW;t++)
    jet_dTFit_ncell_clus_window[t][idx]=0;

  float binsize=m_dTSigma*(fabs(jet_eta[idx])<3.2 ? m_HitTimeSmear2 : m_HitTimeSmear1); // use 2*sigma wide bins

  //
  for(int b=0;b<NMAXTBINS;b++){
    bincount[b]=0;
    binmean[b]=0.;
  }
  for(int c=0;c<ncell;c++){
    if(cell_jet[c] != idx) continue;
    if(cell_clus[c] < 0) continue;
    if(cell_clusdR[c] > m_dTFitCoreBkgSubRadius) continue; 
    
    int bin=(cell_dt[c]+scanwindow)/(binsize);
    if(bin<0 || bin>=NMAXTBINS) continue;
    
    bincount[bin]++;
    binmean[bin] += cell_dt[c];
  }
    
  ////find the maximum bin
  int maxbin=-1; 
  for(int b=0;b<NMAXTBINS;b++){
    if(maxbin==-1 || bincount[b]>bincount[maxbin])
      maxbin=b; 
    
    //also normalize the mean time
    if(bincount[b]>1)
      binmean[b]=binmean[b]/bincount[b];
  }

  float meanmaxbin=0.;
  if(maxbin!=-1)  meanmaxbin = binmean[maxbin];

  jet_dTFit_signalmean[idx]=meanmaxbin;

  //count the number of cells around the signal peak
  for(int c=0;c<ncell;c++){
    if(cell_jet[c] != idx) continue;
    if(cell_clus[c] < 0) continue;
    if(cell_clusdR[c] > m_dTFitCoreBkgSubRadius) continue; 

    jet_dTFit_ncell_clus[idx]++;
    for(long t=0;t<NTWINDOW;t++)
      if(fabs(1000*(cell_dt[c]-meanmaxbin)) < (t+1)*TSTEP ) 
	jet_dTFit_ncell_clus_window[t][idx]++;
  }




  //set the wp
  jet_dTFit_wp70[idx] = 0;
  if(m_VBFJetWPAlgo.compare("ClusCellDTFitFraction")==0){    
    int etabin = getJetWPEtaBin(jet_eta[idx]);
    int ptbin = getJetWPPtBin(jet_pt[idx]);
    if(etabin<0 || ptbin<0)
      jet_dTFit_wp70[idx] = 1;      //jets outside the WP definition range are set to pass.
    else if( jet_dTFit_ncell_clus_window[WPWindowIdx][idx] / float(jet_ncell_clus[idx]) > VBFJetWP[etabin][ptbin] )
      jet_dTFit_wp70[idx] = 1;
  }


}


void LArHitTupleDetGeo::fitJetDTBkgSub(int idx){

  //init results
  jet_dTFitCoreBkgSub_signalmean[idx]=0.;
  jet_dTFitCoreBkgSub_ncellbkg[idx]=0.;
  jet_dTFitCoreBkgSub_ncellsig[idx]=0.;
  jet_dTFitCoreBkgSub_maxbkg[idx]=0.;
  jet_dTFitCoreBkgSub_sF[idx]=0.;
  jet_dTFitCoreBkgSub_avgbkg[idx]=0.;

  jet_dTFitCoreBkgSub_ncell[idx]=0;
  for(long t=0;t<NTWINDOW;t++)
    jet_dTFitCoreBkgSub_ncell_window[t][idx]=0;

  float binsize=m_dTSigma*(fabs(jet_eta[idx])<3.2 ? m_HitTimeSmear2 : m_HitTimeSmear1); // use 2*sigma wide bins
  
  ////sideband distribution
  int bincountbkg[NMAXTBINS];
  for(int b=0;b<NMAXTBINS;b++)
    bincountbkg[b]=0;
  for(int c=0;c<ncell;c++){
    if(cell_jet[c]!=idx) continue;
    if(cell_jetdR[c] < m_dTFitCoreBkgSubRadiusSideband ) continue;
    int bin=(cell_dt[c]+scanwindow)/binsize;
    if(bin<0||bin>=NMAXTBINS) continue;
    bincountbkg[bin]++;
  }

  // //scale the background estimate into the signal region
  // int etaidx=minetaidx + ((((int)(fabs(jet_eta[idx])*10) - minetaidx))/etastep) * etastep;
  // int ptidx=minptidx + ((((int)(jet_pt[idx]/1000) - minptidx))/ptstep) * ptstep;
  // //ATH_MSG_INFO ("fitJetDTBkgSub: etaidx:" <<jet_eta[idx]<<":"<<etaidx<<" , ptidx:"<<jet_pt[idx]<<":"<<ptidx);
  // if(etaidx<minetaidx)etaidx=minetaidx;
  // if(etaidx>maxetaidx)etaidx=maxetaidx;
  // if(ptidx<minptidx)ptidx=minptidx;
  // if(ptidx>maxptidx)ptidx=maxptidx;
  // float sigintegral=FuncdR_Bkg[etaidx][ptidx]->Integral(FuncdR_Bkg[etaidx][ptidx]->GetXmin(),m_dTFitCoreBkgSubRadius);
  // float sideintegral=FuncdR_Bkg[etaidx][ptidx]->Integral(m_dTFitCoreBkgSubRadiusSideband,FuncdR_Bkg[etaidx][ptidx]->GetXmax());

  
  ///////////////////////////////////////////////
  //fill the signal region
  for(int b=0;b<NMAXTBINS;b++)
    bincount[b] = 0;  
  for(int c=0;c<ncell;c++){
    if(cell_jet[c]!=idx) continue;
    if(cell_jetdR[c] > m_dTFitCoreBkgSubRadius) continue;
    int bin=(cell_dt[c]+scanwindow)/binsize;
    if(bin<0||bin>=NMAXTBINS) continue;
    bincount[bin]++;
  }
  

  ///// Perform background subtraction
  // for(int b=0;b<NMAXTBINS;b++){
  //   if(bincount[b]>bincountbkg[b]*(sigintegral/sideintegral)) 
  //     bincount[b] -= bincountbkg[b]*(sigintegral/sideintegral);
  //   else bincount[b]=0;
  // }
    

  ////// scale the sideband
  float sigtot=0.;
  float bkgtot=0.;
  int ntotbins=0;
  int mincountbkg=3;
  for(int b=0;b<NMAXTBINS;b++){
    sigtot+=bincount[b];
    bkgtot+=bincountbkg[b];
    if(bincount[b]>mincountbkg) ntotbins++;
  }

  float scaleF=0;
  for(int s=50;s<100;s++){
    int nneg=0;
    for(int b=0;b<NMAXTBINS;b++){
      if(bincount[b]<=mincountbkg) continue;
      if(bincount[b] -  (s/100.)*(sigtot/bkgtot)*bincountbkg[b]  < 0)
	nneg++;
    }
    if(nneg>0.4*ntotbins){
      scaleF=(s/100.)*(sigtot/bkgtot);
      break;
    }
  }


  ////find the peak (maximum bin)
  int maxbin=-1; 
  for(int b=0;b<NMAXTBINS;b++){
    if(maxbin==-1 || 
       (bincount[b] - scaleF*bincountbkg[b]) > (bincount[maxbin] - scaleF*bincountbkg[maxbin])
       )
      maxbin=b; 
  }

  ///max bin info
  jet_dTFitCoreBkgSub_ncellsig[idx] = bincount[maxbin] - scaleF*bincountbkg[maxbin];
  jet_dTFitCoreBkgSub_signalmean[idx] = (maxbin+0.5)*binsize - scanwindow;
  jet_dTFitCoreBkgSub_sF[idx] = scaleF;


  ///background level away from max bin
  ///--> jet_dTFitCoreBkgSub_ncellbkg[idx] = totcount;
  int avgbkgcount=0;
  int avgbkgcountmax=0;
  int avgbkgcountabs=0;
  int avgbkgcountbins=0;
  for(int b=0;b<NMAXTBINS;b++){
    if(abs(b-maxbin)<=1) continue;//skip max bin
    avgbkgcount+=(bincount[b] - scaleF*bincountbkg[b]);
    avgbkgcountabs+=abs(bincount[b] - scaleF*bincountbkg[b]);
    avgbkgcountbins++;
    
    if((bincount[b] - scaleF*bincountbkg[b]) > avgbkgcountmax)
       avgbkgcountmax=(bincount[b] - scaleF*bincountbkg[b]);
  }
  if(avgbkgcountbins>0){
    jet_dTFitCoreBkgSub_ncellbkg[idx] = avgbkgcount/avgbkgcountbins; 
    jet_dTFitCoreBkgSub_avgbkg[idx]   = avgbkgcountabs/avgbkgcountbins; 
    jet_dTFitCoreBkgSub_maxbkg[idx]   = avgbkgcountmax; 
  }

  //calculate the number of cells on the peak before background subtraction
  for(int c=0;c<ncell;c++){
    if(cell_jet[c]!=idx) continue;
    if(cell_jetdR[c] > m_dTFitCoreBkgSubRadius) continue;
    jet_dTFitCoreBkgSub_ncell[idx]++;
    for(long t=0;t<NTWINDOW;t++)
      if(fabs(1000*(cell_dt[c]-jet_dTFitCoreBkgSub_signalmean[idx])) < (t+1)*TSTEP ) 
	jet_dTFitCoreBkgSub_ncell_window[t][idx]++;
  }
}


void LArHitTupleDetGeo::fitJetDRDT(int idx){
  //ATH_MSG_INFO ("fitJetDRDT: jet" <<jet_eta[idx]<<" , "<<jet_pt[idx]);

  //init results
  jet_dRdTFit_signalmean[idx]=0.;
  jet_dRdTFit_sig[idx]=0.;
  jet_dRdTFit_siger[idx]=0.;
  jet_dRdTFit_slp[idx]=0.;
  jet_dRdTFit_slper[idx]=0.;
  jet_dRdTFit_chi2[idx]=0.;
  jet_dRdTFit_bkg[idx]=0.;
  jet_dRdTFit_bkger[idx]=0.;

  TF1*FIT=NULL;
  if(fabs(jet_eta[idx])<3.0) FIT=dRFitL;
  else if(fabs(jet_eta[idx])<3.4) FIT=dRFitM;
  else FIT=dRFitH;
  if(!FIT) return;
  
  //FIT->SetParLimits(0,1,100000);  
  //FIT->SetParLimits(1,0.001,100);   
  //FIT->SetParLimits(2,0.001,1000);
  FIT->FixParameter(2,1);

  int range = 800;
  int binsize = 1000*(fabs(jet_eta[idx])<3.2 ? 2*m_HitTimeSmear2 : 2*m_HitTimeSmear1); 
  int dRFit_nbin = int(2.*range/binsize + 1.);

  //ATH_MSG_INFO ("fitJetDRDT: jet" <<jet_eta[idx]<<" , "<<jet_pt[idx]);

  float maxfit=0.;
  float maxsig=0.;
  float maxsiger=0.;
  float maxslp=0.;
  float maxslper=0.;
  float maxbkg=0.;
  float maxbkger=0.;
  float maxt=0.;
  float maxchi2=0.;
  for(int t=0;t<dRFit_nbin;t++){
    float tmin=-range+t*binsize;
    float tmax=-range+(t+1)*binsize;

    TH1F H("H","",dRFitArea->GetNbinsX(),dRFitArea->GetXaxis()->GetXmin(),dRFitArea->GetXaxis()->GetXmax());
    for(int c=0;c<ncell;c++){
      if( cell_jet[c]!=idx) continue;
      if( cell_dt[c]*1000<tmin || tmax<cell_dt[c]*1000 ) continue;
      H.Fill(cell_jetdR[c]);
    }
    if(H.Integral()<=0) continue; //use only non-empty slices
    if(H.GetBinContent(1)<=5.) continue;// use only distributions with a core
    
    H.Divide(dRFitArea);

    H.Fit(FIT,"NQ","",H.GetXaxis()->GetXmin(),H.GetXaxis()->GetXmax());

    ////select largest signal
    if(FIT->GetParameter(1)<maxfit) continue;
    maxfit= FIT->GetParameter(1);

    ///selected largest significance
    //if(FIT->GetParError(1)<=0. || FIT->GetParameter(1)/FIT->GetParError(1) <maxfit) continue;
    //maxfit = FIT->GetParameter(1)/FIT->GetParError(1);
    
    maxbkg   = FIT->GetParameter(0);
    maxbkger = FIT->GetParError(0);
    maxsig   = FIT->GetParameter(1);
    maxsiger = FIT->GetParError(1);
    maxslp   = FIT->GetParameter(2);
    maxslper = FIT->GetParError(2);
    maxchi2  = FIT->GetChisquare()/FIT->GetNDF();
    maxt     = (tmax+tmin)/(2*1000.);
    
    //if(jet_truthJ[idx]>=0 && tmin<jet_truedt[idx] && jet_truedt[idx]<tmax )
    //ATH_MSG_INFO ("dRdTFit:" <<jet_eta[idx]<<", "<<newsig<<", "<<(H.GetBinContent(1)-FIT->Eval(H.GetBinCenter(1)))<<"+/-"<< H.GetBinError(1)<<", "<<(H.GetBinContent(2)-FIT->Eval(H.GetBinCenter(2)))<<"+/-"<< H.GetBinError(2)<<", "<<(H.GetBinContent(3)-FIT->Eval(H.GetBinCenter(3)))<<"+/-"<< H.GetBinError(3)<<", "<<(H.GetBinContent(4)-FIT->Eval(H.GetBinCenter(4)))<<"+/-"<< H.GetBinError(4));
      
  }



  jet_dRdTFit_signalmean[idx] = maxt;
  jet_dRdTFit_bkg[idx]        = maxbkg; 
  jet_dRdTFit_bkger[idx]      = maxbkger; 
  jet_dRdTFit_sig[idx]        = maxsig; 
  jet_dRdTFit_siger[idx]      = maxsiger; 
  jet_dRdTFit_slp[idx]        = maxslp; 
  jet_dRdTFit_slper[idx]      = maxslper; 
  jet_dRdTFit_chi2[idx]       = maxchi2; 
}


void LArHitTupleDetGeo::fitJetDTJetCell(int idx){
  ///algorithm uses all cells in jet cone 

  jet_dTFitJetCell_signalmean[idx]=0.;
  for(long t=0;t<NTWINDOW;t++)
      jet_dTFitJetCell_ncell_window[t][idx]=0;

  float binsize=m_dTSigma*(fabs(jet_eta[idx])<3.2 ? m_HitTimeSmear2 : m_HitTimeSmear1); // use 2*sigma wide bins

  for(int b=0;b<NMAXTBINS;b++){
    bincount[b]=0;
    binmean[b]=0.;
  }
  for(int c=0;c<ncell;c++){
    if( cell_jet[c]==idx){
      int bin=(cell_dt[c]+scanwindow)/(binsize);
      if(0<=bin&&bin<NMAXTBINS){
	bincount[bin]++;
	binmean[bin] += cell_dt[c];
      }
    }
  }

  ////find the maximum bin
  int maxbin=-1; 
  for(int b=0;b<NMAXTBINS;b++){
    if(maxbin==-1 || bincount[b]>bincount[maxbin])
      maxbin=b; 
    
    //also normalize the mean time
    if(bincount[b]>1)
      binmean[b]=binmean[b]/bincount[b];
  }

  float meanmaxbin=0.;
  if(maxbin!=-1)  meanmaxbin = binmean[maxbin];

  //count the number of cells around the signal peak
  for(int c=0;c<ncell;c++)
    for(long t=0;t<NTWINDOW;t++)
      if(cell_clus[c]>=0 && clus_jet[cell_clus[c]]==idx && fabs(1000*(cell_dt[c]-meanmaxbin)) < (t+1)*TSTEP ) 
	jet_dTFitJetCell_ncell_window[t][idx]++;

  jet_dTFitJetCell_signalmean[idx]=meanmaxbin;

}


void LArHitTupleDetGeo::fitJetDTJetCore(int idx){
  

  jet_dTFitCore_truencell[idx]=0;//need to define this
  jet_dTFitCore_signalmean[idx]=0.;//this is now only defined for 30ps window see below

  jet_dTFitCore_ncell[idx]=0;
  for(int c=0;c<ncell;c++){
    if(cell_jet[c] != idx) continue;
    if(cell_jetdR[c] > m_dTFitCoreBkgSubRadius ) continue; 
    jet_dTFitCore_ncell[idx]++;
  }

  for(long t=0;t<NTWINDOW;t++){
    jet_dTFitCore_ncell_window[t][idx]=0;
    jet_dTFitCore_meandt_window[t][idx]=-9999;
  
    //float binsize=m_dTSigma*(fabs(jet_eta[idx])<3.2 ? m_HitTimeSmear2 : m_HitTimeSmear1); //for 60 ps cells 2*sigma is too large
    float binsize=(t+1)*TSTEP/1000.; 

    for(int b=0;b<NMAXTBINS;b++){
      bincount[b]=0;
      binmean[b]=0.;
    }

    for(int c=0;c<ncell;c++){
      if(cell_jet[c]!=idx) continue;

      //note that matching radius should not be larger that 0.2 due to nearby jets
      if(cell_jetdR[c]>m_dTFitCoreBkgSubRadius ) continue; 

      int bin=(cell_dt[c]+scanwindow)/(binsize);
      if(0<=bin&&bin<NMAXTBINS){
	bincount[bin]++;
	binmean[bin] += cell_dt[c];
      }
    }

    ////find the maximum bin
    int maxbin=-1; 
    for(int b=0;b<NMAXTBINS;b++){
      if(maxbin==-1 || bincount[b]>bincount[maxbin]) maxbin=b; 
      if(bincount[b]>1) binmean[b]=binmean[b]/bincount[b];
    }

    if(maxbin!=-1){
      if(t==0) jet_dTFitCore_signalmean[idx]=binmean[maxbin];
      jet_dTFitCore_meandt_window[t][idx]=binmean[maxbin];
      jet_dTFitCore_ncell_window[t][idx]=bincount[maxbin];//just use the maximum bin content
    }

  }

}


void LArHitTupleDetGeo::fitJetDTJetCoreHGTrk(int idx){
  
  jet_dTFitCoreHGTrk_ntrktot[idx]=0;
  jet_dTFitCoreHGTrk_ntrk[idx]=0.;
  jet_dTFitCoreHGTrk_dt[idx]=0;

  //use binning depending on the cell resolution, for tracks use 1 sigma
  float binsize=(m_dTSigma/2)*(fabs(jet_eta[idx])<3.2 ? m_HitTimeSmear2 : m_HitTimeSmear1); 

  for(int b=0;b<NMAXTBINS;b++){
    bincount[b]=0;
    binmean[b]=0.;
  }
  for(int c=0;c<nhgtrk;c++){
    if(hgtrk_jet[c]!=idx) continue;

    //note that matching radius should not be larger that 0.2 due to nearby jets
    if(hgtrk_jetdR[c]>m_dTFitCoreBkgSubRadius ) continue; 

    //remove tracklets pointing away from the jet direction
    if(abs(hgtrk_deta[c]) > 0.5 
       && fabs(hgtrk_deta[c] - jet_eta[hgtrk_jet[c]]) > 0.5 ) continue;
      

    int bin=(hgtrk_dt[c]+scanwindow)/(binsize);
    if(0<=bin&&bin<NMAXTBINS){
      bincount[bin]++;
      binmean[bin] += hgtrk_dt[c];
    }

  }

  ////find the maximum bin
  int maxbin=-1; 
  for(int b=0;b<NMAXTBINS;b++){
    
    jet_dTFitCoreHGTrk_ntrktot[idx]+=bincount[b]; 

    if(maxbin==-1 || bincount[b]>bincount[maxbin])
      maxbin=b; 
    
    //also normalize the mean time
    if(bincount[b]>1)
      binmean[b]=binmean[b]/bincount[b];
  }

  if(maxbin!=-1){
    jet_dTFitCoreHGTrk_dt[idx]=binmean[maxbin];
    jet_dTFitCoreHGTrk_ntrk[idx]=bincount[maxbin];
  }

    
}

void LArHitTupleDetGeo::fitJetDTEw(int idx){
  
  /////initialize
  jet_dTFitEw_signalmean[idx]=0.;
  jet_dTFitEw_Etot_clus[idx]=0.;
  for(long t=0;t<NTWINDOW;t++)
      jet_dTFitEw_Ecell_clus_window[t][idx]=0.;


  float binsize=m_dTSigma*(fabs(jet_eta[idx])<3.2 ? m_HitTimeSmear2 : m_HitTimeSmear1); // use 2*sigma wide bins

  float binenergy[NMAXTBINS];
  for(int b=0;b<NMAXTBINS;b++){
    bincount[b]=0;
    binmean[b]=0.;
    binenergy[b]=0.;
  }
  for(int c=0;c<ncell;c++){
    if(cell_clus[c]>=0  // cell is associated to a cluster
       && clus_jet[cell_clus[c]]==idx  // cluster belongs to the jet
       ){
      int bin=(cell_dt[c]+scanwindow)/(binsize);
      if(0<=bin&&bin<NMAXTBINS){
	bincount[bin]++;
	binenergy[bin] += cell_E[c];
	binmean[bin] += cell_dt[c];
      }
    }
  }
  
  ////find the maximum bin
  int maxbin=-1; 
  for(int b=0;b<NMAXTBINS;b++){
    if(maxbin==-1 || binenergy[b]>binenergy[maxbin])
      maxbin=b; 
    
    //also normalize the mean time
    if(bincount[b]>1)
      binmean[b]=binmean[b]/bincount[b];
  }
  
  
  float meanmaxbin=0.;
  if(maxbin!=-1)  meanmaxbin = binmean[maxbin]; 

  jet_dTFitEw_signalmean[idx]=meanmaxbin;

  //count the number of cells around the signal peak
  for(int c=0;c<ncell;c++){
   
    if(cell_clus[c]>=0 && clus_jet[cell_clus[c]]==idx)
      jet_dTFitEw_Etot_clus[idx] += cell_E[c];
    
    for(long t=0;t<NTWINDOW;t++)
      if(cell_clus[c]>=0 && clus_jet[cell_clus[c]]==idx && fabs(1000*(cell_dt[c]-meanmaxbin)) < (t+1)*TSTEP ) 
	jet_dTFitEw_Ecell_clus_window[t][idx] += cell_E[c];
  }
  
  
}


void LArHitTupleDetGeo::fitJetDTEcw(int idx){
  
  /////initialize
  jet_dTFitEcw_signalmean[idx]=0.;
  jet_dTFitEcw_Etot_clus[idx]=0.;
  for(long t=0;t<NTWINDOW;t++)
      jet_dTFitEcw_Ecell_clus_window[t][idx]=0.;

  float binsize=m_dTSigma*(fabs(jet_eta[idx])<3.2 ? m_HitTimeSmear2 : m_HitTimeSmear1); // use 2*sigma wide bins

  ////first determine the signal position
  float binenergy[NMAXTBINS];
  for(int b=0;b<NMAXTBINS;b++){
    bincount[b]=0;
    binmean[b]=0.;
    binenergy[b]=0.;
  }
  for(int c=0;c<ncell;c++){
    if(cell_clus[c]>=0  // cell is associated to a cluster
       && clus_jet[cell_clus[c]]==idx  // cluster belongs to the jet
       ){
      int bin=(cell_dt[c]+scanwindow)/(binsize);
      if(0<=bin&&bin<NMAXTBINS){
	bincount[bin]++;
	binenergy[bin] += clus_E[cell_clus[c]];
	binmean[bin] += cell_dt[c];
      }
    }
  }
  
  ////find the maximum bin
  int maxbin=-1; 
  for(int b=0;b<NMAXTBINS;b++){
    if(maxbin==-1 || binenergy[b]>binenergy[maxbin])
      maxbin=b; 
    
    //also normalize the mean time
    if(bincount[b]>1)
      binmean[b]=binmean[b]/bincount[b];
  }
  
  
  float meanmaxbin=0.;
  if(maxbin!=-1)  meanmaxbin = binmean[maxbin]; 

  jet_dTFitEcw_signalmean[idx]=meanmaxbin;

  //count the number of cells around the signal peak
  for(int c=0;c<ncell;c++){
   
    if(cell_clus[c]>=0 && clus_jet[cell_clus[c]]==idx)
      jet_dTFitEcw_Etot_clus[idx] += clus_E[cell_clus[c]];
    
    for(long t=0;t<NTWINDOW;t++)
      if(cell_clus[c]>=0 && clus_jet[cell_clus[c]]==idx && fabs(1000*(cell_dt[c]-meanmaxbin)) < (t+1)*TSTEP ) 
	jet_dTFitEcw_Ecell_clus_window[t][idx] += clus_E[cell_clus[c]];
  }
  


  //set the wp
  jet_dTFitEcw_wp70[idx] = 0;
  if(m_VBFJetWPAlgo.compare("ClusCellDTFitFractionEcw")==0){
    int etabin = getJetWPEtaBin(jet_eta[idx]);
    int ptbin = getJetWPPtBin(jet_pt[idx]);
    if(etabin<0 || ptbin<0)
      jet_dTFitEcw_wp70[idx] = 1;      //jets outside the WP definition range are set to pass.
    else if( jet_dTFitEcw_Ecell_clus_window[WPWindowIdx][idx] / float(jet_dTFitEcw_Etot_clus[idx]) > VBFJetWP[etabin][ptbin] )
      jet_dTFitEcw_wp70[idx] = 1;
  }

}

void LArHitTupleDetGeo::fitJetDTFix(int idx){
  
  jet_dTFitFix_sfrac[idx]=0.;
  jet_dTFitFix_sbratio[idx]=0.;
  jet_dTFitFix_minnll[idx]=0.;

  float NLL[101];
  float bestLL=999999.;
  int bestS=-1;
  for(int s=0;s<=90;s++){ //scan the signal strength 
 
    NLL[s]=0.;
    for(int c=0;c<ncell;c++){
      if(cell_clus[c]>=0                    // cell is associated to a cluster
	 && clus_jet[cell_clus[c]]==idx  // cluster belongs to the jet
	 ){
	float PDF_PU = HDTTemplate_PU->GetBinContent( 100*(cell_dt[c]+scanwindow) ); //there are 100 bins in the template histogram
	int sbin = 100*(cell_dt[c] + scanwindow);
	float PDF_Sig = (sbin>0 && sbin<=100) ? HDTTemplate_Sig->GetBinContent( sbin ) : 0. ;
	NLL[s] += - log ((1-s/100.)*PDF_PU + (s/100.)*PDF_Sig);	
      }
    }    

    //find the best signal strength and the uncertainty
    if(NLL[s]<bestLL){      
      bestLL=NLL[s];
      bestS=s;
    }

  }

  jet_dTFitFix_sfrac[idx]=bestS/100.;
  jet_dTFitFix_sbratio[idx]=NLL[0]-bestLL;
  jet_dTFitFix_minnll[idx]=bestLL;

}




bool LArHitTupleDetGeo::passJetHGTDCut(int idx){
  
  if(m_VBFJetWPAlgo.compare("ClusCellDTFitFraction")==0)
    return jet_dTFit_wp70[idx];
  
  if(m_VBFJetWPAlgo.compare("ClusCellDTFitFractionEcw")==0)
    return jet_dTFitEcw_wp70[idx];
  
  return 0;
}




void LArHitTupleDetGeo::findVBFJets(){

  vbf_mjj=0.;
  vbf_j1=-1;
  vbf_j2=-1;

  vbfdt1_mjj=0.;
  vbfdt1_j1=-1;
  vbfdt1_j2=-1;

  vbfdt2_mjj=0.;
  vbfdt2_j1=-1;
  vbfdt2_j2=-1;

  vbfcoin_mjj=0.;
  vbfcoin_j1=-1;
  vbfcoin_j2=-1;

  vbfcoinnowp_mjj=0.;
  vbfcoinnowp_j1=-1;
  vbfcoinnowp_j2=-1;

  vbfcoinnowpj1_mjj=0.;
  vbfcoinnowpj1_j1=-1;
  vbfcoinnowpj1_j2=-1;

  vbfcjv_mjj=0.;
  vbfcjv_j1=-1;
  vbfcjv_j2=-1;

  vbfdt2cjv_mjj=0.;
  vbfdt2cjv_j1=-1;
  vbfdt2cjv_j2=-1;
  
  
  for(int j1=0;j1<njet;j1++){    
    for(int j2=0;j2<njet;j2++){
      
      //  j1 will be the leading one
      if(jet_pt[j2]>=jet_pt[j1]) continue;

      //  apply pT cut 
      if(jet_pt[j2]<m_VBFMinPt) continue;

      //  apply dEta cut
      if(jet_eta[j2]*jet_eta[j1]>0) continue; // opposite hemisphere


      // Mjj cut
      TLorentzVector P4J1;
      P4J1.SetPtEtaPhiM(jet_pt[j1],jet_eta[j1],jet_phi[j1],jet_m[j1]);
      TLorentzVector P4J2;
      P4J2.SetPtEtaPhiM(jet_pt[j2],jet_eta[j2],jet_phi[j2],jet_m[j2]);
      float mjj=(P4J1+P4J2).M();
      if(mjj<m_VBFMinMjj) continue;
      

      //// This is the baseline (no timing cut)
      // choose the pair with the highest mjj
      if(mjj>vbf_mjj){
	vbf_mjj=mjj;
	vbf_j1=j1;
	vbf_j2=j2;
      }

      //require timing cut on one jet
      if( mjj>vbfdt1_mjj && passJetHGTDCut(j2) ){
	vbfdt1_mjj=mjj;
	vbfdt1_j1=j1;
	vbfdt1_j2=j2;
      }

      //apply timing cut on both jets
      if( mjj>vbfdt2_mjj 
	  && (passJetHGTDCut(j1) && passJetHGTDCut(j2)) 
	  ){
	vbfdt2_mjj=mjj;
	vbfdt2_j1=j1;
	vbfdt2_j2=j2;
      }
      
      //apply timing cut on both jets and coincidence 
      if( mjj>vbfcoin_mjj 
	  && (passJetHGTDCut(j1) && passJetHGTDCut(j2))
	  && jet_dTFit_signalmean[j1]!=0. && jet_dTFit_signalmean[j2]!=0.
	  && fabs(jet_dTFit_signalmean[j1]-jet_dTFit_signalmean[j2]) < 0.06
	  ){
	vbfcoin_mjj=mjj;
	vbfcoin_j1=j1;
	vbfcoin_j2=j2;
      }

      //apply timing cut on j2 and coincidence
      if( mjj>vbfcoinnowpj1_mjj 
	  && passJetHGTDCut(j2)
	  && jet_dTFit_signalmean[j1]!=0. && jet_dTFit_signalmean[j2]!=0.
	  && fabs(jet_dTFit_signalmean[j1]-jet_dTFit_signalmean[j2]) < 0.06
	  ){
	vbfcoinnowpj1_mjj=mjj;
	vbfcoinnowpj1_j1=j1;
	vbfcoinnowpj1_j2=j2;
      }

     //apply only coincidence
      if( mjj>vbfcoinnowp_mjj 
	  && jet_dTFit_signalmean[j1]!=0. && jet_dTFit_signalmean[j2]!=0.
	  && fabs(jet_dTFit_signalmean[j1]-jet_dTFit_signalmean[j2]) < 0.06
	  ){
	vbfcoinnowp_mjj=mjj;
	vbfcoinnowp_j1=j1;
	vbfcoinnowp_j2=j2;
      }


      // Selection with central jet veto
      // now check if there is no jet in between
      bool cjv=1;
      bool cjvHGTD=1;
      for(int jc=0;jc<njet;jc++){
	if(jc==j1 || jc==j2) continue; //cannot be one of the VBF jets
	if(jet_pt[jc]<m_VBFMinPt) continue; // must be above the pT cut

	if( (jet_eta[j1] < jet_eta[jc] && jet_eta[jc] < jet_eta[j2])   //in case j1 eta is less than j2
	    || (jet_eta[j2] < jet_eta[jc] && jet_eta[jc] < jet_eta[j1])  // in case j2 eta is less than j1
	    ) cjv=0;

	if(!passJetHGTDCut(jc)) continue; //must pass HGTD
	
	if( (jet_eta[j1] < jet_eta[jc] && jet_eta[jc] < jet_eta[j2])   //in case j1 eta is less than j2
	    || (jet_eta[j2] < jet_eta[jc] && jet_eta[jc] < jet_eta[j1])  // in case j2 eta is less than j1
	    ) cjvHGTD=0;
      }
      
      if( mjj>vbfdt2cjv_mjj && cjv){
	vbfcjv_mjj = mjj;
	vbfcjv_j1 = j1;
	vbfcjv_j2 = j2;
      }
      
      if( mjj>vbfdt2cjv_mjj && passJetHGTDCut(j1) && passJetHGTDCut(j2) && cjvHGTD){
	vbfdt2cjv_mjj = mjj;
	vbfdt2cjv_j1 = j1;
	vbfdt2cjv_j2 = j2;
      }
	
      
    }
  }
  
}



 
void LArHitTupleDetGeo::fitEleDT(int idx){ 

  float binsize=m_dTSigma*(fabs(ele_eta[idx])<3.2 ? m_HitTimeSmear2 : m_HitTimeSmear1); // use 2*sigma wide bins

  for(int b=0;b<NMAXTBINS;b++){
    bincount[b]=0;
    binmean[b]=0.;
  }
 
  for(int c=0;c<ncell;c++){
    if(cell_ele[c]==idx //cell is associated to electron
       && fabs(cell_dt[c]) < 1.0){ //limit range to 500 ps
      int bin=(cell_dt[c]+1.0)/(binsize);
      if(0<=bin&&bin<100){
	bincount[bin]+= cell_E[c];
	binmean[bin] += cell_E[c]*cell_dt[c];    
      }
    }
  }

  ///normalize the bin time
  for(int b=0;b<NMAXTBINS;b++){
    if(bincount[b]>1)
      binmean[b]=binmean[b]/bincount[b];
  }

  ////find the bin with the highest number of entries
  int maxbin=0; 
  for(int b=0;b<NMAXTBINS;b++){
    if(bincount[b]>bincount[maxbin])
      maxbin=b; 
  }
  ele_dTFit_signalmean[idx] = binmean[maxbin];


  //count number of PU cells
  ele_ncellPU[idx] = 0; 
  ele_cellEPU[idx] = 0.; 
  for(int c=0; c<ncell; c++){  
    if(cell_ele[c]==idx){

      if(fabs(cell_dt[c]-ele_dTFit_signalmean[idx]) < (fabs(ele_eta[idx])<3.2 ? 3*m_HitTimeSmear2 : 3*m_HitTimeSmear1))
	cell_elePU[c] = -1;
      else {
	cell_elePU[c] = c; 
	ele_ncellPU[idx]++;
	ele_cellEPU[idx]+=cell_E[c];
      }

    }    
  }

}


StatusCode LArHitTupleDetGeo::execute() {  
  ATH_MSG_DEBUG ("Executing " << name() << "...");
  cutflow[0]++;
  

  ////Print event counter
  if(eventCount % 50 == 0) ATH_MSG_INFO("event "<<eventCount); 
  eventCount++;


  // Create a transient object store. Needed for the tools.                                                                                 
  //xAOD::TStore store;

  ///////////////////////////////////////////
  //truth info
  ///////////////////////////////////////////
  if(processTruth()==StatusCode::FAILURE) return StatusCode::FAILURE;

  if(processTruthJets()==StatusCode::FAILURE) return StatusCode::FAILURE;

  cutflow[1]++;


  
  ///////////////////////////////////////////
  //event info 
  ///////////////////////////////////////////
  if(processEventInfo()==StatusCode::FAILURE) return StatusCode::FAILURE;
  //if(nvtx==0) return StatusCode::SUCCESS;//skip event without a primary vertex//not possible for single particle samples

  cutflow[2]++;

  gRandom->SetSeed(event);



  ///////////////////////////////////////////
  //Retrieve the Jets  (and clusters)
  ///////////////////////////////////////////
  if(processJets()==StatusCode::FAILURE) return StatusCode::FAILURE;
  cutflow[3]++;
   
  ///////////////////////////////////////////
  //Retrieve the MET
  ///////////////////////////////////////////
  //if(processMET()==StatusCode::FAILURE) return StatusCode::FAILURE;
  cutflow[4]++;
  
  ///////////////////////////////////////////
  //Retrieve the Electrons  
  ///////////////////////////////////////////
  if(processElectrons()==StatusCode::FAILURE) return StatusCode::FAILURE;
  cutflow[5]++;

  ///////////////////////////////////////////
  //Retrieve the Tracks  
  ///////////////////////////////////////////
  if(processTracks()==StatusCode::FAILURE) return StatusCode::FAILURE;
  setObjectTracks();
  cutflow[6]++;

  ///////////////////////////////////////////
  ///Retrieve the HGTD hits and fill the cells
  ///////////////////////////////////////////
  if(processCells()==StatusCode::FAILURE) return StatusCode::FAILURE;
  //fillHGTracks();//these are the perpendicular
  fillHGTDTracks();//this is the general local tracklet
  setTrackHGTDTrks();
  cutflow[7]++;

  ////////////////////////////////////////////////
  //// associate cells to tracks
  //////////////////////////////////////////
  associateTrackCells();
  setTrackCells();
  cutflow[8]++;

  ///////////////////////////////////////////////////
  //associate cells to  clusters
  ///////////////////////////////////////////////////
  associateClusterCells();
  setClusterCells();
  cutflow[9]++;

  ///////////////////////////////////////////////////
  //associate cells to electrons
  ///////////////////////////////////////////////////
  associateElectronCells();
  setElectronCells();
  cutflow[10]++;

  ///////////////////////////////////////////////////
  //associate cells to jets
  ///////////////////////////////////////////////////
  associateJetCells();
  setJetCells();
  cutflow[11]++;

  ///////////////////////////////////////////////////
  //process trigger tiles
  ///////////////////////////////////////////////////
  //associateTriggerCells();
  //processTrigger();
  cutflow[12]++;

  //////////////////////////////////
  //// find VBF jets
  ///////////////////
  //findVBFJets();
  cutflow[13]++;
  
  //////////apply event selections
  if(mu!=0 && bcid%80 < 12) return StatusCode::SUCCESS; 
  cutflow[14]++;

  if(m_selectJetEvents && njet<=0 ) return StatusCode::SUCCESS;
  cutflow[15]++;

  if(m_selectElectronEvents && nele<=0 ) return StatusCode::SUCCESS;
  cutflow[16]++;


  ///fill Tree
  m_tree->Fill();

  return StatusCode::SUCCESS;
}

StatusCode LArHitTupleDetGeo::beginInputFile() {  
  //example of metadata retrieval:
  //float beamEnergy(0); CHECK( retrieveMetadata("/TagInfo","beam_energy",beamEnergy) );
  //std::vector<float> bunchPattern; CHECK( retrieveMetadata("/Digitiation/Parameters","BeamIntensityPattern",bunchPattern) );


  return StatusCode::SUCCESS;
}



bool LArHitTupleDetGeo::passDeadZone(float x,float y){
  //note: this removes G4 hits not HGTD cells
  //HGTD cells may currently be not aligned with these dead zones

  ///Use ony absolute values since detector is symmetric in x->-x, and y->-y
  float X=fabs(x);
  float Y=fabs(y);

  ///remove inner square hole
  if(X<ASU/2 && Y<ASU/2) return 0;
  
  
  ////////////////////////////
  /// Remove horizontal dead lines
  /// based on first quadrant: https://indico.cern.ch/event/555345/ (Aboud page 7 and 11)
  //////////////////////////
  ///remove horizontal line above center ASU row
  if( 0.5*ASU - ASU_boundary < Y && Y < 0.5*ASU + ASU_boundary) return 0;

  ///remove horizontal line above third ASU row from top
  if( 1.5*ASU - ASU_boundary < Y && Y < 1.5*ASU + ASU_boundary) return 0;

  ///remove horizontal line above second ASU row from top
  if( 2.5*ASU - ASU_boundary < Y && Y < 2.5*ASU + ASU_boundary) return 0;

  ////////////////////////////
  /// Remove vertical dead lines
  /// based on first quadrant: https://indico.cern.ch/event/555345/ (Aboud page 7 and 11)
  //////////////////////////
  //remove Vertical division at x=0
  if(X < LRDETSeparation/2 + ASU_boundary) return 0; //1.5 mm is separation between L and R detector halves
  
  //remove first ASU boundary (only above y=100)
  if(0.5*ASU < Y && 
     LRDETSeparation/2 + 1.0*ASU - ASU_boundary < X && X < LRDETSeparation/2 + 1.0*ASU + ASU_boundary) return 0;
  
  //remove second ASU boundary second ASU row from top
  if(1.5*ASU < Y && Y < 2.5*ASU &&
     LRDETSeparation/2 + 2.0*ASU - ASU_boundary < X && X < LRDETSeparation/2 + 2.0*ASU + ASU_boundary) return 0;
  
  //remove second ASU boundary for third and fourth ASU row from top
  if(Y < 1.5*ASU &&
     LRDETSeparation/2 + 1.5*ASU - ASU_boundary < X && X < LRDETSeparation/2 + 1.5*ASU + ASU_boundary) return 0;
  
  //remove last ASU boundary for third and fourth ASU rows from top
  if(Y < 1.5*ASU &&
     LRDETSeparation/2 + 2.5*ASU - ASU_boundary < X && X < LRDETSeparation/2 + 2.5*ASU + ASU_boundary) return 0;
  

  ////remove the dead zones between the sensors inside the ASU ( 4 sensors per squre ASU) 
  int nasux=(X-LRDETSeparation/2)/(ASU/2);
  if( (X-LRDETSeparation/2-nasux*ASU/2) < SENSOR_boundary || (X-LRDETSeparation/2-nasux*ASU/2) > ASU/2-SENSOR_boundary ) return 0;

  int nasuy=Y/(ASU/2);
  if( (Y-nasuy*ASU/2) < SENSOR_boundary || (Y-nasuy*ASU/2) > ASU/2-SENSOR_boundary ) return 0;


  return 1;
}




bool LArHitTupleDetGeo::passDeadZoneStripStandard(float x,float y){
  //note: this removes G4 hits not HGTD cells
  //HGTD cells may currently be not aligned with these dead zones

  ///detector is symetric in y->-y and x->-x
  float X=fabs(x);
  float Y=fabs(y);

  ///remove the strip horizontal boundaries
  for(int i=0;i<16;i++) if ( fabs(Y-i*40.)<0.5 ) return 0; 
  
  //remove the inner hole, there are three strips strips.
  if(Y<40 && X<120) return 0;
  if(Y<80 && X<114) return 0;
  if(Y<120 && X<90) return 0;

  return 1;
}

// Section of includes for LAr calo tests
#include "LArSimEvent/LArHitContainer.h"
#include "CaloDetDescr/CaloDetDescrElement.h"
#include "CaloDetDescr/CaloDetDescrManager.h"
#include "GeoAdaptors/GeoLArHit.h"

// Section of includes for tile calo tests
#include "TileDetDescr/TileDetDescrManager.h"
#include "CaloIdentifier/TileID.h"
#include "TileSimEvent/TileHit.h"
#include "TileSimEvent/TileHitVector.h"

//#include "CaloIdentifier/HGTD_ID.h"

#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/IToolSvc.h"
#include "GaudiKernel/ITHistSvc.h"

#include "StoreGate/StoreGateSvc.h"

#include "EventInfo/EventInfo.h"
#include "EventInfo/EventID.h"

#include "TTree.h"
#include "TString.h"

#include "HitAnalysis/HGTDHitAnalysis.h"

#include <algorithm>
#include <math.h>
#include <functional>
#include <iostream>

HGTDHitAnalysis::HGTDHitAnalysis(const std::string& name, ISvcLocator* pSvcLocator)
   : AthAlgorithm(name, pSvcLocator)
   , m_storeGate(0)
   , m_cell_eta(0)
   , m_cell_phi(0)
   , m_cell_e(0)
   , m_cell_radius(0)
   , m_tree(0)
   , m_ntupleFileName("/ntuples/file1")
   , m_ntupleDirName("HGTDHitAnalysis")
   , m_ntupleTreeName("HGTDHitAna")
   , m_thistSvc(0)
   , m_tileID(0)
   , m_tileMgr(0)
{
  declareProperty("NtupleFileName", m_ntupleFileName); 
  declareProperty("NtupleDirectoryName", m_ntupleDirName); 
  declareProperty("NtupleTreeName", m_ntupleTreeName); 
}


HGTDHitAnalysis::~HGTDHitAnalysis() 
{;}

StatusCode HGTDHitAnalysis::initialize() {
  ATH_MSG_DEBUG( "Initializing HGTDHitAnalysis" );

  StoreGateSvc* detStore;
  StatusCode sc = service("DetectorStore",detStore);
  if (sc.isFailure()) {
    ATH_MSG_ERROR( "Unable to get pointer to Detector Store Service" );
    return sc;
  }

  sc = detStore->retrieve(m_tileMgr);
  if (sc.isFailure()) {
    ATH_MSG_ERROR( "Unable to retrieve TileDetDescrManager from DetectorStore" );
    m_tileMgr=0;
  }

  sc = detStore->retrieve(m_tileID);
  if (sc.isFailure()) { 
    ATH_MSG_ERROR( "Unable to retrieve TileID helper from DetectorStore" );
    m_tileID=0;
  }

  // // retrieve the HGTD_ID
  // sc = detStore->retrieve(m_hgtdID);
  // if (sc.isFailure()) { 
  //   ATH_MSG_ERROR( "Unable to retrieve HGTD_ID helper from DetectorStore" );
  //   m_hgtdID=0;
  // }

  /** get a handle of StoreGate for access to the Event Store */
  sc = service("StoreGateSvc", m_storeGate);
  if (sc.isFailure()) {
     ATH_MSG_ERROR( "Unable to retrieve pointer to StoreGateSvc" );
     return sc;
  }

  // Grab the Ntuple and histogramming service for the tree
  sc = service("THistSvc",m_thistSvc);
  if (sc.isFailure()) {
     ATH_MSG_ERROR( "Unable to retrieve pointer to THistSvc" );
     return sc;
  }
 
  m_tree = new TTree( TString(m_ntupleTreeName), "HGTDHitAna" );
  std::string fullNtupleName =  "/"+m_ntupleFileName+"/"+m_ntupleDirName+"/"+m_ntupleTreeName; 
  sc = m_thistSvc->regTree(fullNtupleName, m_tree); 
  if (sc.isFailure()) { 
     ATH_MSG_ERROR("Unable to register TTree: " << fullNtupleName); 
     return sc; 
  }  

  /** now add branches and leaves to the tree */
  if (m_tree){
    m_tree->Branch("CellEta", &m_cell_eta);
    m_tree->Branch("CellPhi", &m_cell_phi);
    m_tree->Branch("CellE", &m_cell_e);
    m_tree->Branch("CellRadius", &m_cell_radius);
  }

  //std::cout << "Dirk the helper is " << m_hgtdID << std::endl;

  return StatusCode::SUCCESS;
}		 

StatusCode HGTDHitAnalysis::finalize() {
  return StatusCode::SUCCESS;
}

StatusCode HGTDHitAnalysis::execute() {
  ATH_MSG_DEBUG( "In HGTDHitAnalysis::execute()" );

  m_cell_eta->clear();
  m_cell_phi->clear();
  m_cell_e->clear();
  m_cell_radius->clear();
  std::cout << "Dirk Execute" << std::endl;

  //  std::string  lArKey [4] = {"LArHitEMB", "LArHitEMEC", "LArHitFCAL", "LArHitHEC"};
  std::string  lArKey [1] = {"LArHitHGTD"};
  //  for (unsigned int i=0;i<4;i++){
  for (unsigned int i=0;i<1;i++){
    const DataHandle<LArHitContainer> iter;
    if (m_storeGate->retrieve(iter,lArKey[i])==StatusCode::SUCCESS) {
      LArHitContainer::const_iterator hi;
      std::cout << "DIRK retrieved " << lArKey[i] << std::endl;
      for (hi= iter->begin();hi!=iter->end();hi++){
	std::cout << "DIRK hit channelID " << (*hi)->cellID()  << " " << (*hi)->energy() << " time " << (*hi)->time() << std::endl;
	/*        GeoLArHit ghit(**hi);
        if (!ghit) continue;
        const HGTDDetDescrElement *hitElement = ghit.getDetDescrElement();
        m_cell_e->push_back( ghit.Energy() );
        m_cell_eta->push_back( hitElement->eta() );
        m_cell_phi->push_back( hitElement->phi() );
        m_cell_radius->push_back( hitElement->r() );        */
      } // End while hits
    }    // End statuscode success upon retrieval of hits
  }   // End detector type loop

  /*      m_cell_e->push_back( tot_e );
      m_cell_eta->push_back( ddElement->eta() );
      m_cell_phi->push_back( ddElement->phi() );
      m_cell_radius->push_back( ddElement->r() ); */

  if (m_tree) m_tree->Fill();

  return StatusCode::SUCCESS;
}


#include "HGTDAnalysisTools/LArHitRead.h"

#include "LArSimEvent/LArHitContainer.h"
#include "CaloDetDescr/CaloDetDescrElement.h"
#include "CaloDetDescr/CaloDetDescrManager.h"
//#include "GeoAdaptors/GeoLArHit.h"
//#include "CaloIdentifier/CaloCell_ID.h"
#include "CaloIdentifier/CaloIdManager.h"
#include "CaloIdentifier/HGTD_ID.h"
//#include "Identifier/Identifier.h"

#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/IToolSvc.h"
#include "GaudiKernel/ITHistSvc.h"

#include "StoreGate/StoreGate.h"

#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>


LArHitRead::LArHitRead( const std::string& name, ISvcLocator* pSvcLocator ) : AthAlgorithm( name, pSvcLocator ){

  declareProperty( "InputCollectionName", InputCollectionName ); //example property declaration

}


LArHitRead::~LArHitRead() {}


StatusCode LArHitRead::initialize() {
  ATH_MSG_INFO ("Initializing " << name() << "...");

  m_outfile = new TFile("hits.root","RECREATE");
  if(!m_outfile || m_outfile->IsZombie()){
    ATH_MSG_ERROR ("Failed to create the output file" );
    return StatusCode::FAILURE;
  }

  ATH_MSG_DEBUG ("Output File has been created");

  //create the ntuple
  m_tree = new TTree("tuple","tuple");
  m_tree->SetDirectory(m_outfile);

  m_tree->Branch("event",&event,"event/l");
  m_tree->Branch("run",&run,"run/I");
  
  m_tree->Branch("nhit",&nhit,"nhit/I");
  m_tree->Branch("hit_t",hit_t,"hit_t[nhit]/F");
  m_tree->Branch("hit_E",hit_E,"hit_E[nhit]/F");
  m_tree->Branch("hit_x",hit_x,"hit_x[nhit]/F");
  m_tree->Branch("hit_y",hit_y,"hit_y[nhit]/F");
  m_tree->Branch("hit_z",hit_z,"hit_z[nhit]/F");
  m_tree->Branch("hit_phi",hit_phi,"hit_phi[nhit]/F");
  m_tree->Branch("hit_eta",hit_eta,"hit_eta[nhit]/F");
  m_tree->Branch("hit_S",hit_S,"hit_S[nhit]/I");

  return StatusCode::SUCCESS;
}

StatusCode LArHitRead::finalize() {
  ATH_MSG_INFO ("Finalizing " << name() << "...");
  
  m_outfile->cd();
  m_tree->Write();
  m_outfile->ls();
  m_outfile->Close();
  delete m_outfile;

  return StatusCode::SUCCESS;
}

StatusCode LArHitRead::execute() {  
  ATH_MSG_DEBUG ("Executing " << name() << "...");
 
  ///////////////////////////////////////////
  ///Retrieve the HGTD hits
  ///////////////////////////////////////////
  const DataHandle<LArHitContainer> p_collection ;

  StatusCode sc =  StoreGate::instance().retrieve( p_collection,InputCollectionName.c_str()) ;
  if (sc.isFailure() || !p_collection) {
    ATH_MSG_DEBUG ("Failed to retrieve collection : "<<InputCollectionName.c_str());
    return StatusCode::FAILURE;
  } 

  ATH_MSG_DEBUG ("Found Collection "<< InputCollectionName.c_str() << ", with size: "<< p_collection->size()<<", Going to read. ");


  ///Get the Identifier
  static const CaloDetDescrManager *_man;
  StoreGateSvc *detStore = StoreGate::pointer("DetectorStore");
  if(detStore->retrieve(_man).isFailure()){
    ATH_MSG_DEBUG ("LArHitRead::execute() WARNING  Unable to retrieve CaloDetDescrManager");
  }

  
  //const CaloCell_ID * CaloCell = _man->getCaloCell_ID();
  //const LArEM_ID *    ID       = CaloCell->em_idHelper();

  //retrieve ID helpers
  const DataHandle<CaloIdManager> caloIdMgr;
  if ((detStore->retrieve(caloIdMgr)).isFailure()) {
    ATH_MSG_ERROR(" Unable to retrieve CaloIdManager from DetectoreStore");
    return StatusCode::FAILURE;
  }   
  const HGTD_ID *    ID       = caloIdMgr->getHGTD_ID();//CaloCell->hgtd_idHelper();


  ///fill ntuple
  nhit=0;  
  for (LArHitContainer::const_iterator hi= p_collection->begin(); hi!=p_collection->end() && nhit<NMAXHITS; hi++){
    ATH_MSG_DEBUG ("LArHitRead::execute  cellID:" << (*hi)->cellID()  << "  energy:" << (*hi)->energy() << " time:" << (*hi)->time());

    hit_t[nhit] = (*hi)->time();
    hit_E[nhit] = (*hi)->energy();

    hit_S[nhit] = ID->barrel_ec((*hi)->cellID()) * ( ID->sampling((*hi)->cellID())  + 1);
    hit_x[nhit] = ID->x_index((*hi)->cellID());
    hit_y[nhit] = ID->y_index((*hi)->cellID());
    //hit_eta[nhit] = ID->eta((*hi)->cellID());
    //hit_phi[nhit] = ID->phi((*hi)->cellID());

    nhit++;
  }

  m_tree->Fill();
 
  return StatusCode::SUCCESS;
}

StatusCode LArHitRead::beginInputFile() {  
  //example of metadata retrieval:
  //float beamEnergy(0); CHECK( retrieveMetadata("/TagInfo","beam_energy",beamEnergy) );
  //std::vector<float> bunchPattern; CHECK( retrieveMetadata("/Digitiation/Parameters","BeamIntensityPattern",bunchPattern) );


  return StatusCode::SUCCESS;
}


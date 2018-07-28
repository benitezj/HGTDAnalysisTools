#ifndef HGTD_HIT_ANALYSIS_H
#define HGTD_HIT_ANALYSIS_H

#include "GaudiKernel/ToolHandle.h"
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/ObjectVector.h"
#include "CLHEP/Units/SystemOfUnits.h"
#include "StoreGate/StoreGateSvc.h"

#include "AthenaBaseComps/AthAlgorithm.h"

#include <string>

#include "TH1.h"

//class HGTD_ID;
class TileID;
class TileDetDescrManager;
class TTree;
class ITHistSvc;
 
class HGTDHitAnalysis : public AthAlgorithm {

 public:

   HGTDHitAnalysis(const std::string& name, ISvcLocator* pSvcLocator);
   ~HGTDHitAnalysis();

   virtual StatusCode initialize();
   virtual StatusCode finalize();
   virtual StatusCode execute();

 private:

   /** a handle on Store Gate for access to the Event Store */
   StoreGateSvc* m_storeGate;

   /** Simple variables by Ketevi */
   std::vector<float>* m_cell_eta;
   std::vector<float>* m_cell_phi;
   std::vector<float>* m_cell_e;
   std::vector<float>* m_cell_radius;

   TTree * m_tree;
   std::string m_ntupleFileName; 
   std::string m_ntupleDirName; 
   std::string m_ntupleTreeName; 

   ITHistSvc * m_thistSvc;

   const TileID * m_tileID;
   const TileDetDescrManager * m_tileMgr;

  //const HGTD_ID * m_hgtdID;
};

#endif // HGTD_HIT_ANALYSIS_H


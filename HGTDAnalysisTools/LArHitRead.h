#ifndef HGTDANALYSISTOOLS_LARHITREAD_H
#define HGTDANALYSISTOOLS_LARHITREAD_H 1


#include "GaudiKernel/ToolHandle.h"
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/ObjectVector.h"
#include "CLHEP/Units/SystemOfUnits.h"
#include "AthenaBaseComps/AthAlgorithm.h"

#define NMAXHITS 200000
class TTree;
class TFile;

class LArHitRead: public ::AthAlgorithm{ 
 public: 
  LArHitRead( const std::string& name, ISvcLocator* pSvcLocator );
  virtual ~LArHitRead(); 

  virtual StatusCode  initialize();
  virtual StatusCode  execute();
  virtual StatusCode  finalize();
  
  virtual StatusCode beginInputFile();

 private: 

  std::string InputCollectionName;


  TTree * m_tree=0; 
  TFile * m_outfile=0;

  int run;
  int event;
 
  int nhit;
  float hit_t[NMAXHITS];
  float hit_E[NMAXHITS];
  float hit_x[NMAXHITS];
  float hit_y[NMAXHITS];
  float hit_z[NMAXHITS];
  float hit_eta[NMAXHITS];
  float hit_phi[NMAXHITS];
  int hit_S[NMAXHITS];


}; 

#endif 

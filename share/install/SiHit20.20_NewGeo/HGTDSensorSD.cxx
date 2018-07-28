//
// HGTD Sensitive Detector.
// The Hits are processed here. For every hit I get the position and
// an information on the sensor in which the interaction happened
//

// Class header
#include "HGTDSensorSD.h"

// Athena headers
#include "MCTruth/TrackHelper.h"

// Geant4 headers
#include "G4ChargedGeantino.hh"
#include "G4Geantino.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"

// CLHEP headers
#include "CLHEP/Geometry/Transform3D.h"
#include "CLHEP/Units/SystemOfUnits.h"
#include <stdint.h>
#include <string.h>

// For make unique
#include "CxxUtils/make_unique.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HGTDSensorSD::HGTDSensorSD(const std::string& name, const std::string& hitCollectionName)
  : G4VSensitiveDetector( name )
  , m_HitColl( hitCollectionName )
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Initialize from G4 - necessary to new the write handle for now
void HGTDSensorSD::Initialize(G4HCofThisEvent *)
{
  if (!m_HitColl.isValid()) m_HitColl = CxxUtils::make_unique<SiHitCollection>();
 layer1p=true;
 layer2p=true;
 layer3p=true;
 layer4p=true;

 layer1m=true;
 layer2m=true;
 layer3m=true;
 layer4m=true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool HGTDSensorSD::ProcessHits(G4Step* aStep, G4TouchableHistory* /*ROhist*/)
{
  if (verboseLevel>5) G4cout << "Process Hit" << G4endl;

  G4double edep = aStep->GetTotalEnergyDeposit();
  edep *= CLHEP::MeV;

  //  double double_edep = aStep->GetTotalEnergyDeposit();

  if(edep==0.) {
    if(aStep->GetTrack()->GetDefinition() != G4Geantino::GeantinoDefinition() &&
       aStep->GetTrack()->GetDefinition() != G4ChargedGeantino::ChargedGeantinoDefinition())
      return false;
  }

  //use the global time. i.e. the time from the beginning of the event
  //
  // Get the Touchable History:
  //
  G4TouchableHistory*  myTouch = (G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable());

  if(verboseLevel>5){
    for (int i=0;i<myTouch->GetHistoryDepth();i++){
      std::string detname=myTouch->GetVolume(i)->GetLogicalVolume()->GetName();
      int copyno=myTouch->GetVolume(i)->GetCopyNo();
      G4cout << "Volume " <<detname <<" Copy Nr. " << copyno << G4endl;
    }
  }

  std::string detname_layer=myTouch->GetVolume(0)->GetLogicalVolume()->GetName();

  //
  // Get the hit coordinates. Start and End Point
  //
  G4ThreeVector coord1 = aStep->GetPreStepPoint()->GetPosition();
  G4ThreeVector coord2 = aStep->GetPostStepPoint()->GetPosition();

  G4double TrackTotalEnergy = aStep->GetTrack()->GetTotalEnergy();
  TrackTotalEnergy *= CLHEP::MeV;

  // Calculate the local step begin and end position.
  // From a G4 FAQ:
  // http://geant4-hn.slac.stanford.edu:5090/HyperNews/public/get/geometry/17/1.html
  // This transformation allows to get the local position inside the physical volume
  // and with respect to it. 

  const G4AffineTransform transformation = myTouch->GetHistory()->GetTopTransform();
  G4ThreeVector localPosition1 = transformation.TransformPoint(coord1);
  G4ThreeVector localPosition2 = transformation.TransformPoint(coord2);

  HepGeom::Point3D<double> lP1,lP2;
  // enum
  // { xDep = 0, xPhi = 1, xEta = 2};

  //
  // Get it into a vector in absolute coords and with the right units:

  lP1[SiHit::xEta] = coord1.z()*CLHEP::mm;
  lP1[SiHit::xPhi] = coord1.y()*CLHEP::mm;
  lP1[SiHit::xDep] = coord1.x()*CLHEP::mm;

  lP2[SiHit::xEta] = coord2.z()*CLHEP::mm;
  lP2[SiHit::xPhi] = coord2.y()*CLHEP::mm;
  lP2[SiHit::xDep] = coord2.x()*CLHEP::mm;

  // get the barcode from the track helper
  TrackHelper trHelp(aStep->GetTrack());
  int barcode = trHelp.GetBarcode();

  int BrlEcap = 0;
  int LayerDisk = 0;
  int etaMod = 0;
  int phiMod = 0;
  int side = 0;

  int pdgID = aStep->GetTrack()->GetDefinition()->GetPDGEncoding();

  // Define the Silicon layer
  // if(detname_layer == "LArMgr::HGTDsi1" ) LayerDisk=1;
  // else if(detname_layer == "LArMgr::HGTDsi2" ) LayerDisk=2;
  // else if(detname_layer == "LArMgr::HGTDsi3" ) LayerDisk=3;
  // else if(detname_layer == "LArMgr::HGTDsi4" ) LayerDisk=4;
  // else {
  //   // Do not expect other names. Need to fix HGTDGeoModel if this occurs.
  //   G4ExceptionDescription description;
  //   description << "ProcessHits: Unrecognized geometry in HGTD sensitive detector. Please contact the maintainer of the HGTD Detector Description.";
  //   G4Exception("HGTDSensorSD", "UnrecognizedHGTDGeometry", FatalException, description);
  //   abort();
  // }

  if ( detname_layer.find("Sensor") == std::string::npos ) {
    // Do not expect other names. Need to fix HGTDGeoModel if this occurs.
    G4ExceptionDescription description;
    description << "ProcessHits: No HGTD sensitive detector with substring Sensor found. Check HGTD Detector Description.";
    G4Exception("HGTDSensorSD", "UnrecognizedHGTDGeometry", FatalException, description);
    abort();
  } 
  // Define the Silicon layer from the index
  LayerDisk = atoi(detname_layer.substr(detname_layer.size()-1,1).c_str());


  int BEcopyNo =  myTouch->GetVolume()->GetCopyNo();

  float timing=aStep->GetPreStepPoint()->GetGlobalTime();

  // std::cout << " Hit Layer: " <<  detname_layer << std::endl;
  // std::cout << " timing " << timing << std::endl;
  // std::cout << "TrackTotalEnergy  " << TrackTotalEnergy << std::endl;
  // std::cout << "TotalEnergyDeposit  " << edep << std::endl;
  
  // std::cout << " LayerDisk " << LayerDisk << std::endl;
  // std::cout << " PreStepPoint " <<  std::endl;
  // std::cout << " x (global/local) " << coord1.x()*CLHEP::mm << " " << localPosition1[0]*CLHEP::mm << std::endl;
  // std::cout << " y (global/local) " << coord1.y()*CLHEP::mm << " " << localPosition1[1]*CLHEP::mm << std::endl;
  // std::cout << " z (global/local) " << coord1.z()*CLHEP::mm << " " << localPosition1[2]*CLHEP::mm << std::endl;   
  
  // std::cout << " PostStepPoint: " <<  std::endl;
  // std::cout << " x (global/local) " << coord2.x()*CLHEP::mm << " " << localPosition2[0]*CLHEP::mm << std::endl;
  // std::cout << " y (global/local) " << coord2.y()*CLHEP::mm << " " << localPosition2[1]*CLHEP::mm << std::endl;
  // std::cout << " z (global/local) " << coord2.z()*CLHEP::mm << " " << localPosition2[2]*CLHEP::mm << std::endl;

  m_HitColl->Emplace(lP1,
                     lP2, edep,
                     timing,
                     barcode,
                     0,BrlEcap,LayerDisk,etaMod,phiMod,side);

  return true;
}


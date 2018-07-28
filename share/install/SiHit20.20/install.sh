setupATLAS
asetup 20.20.2.2,AtlasProduction,here

INPATH=HGTDAnalysisTools/share/install/SiHit20.20

cmt co -r HGTDG4SD-00-00-02 LArCalorimeter/LArG4/HGTDG4SD/
/bin/cp $INPATH/requirements  LArCalorimeter/LArG4/HGTDG4SD/cmt/

pkgco.py AthenaCommon
/bin/cp $INPATH/DetFlags.py Control/AthenaCommon/python/

pkgco.py G4AtlasTools
/bin/cp $INPATH/G4AtlasToolsConfig.py Simulation/G4Atlas/G4AtlasTools/python/

##fix for R down to 47mm
pkgco.py LArGeoEndcap-00-00-92
/bin/cp $INPATH/EndcapCryostatConstruction.cxx LArCalorimeter/LArGeoModel/LArGeoEndcap/src/

pkgco.py G4AtlasApps
/bin/cp $INPATH/atlas_common.py Simulation/G4Atlas/G4AtlasApps/python/

#fix for empty hit events
pkgco.py ISF_Services
/bin/cp $INPATH/SimHitSvc.h Simulation/ISF/ISF_Core/ISF_Services/src/
/bin/cp $INPATH/SimHitSvc.cxx Simulation/ISF/ISF_Core/ISF_Services/src/

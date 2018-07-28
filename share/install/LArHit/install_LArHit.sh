#####################
##Installation in 20.20.2.1, 
## uses LArHit class but wrong (LArEM_ID) identifiers
#######################
pkgco.py AthenaCommon
/bin/cp HGTDAnalysisTools/share/install/DetFlags.py Control/AthenaCommon/python/

pkgco.py G4AtlasTools
/bin/cp HGTDAnalysisTools/share/install/G4AtlasToolsConfig.py Simulation/G4Atlas/G4AtlasTools/python/

##needed with the HGTD geometry
pkgco.py IdDictParser
/bin/cp HGTDAnalysisTools/share/install/LArHit/IdDictLArCalorimeter_HGTD_01.xml DetectorDescription/IdDictParser/data/

cmt co -r LArGeoEndcap-00-00-92 LArCalorimeter/LArGeoModel/LArGeoEndcap/
cmt co -r HGTDG4SD-00-00-02 LArCalorimeter/LArG4/HGTDG4SD/

##replace HGTD SD files with ones using LArHit class
/bin/cp HGTDAnalysisTools/share/install/LArHit/HGTDSensorSD* LArCalorimeter/LArG4/HGTDG4SD/src/
/bin/cp HGTDAnalysisTools/share/install/LArHit/requirements LArCalorimeter/LArG4/HGTDG4SD/cmt/

/bin/cp HGTDAnalysisTools/share/install/preInclude.HGTD.py preInclude.HGTD.py

##Compile
cmt find_packages
cmt compile

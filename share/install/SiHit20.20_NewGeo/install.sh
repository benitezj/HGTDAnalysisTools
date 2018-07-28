setupATLAS
asetup 20.20.0.1,here

####Install from eos
##alias CP="eos cp"
##INPATH=/eos/atlas/atlascerngroupdisk/det-hgtd/software

####install from local
alias CP="/bin/cp"
INPATH=HGTDAnalysisTools/share/install/SiHit20.20_NewGeo

###################################
pkgco.py AthenaCommon-03-03-08-01
pkgco.py IdDictParser-00-02-35
pkgco.py LArGeoEndcap-00-00-93

cmt co -r HGTDG4SD-00-00-02 LArCalorimeter/LArG4/HGTDG4SD/
CP $INPATH/requirements  LArCalorimeter/LArG4/HGTDG4SD/cmt/
CP $INPATH/HGTDSensorSD.cxx LArCalorimeter/LArG4/HGTDG4SD/src/
CP $INPATH/HGTDG4SDConfig.py LArCalorimeter/LArG4/HGTDG4SD/python/

pkgco.py G4AtlasTools
CP $INPATH/G4AtlasToolsConfig.py Simulation/G4Atlas/G4AtlasTools/python/

pkgco.py G4AtlasApps
CP $INPATH/atlas_common.py Simulation/G4Atlas/G4AtlasApps/python/

pkgco.py ISF_Services
CP $INPATH/SimHitSvc.h Simulation/ISF/ISF_Core/ISF_Services/src
CP $INPATH/SimHitSvc.cxx Simulation/ISF/ISF_Core/ISF_Services/src


#####################################################
#This dictionary file appears to be needed for reading the hits from the above installation
mkdir InDetIdDictFiles
CP $INPATH/IdDictInnerDetector_SLHC_ExtBrl_4.xml InDetIdDictFiles/

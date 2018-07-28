setupATLAS
asetup 20.20.0.1,here

###Install from eos
#alias CP="eos cp"
#INPATH=/eos/atlas/atlascerngroupdisk/det-hgtd/software

###install from local
alias CP="/bin/cp -f"
INPATH=HGTDAnalysisTools/share/install/LArHit20.20May4

CP $INPATH/preInclude.HGTD.py .
CP $INPATH/preInclude.MuonOff.py .

pkgco.py AthenaCommon-03-03-08-01
pkgco.py IdDictParser-00-02-35
pkgco.py LArSimEventTPCnv
pkgco.py LArGeoEndcap-00-00-93
pkgco.py CaloIdCnv-00-00-42-02
pkgco.py DetDescrCnvSvc-00-04-11-01

pkgco.py CaloIdentifier
CP $INPATH/CaloIdManager.h Calorimeter/CaloIdentifier/CaloIdentifier/CaloIdManager.h
CP $INPATH/CaloID.h Calorimeter/CaloIdentifier/CaloIdentifier/CaloID.h  
CP $INPATH/CaloCell_ID.h  Calorimeter/CaloIdentifier/CaloIdentifier/CaloCell_ID.h         
CP $INPATH/CaloCell_Base_ID.icc  Calorimeter/CaloIdentifier/CaloIdentifier/CaloCell_Base_ID.icc 
CP $INPATH/CaloCell_Base_ID.h Calorimeter/CaloIdentifier/CaloIdentifier/CaloCell_Base_ID.h
CP $INPATH/CaloCell_SuperCell_ID.h Calorimeter/CaloIdentifier/CaloIdentifier/CaloCell_SuperCell_ID.h  
CP $INPATH/HGTD_ID.icc Calorimeter/CaloIdentifier/CaloIdentifier/HGTD_ID.icc
CP $INPATH/HGTD_ID.h Calorimeter/CaloIdentifier/CaloIdentifier/HGTD_ID.h
CP $INPATH/CaloIdManager.cxx Calorimeter/CaloIdentifier/src/CaloIdManager.cxx
CP $INPATH/CaloCell_Base_ID.cxx  Calorimeter/CaloIdentifier/src/CaloCell_Base_ID.cxx
CP $INPATH/CaloCell_SuperCell_ID.cxx   Calorimeter/CaloIdentifier/src/CaloCell_SuperCell_ID.cxx
CP $INPATH/CaloCell_ID.cxx Calorimeter/CaloIdentifier/src/CaloCell_ID.cxx
CP $INPATH/HGTD_ID.cxx Calorimeter/CaloIdentifier/src/HGTD_ID.cxx
CP $INPATH/test_cell_id.cxx  Calorimeter/CaloIdentifier/test/test_cell_id.cxx 

pkgco.py G4AtlasTools
CP $INPATH/G4AtlasToolsConfig.py Simulation/G4Atlas/G4AtlasTools/python/G4AtlasToolsConfig.py 

pkgco.py LArCalorimeter/LArG4/HGTDG4SD-00-00-03
CP $INPATH/requirements LArCalorimeter/LArG4/HGTDG4SD/cmt/requirements

###fix below r=130mm (not yet on twiki)
pkgco.py G4AtlasApps
CP $INPATH/atlas_common.py Simulation/G4Atlas/G4AtlasApps/python/atlas_common.py

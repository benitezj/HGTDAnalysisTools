#####################
##uses SiHit, some files needed modification w.r.t /eos/HGTD/software
#######################
asetup 20.3.3.1,AtlasProduction,here
setupWorkArea.py

cmt co -r LArGeoEndcap-00-00-92 LArCalorimeter/LArGeoModel/LArGeoEndcap/

cmt co -r HGTDG4SD-00-00-02 LArCalorimeter/LArG4/HGTDG4SD/
/bin/cp HGTDAnalysisTools/share/install/SiHit20.3/requirements  LArCalorimeter/LArG4/HGTDG4SD/cmt/

pkgco.py AthenaCommon
/bin/cp HGTDAnalysisTools/share/install/SiHit20.3/DetFlags.py Control/AthenaCommon/python/

pkgco.py G4AtlasTools
/bin/cp HGTDAnalysisTools/share/install/SiHit20.3/G4AtlasToolsConfig.py Simulation/G4Atlas/G4AtlasTools/python/

/bin/cp HGTDAnalysisTools/share/preInclude.HGTD.py ./

##Compile
cmt find_packages
cmt compile

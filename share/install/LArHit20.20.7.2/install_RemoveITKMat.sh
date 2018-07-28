setupATLAS
asetup 20.20.7.2,here

INPATH=HGTDAnalysisTools/share/install/LArHit20.20.7.2

###Jet calibration
#cmt co -r  JetCalibTools-00-04-64  Reconstruction/Jet/JetCalibTools
#rm -f Reconstruction/Jet/JetCalibTools/util/JetCalibTools_Example.cxx

##########################
###For Geantino scan:
##########################
## - checkout G4UserActions
## - modify src/LengthIntegrator.cxx
#-    m_doRL = true;
#+    m_doRL = false;
#-    m_doEl = true;
#+    m_doEl = false;
#-    double zLimit = 3475.0; // For ITk studies: 3475mm is the z-limit of the ID End-Plate Cryostat (cf. talk by Jan Stark at Simu meeting on Dec. 14th,2016).
#+    double zLimit = 3650; // For ITk studies: 3475mm is the z-limit of the ID End-Plate Cryostat (cf. talk by Jan Stark at Simu meeting on Dec. 14th,2016).
#-	    m_rzMapRL[plotstring]=getOrCreateProfile(rznameReg, rzname, "Z [mm]", 1000,-3475.,3475.,"R [mm]",1000,0.,1200.,"%X0");
#+	    m_rzMapRL[plotstring]=getOrCreateProfile(rznameReg, rzname, "Z [mm]", 1000,-zLimit,zLimit,"R [mm]",1000,0.,1200.,"%X0");
#-	    m_rzMapIL[plotstring]=getOrCreateProfile(rznameReg, rzname, "Z [mm]", 1000,-3475.,3475.,"R [mm]",1000,0.,1200.,"#lambda");
#+	    m_rzMapIL[plotstring]=getOrCreateProfile(rznameReg, rzname, "Z [mm]", 1000,-zLimit,zLimit,"R [mm]",1000,0.,1200.,"#lambda");
#-	      m_rzMapRL[elementName]=getOrCreateProfile(rznameReg, rzname, "Z [mm]", 1000,-3475.,3475.,"R [mm]",1000,0.,1200.,"%X0");
#+	      m_rzMapRL[elementName]=getOrCreateProfile(rznameReg, rzname, "Z [mm]", 1000,-zLimit,zLimit,"R [mm]",1000,0.,1200.,"%X0");
#-	      m_rzMapIL[elementName]=getOrCreateProfile(rznameReg, rzname, "Z [mm]", 1000,-3475.,3475.,"R [mm]",1000,0.,1200.,"#lambda");
#+	      m_rzMapIL[elementName]=getOrCreateProfile(rznameReg, rzname, "Z [mm]", 1000,-zLimit,zLimit,"R [mm]",1000,0.,1200.,"#lambda");
pkgco.py G4UserActions-00-01-25
/bin/cp $INPATH/LengthIntegrator.cxx  Simulation/G4Utilities/G4UserActions/src/LengthIntegrator.cxx

###############################
### Remove ITK PP and Moderator
###############################
pkgco.py BarrelInclinedRef
/bin/cp $INPATH/InclBrl_PixelSimpleService.xml InnerDetector/InDetDetDescr/PixelLayouts/BarrelInclinedRef/data/InclBrl_PixelSimpleService.xml


#######################
### Remove SCT Moderator
#######################
pkgco.py GmxLayouts
/bin/cp $INPATH/ITk.gmx  InnerDetector/InDetDetDescr/GmxLayouts/data/ITk/Strip/LTF/ITk.gmx

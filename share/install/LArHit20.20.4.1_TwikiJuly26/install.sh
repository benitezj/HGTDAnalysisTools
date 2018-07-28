setupATLAS
asetup 20.20.4.1,here

INPATH=HGTDAnalysisTools/share/install/LArHit20.20.4.1_TwikiJuly26

pkgco.py G4AtlasTools-00-00-45-22
/bin/cp $INPATH/G4GeometryConfig.py Simulation/G4Atlas/G4AtlasTools/python/

pkgco.py ISF_Config-00-02-07
pkgco.py ISF_HepMC_Tools-00-07-00-04
pkgco.py ISF_Example-00-02-60

pkgco.py BarrelExtendedRef
/bin/cp $INPATH/GeoPixelEnvelopeExtRefTool.cxx  InnerDetector/InDetDetDescr/PixelLayouts/BarrelExtendedRef/src/

pkgco.py EndcapRingRef
/bin/cp $INPATH/GeoPixelEndcapECRingRefTool InnerDetector/InDetDetDescr/PixelLayouts/EndcapRingRef/src/

pkgco.py PixelServicesTool
/bin/cp $INPATH/PixelServicesTool.cxx InnerDetector/InDetDetDescr/PixelLayouts/PixelServicesTool/src/

pkgco.py Digitization-02-00-05
/bin/cp $INPATH/PileUpMergeSvcConfig.py Simulation/Digitization/python/

##should be this one, but need to check .py file
#pkgco.py Digitization-02-00-07


pkgco.py LArDigitization-03-82-36-04

pkgco.py CaloTools
/bin/cp $INPATH/CaloNoiseToolDB.cxx Calorimeter/CaloTools/src

pkgco.py CaloRecEx
/bin/cp $INPATH/CaloRecOutputItemList_jobOptions.py Calorimeter/CaloExample/CaloRecEx/share

/bin/cp $INPATH/preInclude.HGTD.py .
/bin/cp $INPATH/preInclude.MuonOff.py .
/bin/cp $INPATH/postInclude.TGCcabling.py .

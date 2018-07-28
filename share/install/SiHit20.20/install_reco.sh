##this installation works for samples produced without MuonOff
export INPATH=./HGTDAnalysisTools/share/install/SiHit20.20

#cp   Reconstruction/MET/METReconstruction/python/METConfig_RefFinal.py $INPATH
#cp   Reconstruction/MET/METReconstruction/python/METConfig_Truth.py $INPATH
#cp   Reconstruction/MET/METReconstruction/python/METConfig_Associator.py $INPATH
#cp   Reconstruction/MET/METReconstruction/python/METAssocConfig.py $INPATH
#cp   Reconstruction/MET/METReconstruction/python/METRecoConfig.py $INPATH
#cp   Reconstruction/MET/METReconstruction/share/BuildAssociation.py $INPATH
#cp   Reconstruction/MET/METReconstruction/share/RunMETReco.py $INPATH
#cp   Reconstruction/MET/METUtilities/src/METMakerAlg.cxx $INPATH

pkgco.py METReconstruction-00-02-05
/bin/cp -f $INPATH/METConfig_RefFinal.py    Reconstruction/MET/METReconstruction/python/METConfig_RefFinal.py
/bin/cp -f $INPATH/METConfig_Truth.py    Reconstruction/MET/METReconstruction/python/METConfig_Truth.py
/bin/cp -f $INPATH/METConfig_Associator.py    Reconstruction/MET/METReconstruction/python/METConfig_Associator.py
/bin/cp -f $INPATH/METAssocConfig.py    Reconstruction/MET/METReconstruction/python/METAssocConfig.py
/bin/cp -f $INPATH/METRecoConfig.py    Reconstruction/MET/METReconstruction/python/METRecoConfig.py
/bin/cp -f $INPATH/BuildAssociation.py    Reconstruction/MET/METReconstruction/share/BuildAssociation.py
/bin/cp -f $INPATH/RunMETReco.py    Reconstruction/MET/METReconstruction/share/RunMETReco.py

pkgco.py METUtilities-00-02-04
/bin/cp -f $INPATH/METMakerAlg.cxx Reconstruction/MET/METUtilities/src/METMakerAlg.cxx

##for jet calibration
cmt co -r  JetCalibTools-00-04-64  Reconstruction/Jet/JetCalibTools
rm -f Reconstruction/Jet/JetCalibTools/util/JetCalibTools_Example.cxx

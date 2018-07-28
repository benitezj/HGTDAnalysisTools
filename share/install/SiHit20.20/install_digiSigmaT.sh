
INPATH=HGTDAnalysisTools/share/install/SiHit20.20

pkgco.py Digitization-01-17-02
/bin/cp -f $INPATH/DigitizationWriteMetaData.py  Simulation/Digitization/python/
/bin/cp -f $INPATH/DigitizationReadMetaData.py  Simulation/Digitization/python/
/bin/cp -f $INPATH/DigiOutput.py  Simulation/Digitization/python/
/bin/cp -f $INPATH/PileUpMergeSvcConfig.py  Simulation/Digitization/python/
/bin/cp -f $INPATH/DigiAlgConfig.py  Simulation/Digitization/python/

mkdir ./LArCalorimeter/
svn co svn+ssh://svn.cern.ch/reps/atlas-benitezj/HGTDDigitization/tags/June14 LArCalorimeter/HGTDDigitization

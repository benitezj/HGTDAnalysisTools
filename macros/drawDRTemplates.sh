
MYAFS=/afs/cern.ch/user/b/benitezj/www/HGTDStudies/HitPlots

PROD=LArHitNov11/LArHit_July26

export PRESHOWER=$1
if [ "$PRESHOWER" != "" ]; then
echo "preshower=${PRESHOWER}="
PROD=${PROD}_${PRESHOWER}
fi

###mu=0 signal plots
#root -b -q HGTDAnalysisTools/macros/plotTuple.C\(\"${PROD}_mu0_3CELLSIZE_40CELLENERGY_30TIMESMEAR\"\)
#root -b -q HGTDAnalysisTools/macros/drawDRTemplates.C\(\"${MYAFS}/VBFH125inv_${PROD}_mu0_3CELLSIZE_40CELLENERGY_30TIMESMEAR\"\)

###mu=200 signal and pu plots
root -b -q HGTDAnalysisTools/macros/plotTuple.C\(\"${PROD}_mu200_3CELLSIZE_40CELLENERGY_30TIMESMEAR\"\)
root -b -q HGTDAnalysisTools/macros/drawDRTemplates.C\(\"${MYAFS}/VBFH125inv_${PROD}_mu200_3CELLSIZE_40CELLENERGY_30TIMESMEAR\"\)
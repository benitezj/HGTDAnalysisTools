export PROD=$1
export PRESHOWER=$2
for t in 30 50 ; do
export TRES="${t}"

## simple peak finding
#rootb HGTDAnalysisTools/macros/findWP.C\(\"${PROD}\",\"${PRESHOWER}\",${t},30,40,\"ClusCellDTFitFraction\",0.0015\)
#rootb HGTDAnalysisTools/macros/findWP.C\(\"${PROD}\",\"${PRESHOWER}\",${t},40,50,\"ClusCellDTFitFraction\",0.0015\)
#rootb HGTDAnalysisTools/macros/findWP.C\(\"${PROD}\",\"${PRESHOWER}\",${t},50,70,\"ClusCellDTFitFraction\",0.0015\)

## with cluster energy weighting
rootb HGTDAnalysisTools/macros/findWP.C\(\"${PROD}\",\"${PRESHOWER}\",${t},30,40,\"ClusCellDTFitFractionEcw\",0.0015\)
rootb HGTDAnalysisTools/macros/findWP.C\(\"${PROD}\",\"${PRESHOWER}\",${t},40,50,\"ClusCellDTFitFractionEcw\",0.0015\)
rootb HGTDAnalysisTools/macros/findWP.C\(\"${PROD}\",\"${PRESHOWER}\",${t},50,70,\"ClusCellDTFitFractionEcw\",0.0015\)

done

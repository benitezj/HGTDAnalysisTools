

#for mu in mu0 mu80 mu140 mu200; do
#export TAG=SiHit_April12_${D}_10CELLSIZE_20CELLENERGY
for mu in mu0 mu140 mu200; do
export TAG=SiHit_April12_Preshower_${mu}_10CELLSIZE_20CELLENERGY
echo $TAG
root -b HGTDAnalysisTools/macros/plotTuple.C\(\"${TAG}\"\)
done

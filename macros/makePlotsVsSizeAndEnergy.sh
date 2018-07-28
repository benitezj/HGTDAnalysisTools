
for c in 3 5 10 20; do 
for e in 20 40 60 80; do
#export TAG=SiHit_April12_mu200_${c}CELLSIZE_${e}CELLENERGY
export TAG=SiHit_April12_Preshower_mu200_${c}CELLSIZE_${e}CELLENERGY
echo $TAG
root -b HGTDAnalysisTools/macros/plotTuple.C\(\"${TAG}\"\) &
done
done

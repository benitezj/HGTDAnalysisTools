##Timing
for t in 1 10 20 30 40 50 60 80 100 120 140; do
rootb HGTDAnalysisTools/macros/plotTuple.C\(\"SiHit_April12_mu0_3CELLSIZE_20CELLENERGY_${t}TIMESMEAR\"\)
rootb HGTDAnalysisTools/macros/plotTuple.C\(\"SiHit_April12_mu200_3CELLSIZE_20CELLENERGY_${t}TIMESMEAR\"\)
done

##Preshower
for t in 40 60 80 100 120 140; do
rootb HGTDAnalysisTools/macros/plotTuple.C\(\"SiHit_April12_Preshower_mu0_3CELLSIZE_20CELLENERGY_${t}TIMESMEAR\"\)
rootb HGTDAnalysisTools/macros/plotTuple.C\(\"SiHit_April12_Preshower_mu200_3CELLSIZE_20CELLENERGY_${t}TIMESMEAR\"\)
done

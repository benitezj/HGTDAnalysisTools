pkgco.py HitAnalysis
cp HGTDAnalysisTools/share/install/HitAna/HGTDHitAnalysis.cxx Simulation/Tools/HitAnalysis/src/HGTDHitAnalysis.cxx
cp HGTDAnalysisTools/share/install/HitAna/HitAnalysis_entries.cxx Simulation/Tools/HitAnalysis/src/components/HitAnalysis_entries.cxx
cp HGTDAnalysisTools/share/install/HitAna/HGTDHitAnalysis.h  Simulation/Tools/HitAnalysis/HitAnalysis/HGTDHitAnalysis.h
cp HGTDAnalysisTools/share/install/HitAna/requirementsLArHits Simulation/Tools/HitAnalysis/cmt/requirements
cp HGTDAnalysisTools/share/install/HitAna/HGTDHitAnalysis_topOptions.py ./HGTDHitAnalysis_topOptions.py
cmt find_packages
cmt compile 

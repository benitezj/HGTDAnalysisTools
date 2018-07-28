##installation from Daniele 
setupATLAS
asetup 20.20.X-VAL,rel_5,here

alias CP="/bin/cp -f"
INPATH=HGTDAnalysisTools/share/install/LArHit20.20.XVAL

pkgco.py G4AtlasTools
CP $INPATH/G4AtlasToolsConfig.py Simulation/G4Atlas/G4AtlasTools/python/

# xrdcp -f root://eosatlas//eos/atlas/atlascerngroupdisk/det-hgtd/software/preInclude.MuonOff.py preInclude.MuonOff.py 

#Sim_tf.py  \
#--inputEVNTFile /afs/cern.ch/user/j/jabdalla/public/HGTD/EvtGen/MC15.361021.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ1W.pool.root \
#--outputHITSFile HITS.root \
#--physicsList 'FTFP_BERT' \
#--truthStrategy 'MC12' \
#--geometryVersion 'all:ATLAS-P2-ITK-06-01-00_VALIDATION' \
#--conditionsTag 'all:OFLCOND-MC12-ITK-27-00' \
#--simulator 'MC12G4' \
#--DataRunNumber '99999' \
#--maxEvents 5 \
#--skipEvents 0 \
#--randomSeed  466087 \
#--preInclude 'EVNTtoHITS:InDetSLHC_Example/preInclude.SLHC.py,InDetSLHC_Example/preInclude.NoTRT.py,InDetSLHC_Example/preInclude.SLHC_Setup_ExtBrl_4.py,preInclude.MuonOff.py' \
#--HGTDOn True \
#--postInclude 'all:PyJobTransforms/UseFrontier.py,InDetSLHC_Example/postInclude.SLHC_Setup_ExtBrl_4.py'

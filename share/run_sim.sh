
INFILE=$1
if [ "$INFILE" == "" ]; then
#INFILE=/nfs/uiowapc/data/det-hgtd/evgen/VBFH125_bb/MC15.341566.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_bb_11.pool.root
INFILE=/nfs/uiowapc/data/det-hgtd/evgen/mc15_13TeV.305174.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH500_ZZ4nu.evgen.EVNT.e5019/EVNT.08241448._000010.pool.root.1
fi

NEVT=$2
if [ "$NEVT" == "" ]; then
NEVT=1
fi

SKIP=$3
if [ "$SKIP" == "" ]; then
SKIP=0
fi

RNDMSEED=$4
if [ "$RNDMSEED" == "" ]; then
RNDMSEED=873254
fi

####For 20.3 installation
#Sim_tf.py \
#    --inputEVNTFile $INFILE \
#    --outputHITSFile  HITS.pool.root  \
#    --physicsList 'FTFP_BERT' \
#    --truthStrategy 'MC12' \
#    --geometryVersion 'all:ATLAS-P2-ITK-01-02-00_VALIDATION' \
#    --conditionsTag 'all:OFLCOND-MC12-ITK-27-00' \
#    --simulator 'MC12G4' \
#    --DataRunNumber '195847' \
#    --skipEvents $SKIP \
#    --maxEvents $NEVT \
#    --randomSeed $RNDMSEED \
#    --preInclude 'all:InDetSLHC_Example/preInclude.SLHC.py,InDetSLHC_Example/preInclude.NoTRT.py,HGTDAnalysisTools/preInclude.MuonOff.py,HGTDAnalysisTools/preInclude.HGTD.py' \
#    --postInclude 'all:PyJobTransforms/UseFrontier.py,InDetSLHC_Example/postInclude.SLHC_Setup.py'

#####20.20 with LoI 
###/nfs/home/benitezj/HGTD/20.20_SiHit_OldGeo
#Sim_tf.py \
#--inputEVNTFile $INFILE \
#--outputHITSFile HITS.pool.root \
#--skipEvents $SKIP \
#--maxEvents $NEVT \
#--geometryVersion ATLAS-P2-ITK-01-02-00_VALIDATION \
#--conditionsTag OFLCOND-MC12-ITK-27-00 \
#--physicsList FTFP_BERT  \
#--truthStrategy MC12 \
#--simulator MC12G4 \
#--DataRunNumber=195847 \
#--randomSeed $RNDMSEED \
#--preInclude InDetSLHC_Example/preInclude.SLHC.py,InDetSLHC_Example/preInclude.NoTRT.py,InDetSLHC_Example/preInclude.SLHC_Setup_LoI.py,HGTDAnalysisTools/preInclude.MuonOff.py,HGTDAnalysisTools/preInclude.HGTD.py \
#--postInclude PyJobTransforms/UseFrontier.py,InDetSLHC_Example/postInclude.SLHC_Setup_LoI.py \
#--postExec 'ToolSvc.ISF_ParticlePositionFilterDynamic.CheckRegion=[1]'


#####HGTD in 20.20-XVAL 
Sim_tf.py \
--inputEVNTFile $INFILE \
--outputHITSFile HITS.pool.root \
--skipEvents $SKIP \
--maxEvents $NEVT \
--physicsList 'FTFP_BERT'  \
--truthStrategy 'MC12' \
--geometryVersion 'all:ATLAS-P2-ITK-06-02-00_VALIDATION' \
--conditionsTag 'default:OFLCOND-MC12-ITK-27-00' \
--simulator 'MC12G4' \
--DataRunNumber '195847' \
--randomSeed $RNDMSEED \
--preInclude 'EVNTtoHITS:InDetSLHC_Example/preInclude.SLHC.py,InDetSLHC_Example/preInclude.NoTRT.py,InDetSLHC_Example/preInclude.SLHC_Setup_ExtBrl_4.py' \
--postInclude 'all:PyJobTransforms/UseFrontier.py,InDetSLHC_Example/postInclude.SLHC_Setup_ExtBrl_4.py' \
--HGTDOn True \

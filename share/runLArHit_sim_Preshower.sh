
INFILE=$1
if [ "$INFILE" == "" ]; then
INFILE=/nfs/uiowapc/data/det-hgtd/evgen/VBFH125_bb/MC15.341566.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_bb_11.pool.root
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

######### LArHit 2016-07-26 setup with 20.20.4.1
Sim_tf.py \
      --inputEVNTFile $INFILE \
      --outputHITSFile HITS.pool.root  \
      --geometryVersion 'all:ATLAS-P2-ITK-06-01-00_VALIDATION' \
      --conditionsTag 'default:OFLCOND-MC12-ITK-27-00' \
      --DataRunNumber '195847' \
      --maxEvents $NEVT \
      --skipEvents $SKIP \
      --randomSeed $RNDMSEED \
      --postExec 'EVNTtoHITS:ToolSvc.ISF_ParticlePositionFilterDynamic.CheckRegion=[1]' \
      --preInclude 'EVNTtoHITS:InDetSLHC_Example/preInclude.SLHC.py,InDetSLHC_Example/preInclude.NoTRT.py,InDetSLHC_Example/preInclude.SLHC_Setup_ExtBrl_4.py,HGTDAnalysisTools/preInclude.MuonOff.py,HGTDAnalysisTools/preInclude.HGTD.py' \
      --postInclude 'all:PyJobTransforms/UseFrontier.py,InDetSLHC_Example/postInclude.SLHC_Setup_ExtBrl_4.py,HGTDAnalysisTools/postInclude.VertexPositioner.py'

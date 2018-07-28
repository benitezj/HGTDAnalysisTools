
INFILE=$1
if [ "$INFILE" == "" ]; then
#INFILE=/nfs/uiowapc/data/det-hgtd/evgen/VBFH125_bb/MC15.341566.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_bb_11.pool.root
INFILE=/nfs/uiowapc/data/det-hgtd/evgen/mc15_13TeV.341080.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_WWlvlv_EF_15_5.evgen.EVNT.e3871/EVNT.05454835._000057.pool.root.1
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


######HGTD in 20.20-XVAL 
#Sim_tf.py \
#--inputEVNTFile $INFILE \
#--outputHITSFile HITS.pool.root \
#--skipEvents $SKIP \
#--maxEvents $NEVT \
#--physicsList 'FTFP_BERT'  \
#--truthStrategy 'MC12' \
#--geometryVersion 'all:ATLAS-P2-ITK-06-02-00_VALIDATION' \
#--conditionsTag 'default:OFLCOND-MC12-ITK-27-00' \
#--simulator 'MC12G4' \
#--DataRunNumber '195847' \
#--randomSeed $RNDMSEED \
#--preInclude 'EVNTtoHITS:InDetSLHC_Example/preInclude.SLHC.py,InDetSLHC_Example/preInclude.NoTRT.py,InDetSLHC_Example/preInclude.SLHC_Setup_ExtBrl_4.py' \
#--postInclude 'all:PyJobTransforms/UseFrontier.py,InDetSLHC_Example/postInclude.SLHC_Setup_ExtBrl_4.py' \
#--HGTDOn True \

########## LArHit 2016-07-26 setup with 20.20.4.1
###    --loglevel DEBUG \
#Sim_tf.py \
#    --inputEVNTFile $INFILE \
#    --outputHITSFile HITS.pool.root  \
#    --geometryVersion 'all:ATLAS-P2-ITK-06-02-00_VALIDATION' \
#    --conditionsTag 'default:OFLCOND-MC12-ITK-27-00' \
#    --DataRunNumber '195847' \
#    --maxEvents $NEVT \
#    --skipEvents $SKIP \
#    --randomSeed $RNDMSEED \
#    --postExec 'EVNTtoHITS:ToolSvc.ISF_ParticlePositionFilterDynamic.CheckRegion=[1]' \
#    --preInclude 'EVNTtoHITS:InDetSLHC_Example/preInclude.SLHC.py,InDetSLHC_Example/preInclude.NoTRT.py,InDetSLHC_Example/preInclude.SLHC_Setup_ExtBrl_4.py,HGTDAnalysisTools/preInclude.MuonOff.py,HGTDAnalysisTools/preInclude.HGTD.py' \
#    --postInclude 'all:PyJobTransforms/UseFrontier.py,InDetSLHC_Example/postInclude.SLHC_Setup_ExtBrl_4.py,HGTDAnalysisTools/postInclude.VertexPositioner.py'


######### LArHit 2016-07-26 setup with 20.20.7.2
##    --loglevel DEBUG \ 
#### Inclined Barrel ITK
#Sim_tf.py \
#    --inputEVNTFile $INFILE \
#    --outputHITSFile HITS.pool.root  \
#    --DataRunNumber '195847' \
#    --maxEvents $NEVT \
#    --skipEvents $SKIP \
#    --randomSeed $RNDMSEED \
#    --HGTDOn True \
#    --conditionsTag 'default:OFLCOND-MC15c-SDR-13' \
#    --geometryVersion 'all:ATLAS-P2-ITK-10-02-01_VALIDATION' \
#    --postExec "EVNTtoHITS:ServiceMgr.DetDescrCnvSvc.DoInitNeighbours = False" \
#    --postInclude "all:PyJobTransforms/UseFrontier.py,InDetSLHC_Example/postInclude.SLHC_Setup_InclBrl_4.py,InDetSLHC_Example/postInclude.SLHC_Setup.py" "EVNTtoHITS:BeamEffects/postInclude.CrabKissingVertexPositioner_Nominal.py" \
#    --preExec "all:from InDetSLHC_Example.SLHC_JobProperties import SLHC_Flags;SLHC_Flags.doGMX.set_Value_and_Lock(True)" \
#    --preInclude "all:InDetSLHC_Example/preInclude.SLHC.py,InDetSLHC_Example/preInclude.NoTRT_NoBCM_NoDBM.py,InDetSLHC_Example/preInclude.SLHC_Setup_InclBrl_4.py,InDetSLHC_Example/preInclude.SLHC_Setup_Strip_GMX.py" 

#### Extended Barrel ITK
#Sim_tf.py \
#    --inputEVNTFile $INFILE \
#    --outputHITSFile HITS.pool.root  \
#    --DataRunNumber '195847' \
#    --maxEvents $NEVT \
#    --skipEvents $SKIP \
#    --randomSeed $RNDMSEED \
#    --HGTDOn True \
#    --conditionsTag "default:OFLCOND-MC15c-SDR-13" \
#    --geometryVersion="default:ATLAS-P2-ITK-09-02-01_VALIDATION" \
#    --postExec "EVNTtoHITS:ServiceMgr.DetDescrCnvSvc.DoInitNeighbours = False" \
#    --postInclude "all:PyJobTransforms/UseFrontier.py,InDetSLHC_Example/postInclude.SLHC_Setup_ExtBrl_4.py,InDetSLHC_Example/postInclude.SLHC_Setup.py" "EVNTtoHITS:BeamEffects/postInclude.CrabKissingVertexPositioner_Nominal.py" \
#    --preExec "all:from InDetSLHC_Example.SLHC_JobProperties import SLHC_Flags;SLHC_Flags.doGMX.set_Value_and_Lock(True)" \
#    --preInclude "all:InDetSLHC_Example/preInclude.SLHC.py,InDetSLHC_Example/preInclude.NoTRT_NoBCM_NoDBM.py,InDetSLHC_Example/preInclude.SLHC_Setup_ExtBrl_4.py,InDetSLHC_Example/preInclude.SLHC_Setup_Strip_GMX.py"


######### LArHit  20.20.7.2, s3072
##[benitezj@uiowapc:0] LArHit20.20.7.2_Jan2017 >GetTfCommand.py --AMI=s3072
Sim_tf.py \
--conditionsTag 'default:OFLCOND-MC15c-SDR-13' \
--postInclude 'all:PyJobTransforms/UseFrontier.py,InDetSLHC_Example/postInclude.SLHC_Setup_InclBrl_4.py,InDetSLHC_Example/postInclude.SLHC_Setup.py' 'EVNTtoHITS:BeamEffects/postInclude.CrabKissingVertexPositioner_Nominal.py' \
--preExec 'all:from InDetSLHC_Example.SLHC_JobProperties import SLHC_Flags;SLHC_Flags.doGMX.set_Value_and_Lock(True)' \
--preInclude 'all:InDetSLHC_Example/preInclude.SLHC.py,InDetSLHC_Example/preInclude.NoTRT_NoBCM_NoDBM.py,InDetSLHC_Example/preInclude.SLHC_Setup_InclBrl_4.py,InDetSLHC_Example/preInclude.SLHC_Setup_Strip_GMX.py' \
--DataRunNumber '240000' \
--postExec 'EVNTtoHITS:ServiceMgr.DetDescrCnvSvc.DoInitNeighbours = False' \
--geometryVersion 'default:ATLAS-P2-ITK-10-02-01_VALIDATION' \
--HGTDOn 'True' \
--outputHITSFile HITS.pool.root  \
--inputEVNTFile $INFILE \
--maxEvents $NEVT \
--skipEvents $SKIP \
--randomSeed $RNDMSEED 

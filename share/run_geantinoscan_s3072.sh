INFILE=$1
if [ "$INFILE" == "" ]; then
echo "NO input provided"
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


###Sim_tf command comes from this:
##[benitezj@uiowapc:0] LArHit20.20.7.2_Jan2017 >GetTfCommand.py --AMI=s3072
#--inputEVNTFile=/afs/cern.ch/user/b/bsmart/public/ITk_FastGeoModel_for_X0_studies/pgun_2M_10GeV_geantinos_Eta6_v2_EVNT.root 
Sim_tf.py \
--conditionsTag 'default:OFLCOND-MC15c-SDR-13' \
--postInclude 'all:PyJobTransforms/UseFrontier.py,InDetSLHC_Example/postInclude.SLHC_Setup_InclBrl_4.py,InDetSLHC_Example/postInclude.SLHC_Setup.py' 'EVNTtoHITS:BeamEffects/postInclude.CrabKissingVertexPositioner_Nominal.py' \
--preExec 'all:from InDetSLHC_Example.SLHC_JobProperties import SLHC_Flags;SLHC_Flags.doGMX.set_Value_and_Lock(True)' \
--preInclude 'all:InDetSLHC_Example/preInclude.SLHC.py,InDetSLHC_Example/preInclude.NoTRT_NoBCM_NoDBM.py,InDetSLHC_Example/preInclude.SLHC_Setup_InclBrl_4.py,InDetSLHC_Example/preInclude.SLHC_Setup_Strip_GMX.py,G4UserActions/LengthIntegrator_options.py' \
--DataRunNumber '240000' \
--postExec 'EVNTtoHITS:ServiceMgr.DetDescrCnvSvc.DoInitNeighbours = False' \
--geometryVersion 'default:ATLAS-P2-ITK-10-02-01_VALIDATION' \
--HGTDOn 'True' \
--outputHITSFile=OUT.HITS_ExtBrl4.pool.root \
--inputEVNTFile $INFILE \
--maxEvents $NEVT \
--skipEvents $SKIP \
--randomSeed $RNDMSEED 

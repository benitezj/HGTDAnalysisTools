INFILE=$1

NEVT=$2
if [ "$NEVT" == "" ]; then
NEVT=-1
fi

SKIP=$3
if [ "$SKIP" == "" ]; then
SKIP=0
fi

##########################################
####20.20 With HGTD hits saved in SiHit
#########################################
## https://twiki.cern.ch/twiki/bin/viewauth/Atlas/UpgradeSimulationInnerTrackerRel20p3p3WithPileup#Reconstruction_with_pileup
Reco_tf.py \
--inputRDOFile $INFILE  \
--outputESDFile ESD.pool.root \
--skipEvents $SKIP \
--maxEvents  $NEVT \
--geometryVersion ATLAS-P2-ITK-01-02-00 \
--conditionsTag 'all:OFLCOND-MC12-ITK-F-29-200-25' \
--preExec 'all:from LArROD.LArRODFlags import larRODFlags; larRODFlags.doOFCPileupOptimization.set_Value_and_Lock(True); larRODFlags.NumberOfCollisions.set_Value_and_Lock(200); from CaloTools.CaloNoiseFlags import jobproperties; jobproperties.CaloNoiseFlags.FixedLuminosity.set_Value_and_Lock(200.0*1.45/8*50/25); from AthenaCommon.BeamFlags import jobproperties; jobproperties.Beam.bunchSpacing=25' 'RAWtoESD:from CaloRec.CaloCellFlags import jobproperties;jobproperties.CaloCellFlags.doLArCellEmMisCalib=False' 'RAWtoESD:from JetRec.JetRecFlags import jetFlags; jetFlags.useMuonSegments.set_Value_and_Lock(False); jetFlags.useBTagging.set_Value_and_Lock(False); from RecExConfig.RecFlags import rec;  rec.doBTagging.set_Value_and_Lock(False);' 'RAWtoESD:InDetFlags.doSLHCVeryForward.set_Value_and_Lock(True);' \
--preInclude InDetSLHC_Example/preInclude.SLHC.NoTRT.Reco.py,InDetSLHC_Example/preInclude.SLHC_Setup_LoI.py,HGTDAnalysisTools/preInclude.MuonOff.py \
--postInclude PyJobTransforms/UseFrontier.py,InDetSLHC_Example/postInclude.SLHC_Setup_LoI.py r2e:InDetSLHC_Example/postInclude.DigitalClustering.py \
--outputAODFile AOD.pool.root \
--postExec 'RAWtoESD:StreamESD.ItemList+=["SiHitCollection#HGTDHits","SiHitCollection#MergedHGTDHits"]' 'ESDtoAOD:StreamAOD.ItemList+=["SiHitCollection#MergedHGTDHits","SiHitCollection#MergedHGTDHits"]'


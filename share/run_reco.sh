INFILE=$1

NEVT=$2
if [ "$NEVT" == "" ]; then
NEVT=-1
fi

SKIP=$3
if [ "$SKIP" == "" ]; then
SKIP=0
fi


#####Twiki + Jalal geo tag --> this seems to work get AntiKt4EMTopo jets
#Reco_tf.py \
#--maxEvents -1 \
#--inputRDOFile $INFILE \
#--outputESDFile ESD.pool.root \
#--outputAODFile AOD.pool.root \
#--skipEvents 0 \
#--maxEvents $NEVT \
#--conditionsTag 'all:OFLCOND-MC12-ITK-F-29-80-25' \
#--geometryVersion 'all:ATLAS-P2-ITK-01-01-00' \
#--autoConfiguration everything \
#--preInclude 'RAWtoESD:InDetSLHC_Example/preInclude.SLHC.NoTRT.Reco.py' 'ESDtoAOD:InDetSLHC_Example/preInclude.SLHC.NoTRT.Reco.py' \
#--postInclude 'all:PyJobTransforms/UseFrontier.py,InDetSLHC_Example/postInclude.SLHC_Setup.py' 'RAWtoESD:InDetSLHC_Example/postInclude.DigitalClustering.py' \
#--preExec 'all:from LArROD.LArRODFlags import larRODFlags; larRODFlags.doOFCPileupOptimization.set_Value_and_Lock(True); larRODFlags.NumberOfCollisions.set_Value_and_Lock(80); from CaloTools.CaloNoiseFlags import jobproperties; jobproperties.CaloNoiseFlags.FixedLuminosity.set_Value_and_Lock(80.0*1.45/8*50/25); from AthenaCommon.BeamFlags import jobproperties; jobproperties.Beam.bunchSpacing=25' 'RAWtoESD:from CaloRec.CaloCellFlags import jobproperties;jobproperties.CaloCellFlags.doLArCellEmMisCalib=False' \
#--postExec 'RAWtoESD:xAODMaker__xAODTruthCnvAlg("GEN_AOD2xAOD",WriteInTimePileUpTruth=True)'

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
--conditionsTag 'all:OFLCOND-MC12-ITK-F-29-80-25' \
--preExec 'all:from LArROD.LArRODFlags import larRODFlags; larRODFlags.doOFCPileupOptimization.set_Value_and_Lock(True); larRODFlags.NumberOfCollisions.set_Value_and_Lock(80); from CaloTools.CaloNoiseFlags import jobproperties; jobproperties.CaloNoiseFlags.FixedLuminosity.set_Value_and_Lock(80.0*1.45/8*50/25); from AthenaCommon.BeamFlags import jobproperties; jobproperties.Beam.bunchSpacing=25' 'RAWtoESD:from CaloRec.CaloCellFlags import jobproperties;jobproperties.CaloCellFlags.doLArCellEmMisCalib=False' 'RAWtoESD:from JetRec.JetRecFlags import jetFlags; jetFlags.useMuonSegments.set_Value_and_Lock(False); jetFlags.useBTagging.set_Value_and_Lock(False); from RecExConfig.RecFlags import rec;  rec.doBTagging.set_Value_and_Lock(False);' 'RAWtoESD:InDetFlags.doSLHCVeryForward.set_Value_and_Lock(True);' \
--preInclude InDetSLHC_Example/preInclude.SLHC.NoTRT.Reco.py,InDetSLHC_Example/preInclude.SLHC_Setup_LoI.py,HGTDAnalysisTools/preInclude.MuonOff.py \
--postInclude PyJobTransforms/UseFrontier.py,InDetSLHC_Example/postInclude.SLHC_Setup_LoI.py r2e:InDetSLHC_Example/postInclude.DigitalClustering.py \
--outputAODFile AOD.pool.root \
--postExec 'RAWtoESD:StreamESD.ItemList+=["SiHitCollection#HGTDHits","SiHitCollection#MergedHGTDHits"]' 'ESDtoAOD:StreamAOD.ItemList+=["SiHitCollection#MergedHGTDHits","SiHitCollection#MergedHGTDHits"]'

#--preExec 'all:from LArROD.LArRODFlags import larRODFlags; larRODFlags.doOFCPileupOptimization.set_Value_and_Lock(True); larRODFlags.NumberOfCollisions.set_Value_and_Lock(80); from CaloTools.CaloNoiseFlags import jobproperties; jobproperties.CaloNoiseFlags.FixedLuminosity.set_Value_and_Lock(80.0*1.45/8*50/25); from AthenaCommon.BeamFlags import jobproperties; jobproperties.Beam.bunchSpacing=25' 'RAWtoESD:from CaloRec.CaloCellFlags import jobproperties;jobproperties.CaloCellFlags.doLArCellEmMisCalib=False' 'r2e:from JetRec.JetRecFlags import jetFlags; jetFlags.useMuonSegments.set_Value_and_Lock(False); jetFlags.useBTagging.set_Value_and_Lock(False); InDetFlags.doSLHCVeryForward.set_Value_and_Lock(True); from RecExConfig.RecFlags import rec;  rec.doBTagging.set_Value_and_Lock(False);' \

#########################################
####20.20 With HGTD hits saved in LArHit
#########################################
#Reco_tf.py \
#--maxEvents -1 \
#--inputRDOFile $INFILE \
#--outputESDFile ESD.pool.root \
#--outputAODFile AOD.pool.root \
#--skipEvents $SKIP \
#--maxEvents $NEVT \
#--geometryVersion all:ATLAS-P2-ITK-06-01-00 \
#--conditionsTag all:OFLCOND-MC15c-SDR-06 \
#--autoConfiguration everything \
#--preInclude InDetSLHC_Example/preInclude.SLHC.NoTRT.Reco.py InDetSLHC_Example/preInclude.SLHC_Setup_ExtBrl_4.py InDetSLHC_Example/SLHC_Setup_Reco_Alpine.py \
#--postInclude PyJobTransforms/UseFrontier.py,InDetSLHC_Example/postInclude.SLHC_Setup_ExtBrl_4.py  r2e:InDetSLHC_Example/postInclude.DigitalClustering.py \
#--preExec 'r2e:InDetFlags.doSLHCVeryForward.set_Value_and_Lock(True)' \
#--postExec 'RAWtoESD:StreamESD.ItemList+=["LArHitContainer#LArHitHGTD"]' 'ESDtoAOD:StreamAOD.ItemList+=["LArHitContainer#LArHitHGTD"]'

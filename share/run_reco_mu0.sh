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
--conditionsTag OFLCOND-MC12-ITK-27-00 \
--autoConfiguration everything \
--preInclude InDetSLHC_Example/preInclude.SLHC.NoTRT.Reco.py,InDetSLHC_Example/preInclude.SLHC_Setup_LoI.py,HGTDAnalysisTools/preInclude.MuonOff.py \
--postInclude PyJobTransforms/UseFrontier.py,InDetSLHC_Example/postInclude.SLHC_Setup_LoI.py r2e:InDetSLHC_Example/postInclude.DigitalClustering.py \
--preExec  'RAWtoESD:from JetRec.JetRecFlags import jetFlags; jetFlags.useMuonSegments.set_Value_and_Lock(False); jetFlags.useBTagging.set_Value_and_Lock(False); from RecExConfig.RecFlags import rec;  rec.doBTagging.set_Value_and_Lock(False);' 'RAWtoESD:InDetFlags.doSLHCVeryForward.set_Value_and_Lock(True);' \
--outputAODFile AOD.pool.root \
--postExec 'RAWtoESD:StreamESD.ItemList+=["SiHitCollection#HGTDHits","SiHitCollection#MergedHGTDHits"]' 'ESDtoAOD:StreamAOD.ItemList+=["SiHitCollection#MergedHGTDHits","SiHitCollection#MergedHGTDHits"]'


#Reco_tf.py \
#--inputRDOFile $INFILE \
#--skipEvents $SKIP \
#--maxEvents  $NEVT \
#--outputESDFile ESD.pool.root \
#--outputAODFile AOD.pool.root \
#--geometryVersion 'all:ATLAS-P2-ITK-01-02-00' \
#--conditionsTag 'all:OFLCOND-MC12-ITK-F-29-80-25' \
#--preInclude 'all:InDetSLHC_Example/preInclude.SLHC.NoTRT.Reco.py' 'HGTDAnalysisTools/preInclude.MuonOff.py' \
#--postInclude 'all:PyJobTransforms/UseFrontier.py,InDetSLHC_Example/postInclude.SLHC_Setup.py' 'RAWtoESD:InDetSLHC_Example/postInclude.DigitalClustering.py' \
#--preExec 'all:from CaloTools.CaloNoiseFlags import jobproperties; jobproperties.CaloNoiseFlags.FixedLuminosity.set_Value_and_Lock(0.)'  'RAWtoESD:from JetRec.JetRecFlags import jetFlags; jetFlags.useMuonSegments.set_Value_and_Lock(False); jetFlags.useBTagging.set_Value_and_Lock(False); from RecExConfig.RecFlags import rec;  rec.doBTagging.set_Value_and_Lock(False);' 'RAWtoESD:InDetFlags.doSLHCVeryForward.set_Value_and_Lock(True);' \
#--postExec 'RAWtoESD:StreamESD.ItemList+=["SiHitCollection#HGTDHits","SiHitCollection#MergedHGTDHits"]' 'ESDtoAOD:StreamAOD.ItemList+=["SiHitCollection#MergedHGTDHits","SiHitCollection#MergedHGTDHits"]' 

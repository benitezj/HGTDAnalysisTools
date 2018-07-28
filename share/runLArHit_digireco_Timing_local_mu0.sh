INFILE=$1

NEVT=$2
if [ "$NEVT" == "" ]; then
NEVT=9999999
fi

SKIP=$3
if [ "$SKIP" == "" ]; then
SKIP=0
fi

RNDMSEED=$4
if [ "$RNDMSEED" == "" ]; then
RNDMSEED=50100
fi


##STEP1.2 samples: GetTfCommand.py --AMI=r8069
#Reco_tf.py \
#    --digiSteeringConf 'StandardInTimeOnlyTruth' \
#    --conditionsTag 'default:OFLCOND-MC15c-SDR-06' \
#    --postInclude 'all:PyJobTransforms/UseFrontier.py,InDetSLHC_Example/postInclude.SLHC_Setup_ExtBrl_4.py,HGTDAnalysisTools/postInclude.TGCcabling.py' 'HITtoRDO:InDetSLHC_Example/postInclude.SLHC_Digitization_lowthresh.py' 'RAWtoESD:InDetSLHC_Example/postInclude.DigitalClustering.py' \
#    --autoConfiguration 'everything' \
#    --DBRelease 'all:current' \
#    --geometryVersion 'default:ATLAS-P2-ITK-06-02-00' \
#    --preInclude 'all:InDetSLHC_Example/preInclude.SLHC_Setup_ExtBrl_4.py,HGTDAnalysisTools/preInclude.HGTD.py' 'HITtoRDO:InDetSLHC_Example/preInclude.SLHC.py,InDetSLHC_Example/preInclude.NoTRT.py' 'RAWtoESD:InDetSLHC_Example/preInclude.SLHC.NoTRT.Reco.py,InDetSLHC_Example/SLHC_Setup_Reco_TrackingGeometry.py' 'ESDtoAOD:InDetSLHC_Example/preInclude.SLHC.NoTRT.Reco.py,InDetSLHC_Example/SLHC_Setup_Reco_TrackingGeometry.py' 'ESDtoDPD:InDetSLHC_Example/preInclude.SLHC.NoTRT.Reco.py,InDetSLHC_Example/SLHC_Setup_Reco_TrackingGeometry.py' \
#    --DataRunNumber '240000' \
#    --preExec 'all:from LArROD.LArRODFlags import larRODFlags; larRODFlags.NumberOfCollisions.set_Value_and_Lock(0); from AthenaCommon.BeamFlags import jobproperties; jobproperties.Beam.bunchSpacing=25; from RecExConfig.RecFlags import rec; rec.doTrigger.set_Value_and_Lock(False)' 'HITtoRDO:from Digitization.DigitizationFlags import digitizationFlags; digitizationFlags.doInDetNoise.set_Value_and_Lock(False); digitizationFlags.overrideMetadata+=["SimLayout","PhysicsList"]; from CaloTools.CaloNoiseFlags import jobproperties; jobproperties.CaloNoiseFlags.FixedLuminosity.set_Value_and_Lock(0.)' 'RAWtoESD:from CaloRec.CaloCellFlags import jobproperties;jobproperties.CaloCellFlags.doLArCellEmMisCalib=False; InDetFlags.doSLHCVeryForward.set_Value_and_Lock(True);InDetFlags.writeRDOs.set_Value_and_Lock(True);InDetFlags.doPseudoTracking.set_Value_and_Lock(True)' 'ESDtoDPD:rec.doDPD.set_Value_and_Lock(True); rec.DPDMakerScripts.set_Value_and_Lock(["InDetPrepRawDataToxAOD/InDetDxAOD.py","PrimaryDPDMaker/PrimaryDPDMaker.py"]); from InDetPrepRawDataToxAOD.InDetDxAODJobProperties import InDetDxAODFlags; InDetDxAODFlags.ThinHitsOnTrack.set_Value_and_Lock(False); InDetDxAODFlags.ThinTrackSelection.set_Value_and_Lock("InDetTrackParticles.pt > 0.0*GeV"); InDetDxAODFlags.DumpUnassociatedHits.set_Value_and_Lock(False); InDetDxAODFlags.DumpPixelInfo.set_Value_and_Lock(True); InDetDxAODFlags.DumpPixelRdoInfo.set_Value_and_Lock(True); InDetDxAODFlags.DumpSctInfo.set_Value_and_Lock(True); InDetDxAODFlags.DumpSctRdoInfo.set_Value_and_Lock(True);' \
#    --postExec 'all:from IOVDbSvc.CondDB import conddb' 'HITtoRDO:pixeldigi.EnableSpecialPixels=False; CfgMgr.MessageSvc().setError+=["HepMcParticleLink"]; from AthenaCommon.CfgGetter import getPublicTool; getPublicTool("PixelDigitizationTool").ParticleBarcodeVeto=0; getPublicTool("SCT_DigitizationTool").ParticleBarcodeVeto=0;' 'ESDtoAOD:xAODMaker__xAODTruthCnvAlg("GEN_AOD2xAOD",WriteInTimePileUpTruth=True);' 'ESDtoDPD:xAODMaker__xAODTruthCnvAlg("GEN_AOD2xAOD",WriteInTimePileUpTruth=True);' \
#    --inputHITSFile $INFILE \
#    --outputESDFile ESD.pool.root \
#    --outputAODFile AOD.pool.root \
#    --maxEvents $NEVT \
#    --skipEvents $SKIP \
#    --digiSeedOffset1 1${RNDMSEED} \
#    --digiSeedOffset2 1${RNDMSEED}001



#### for single pi samples in 20.20.7.2 
##GetTfCommand.py --AMI=r8956
Reco_tf.py \
    --digiSteeringConf 'StandardInTimeOnlyTruth' \
    --conditionsTag 'default:OFLCOND-MC15c-SDR-13' \
    --postInclude 'all:InDetSLHC_Example/postInclude.SLHC_Setup_InclBrl_4.py' 'HITtoRDO:InDetSLHC_Example/postInclude.SLHC_Digitization_lowthresh.py' 'RAWtoESD:InDetSLHC_Example/postInclude.DigitalClustering.py,InDetSLHC_Example/postInclude.NoJetPtFilter.py' \
    --geometryVersion 'default:ATLAS-P2-ITK-10-02-01' \
    --preInclude 'all:InDetSLHC_Example/preInclude.SLHC_Setup_InclBrl_4.py,InDetSLHC_Example/preInclude.SLHC_Setup_Strip_GMX.py,InDetSLHC_Example/preInclude.SLHC_Calorimeter_mu0.py' 'HITtoRDO:InDetSLHC_Example/preInclude.SLHC.py,InDetSLHC_Example/preInclude.NoTRT_NoBCM_NoDBM.py' 'default:InDetSLHC_Example/preInclude.SLHC.NoTRT_NoBCM_NoDBM.Reco.py,InDetSLHC_Example/SLHC_Setup_Reco_TrackingGeometry_GMX.py' 'RDOMergeAthenaMP:InDetSLHC_Example/preInclude.SLHC.py,InDetSLHC_Example/preInclude.NoTRT_NoBCM_NoDBM.py' 'POOLMergeAthenaMPAOD0:InDetSLHC_Example/preInclude.SLHC.NoTRT_NoBCM_NoDBM.Ana.py,InDetSLHC_Example/SLHC_Setup_Reco_Alpine.py' 'POOLMergeAthenaMPDAODIDTRKVALID0:InDetSLHC_Example/preInclude.SLHC.NoTRT_NoBCM_NoDBM.Ana.py,InDetSLHC_Example/SLHC_Setup_Reco_Alpine.py' \
    --DataRunNumber '240000' \
    --preExec 'all:from AthenaCommon.GlobalFlags import globalflags; globalflags.DataSource.set_Value_and_Lock("geant4");' 'HITtoRDO:from Digitization.DigitizationFlags import digitizationFlags; digitizationFlags.doInDetNoise.set_Value_and_Lock(False); digitizationFlags.overrideMetadata+=["SimLayout","PhysicsList"];' 'ESDtoDPD:rec.DPDMakerScripts.set_Value_and_Lock(["InDetPrepRawDataToxAOD/InDetDxAOD.py","PrimaryDPDMaker/PrimaryDPDMaker.py"]);' \
    --HGTDOn 'True' \
    --postExec 'HITtoRDO:pixeldigi.EnableSpecialPixels=False; CfgMgr.MessageSvc().setError+=["HepMcParticleLink"];' 'RAWtoESD:ToolSvc.InDetSCT_ClusteringTool.useRowInformation=True;' 'ESDtoAOD:StreamAOD.ItemList+=["CaloCellContainer#AllCalo"]' \
    --inputHITSFile $INFILE \
    --outputESDFile ESD.pool.root \
    --outputAODFile AOD.pool.root \
    --maxEvents $NEVT \
    --skipEvents $SKIP \
    --digiSeedOffset1 1${RNDMSEED} \
    --digiSeedOffset2 1${RNDMSEED}001

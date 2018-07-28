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


Reco_tf.py \
    --digiSteeringConf 'StandardInTimeOnlyTruth' \
    --conditionsTag OFLCOND-MC12-ITK-F-29-200-25 \
    --numberOfHighPtMinBias '0.69064' \
    --autoConfiguration 'everything' \
    --numberOfLowPtMinBias '199.304' \
    --DBRelease 'all:current' \
    --geometryVersion 'default:ATLAS-P2-ITK-06-01-00' \
    --preInclude 'all:InDetSLHC_Example/preInclude.SLHC_Setup_ExtBrl_4.py,HGTDAnalysisTools/preInclude.HGTD.py' 'default:InDetSLHC_Example/preInclude.SLHC.NoTRT.Reco.py,InDetSLHC_Example/SLHC_Setup_Reco_TrackingGeometry.py' 'HITtoRDO:InDetSLHC_Example/preInclude.SLHC.py,InDetSLHC_Example/preInclude.NoTRT.py,Digitization/ForceUseOfPileUpTools.py,SimulationJobOptions/preInclude.PileUpBunchTrains2012Config1_DigitConfig.py,RunDependentSimData/configLumi_muRange.py' 'RDOMergeAthenaMP:InDetSLHC_Example/preInclude.SLHC.py,InDetSLHC_Example/preInclude.NoTRT.py' 'POOLMergeAthenaMPAOD:InDetSLHC_Example/preInclude.SLHC.NoTRT.Ana.py,InDetSLHC_Example/SLHC_Setup_Reco_TrackingGeometry.py' 'POOLMergeAthenaMPDAODIDTRKVALID:InDetSLHC_Example/preInclude.SLHC.NoTRT.Ana.py,InDetSLHC_Example/SLHC_Setup_Reco_TrackingGeometry.py' \
    --DataRunNumber '240020' \
    --postInclude 'all:PyJobTransforms/UseFrontier.py,InDetSLHC_Example/postInclude.SLHC_Setup_ExtBrl_4.py' 'HITtoRDO:InDetSLHC_Example/postInclude.SLHC_Digitization_lowthresh.py' 'RAWtoESD:InDetSLHC_Example/postInclude.DigitalClustering.py,HGTDAnalysisTools/postInclude.TGCcabling.py' \
    --preExec 'all:userRunLumiOverride={"run":240020, "startmu":190.0, "endmu":210.0, "stepmu":1.0, "lb":1, "starttstamp":1410020000}; from LArROD.LArRODFlags import larRODFlags; larRODFlags.doOFCPileupOptimization.set_Value_and_Lock(True); larRODFlags.NumberOfCollisions.set_Value_and_Lock(200); from AthenaCommon.BeamFlags import jobproperties; jobproperties.Beam.bunchSpacing=25; from CaloTools.CaloNoiseFlags import jobproperties; jobproperties.CaloNoiseFlags.FixedLuminosity.set_Value_and_Lock(200*1.45/8*50/25); rec.doTrigger.set_Value_and_Lock(False)' 'HITtoRDO:from Digitization.DigitizationFlags import digitizationFlags; digitizationFlags.doInDetNoise.set_Value_and_Lock(False); digitizationFlags.overrideMetadata+=["SimLayout","PhysicsList"];' 'RAWtoESD:from CaloRec.CaloCellFlags import jobproperties;jobproperties.CaloCellFlags.doLArCellEmMisCalib=False; InDetFlags.doSLHCConversionFinding.set_Value_and_Lock(False);' 'ESDtoDPD:rec.doDPD.set_Value_and_Lock(True); rec.DPDMakerScripts.set_Value_and_Lock(["InDetPrepRawDataToxAOD/InDetDxAOD.py","PrimaryDPDMaker/PrimaryDPDMaker.py"]); from InDetPrepRawDataToxAOD.InDetDxAODJobProperties import InDetDxAODFlags; InDetDxAODFlags.ThinHitsOnTrack.set_Value_and_Lock(False); InDetDxAODFlags.ThinTrackSelection.set_Value_and_Lock("InDetTrackParticles.pt > 0.0*GeV"); InDetDxAODFlags.DumpTriggerInfo.set_Value_and_Lock(False); InDetDxAODFlags.DumpUnassociatedHits.set_Value_and_Lock(False); InDetDxAODFlags.DumpPixelInfo.set_Value_and_Lock(True); InDetDxAODFlags.DumpPixelRdoInfo.set_Value_and_Lock(True); InDetDxAODFlags.DumpSctInfo.set_Value_and_Lock(True); InDetDxAODFlags.DumpSctRdoInfo.set_Value_and_Lock(True)' \
    --postExec 'all:from IOVDbSvc.CondDB import conddb' 'HITtoRDO:pixeldigi.EnableSpecialPixels=False; CfgMgr.MessageSvc().setError+=["HepMcParticleLink"]; from AthenaCommon.CfgGetter import getPublicTool; getPublicTool("PixelDigitizationTool").ParticleBarcodeVeto=0; getPublicTool("SCT_DigitizationTool").ParticleBarcodeVeto=0;' 'ESDtoAOD:xAODMaker__xAODTruthCnvAlg("GEN_AOD2xAOD",WriteInTimePileUpTruth=True)' 'ESDtoDPD:xAODMaker__xAODTruthCnvAlg("GEN_AOD2xAOD",WriteInTimePileUpTruth=True)' \
    --inputHITSFile $INFILE \
    --inputLowPtMinbiasHitsFile root://eosatlas//eos/atlas/user/b/benitezj/HGTD/hits/LArHit_July26/mc15_valid.361034.Pythia8EvtGen_A2MSTW2008LO_minbias_inelastic_low.evgen.EVNT.e4800/ATLAS-P2-ITK-06-01-00_VALIDATION/Nominal/*.root \
    --inputHighPtMinbiasHitsFile root://eosatlas//eos/atlas/user/b/benitezj/HGTD/hits/LArHit_July26/mc15_valid.361035.Pythia8EvtGen_A2MSTW2008LO_minbias_inelastic_high.evgen.EVNT.e4800/ATLAS-P2-ITK-06-01-00_VALIDATION/Nominal/*.root \
    --outputESDFile ESD.pool.root \
    --outputAODFile AOD.pool.root \
    --maxEvents $NEVT \
    --skipEvents $SKIP \
    --jobNumber 1${RNDMSEED} \
    --digiSeedOffset1 1${RNDMSEED} \
    --digiSeedOffset2 1${RNDMSEED}001

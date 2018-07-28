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

###mu=200
Digi_tf.py \
    --inputHITSFile $INFILE \
    --outputRDOFile RDO.pool.root \
    --maxEvents $NEVT \
    --skipEvents $SKIP \
    --jobNumber 1${RNDMSEED} \
    --digiSeedOffset1 1${RNDMSEED} \
    --digiSeedOffset2 1${RNDMSEED}001
    --digiSteeringConf 'StandardInTimeOnlyTruth' \
    --conditionsTag OFLCOND-MC12-ITK-F-29-200-25 \
    --DataRunNumber '240020' \
    --numberOfHighPtMinBias '0.69064' \
    --numberOfLowPtMinBias '199.304' \
    --geometryVersion 'default:ATLAS-P2-ITK-06-02-00' \
    --inputLowPtMinbiasHitsFile /nfs/uiowapc/data/det-hgtd/hits/LArHit_July26_Merged/mc15_valid.361034.Pythia8EvtGen_A2MSTW2008LO_minbias_inelastic_low.evgen.EVNT.e4800/ATLAS-P2-ITK-06-02-00_VALIDATION/Nominal/*.root \
    --inputHighPtMinbiasHitsFile /nfs/uiowapc/data/det-hgtd/hits/LArHit_July26_Merged/mc15_valid.361035.Pythia8EvtGen_A2MSTW2008LO_minbias_inelastic_high.evgen.EVNT.e4800/ATLAS-P2-ITK-06-02-00_VALIDATION/Nominal/*.root \
    --preInclude 'all:InDetSLHC_Example/preInclude.SLHC_Setup_ExtBrl_4.py,HGTDAnalysisTools/preInclude.HGTD.py' 'HITtoRDO:InDetSLHC_Example/preInclude.SLHC.py,InDetSLHC_Example/preInclude.NoTRT.py,Digitization/ForceUseOfPileUpTools.py,SimulationJobOptions/preInclude.PileUpBunchTrains2012Config1_DigitConfig.py,RunDependentSimData/configLumi_muRange.py' \
    --postInclude 'all:PyJobTransforms/UseFrontier.py,InDetSLHC_Example/postInclude.SLHC_Setup_ExtBrl_4.py' 'HITtoRDO:InDetSLHC_Example/postInclude.SLHC_Digitization_lowthresh.py' \
    --preExec 'all:userRunLumiOverride={"run":240020, "startmu":190.0, "endmu":210.0, "stepmu":1.0, "lb":1, "starttstamp":1410020000}; from LArROD.LArRODFlags import larRODFlags; larRODFlags.doOFCPileupOptimization.set_Value_and_Lock(True); larRODFlags.NumberOfCollisions.set_Value_and_Lock(200); from AthenaCommon.BeamFlags import jobproperties; jobproperties.Beam.bunchSpacing=25; from CaloTools.CaloNoiseFlags import jobproperties; jobproperties.CaloNoiseFlags.FixedLuminosity.set_Value_and_Lock(200*1.45/8*50/25); rec.doTrigger.set_Value_and_Lock(False)' 'HITtoRDO:from Digitization.DigitizationFlags import digitizationFlags; digitizationFlags.doInDetNoise.set_Value_and_Lock(False); digitizationFlags.overrideMetadata+=["SimLayout","PhysicsList"];' \
    --postExec 'all:from IOVDbSvc.CondDB import conddb' 'HITtoRDO:pixeldigi.EnableSpecialPixels=False; CfgMgr.MessageSvc().setError+=["HepMcParticleLink"]; from AthenaCommon.CfgGetter import getPublicTool; getPublicTool("PixelDigitizationTool").ParticleBarcodeVeto=0; getPublicTool("SCT_DigitizationTool").ParticleBarcodeVeto=0;' 


#### mu=80
#igi_tf.py \
#   --inputHITSFile $INFILE \
#   --outputRDOFile RDO.pool.root \
#   --maxEvents $NEVT \
#   --skipEvents $SKIP \
#   --jobNumber 1${RNDMSEED} \
#   --digiSeedOffset1 1${RNDMSEED} \
#   --digiSeedOffset2 1${RNDMSEED}001 \
#   --digiSteeringConf 'StandardInTimeOnlyTruth' \
#   --conditionsTag OFLCOND-MC12-ITK-F-29-80-25 \
#   --DataRunNumber '240020' \
#   --numberOfHighPtMinBias '0.247398' \
#   --numberOfLowPtMinBias '79.7526' \
#   --geometryVersion 'default:ATLAS-P2-ITK-06-02-00' \
#   --inputLowPtMinbiasHitsFile /nfs/uiowapc/data/det-hgtd/hits/LArHit_July26_Merged/mc15_valid.361034.Pythia8EvtGen_A2MSTW2008LO_minbias_inelastic_low.evgen.EVNT.e4800/ATLAS-P2-ITK-06-02-00_VALIDATION/Nominal/*.root \
#   --inputHighPtMinbiasHitsFile /nfs/uiowapc/data/det-hgtd/hits/LArHit_July26_Merged/mc15_valid.361035.Pythia8EvtGen_A2MSTW2008LO_minbias_inelastic_high.evgen.EVNT.e4800/ATLAS-P2-ITK-06-02-00_VALIDATION/Nominal/*.root \
#   --preInclude 'all:InDetSLHC_Example/preInclude.SLHC_Setup_ExtBrl_4.py,HGTDAnalysisTools/preInclude.HGTD.py' 'HITtoRDO:InDetSLHC_Example/preInclude.SLHC.py,InDetSLHC_Example/preInclude.NoTRT.py,Digitization/ForceUseOfPileUpTools.py,SimulationJobOptions/preInclude.PileUpBunchTrains2012Config1_DigitConfig.py,RunDependentSimData/configLumi_muRange.py' \
#   --postInclude 'all:PyJobTransforms/UseFrontier.py,InDetSLHC_Example/postInclude.SLHC_Setup_ExtBrl_4.py' 'HITtoRDO:InDetSLHC_Example/postInclude.SLHC_Digitization_lowthresh.py'  \
#   --preExec 'all:userRunLumiOverride={"run":240020, "startmu":79.00, "endmu":81.00, "stepmu":0.1, "lb":1, "starttstamp":1410020000}; from LArROD.LArRODFlags import larRODFlags; larRODFlags.doOFCPileupOptimization.set_Value_and_Lock(True); larRODFlags.NumberOfCollisions.set_Value_and_Lock(80); from AthenaCommon.BeamFlags import jobproperties; jobproperties.Beam.bunchSpacing=25; from CaloTools.CaloNoiseFlags import jobproperties; jobproperties.CaloNoiseFlags.FixedLuminosity.set_Value_and_Lock(80*1.45/8*50/25); rec.doTrigger.set_Value_and_Lock(False)' 'HITtoRDO:from Digitization.DigitizationFlags import digitizationFlags; digitizationFlags.doInDetNoise.set_Value_and_Lock(False); digitizationFlags.overrideMetadata+=["SimLayout","PhysicsList"];' \
#   --postExec 'all:from IOVDbSvc.CondDB import conddb' 'HITtoRDO:pixeldigi.EnableSpecialPixels=False; CfgMgr.MessageSvc().setError+=["HepMcParticleLink"]; from AthenaCommon.CfgGetter import getPublicTool; getPublicTool("PixelDigitizationTool").ParticleBarcodeVeto=0; getPublicTool("SCT_DigitizationTool").ParticleBarcodeVeto=0;' \ 


##### mu=2: sampling seems to be ok
#Digi_tf.py \
#    --inputHITSFile $INFILE \
#    --outputRDOFile RDO.pool.root \
#    --maxEvents $NEVT \
#    --skipEvents $SKIP \
#    --jobNumber 1${RNDMSEED} \
#    --digiSeedOffset1 1${RNDMSEED} \
#    --digiSeedOffset2 1${RNDMSEED}001 \
#    --digiSteeringConf 'StandardInTimeOnlyTruth' \
#    --conditionsTag OFLCOND-MC12-ITK-27-00 \
#    --DataRunNumber '240020' \
#    --numberOfHighPtMinBias '0.0069064' \
#    --numberOfLowPtMinBias '1.99304' \
#    --geometryVersion 'default:ATLAS-P2-ITK-06-02-00' \
#    --inputLowPtMinbiasHitsFile /nfs/uiowapc/data/det-hgtd/hits/LArHit_July26_Merged/mc15_valid.361034.Pythia8EvtGen_A2MSTW2008LO_minbias_inelastic_low.evgen.EVNT.e4800/ATLAS-P2-ITK-06-02-00_VALIDATION/Nominal/*.root \
#    --inputHighPtMinbiasHitsFile /nfs/uiowapc/data/det-hgtd/hits/LArHit_July26_Merged/mc15_valid.361035.Pythia8EvtGen_A2MSTW2008LO_minbias_inelastic_high.evgen.EVNT.e4800/ATLAS-P2-ITK-06-02-00_VALIDATION/Nominal/*.root \
#    --preInclude 'all:InDetSLHC_Example/preInclude.SLHC_Setup_ExtBrl_4.py,HGTDAnalysisTools/preInclude.HGTD.py' 'HITtoRDO:InDetSLHC_Example/preInclude.SLHC.py,InDetSLHC_Example/preInclude.NoTRT.py,Digitization/ForceUseOfPileUpTools.py,SimulationJobOptions/preInclude.PileUpBunchTrains2012Config1_DigitConfig.py,RunDependentSimData/configLumi_muRange.py' \
#    --postInclude 'all:PyJobTransforms/UseFrontier.py,InDetSLHC_Example/postInclude.SLHC_Setup_ExtBrl_4.py' 'HITtoRDO:InDetSLHC_Example/postInclude.SLHC_Digitization_lowthresh.py'  \
#    --preExec 'all:userRunLumiOverride={"run":240020, "startmu":1.900, "endmu":2.100, "stepmu":0.01, "lb":1, "starttstamp":1410020000}; from LArROD.LArRODFlags import larRODFlags; larRODFlags.doOFCPileupOptimization.set_Value_and_Lock(True); larRODFlags.NumberOfCollisions.set_Value_and_Lock(2); from AthenaCommon.BeamFlags import jobproperties; jobproperties.Beam.bunchSpacing=25; from CaloTools.CaloNoiseFlags import jobproperties; jobproperties.CaloNoiseFlags.FixedLuminosity.set_Value_and_Lock(2*1.45/8*50/25); rec.doTrigger.set_Value_and_Lock(False)' 'HITtoRDO:from Digitization.DigitizationFlags import digitizationFlags; digitizationFlags.doInDetNoise.set_Value_and_Lock(False); digitizationFlags.overrideMetadata+=["SimLayout","PhysicsList"];' \
#    --postExec 'all:from IOVDbSvc.CondDB import conddb' 'HITtoRDO:pixeldigi.EnableSpecialPixels=False; CfgMgr.MessageSvc().setError+=["HepMcParticleLink"]; from AthenaCommon.CfgGetter import getPublicTool; getPublicTool("PixelDigitizationTool").ParticleBarcodeVeto=0; getPublicTool("SCT_DigitizationTool").ParticleBarcodeVeto=0;' \ 

##mu=0 : this works fine gives valid sampling values
#Digi_tf.py \
#    --inputHITSFile $INFILE \
#    --outputRDOFile RDO.pool.root \
#    --maxEvents $NEVT \
#    --skipEvents $SKIP \
#    --jobNumber 1${RNDMSEED} \
#    --digiSeedOffset1 1${RNDMSEED} \
#    --digiSeedOffset2 1${RNDMSEED}001 \
#    --digiSteeringConf 'StandardInTimeOnlyTruth' \
#    --conditionsTag OFLCOND-MC12-ITK-27-00 \
#    --geometryVersion ATLAS-P2-ITK-06-02-00 \
#    --preInclude 'all:InDetSLHC_Example/preInclude.SLHC_Setup_ExtBrl_4.py,HGTDAnalysisTools/preInclude.HGTD.py,InDetSLHC_Example/preInclude.SLHC.py,InDetSLHC_Example/preInclude.NoTRT.py' \
#    --postInclude 'all:PyJobTransforms/UseFrontier.py,InDetSLHC_Example/postInclude.SLHC_Setup_ExtBrl_4.py,InDetSLHC_Example/postInclude.SLHC_Digitization_lowthresh.py'  \
#    --postExec 'all:from IOVDbSvc.CondDB import conddb; pixeldigi.EnableSpecialPixels=False; CfgMgr.MessageSvc().setError+=["HepMcParticleLink"]; from AthenaCommon.CfgGetter import getPublicTool; getPublicTool("PixelDigitizationTool").ParticleBarcodeVeto=0; getPublicTool("SCT_DigitizationTool").ParticleBarcodeVeto=0;' 

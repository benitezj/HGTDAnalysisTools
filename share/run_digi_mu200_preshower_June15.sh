
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

###############################################
###Pileup Parameters:
#################################################
#root [1] pileupParameters(20)
# numberOfLowPtMinBias=19.9382  numberOfHighPtMinBias=0.0618495
#root [2] pileupParameters(40)
# numberOfLowPtMinBias=39.8763  numberOfHighPtMinBias=0.123699
#root [2] pileupParameters(80)
# numberOfLowPtMinBias=79.7526  numberOfHighPtMinBias=0.247398
#root [2] pileupParameters(100)
# numberOfLowPtMinBias=99.6908  numberOfHighPtMinBias=0.309247
#root [3] pileupParameters(140)
# numberOfLowPtMinBias=139.567  numberOfHighPtMinBias=0.432946
#root [4] pileupParameters(200)
# numberOfLowPtMinBias=199.382  numberOfHighPtMinBias=0.618495

############################################
### command from these twikis 
##########################################
##Works here: /nfs/home/benitezj/HGTD/20.20_SiHit_OldGeo_Digi
##https://twiki.cern.ch/twiki/bin/viewauth/Atlas/UpgradeSimulationInnerTrackerRel20p3p3WithPileup
##https://twiki.cern.ch/twiki/bin/view/Atlas/UpgradeSimulationInnerTrackerMigrationRel20p3#Conditions_tags
## Note: the conditionsTag should match the mu value 
## Note: the preExec should match the mu value
## Note: Johns fix for the truth merging in the postExec
Digi_tf.py \
    --inputHITSFile $INFILE \
    --outputRDOFile RDO.pool.root \
    --skipEvents $SKIP \
    --maxEvents $NEVT \
    --geometryVersion ATLAS-P2-ITK-01-02-00 \
    --conditionsTag OFLCOND-MC12-ITK-F-29-200-25 \
    --digiSeedOffset1 1${RNDMSEED} \
    --digiSeedOffset2 1${RNDMSEED}001 \
    --inputLowPtMinbiasHitsFile /nfs/uiowapc/data/det-hgtd/hits/SiHit_June15_Preshower/mc15_valid.361034.Pythia8EvtGen_A2MSTW2008LO_minbias_inelastic_low.evgen.EVNT.e4800/HITS_*.root \
    --inputHighPtMinbiasHitsFile /nfs/uiowapc/data/det-hgtd/hits/SiHit_June15_Preshower/mc15_valid.361035.Pythia8EvtGen_A2MSTW2008LO_minbias_inelastic_high.evgen.EVNT.e4800/HITS_*.root \
    --numberOfLowPtMinBias=199.382 \
    --numberOfHighPtMinBias=0.618495 \
    --pileupFinalBunch 6 \
    --jobNumber 1${RNDMSEED} \
    --preInclude 'all:InDetSLHC_Example/preInclude.SLHC.py,InDetSLHC_Example/preInclude.NoTRT.py,InDetSLHC_Example/preInclude.SLHC_Setup_LoI.py,HGTDAnalysisTools/preInclude.MuonOff.py,HGTDAnalysisTools/preInclude.HGTD.py' 'HITtoRDO:Digitization/ForceUseOfPileUpTools.py,SimulationJobOptions/preInclude.PileUpBunchTrains2012Config1_DigitConfig.py,RunDependentSimData/configLumi_user.py' \
    --postInclude 'all:PyJobTransforms/UseFrontier.py,InDetSLHC_Example/postInclude.SLHC_Digitization_lowthresh.py,InDetSLHC_Example/postInclude.SLHC_Setup_LoI.py' \
    --preExec 'HITtoRDO:from LArROD.LArRODFlags import larRODFlags; larRODFlags.doOFCPileupOptimization.set_Value_and_Lock(True); larRODFlags.NumberOfCollisions.set_Value_and_Lock(200); from Digitization.DigitizationFlags import digitizationFlags; digitizationFlags.doInDetNoise.set_Value_and_Lock(False); digitizationFlags.overrideMetadata+=["SimLayout","PhysicsList"]; from AthenaCommon.BeamFlags import jobproperties; jobproperties.Beam.bunchSpacing=25; from CaloTools.CaloNoiseFlags import jobproperties; jobproperties.CaloNoiseFlags.FixedLuminosity.set_Value_and_Lock(200.0*1.45/8*50/25); userRunLumiOverride={"run":195847, "lb":1, "starttstamp":1328040250, "mu":200.0}' \
    --postExec 'HITtoRDO:from AthenaCommon.CfgGetter import getPublicTool;mergeMcEventCollTool=getPublicTool("MergeMcEventCollTool");mergeMcEventCollTool.OnlySaveSignalTruth=False;mergeMcEventCollTool.FirstXing=0;mergeMcEventCollTool.LastXing=0;mergeMcEventCollTool.DoSlimming=False;pixeldigi.EnableSpecialPixels=False;CfgMgr.MessageSvc().setError+=["HepMcParticleLink"];streamRDO.ItemList+=["SiHitCollection#HGTDHits","SiHitCollection#MergedHGTDHits"]'



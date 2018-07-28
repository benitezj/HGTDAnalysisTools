Digi_tf.py \
--inputHITSFile g4hits_withHGTD.pool.root \
--outputRDOFile RDO.pool.root \
--skipEvents 0 \
--maxEvents 1 \
--geometryVersion 'all:ATLAS-P2-ITK-06-01-00' \
--conditionsTag 'all:OFLCOND-MC12-ITK-F-29-80-25' \
--digiSeedOffset1 459 \
--digiSeedOffset2 459 \
--jobNumber 459 \
--LowPtMinbiasHitsFile minbiasLow.HITS.pool.root \
--numberOfLowPtMinBias 79.7216 \
--HighPtMinbiasHitsFile minbiasHigh.HITS.pool.root \
--numberOfHighPtMinBias 0.276256 \
--pileupFinalBunch 6 \
--preInclude 'all:InDetSLHC_Example/preInclude.SLHC.py,InDetSLHC_Example/preInclude.SLHC_Setup_ExtBrl_4.py,InDetSLHC_Example/preInclude.NoTRT.py,preInclude.HGTD.py' 'HITtoRDO:Digitization/ForceUseOfPileUpTools.py,SimulationJobOptions/preInclude.PileUpBunchTrains2012Config1_DigitConfig.py,RunDependentSimData/configLumi_user.py' \
--postInclude 'all:PyJobTransforms/UseFrontier.py,InDetSLHC_Example/postInclude.SLHC_Setup.py,InDetSLHC_Example/postInclude.SLHC_Setup_ExtBrl_4.py' 'HITtoRDO:InDetSLHC_Example/postInclude.SLHC_Digitization_lowthresh.py' \
--preExec 'HITtoRDO:from LArROD.LArRODFlags import larRODFlags; larRODFlags.doOFCPileupOptimization.set_Value_and_Lock(True); larRODFlags.NumberOfCollisions.set_Value_and_Lock(80); from Digitization.DigitizationFlags import digitizationFlags; digitizationFlags.doInDetNoise.set_Value_and_Lock(False); digitizationFlags.overrideMetadata+=["SimLayout","PhysicsList"]; from AthenaCommon.BeamFlags import jobproperties; jobproperties.Beam.bunchSpacing=25; from CaloTools.CaloNoiseFlags import jobproperties; jobproperties.CaloNoiseFlags.FixedLuminosity.set_Value_and_Lock(80.0*1.45/8*50/25); userRunLumiOverride={"run":195847, "lb":1, "starttstamp":1328040250, "mu":80.0}' \
--postExec 'HITtoRDO:mergeMcEventCollTool=job.StandardPileUpToolsAlg.PileUpTools["MergeMcEventCollTool"]; mergeMcEventCollTool.OnlySaveSignalTruth=False;mergeMcEventCollTool.FirstXing=0; mergeMcEventCollTool.LastXing=0; mergeMcEventCollTool.DoSlimming=False; pixeldigi.EnableSpecialPixels=False; CfgMgr.MessageSvc().setError+=["HepMcParticleLink"]'

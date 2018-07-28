Sim_tf.py \
      --inputEVNTFile /afs/cern.ch/user/j/jabdalla/public/HGTD/EvtGen/MC15.361021.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ1W.pool.root \
      --outputHITSFile g4hits_withHGTD.pool.root \
      --geometryVersion 'all:ATLAS-P2-ITK-06-01-00_VALIDATION' \
      --conditionsTag 'default:OFLCOND-MC12-ITK-27-00' \
      --DataRunNumber '195847' \
      --maxEvents 1 \
      --randomSeed 873254 \
      --postExec 'EVNTtoHITS:ToolSvc.ISF_ParticlePositionFilterDynamic.CheckRegion=[1]' \
      --preInclude 'EVNTtoHITS:InDetSLHC_Example/preInclude.SLHC.py,InDetSLHC_Example/preInclude.NoTRT.py,InDetSLHC_Example/preInclude.SLHC_Setup_ExtBrl_4.py,preInclude.MuonOff.py,preInclude.HGTD.py' \
      --postInclude 'all:PyJobTransforms/UseFrontier.py,InDetSLHC_Example/postInclude.SLHC_Setup_ExtBrl_4.py,Simulation/ISF/ISF_HepMC/ISF_HepMC_Tools/share/postInclude.CrabKissingVertexPositioner_Nominal.py'

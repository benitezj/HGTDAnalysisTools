from METReconstruction.METRecoFlags import metFlags
from METReconstruction.METAssocConfig import AssocConfig, METAssocConfig

############################################################################
# AntiKt4LCTopo
JetType = 'LCJet'

associators = [AssocConfig(JetType),
               AssocConfig('Ele'),
               AssocConfig('Gamma'),
               AssocConfig('Tau'),
               AssocConfig('Soft')]
cfg_akt4lc = METAssocConfig('AntiKt4LCTopo',
                            associators,
                            doPFlow=False
                            )

metFlags.METAssocConfigs()[cfg_akt4lc.suffix] = cfg_akt4lc
metFlags.METAssocOutputList().append(cfg_akt4lc.suffix)

############################################################################
# AntiKt4EMTopo
JetType = 'EMJet'
    
associators = [AssocConfig(JetType),
               AssocConfig('Ele'),
               AssocConfig('Gamma'),
               AssocConfig('Tau'),
               AssocConfig('Soft')]
cfg_akt4em = METAssocConfig('AntiKt4EMTopo',
                            associators,
                            doPFlow=False
                            )

metFlags.METAssocConfigs()[cfg_akt4em.suffix] = cfg_akt4em
metFlags.METAssocOutputList().append(cfg_akt4em.suffix)

############################################################################
# PFlow
from RecExConfig.RecFlags import rec
if rec.doInDet() and metFlags.DoPFlow():
    JetType = 'PFlowJet'
    
    associators = [AssocConfig(JetType),
                   AssocConfig('Ele'),
                   AssocConfig('Gamma'),
                   AssocConfig('Tau'),
                   AssocConfig('Soft')]
    cfg_akt4pf = METAssocConfig('AntiKt4EMPFlow',
                                associators,
                                doPFlow=True
                                )

    metFlags.METAssocConfigs()[cfg_akt4pf.suffix] = cfg_akt4pf
    metFlags.METAssocOutputList().append(cfg_akt4pf.suffix)

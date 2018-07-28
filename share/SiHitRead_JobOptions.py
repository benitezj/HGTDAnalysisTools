from AthenaCommon.AppMgr import ServiceMgr
import AthenaPoolCnvSvc.ReadAthenaPool

from PartPropSvc.PartPropSvcConf import PartPropSvc

include( "ParticleBuilderOptions/McAOD_PoolCnv_jobOptions.py")
include( "EventAthenaPool/EventAthenaPool_joboptions.py" )

import os
from glob import glob
from AthenaCommon.AthenaCommonFlags  import athenaCommonFlags
#athenaCommonFlags.FilesInput = glob( "/tmp/"+os.environ['USER']+"HITS*root*" )
#athenaCommonFlags.FilesInput = glob("/afs/cern.ch/user/d/druini/public/HGTD/g4hits_electron_new.pool.root")
#athenaCommonFlags.FilesInput = glob("/afs/cern.ch/user/d/dzerwas/public/HGTD/g4hits_withHGTD.pool.root")
#athenaCommonFlags.FilesInput = glob("/nfs/uiowapc/data/det-hgtd/hits/SiHit_April8/mc15_valid.361034.Pythia8EvtGen_A2MSTW2008LO_minbias_inelastic_low.evgen.EVNT.e4800/HITS_0.root")
athenaCommonFlags.FilesInput = glob("/nfs/uiowapc/data/det-hgtd/hits/SiHit_April12_Preshower/mc15_13TeV.341080.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_WWlvlv_EF_15_5.evgen.EVNT.e4397/HITS_*.root")
#athenaCommonFlags.FilesInput = glob("/nfs/uiowapc/data/det-hgtd/digit/SiHit_April12/mc15_13TeV.341080.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_WWlvlv_EF_15_5.evgen.EVNT.e4397/RDO_0.root")
#athenaCommonFlags.FilesInput = glob("/nfs/uiowapc/data/det-hgtd/aod/SiHit_April12_Preshower/mc15_13TeV.341080.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_WWlvlv_EF_15_5.evgen.EVNT.e4397/AOD_0.root")
#athenaCommonFlags.FilesInput = glob("./HITS.pool.root")
#athenaCommonFlags.FilesInput = glob("./g4hits_withHGTD.pool.root")
#athenaCommonFlags.FilesInput = glob("./RDO.pool.root")
#athenaCommonFlags.FilesInput = glob("./AOD.pool.root")
#athenaCommonFlags.FilesInput = glob("/nfs/uiowapc/data/det-hgtd/hits/LArHit/mc15_13TeV.341080.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_WWlvlv_EF_15_5.evgen.EVNT.e4397/HITS_0.root")
#athenaCommonFlags.FilesInput = glob("/nfs/uiowapc/data/det-hgtd/hits/LArHit_March25/mc15_13TeV.341080.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_WWlvlv_EF_15_5.evgen.EVNT.e4397/HITS_*.root")
#athenaCommonFlags.FilesInput = glob("./RDO_mu100.pool.root")
ServiceMgr.EventSelector.InputCollections = athenaCommonFlags.FilesInput() # This is stupid and redundant, but necessary

from AthenaCommon.AlgSequence import AlgSequence
topSequence = AlgSequence()


from HGTDAnalysisTools.HGTDAnalysisToolsConf import SiHitRead as HitRead
HitRead = HitRead("HitRead")
HitRead.OutputLevel = DEBUG
HitRead.InputCollectionName="HGTDHits"
#HitRead.InputCollectionName="MergedHGTDHits"
topSequence += HitRead


from GaudiSvc.GaudiSvcConf import THistSvc
ServiceMgr += THistSvc()
ServiceMgr.THistSvc.Output += [ "HGTDHitAnalysis DATAFILE='HGTDHitAnalysis.root' OPT='RECREATE'" ]


theApp.EvtMax = -1


### To set up a geometry
#from AthenaCommon.DetFlags  import DetFlags
#DetFlags.dcs.all_setOn()
#DetFlags.detdescr.all_setOn()
#DetFlags.makeRIO.all_setOn()
#DetFlags.TRT_setOff()
#DetFlags.pixel_setOff()
#DetFlags.dcs.HGTD_setOn()
#DetFlags.detdescr.HGTD_setOn()
#DetFlags.makeRIO.HGTD_setOn()
#
###**Note: Since geometry tag is not provided it will take it from the input file**
#from RecExConfig.AutoConfiguration import *
#ConfigureFieldAndGeo() # Configure the settings for the geometry
#include("RecExCond/AllDet_detDescr.py") # Actually load the geometry



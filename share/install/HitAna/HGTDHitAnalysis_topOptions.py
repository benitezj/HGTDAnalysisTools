#from AthenaCommon.AppMgr import ToolSvc
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
#athenaCommonFlags.FilesInput = glob("/afs/cern.ch/user/d/dzerwas/private/HGTD/g4hits_withHGTD.pool.root")
athenaCommonFlags.FilesInput = glob("./HITS.pool.root")
ServiceMgr.EventSelector.InputCollections = athenaCommonFlags.FilesInput() # This is stupid and redundant, but necessary

from AthenaCommon.AlgSequence import AlgSequence
topSequence = AlgSequence()

from HitAnalysis.HitAnalysisConf import HGTDHitAnalysis
topSequence += HGTDHitAnalysis() 
HGTDHitAnalysis = HGTDHitAnalysis()
HGTDHitAnalysis.NtupleFileName = 'HGTDHitAnalysis'

from GaudiSvc.GaudiSvcConf import THistSvc
ServiceMgr += THistSvc()
ServiceMgr.THistSvc.Output += [ "HGTDHitAnalysis DATAFILE='HGTDHitAnalysis.root' OPT='RECREATE'" ]

ServiceMgr.MessageSvc.OutputLevel = INFO
ServiceMgr.MessageSvc.defaultLimit = 9999999

theApp.EvtMax = -1

ServiceMgr.AuditorSvc.Auditors  += [ "ChronoAuditor"]

AthenaPoolCnvSvc = Service("AthenaPoolCnvSvc")
AthenaPoolCnvSvc.UseDetailChronoStat = TRUE

# To set up a geometry
#include("RecExCond/RecExCommon_DetFlags.py")
from AthenaCommon.DetFlags  import DetFlags
DetFlags.dcs.all_setOn()
DetFlags.detdescr.all_setOn()
DetFlags.makeRIO.all_setOn()
# these have to be set off by hand!
DetFlags.TRT_setOff()
DetFlags.pixel_setOff()
# these have to be set On by hand!
DetFlags.dcs.HGTD_setOn()
DetFlags.detdescr.HGTD_setOn()
DetFlags.makeRIO.HGTD_setOn()


from RecExConfig.AutoConfiguration import *
ConfigureFieldAndGeo() # Configure the settings for the geometry
include("RecExCond/AllDet_detDescr.py") # Actually load the geometry
#include( "TrkDetDescrSvc/AtlasTrackingGeometrySvc.py" ) # Tracking geometry, handy for ID work
#DetFlags.pixel_setOff()


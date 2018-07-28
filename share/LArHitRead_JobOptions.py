from AthenaCommon.AlgSequence import AlgSequence
topSequence = AlgSequence()

theApp.EvtMax = 2


#### load the input file
from glob import glob
from AthenaCommon.AthenaCommonFlags  import athenaCommonFlags
from AthenaCommon.AppMgr import ServiceMgr
import AthenaPoolCnvSvc.ReadAthenaPool
#athenaCommonFlags.FilesInput = glob("./AOD.pool.root")
athenaCommonFlags.FilesInput = glob("./HITS.pool.root")
ServiceMgr.EventSelector.InputCollections = athenaCommonFlags.FilesInput() 


####include the test module
from HGTDAnalysisTools.HGTDAnalysisToolsConf import LArHitRead
LArHitRead = LArHitRead("LArHitRead")
#LArHitRead.OutputLevel = DEBUG
#LArHitRead.InputCollectionName="LArHitHGTD" # for HITs
LArHitRead.InputCollectionName="HGTDDigitContainer_MC" #for AOD
topSequence += LArHitRead


###set special detector flags
from AthenaCommon.DetFlags  import DetFlags
DetFlags.dcs.all_setOn()
DetFlags.detdescr.all_setOn()
DetFlags.makeRIO.all_setOn()
DetFlags.TRT_setOff()
DetFlags.pixel_setOff()
DetFlags.dcs.HGTD_setOn()
DetFlags.detdescr.HGTD_setOn()
DetFlags.makeRIO.HGTD_setOn()

# Tools configuration of the GMX based Strip description
from SCT_GeoModelXml.SCT_GeoModelXmlConf import SCT_GMX_DetectorTool
SCT_GMX_DetectorTool.GmxFilename = "ITkStrip.gmx"
SCT_GMX_DetectorTool.OutputLevel = VERBOSE

from AthenaCommon.CfgGetter import getPublicTool
getPublicTool("SLHC_SctSensorSD").GmxSensor=True 

###Load geometry
from RecExConfig.AutoConfiguration import *
ConfigureFieldAndGeo() # Configure the settings for the geometry
include("RecExCond/AllDet_detDescr.py") # Actually load the geometry


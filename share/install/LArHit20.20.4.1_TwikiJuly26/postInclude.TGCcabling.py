from MuonCablingServers.MuonCablingServersConf import TGCcablingServerSvc
ServiceMgr += TGCcablingServerSvc()
theApp.CreateSvc += [ "TGCcablingServerSvc" ]
ServiceMgr.TGCcablingServerSvc.Atlas=True
ServiceMgr.TGCcablingServerSvc.forcedUse=True
ServiceMgr.TGCcablingServerSvc.useMuonTGC_CablingSvc=True
from TGC_CondCabling.TGC_CondCablingConf import TGCCablingDbTool
ToolSvc += TGCCablingDbTool()
from IOVDbSvc.CondDB import conddb
conddb.addFolderSplitMC('TGC','/TGC/CABLING/MAP_SCHEMA','/TGC/CABLING/MAP_SCHEMA')

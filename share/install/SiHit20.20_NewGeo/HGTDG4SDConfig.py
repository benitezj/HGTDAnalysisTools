from AthenaCommon import CfgMgr
def getHGTDSensorSD(name="HGTDSensorSD", **kwargs):
    kwargs.setdefault("LogicalVolumeNames", ["LArMgr::HGTDSiSensor0", "LArMgr::HGTDSiSensor1", "LArMgr::HGTDSiSensor2", "LArMgr::HGTDSiSensor3"])
    kwargs.setdefault("OutputCollectionNames", ["HGTDHits"])
    return CfgMgr.HGTDSensorSDTool(name, **kwargs)


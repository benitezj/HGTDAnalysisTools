from AthenaCommon import CfgMgr
def getHGTDSensorSD(name="HGTDSensorSD", **kwargs):
    kwargs.setdefault("LogicalVolumeNames", ["LArMgr::HGTDsi1", "LArMgr::HGTDsi2", "LArMgr::HGTDsi3", "LArMgr::HGTDsi4"])
    kwargs.setdefault("OutputCollectionNames", ["LArHitHGTD"])
    kwargs.setdefault("TimeBinType", "Uniform")
    kwargs.setdefault("TimeBinWidth", 0.005)
    kwargs.setdefault("TimeWindowMin",0.)
    kwargs.setdefault("TimeWindowMax",14.5)
    return CfgMgr.HGTDSensorSDTool(name, **kwargs)

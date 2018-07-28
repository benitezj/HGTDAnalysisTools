from AthenaCommon import CfgMgr
from AthenaCommon import CfgGetter # import getService, getPrivateTool

def getTestPileUpTool(name="TestPileUpTool", **kwargs):
    kwargs.setdefault('OutputLevel', VERBOSE )
    kwargs.setdefault('FirstXing', -300 )
    kwargs.setdefault('LastXing', +300 )
    return CfgGetter.TestPileUpTool(name, **kwargs)


def getTestPileUpToolsAlg(name="TestPileUpToolsAlg", **kwargs):
    PileUpToolsList = [ "TestPileUpTool" ]
    kwargs.setdefault('PileupTools', PileUpToolsList )
    from PileUpComps.PileUpCompsConf import PileUpToolsAlg
    return PileUpToolsAlg(name, **kwargs)


def getTestDigitizationAlg(name="TestDigitizationAlg", **kwargs):
    PileUpToolsList = [ "TestPileUpTool" ]
    kwargs.setdefault('PileupTools', PileUpToolsList )
    from PileUpComps.PileUpCompsConf import DigitizationAlg
    return DigitizationAlg(name, **kwargs)

#####################################
## PileUpToolsList filling methods ##
#####################################

def getStandardTruthPileUpTools():
    from AthenaCommon.DetFlags import DetFlags
    PileUpToolsList = []
    if DetFlags.pileup.Truth_on():
        PileUpToolsList += [ "MergeMcEventCollTool" ]
        ##Jose: this tool is throwing many errors 
        #PileUpToolsList += [ "MergeTruthJetsTool" ]
        if DetFlags.writeRDOPool.Muon_on(): #possibly this should be digitize.Muon_on()
            PileUpToolsList += [ "MergeTrackRecordCollTool" ]
        if DetFlags.writeRDOPool.Calo_on(): #possibly this should be digitize.Calo_on()
            PileUpToolsList += [ "MergeCalibHitsTool" ]
    return PileUpToolsList

def getStandardSignalOnlyTruthPileUpTools():
    from AthenaCommon.DetFlags import DetFlags
    PileUpToolsList = []
    if DetFlags.pileup.Truth_on():
        PileUpToolsList += [ "SignalOnlyMcEventCollTool" ]
        PileUpToolsList += [ "MergeTruthJetsTool" ]
        if DetFlags.writeRDOPool.Muon_on(): #possibly this should be digitize.Muon_on()
            PileUpToolsList += [ "MergeTrackRecordCollTool" ]
        if DetFlags.writeRDOPool.Calo_on(): #possibly this should be digitize.Calo_on()
            PileUpToolsList += [ "MergeCalibHitsTool" ]
    return PileUpToolsList

def getStandardInTimeOnlyTruthPileUpTools():
    from AthenaCommon.DetFlags import DetFlags
    PileUpToolsList = []
    if DetFlags.pileup.Truth_on():
        PileUpToolsList += [ "InTimeOnlyMcEventCollTool" ]
        PileUpToolsList += [ "MergeTruthJetsTool" ]
        if DetFlags.writeRDOPool.Muon_on(): #possibly this should be digitize.Muon_on()
            PileUpToolsList += [ "MergeTrackRecordCollTool" ]
        if DetFlags.writeRDOPool.Calo_on(): #possibly this should be digitize.Calo_on()
            PileUpToolsList += [ "MergeCalibHitsTool" ]
    return PileUpToolsList

def getStandardForwardPileUpTools():
    from AthenaCommon.DetFlags import DetFlags
    PileUpToolsList = []
    if DetFlags.digitize.Lucid_on():
        PileUpToolsList += [ "LUCID_PileUpTool" ]
    if DetFlags.digitize.ZDC_on():
        PileUpToolsList += [ "ZDC_PileUpTool" ]
    if DetFlags.digitize.ALFA_on():
        PileUpToolsList += [ "ALFA_PileUpTool" ]
    if hasattr(DetFlags.digitize, 'AFP_on') and DetFlags.digitize.AFP_on():
        PileUpToolsList += [ "AFP_PileUpTool" ]
    return PileUpToolsList

def getStandardInDetPileUpTools():
    from AthenaCommon.DetFlags import DetFlags
    PileUpToolsList = []
    if DetFlags.digitize.BCM_on():
        PileUpToolsList += [ "BCM_DigitizationTool" ]
    if DetFlags.digitize.pixel_on():
        PileUpToolsList += [ "PixelDigitizationTool" ]
    if DetFlags.digitize.SCT_on():
        PileUpToolsList += [ "SCT_DigitizationTool" ]
    if DetFlags.digitize.TRT_on():
        PileUpToolsList += [ "TRTDigitizationTool" ]
    return PileUpToolsList

def getFastInDetPileUpTools():
    from AthenaCommon.DetFlags import DetFlags
    from Digitization.DigitizationFlags import digitizationFlags
    PileUpToolsList = []
    if DetFlags.digitize.BCM_on():
        PileUpToolsList += [ "BCM_DigitizationTool" ]
    if DetFlags.digitize.pixel_on():
        if 'doFastPixelDigi' in digitizationFlags.experimentalDigi():
            PileUpToolsList += [ "PixelFastDigitizationTool" ]
        else:
            PileUpToolsList += [ "PixelDigitizationTool" ]
    if DetFlags.digitize.SCT_on():
        if 'doFastSCT_Digi' in digitizationFlags.experimentalDigi():
            PileUpToolsList += [ "SCT_FastDigitizationTool" ]
        else:
            PileUpToolsList += [ "SCT_DigitizationTool" ]
    if DetFlags.digitize.TRT_on():
        if 'doFastTRT_Digi' in digitizationFlags.experimentalDigi():
            PileUpToolsList += [ "TRTFastDigitizationTool" ]
        else:
            PileUpToolsList += [ "TRTDigitizationTool" ]
    return PileUpToolsList

def getSplitInDetPileUpTools():
    from AthenaCommon.DetFlags import DetFlags
    PileUpToolsList = []
    if DetFlags.digitize.BCM_on():
        PileUpToolsList += [ "BCM_DigitizationTool" ]
    if DetFlags.digitize.pixel_on():
        PileUpToolsList += [ "PixelDigitizationToolHS" ]
        PileUpToolsList += [ "PixelDigitizationToolPU" ]
    if DetFlags.digitize.SCT_on():
        PileUpToolsList += [ "SCT_DigitizationToolHS" ]
        PileUpToolsList += [ "SCT_DigitizationToolPU" ]
    if DetFlags.digitize.TRT_on():
        PileUpToolsList += [ "TRTDigitizationToolHS" ]
        PileUpToolsList += [ "TRTDigitizationToolPU" ]
    return PileUpToolsList

def getSplitNoMergeInDetPileUpTools():
    """ This is for testing the fast chain with full digitization in split PU mode without
merging pileup
    """
    from AthenaCommon.DetFlags import DetFlags
    PileUpToolsList = []
    if DetFlags.digitize.BCM_on():
        PileUpToolsList += [ "BCM_DigitizationTool" ]
    if DetFlags.digitize.pixel_on():
        PileUpToolsList += [ "PixelDigitizationTool" ]
        PileUpToolsList += [ "PixelDigitizationToolSplitNoMergePU" ]
    if DetFlags.digitize.SCT_on():
        PileUpToolsList += [ "SCT_DigitizationTool" ]
        PileUpToolsList += [ "SCT_DigitizationToolSplitNoMergePU" ]
    if DetFlags.digitize.TRT_on():
        PileUpToolsList += [ "TRTDigitizationTool" ]
        PileUpToolsList += [ "TRTDigitizationToolSplitNoMergePU" ]
    return PileUpToolsList

def getSplitNoMergeSFInDetPileUpTools():
    """ This is for testing the fast chain with full digitization in split PU mode without
merging pileup
    """
    from AthenaCommon.DetFlags import DetFlags
    PileUpToolsList = []
    if DetFlags.digitize.BCM_on():
        PileUpToolsList += [ "BCM_DigitizationTool" ]
    if DetFlags.digitize.pixel_on():
        PileUpToolsList += [ "PixelDigitizationTool" ]
        PileUpToolsList += [ "PixelFastDigitizationToolSplitNoMergePU" ]
    if DetFlags.digitize.SCT_on():
        PileUpToolsList += [ "SCT_DigitizationTool" ]
        PileUpToolsList += [ "SCT_FastDigitizationToolSplitNoMergePU" ]
    if DetFlags.digitize.TRT_on():
        PileUpToolsList += [ "TRTDigitizationTool" ]
        PileUpToolsList += [ "TRTFastDigitizationToolSplitNoMergePU" ]
    return PileUpToolsList

def getSplitNoMergeFSInDetPileUpTools():
    """ This is for testing the fast chain with full digitization in split PU mode without
merging pileup
    """
    from AthenaCommon.DetFlags import DetFlags
    PileUpToolsList = []
    if DetFlags.digitize.BCM_on():
        PileUpToolsList += [ "BCM_DigitizationTool" ]
    if DetFlags.digitize.pixel_on():
        PileUpToolsList += [ "PixelFastDigitizationTool" ]
        PileUpToolsList += [ "PixelDigitizationToolSplitNoMergePU" ]
    if DetFlags.digitize.SCT_on():
        PileUpToolsList += [ "SCT_FastDigitizationTool" ]
        PileUpToolsList += [ "SCT_DigitizationToolSplitNoMergePU" ]
    if DetFlags.digitize.TRT_on():
        PileUpToolsList += [ "TRTFastDigitizationTool" ]
        PileUpToolsList += [ "TRTDigitizationToolSplitNoMergePU" ]
    return PileUpToolsList

def getSplitNoMergeFFInDetPileUpTools():
    """ This is for testing the fast chain with full digitization in split PU mode without
merging pileup
    """
    from AthenaCommon.DetFlags import DetFlags
    PileUpToolsList = []
    if DetFlags.digitize.BCM_on():
        PileUpToolsList += [ "BCM_DigitizationTool" ]
    if DetFlags.digitize.pixel_on():
        PileUpToolsList += [ "PixelFastDigitizationTool" ]
        PileUpToolsList += [ "PixelFastDigitizationToolSplitNoMergePU" ]
    if DetFlags.digitize.SCT_on():
        PileUpToolsList += [ "SCT_FastDigitizationTool" ]
        PileUpToolsList += [ "SCT_FastDigitizationToolSplitNoMergePU" ]
    if DetFlags.digitize.TRT_on():
        PileUpToolsList += [ "TRTFastDigitizationTool" ]
        PileUpToolsList += [ "TRTFastDigitizationToolSplitNoMergePU" ]
    return PileUpToolsList

def getStandardCaloPileUpTools():
    from AthenaCommon.DetFlags import DetFlags
    PileUpToolsList = []
    if DetFlags.digitize.LAr_on():
        PileUpToolsList += [ "LArPileUpTool" ]
    if DetFlags.digitize.HGTD_on():
        PileUpToolsList += [ "HGTDDigitizationTool" ]
    if DetFlags.digitize.Tile_on():
        PileUpToolsList += [ "TileHitVecToCntTool" ]
    return PileUpToolsList

def getStandardMuonPileUpTools():
    from AthenaCommon.DetFlags import DetFlags
    PileUpToolsList = []
    if DetFlags.digitize.CSC_on():
        PileUpToolsList += [ "CscDigitizationTool" ]
    if DetFlags.digitize.MDT_on():
        PileUpToolsList += [ "MdtDigitizationTool" ]
    if DetFlags.digitize.RPC_on():
        PileUpToolsList += [ "RpcDigitizationTool" ]
    if DetFlags.digitize.TGC_on():
        PileUpToolsList += [ "TgcDigitizationTool" ]
    if DetFlags.digitize.sTGC_on():
        PileUpToolsList += [ "sTgcDigitizationTool" ]
    if DetFlags.digitize.Micromegas_on():
        PileUpToolsList += [ "MmDigitizationTool" ]
    return PileUpToolsList

def getStandardPileUpToolsList():
    PileUpToolsList = []
    ## Truth information
    PileUpToolsList += getStandardTruthPileUpTools()
    ## Forward Detector Digitization
    PileUpToolsList += getStandardForwardPileUpTools()
    ## Inner Detector Digitization
    PileUpToolsList += getStandardInDetPileUpTools()
    ## Calo Digitization
    PileUpToolsList += getStandardCaloPileUpTools()
    ## Muon System Digitization
    PileUpToolsList += getStandardMuonPileUpTools()
    ## RecoTimingObj
    PileUpToolsList += [ "MergeRecoTimingObjTool" ]
    return PileUpToolsList

def getStandardSignalOnlyTruthPileUpToolsList():
    PileUpToolsList = []
    ## Truth information
    PileUpToolsList += getStandardSignalOnlyTruthPileUpTools()
    ## Forward Detector Digitization
    PileUpToolsList += getStandardForwardPileUpTools()
    ## Inner Detector Digitization
    PileUpToolsList += getStandardInDetPileUpTools()
    ## Calo Digitization
    PileUpToolsList += getStandardCaloPileUpTools()
    ## Muon System Digitization
    PileUpToolsList += getStandardMuonPileUpTools()
    ## RecoTimingObj
    PileUpToolsList += [ "MergeRecoTimingObjTool" ]
    return PileUpToolsList

def getStandardInTimeOnlyTruthPileUpToolsList():
    PileUpToolsList = []
    ## Truth information
    PileUpToolsList += getStandardInTimeOnlyTruthPileUpTools()
    ## Forward Detector Digitization
    PileUpToolsList += getStandardForwardPileUpTools()
    ## Inner Detector Digitization
    PileUpToolsList += getStandardInDetPileUpTools()
    ## Calo Digitization
    PileUpToolsList += getStandardCaloPileUpTools()
    ## Muon System Digitization
    PileUpToolsList += getStandardMuonPileUpTools()
    ## RecoTimingObj
    PileUpToolsList += [ "MergeRecoTimingObjTool" ]
    return PileUpToolsList

def getFastPileUpToolsList():
    PileUpToolsList = []
    ## Truth information
    PileUpToolsList += getStandardTruthPileUpTools()
    ## Forward Detector Digitization
    PileUpToolsList += getStandardForwardPileUpTools()
    ## Inner Detector Digitization
    PileUpToolsList += getFastInDetPileUpTools()
    ## Calo Digitization
    PileUpToolsList += getStandardCaloPileUpTools()
    ## Muon System Digitization
    PileUpToolsList += getStandardMuonPileUpTools()
    ## RecoTimingObj
    PileUpToolsList += [ "MergeRecoTimingObjTool" ]
    return PileUpToolsList

def getSplitPileUpToolsList():
    PileUpToolsList = []
    ## Truth information
    PileUpToolsList += getStandardTruthPileUpTools()
    ## Forward Detector Digitization
    PileUpToolsList += getStandardForwardPileUpTools()
    ## Inner Detector Digitization - non-standard
    PileUpToolsList += getSplitInDetPileUpTools()
    ## Calo Digitization
    PileUpToolsList += getStandardCaloPileUpTools()
    ## Muon System Digitization
    PileUpToolsList += getStandardMuonPileUpTools()
    ## RecoTimingObj
    PileUpToolsList += [ "MergeRecoTimingObjTool" ]
    return PileUpToolsList

def getSplitNoMergePileUpToolsList():
    PileUpToolsList = []
    ## Truth information
    PileUpToolsList += getStandardTruthPileUpTools()
    ## Forward Detector Digitization
    PileUpToolsList += getStandardForwardPileUpTools()
    ## Inner Detector Digitization - non-standard
    PileUpToolsList += getSplitNoMergeInDetPileUpTools()
    ## Calo Digitization
    PileUpToolsList += getStandardCaloPileUpTools()
    ## Muon System Digitization
    PileUpToolsList += getStandardMuonPileUpTools()
    ## RecoTimingObj
    PileUpToolsList += [ "MergeRecoTimingObjTool" ]
    return PileUpToolsList

def getSplitNoMergeSFPileUpToolsList():
    PileUpToolsList = []
    ## Truth information
    PileUpToolsList += getStandardTruthPileUpTools()
    ## Forward Detector Digitization
    PileUpToolsList += getStandardForwardPileUpTools()
    ## Inner Detector Digitization - non-standard
    PileUpToolsList += getSplitNoMergeSFInDetPileUpTools()
    ## Calo Digitization
    PileUpToolsList += getStandardCaloPileUpTools()
    ## Muon System Digitization
    PileUpToolsList += getStandardMuonPileUpTools()
    ## RecoTimingObj
    PileUpToolsList += [ "MergeRecoTimingObjTool" ]
    return PileUpToolsList

def getSplitNoMergeFSPileUpToolsList():
    PileUpToolsList = []
    ## Truth information
    PileUpToolsList += getStandardTruthPileUpTools()
    ## Forward Detector Digitization
    PileUpToolsList += getStandardForwardPileUpTools()
    ## Inner Detector Digitization - non-standard
    PileUpToolsList += getSplitNoMergeFSInDetPileUpTools()
    ## Calo Digitization
    PileUpToolsList += getStandardCaloPileUpTools()
    ## Muon System Digitization
    PileUpToolsList += getStandardMuonPileUpTools()
    ## RecoTimingObj
    PileUpToolsList += [ "MergeRecoTimingObjTool" ]
    return PileUpToolsList

def getSplitNoMergeFFPileUpToolsList():
    PileUpToolsList = []
    ## Truth information
    PileUpToolsList += getStandardTruthPileUpTools()
    ## Forward Detector Digitization
    PileUpToolsList += getStandardForwardPileUpTools()
    ## Inner Detector Digitization - non-standard
    PileUpToolsList += getSplitNoMergeFFInDetPileUpTools()
    ## Calo Digitization
    PileUpToolsList += getStandardCaloPileUpTools()
    ## Muon System Digitization
    PileUpToolsList += getStandardMuonPileUpTools()
    ## RecoTimingObj
    PileUpToolsList += [ "MergeRecoTimingObjTool" ]
    return PileUpToolsList

def getStandardPileUpToolsAlg(name="StandardPileUpToolsAlg", **kwargs):
    kwargs.setdefault('PileUpTools', getStandardPileUpToolsList() )
    from Digitization.DigitizationFlags import digitizationFlags
    if digitizationFlags.doXingByXingPileUp():
        from PileUpComps.PileUpCompsConf import PileUpToolsAlg
        return PileUpToolsAlg(name, **kwargs)
    else:
        from PileUpComps.PileUpCompsConf import DigitizationAlg
        return DigitizationAlg(name, **kwargs)

def getFastPileUpToolsAlg(name="FastPileUpToolsAlg", **kwargs):
    kwargs.setdefault('PileUpTools', getFastPileUpToolsList() )
    from Digitization.DigitizationFlags import digitizationFlags
    if digitizationFlags.doXingByXingPileUp():
        from PileUpComps.PileUpCompsConf import PileUpToolsAlg
        return PileUpToolsAlg(name, **kwargs)
    else:
        from PileUpComps.PileUpCompsConf import DigitizationAlg
        return DigitizationAlg(name, **kwargs)

def getSplitPileUpToolsAlg(name="SplitPileUpToolsAlg", **kwargs):
    kwargs.setdefault('PileUpTools', getSplitPileUpToolsList() )
    from Digitization.DigitizationFlags import digitizationFlags
    if digitizationFlags.doXingByXingPileUp():
        from PileUpComps.PileUpCompsConf import PileUpToolsAlg
        return PileUpToolsAlg(name, **kwargs)
    else:
        from PileUpComps.PileUpCompsConf import DigitizationAlg
        return DigitizationAlg(name, **kwargs)

def getStandardSignalOnlyTruthPileUpToolsAlg(name="StandardSignalOnlyTruthPileUpToolsAlg", **kwargs):
    kwargs.setdefault('PileUpTools', getStandardSignalOnlyTruthPileUpToolsList() )
    from Digitization.DigitizationFlags import digitizationFlags
    if digitizationFlags.doXingByXingPileUp():
        from PileUpComps.PileUpCompsConf import PileUpToolsAlg
        return PileUpToolsAlg(name, **kwargs)
    else:
        from PileUpComps.PileUpCompsConf import DigitizationAlg
        return DigitizationAlg(name, **kwargs)

def getStandardInTimeOnlyTruthPileUpToolsAlg(name="StandardInTimeOnlyTruthPileUpToolsAlg", **kwargs):
    kwargs.setdefault('PileUpTools', getStandardInTimeOnlyTruthPileUpToolsList() )
    from Digitization.DigitizationFlags import digitizationFlags
    if digitizationFlags.doXingByXingPileUp():
        from PileUpComps.PileUpCompsConf import PileUpToolsAlg
        return PileUpToolsAlg(name, **kwargs)
    else:
        from PileUpComps.PileUpCompsConf import DigitizationAlg
        return DigitizationAlg(name, **kwargs)

def getSplitNoMergePileUpToolsAlg(name="SplitNoMergePileUpToolsAlg", **kwargs):
    kwargs.setdefault('PileUpTools', getSplitNoMergePileUpToolsList() )
    from PileUpComps.PileUpCompsConf import DigitizationAlg
    return DigitizationAlg(name, **kwargs)

def getSplitNoMergeSFPileUpToolsAlg(name="SplitNoMergeSFPileUpToolsAlg", **kwargs):
    kwargs.setdefault('PileUpTools', getSplitNoMergeSFPileUpToolsList() )
    from PileUpComps.PileUpCompsConf import DigitizationAlg
    return DigitizationAlg(name, **kwargs)

def getSplitNoMergeFSPileUpToolsAlg(name="SplitNoMergeFSPileUpToolsAlg", **kwargs):
    kwargs.setdefault('PileUpTools', getSplitNoMergeFSPileUpToolsList() )
    from PileUpComps.PileUpCompsConf import DigitizationAlg
    return DigitizationAlg(name, **kwargs)

def getSplitNoMergeFFPileUpToolsAlg(name="SplitNoMergeFFPileUpToolsAlg", **kwargs):
    kwargs.setdefault('PileUpTools', getSplitNoMergeFFPileUpToolsList() )
    from PileUpComps.PileUpCompsConf import DigitizationAlg
    return DigitizationAlg(name, **kwargs)

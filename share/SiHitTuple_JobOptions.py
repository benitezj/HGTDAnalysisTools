
import os
import re
import AthenaPoolCnvSvc.ReadAthenaPool                   #sets up reading of POOL files (e.g. xAODs)
from glob import glob
include("AthAnalysisBaseComps/SuppressLogging.py")       #Optional include to suppress as much athena output as possible

HitsTuple = CfgMgr.SiHitTuple()                  

################
##Get the testArea path
################
TestAreaPath='.'
if os.environ.get('TestArea'):
    TestAreaPath = os.environ.get('TestArea')


#######################
###Set the number of events
#######################
theApp.EvtMax=-1
print 'EvtMax =',theApp.EvtMax

########
##the output dir can be used to set some variables
######
OUTPUTDIR=''
if os.environ.get('OUTPUTDIR'):
    OUTPUTDIR = os.environ.get('OUTPUTDIR')
else :
    OUTPUTDIR='.'
print 'OUTPUTDIR = ' +OUTPUTDIR

INPUTFILELIST=''
if os.environ.get('INPUTFILELIST'):
    INPUTFILELIST = os.environ.get('INPUTFILELIST')
print 'INPUTFILELIST= ' + INPUTFILELIST


################
##Set the sample
################
if os.environ.get('TUPLESAMPLE'):
    svcMgr.EventSelector.InputCollections = glob(os.environ.get('TUPLESAMPLE'))
else:    
    svcMgr.EventSelector.InputCollections = glob("./AOD.pool.root")
    #svcMgr.EventSelector.InputCollections = glob("/nfs/uiowapc/data/det-hgtd/aod/SiHit_April12/mc15_13TeV.341080.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_WWlvlv_EF_15_5.evgen.EVNT.e4397/AOD_0.root")
    #svcMgr.EventSelector.InputCollections = glob("/nfs/uiowapc/data/det-hgtd/aod/SiHit_April12_Preshower/mc15_13TeV.341080.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_WWlvlv_EF_15_5.evgen.EVNT.e4397/AOD_0.root")
print "Sample : ", svcMgr.EventSelector.InputCollections[0] 

###############
##set the geometry based on the sample name
##############
if re.search('Preshower',INPUTFILELIST) or re.search('PreShower',INPUTFILELIST) or re.search('preshower',INPUTFILELIST):
 GEOM ="ATLAS-P2-ITK-01-01-00"
else :
 GEOM ="ATLAS-P2-ITK-01-02-00"
print 'Geometry = ' + GEOM

###############
##set the cells size OUTPUTDIR
##############
if re.search('_1CELLSIZE',OUTPUTDIR):
    HitsTuple.CellSize = 1
elif re.search('_3CELLSIZE',OUTPUTDIR):
    HitsTuple.CellSize = 3
elif re.search('_5CELLSIZE',OUTPUTDIR):
    HitsTuple.CellSize = 5
elif re.search('_10CELLSIZE',OUTPUTDIR):
    HitsTuple.CellSize = 10
elif re.search('_20CELLSIZE',OUTPUTDIR):
    HitsTuple.CellSize = 20
else :
    print "No match for cell size. Set to default"
    HitsTuple.CellSize = 3
print 'cell Size= ',HitsTuple.CellSize 

#################
##min cell energy
##################
if re.search('_20CELLENERGY',OUTPUTDIR):
    HitsTuple.CellMinEnergy = 0.020
elif re.search('_40CELLENERGY',OUTPUTDIR):
    HitsTuple.CellMinEnergy = 0.040
elif re.search('_60CELLENERGY',OUTPUTDIR):
    HitsTuple.CellMinEnergy = 0.060
elif re.search('_80CELLENERGY',OUTPUTDIR):
    HitsTuple.CellMinEnergy = 0.080
else :
    HitsTuple.CellMinEnergy = 0.020
print 'min cell energy= ',HitsTuple.CellMinEnergy


#################
##min cell energy
################
if re.search('_1TIMESMEAR',OUTPUTDIR):
    HitsTuple.HitTimeSmear = 0.001
elif re.search('_10TIMESMEAR',OUTPUTDIR):
    HitsTuple.HitTimeSmear = 0.010
elif re.search('_20TIMESMEAR',OUTPUTDIR):
    HitsTuple.HitTimeSmear = 0.020
elif re.search('_30TIMESMEAR',OUTPUTDIR):
    HitsTuple.HitTimeSmear = 0.030
elif re.search('_40TIMESMEAR',OUTPUTDIR):
    HitsTuple.HitTimeSmear = 0.040
elif re.search('_50TIMESMEAR',OUTPUTDIR):
    HitsTuple.HitTimeSmear = 0.050
elif re.search('_60TIMESMEAR',OUTPUTDIR):
    HitsTuple.HitTimeSmear = 0.060
elif re.search('_70TIMESMEAR',OUTPUTDIR):
    HitsTuple.HitTimeSmear = 0.070
elif re.search('_80TIMESMEAR',OUTPUTDIR):
    HitsTuple.HitTimeSmear = 0.080
elif re.search('_90TIMESMEAR',OUTPUTDIR):
    HitsTuple.HitTimeSmear = 0.090
elif re.search('_100TIMESMEAR',OUTPUTDIR):
    HitsTuple.HitTimeSmear = 0.100
elif re.search('_110TIMESMEAR',OUTPUTDIR):
    HitsTuple.HitTimeSmear = 0.110
elif re.search('_120TIMESMEAR',OUTPUTDIR):
    HitsTuple.HitTimeSmear = 0.120
elif re.search('_130TIMESMEAR',OUTPUTDIR):
    HitsTuple.HitTimeSmear = 0.130
elif re.search('_140TIMESMEAR',OUTPUTDIR):
    HitsTuple.HitTimeSmear = 0.140
elif re.search('_150TIMESMEAR',OUTPUTDIR):
    HitsTuple.HitTimeSmear = 0.150
else :
    HitsTuple.HitTimeSmear = 0.010
print 'hit time smear= ',HitsTuple.HitTimeSmear




##############################
##Configure the tuple maker
############################
HitsTuple.OutputLevel = INFO # DEBUG # INFO #
HitsTuple.geometry= GEOM  # "ATLAS-P2-ITK-01-02-00"
HitsTuple.VertexCollection = "PrimaryVertices"
HitsTuple.HGTDHitCollection = "MergedHGTDHits"
HitsTuple.JetCollection = "AntiKt4EMTopoJets" #"AntiKt4LCTopoJets"
HitsTuple.METCollection =  "MET_Reference_AntiKt4EMTopo" #"MET_Reference_AntiKt4LCTopo"
HitsTuple.METSoftTerm =  "FinalClus" #FinalClus #FinalTrk
HitsTuple.ElectronCollection = "ForwardElectrons" # # Electrons 
HitsTuple.maxHits = 100


HitsTuple.dTVertex = 1

#HitsTuple.CellSize = 10
#HitsTuple.CellMinEnergy = 0.020
#HitsTuple.HitTimeSmear = 0.050

HitsTuple.JetPtCut = 30000
HitsTuple.JetMinEta = 0  
HitsTuple.JetMinHitDR = 0.4

HitsTuple.EleMinEta = 2.2
HitsTuple.EleMinHitDR = 0.05

HitsTuple.ClusterMinEta = 2.3
HitsTuple.ClusterMinCellsWindow = 1
HitsTuple.ClusterMinHitDR = 0.05

HitsTuple.DTTemplatesFilePath = '{}/HGTDAnalysisTools/share/DTTemplates.root'.format(TestAreaPath)
HitsTuple.DTTemplateHistoSig = 'Signal_{}'.format(int(1000*HitsTuple.HitTimeSmear))
HitsTuple.DTTemplateHistoPU = 'PU_{}'.format(int(1000*HitsTuple.HitTimeSmear))
print 'DT Fit config: ',HitsTuple.DTTemplatesFilePath,',',HitsTuple.DTTemplateHistoSig,',',HitsTuple.DTTemplateHistoPU


#####VBF selection
HitsTuple.VBFJetWPAlgo="ClusCellDTFitFractionEcw"
HitsTuple.VBFJetWPInputFile='{}/HGTDAnalysisTools/share/WP70_VBFHinv_June29_{}_{}.txt'.format(TestAreaPath,int(HitsTuple.HitTimeSmear*1000),HitsTuple.VBFJetWPAlgo)
print 'VBFJetWPInputFile: ',HitsTuple.VBFJetWPInputFile

HitsTuple.VBFMinPt=HitsTuple.JetPtCut
HitsTuple.VBFMinMjj=500000
print 'VBF min jet pT : ',HitsTuple.VBFMinPt
print 'VBF min Mjj : ',HitsTuple.VBFMinMjj

algseq = CfgMgr.AthSequencer("AthAlgSeq")                #gets the main AthSequencer
algseq += HitsTuple


##################################
##configure the jet calibration tool
##################################
from AthenaCommon.AppMgr import ToolSvc
ToolSvc += CfgMgr.JetCalibrationTool("myJESTool",
                                     IsData=False,
                                     ConfigFile="HLLHC/JES_MC15_HLLHC_r7769_May2016.config",
                                     CalibSequence="JetArea_Residual_Origin_EtaJES",
                                     JetCollection="AntiKt4EMTopo")


import os
import re
from glob import glob
from AthenaCommon.AthenaCommonFlags  import athenaCommonFlags
from AthenaCommon.AppMgr import ServiceMgr
import AthenaPoolCnvSvc.ReadAthenaPool         
include("AthAnalysisBaseComps/SuppressLogging.py")

from AthenaCommon.AlgSequence import AlgSequence
topSequence = AlgSequence()

theApp.EvtMax=-1
print 'EvtMax =',theApp.EvtMax

################
##Get the testArea path
################
TestAreaPath='.'
if os.environ.get('TestArea'):
    TestAreaPath = os.environ.get('TestArea')

########
##the output dir can be used to set some variables
######
OUTPUTDIR=''
if os.environ.get('OUTPUTDIR'):
    OUTPUTDIR = os.environ.get('OUTPUTDIR')
else :
    OUTPUTDIR='.'
print 'OUTPUTDIR = ' +OUTPUTDIR

################
##Load the input files 
################
if os.environ.get('TUPLESAMPLE'):
    athenaCommonFlags.FilesInput = glob(os.environ.get('TUPLESAMPLE'))
else:    
    athenaCommonFlags.FilesInput =  glob("AOD.pool.root") 
ServiceMgr.EventSelector.InputCollections = athenaCommonFlags.FilesInput() 
print "Sample : ", svcMgr.EventSelector.InputCollections[0] 

#################################
## create tuple maker
##############################
#HitsTuple = CfgMgr.LArHitTuple()                  
HitsTuple = CfgMgr.LArHitTupleDetGeo()                  

###############
##set the geometry based on the sample name
##############
if re.search('Preshower',OUTPUTDIR) or re.search('PreShower',OUTPUTDIR) or re.search('preshower',OUTPUTDIR):
    HitsTuple.geometry= "Preshower"
else :
    HitsTuple.geometry= "Timing"
print 'Geometry = ' + HitsTuple.geometry
    


##############################
##other configuration
############################
HitsTuple.OutputLevel = INFO # DEBUG # INFO #
HitsTuple.VertexCollection = "PrimaryVertices"
HitsTuple.HGTDHitCollection = "HGTDDigitContainer_MC" #MergedHGTDHits"
HitsTuple.JetCollection = "AntiKt4EMTopoJets" 
HitsTuple.METCollection =  "MET_Reference_AntiKt4EMTopo" 
HitsTuple.METSoftTerm =  "FinalClus" #FinalClus #FinalTrk
HitsTuple.ElectronCollection = "ForwardElectrons" # # Electrons 


HitsTuple.ASU = 198
#HitsTuple.HGTDHEIGHTIN = HitsTuple.ASU/2 + HitsTuple.ASU 
#HitsTuple.HGTDHEIGHTOUT = 600 
#HitsTuple.deadZoneType = 1

HitsTuple.HGTDHEIGHTIN = 600
HitsTuple.HGTDHEIGHTOUT = 3 
HitsTuple.deadZoneType = 2


HitsTuple.CellSize1 = 1
HitsTuple.CellSize2 = 3
HitsTuple.CellMinEnergy = 0.020

HitsTuple.dTVertex = 1

HitsTuple.HitTimeSmear = 0.030
HitsTuple.HitTimeSmear2 = 0.060
HitsTuple.dTSigma = 2 

HitsTuple.JetPtCut = 30000
HitsTuple.JetPtCutMax = 1000000
HitsTuple.JetMinEta = 2.4  
HitsTuple.JetMinHitDR = 0.2

HitsTuple.EleMinEta = 2.3
HitsTuple.EleMinHitDR = 0.1

HitsTuple.ClusterMinEta = 2.3
HitsTuple.ClusterMinHitDR = 0.1

HitsTuple.selectJetEvents = 1
HitsTuple.selectElectronEvents = 0
HitsTuple.WriteAllCells = 0

HitsTuple.TrackMatchRadius = 5
HitsTuple.RunTrkExtraPol = 2 # 1=Trk::Trk  , 2=xAOD track

#####load dT templates
HitsTuple.DTTemplatesFilePath = '{}/HGTDAnalysisTools/share/DTTemplates.root'.format(TestAreaPath)
HitsTuple.DTTemplateHistoSig = 'Signal_{}'.format(int(1000*HitsTuple.HitTimeSmear))
HitsTuple.DTTemplateHistoPU = 'PU_{}'.format(int(1000*HitsTuple.HitTimeSmear))
print 'DT Fit config: ',HitsTuple.DTTemplatesFilePath,',',HitsTuple.DTTemplateHistoSig,',',HitsTuple.DTTemplateHistoPU


#####Load the cell dR templates
HitsTuple.dTFitCoreBkgSubRadius = 0.20
HitsTuple.dTFitCoreBkgSubRadiusSideband = 0.20

####not used now
if re.search('Preshower',OUTPUTDIR) or re.search('PreShower',OUTPUTDIR) or re.search('preshower',OUTPUTDIR):
    HitsTuple.dRTemplatesFilePath_Sig = '{}/HGTDAnalysisTools/share/dRTemplates_Preshower_mu0.root'.format(TestAreaPath)
    HitsTuple.dRTemplatesFilePath_Bkg = '{}/HGTDAnalysisTools/share/dRTemplates_Preshower_mu200.root'.format(TestAreaPath)
else :
    HitsTuple.dRTemplatesFilePath_Sig = '{}/HGTDAnalysisTools/share/dRTemplates_mu0.root'.format(TestAreaPath)
    HitsTuple.dRTemplatesFilePath_Bkg = '{}/HGTDAnalysisTools/share/dRTemplates_mu200.root'.format(TestAreaPath)
    
print 'dR Sig Templates: ',HitsTuple.dRTemplatesFilePath_Sig
print 'dR Bkg Templates: ',HitsTuple.dRTemplatesFilePath_Bkg


#####VBF selection
HitsTuple.VBFJetWPAlgo="ClusCellDTFitFractionEcw"
HitsTuple.VBFJetWPInputFile='{}/HGTDAnalysisTools/share/WP70_VBFHinv_June29_{}_{}.txt'.format(TestAreaPath,int(HitsTuple.HitTimeSmear*1000),HitsTuple.VBFJetWPAlgo)
print 'VBFJetWPInputFile: ',HitsTuple.VBFJetWPInputFile
HitsTuple.VBFMinPt=HitsTuple.JetPtCut
print 'VBF min jet pT : ',HitsTuple.VBFMinPt
HitsTuple.VBFMinMjj=500000
print 'VBF min Mjj : ',HitsTuple.VBFMinMjj


####Trigger
if re.search('Preshower',OUTPUTDIR):
    if re.search('_mu0',OUTPUTDIR):
        HitsTuple.TrigCut="((x<40)*(24*(110<y&&y<=120)+29*(120<y&&y<=130)+28*(140<y&&y<=150)+28*(150<y&&y<=160)+29*(160<y&&y<=170)+19*(180<y&&y<=190)+19*(190<y&&y<=200))+(40<x&&x<60)*(41*(170<y&&y<=180)+29*(210<y&&y<=220)+22*(220<y&&y<=230)+19*(230<y&&y<=240)+27*(240<y&&y<=250)+22*(250<y&&y<=260)+29*(270<y&&y<=280)+26*(280<y&&y<=290)+46*(290<y&&y<=300)+56*(320<y&&y<=330))+(60<x&&x<120)*(67*(350<y&&y<=360)+47*(370<y&&y<=380)+36*(420<y&&y<=430)+26*(490<y&&y<=500))+(x>120)*(58*(500<y&&y<=510)+28*(570<y&&y<=580)))"
    else:
        HitsTuple.TrigCut="((x<40)*(46*(110<y&&y<=120)+48*(120<y&&y<=130)+49*(140<y&&y<=150)+47*(150<y&&y<=160)+43*(160<y&&y<=170)+34*(180<y&&y<=190)+30*(190<y&&y<=200))+(40<x&&x<60)*(69*(170<y&&y<=180)+51*(210<y&&y<=220)+43*(220<y&&y<=230)+46*(230<y&&y<=240)+45*(240<y&&y<=250)+39*(250<y&&y<=260)+60*(270<y&&y<=280)+73*(280<y&&y<=290)+81*(290<y&&y<=300)+92*(320<y&&y<=330))+(60<x&&x<120)*(140*(350<y&&y<=360)+118*(370<y&&y<=380)+85*(420<y&&y<=430)+61*(490<y&&y<=500))+(x>120)*(180*(500<y&&y<=510)+99*(570<y&&y<=580)))"
else: 
    if re.search('_mu0',OUTPUTDIR):
        HitsTuple.TrigCut="((x<40)*(23*(110<y&&y<=120)+24*(120<y&&y<=130)+28*(140<y&&y<=150)+36*(150<y&&y<=160)+24*(160<y&&y<=170)+17*(180<y&&y<=190)+18*(190<y&&y<=200))+(40<x&&x<60)*(39*(170<y&&y<=180)+25*(210<y&&y<=220)+25*(220<y&&y<=230)+25*(230<y&&y<=240)+19*(240<y&&y<=250)+19*(250<y&&y<=260)+16*(270<y&&y<=280)+19*(280<y&&y<=290)+13*(290<y&&y<=300)+13*(320<y&&y<=330))+(60<x&&x<120)*(21*(350<y&&y<=360)+17*(370<y&&y<=380)+11*(420<y&&y<=430)+9*(490<y&&y<=500))+(x>120)*(18*(500<y&&y<=510)+10*(570<y&&y<=580)))"
    else:
        HitsTuple.TrigCut="((x<40)*(46*(110<y&&y<=120)+45*(120<y&&y<=130)+50*(140<y&&y<=150)+52*(150<y&&y<=160)+44*(160<y&&y<=170)+35*(180<y&&y<=190)+29*(190<y&&y<=200))+(40<x&&x<60)*(72*(170<y&&y<=180)+50*(210<y&&y<=220)+46*(220<y&&y<=230)+47*(230<y&&y<=240)+43*(240<y&&y<=250)+37*(250<y&&y<=260)+38*(270<y&&y<=280)+32*(280<y&&y<=290)+26*(290<y&&y<=300)+26*(320<y&&y<=330))+(60<x&&x<120)*(67*(350<y&&y<=360)+58*(370<y&&y<=380)+39*(420<y&&y<=430)+30*(490<y&&y<=500))+(x>120)*(84*(500<y&&y<=510)+46*(570<y&&y<=580)))"

print 'Trigger Cut ',HitsTuple.TrigCut


#############################
## actually load the module
#############################
topSequence += HitsTuple


##################################
##configure the jet calibration tool
##################################
from AthenaCommon.AppMgr import ToolSvc
ToolSvc += CfgMgr.JetCalibrationTool("myJESTool",
                                     IsData=False,
                                     ConfigFile="HLLHC/JES_MC15_HLLHC_r7769_May2016_v2.config",
                                     CalibSequence="JetArea_Residual_Origin_EtaJES",
                                     JetCollection="AntiKt4EMTopo")


###################################
### To set up a geometry needed for the identifier
###################################
###set special detector flags
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
##### Tools configuration of the GMX based Strip description
#from SCT_GeoModelXml.SCT_GeoModelXmlConf import SCT_GMX_DetectorTool
#SCT_GMX_DetectorTool.GmxFilename = "ITkStrip.gmx"
#SCT_GMX_DetectorTool.OutputLevel = VERBOSE
#from AthenaCommon.CfgGetter import getPublicTool
#getPublicTool("SLHC_SctSensorSD").GmxSensor=True 
#
########Load geometry
#from RecExConfig.AutoConfiguration import *
#ConfigureFieldAndGeo() # Configure the settings for the geometry
#include("RecExCond/AllDet_detDescr.py") # Actually load the geometry
#
###### extrapolator
#from TrkDetDescrSvc.AtlasTrackingGeometrySvc import AtlasTrackingGeometrySvc 




from AthenaCommon.GlobalFlags import globalflags
#globalflags.DetGeo = 'atlas'

from RecExConfig.InputFilePeeker import inputFileSummary
globalflags.DataSource = 'data' if inputFileSummary['evt_type'][0] == "IS_DATA" else 'geant4'
globalflags.DetDescrVersion = inputFileSummary['geometry']

xmlTags = [["ATLAS-P2-ITK-09-01-01","InclBrl_4","GMX"],
           ["ATLAS-P2-ITK-09-02-01","InclBrl_4","GMX"],
           ["ATLAS-P2-ITK-10-01-01","InclBrl_4","GMX"],
           ["ATLAS-P2-ITK-10-02-01","InclBrl_4","GMX"],]

print "InputFile DetDescrVersion : " , globalflags.DetDescrVersion()


for geoTag, layoutDescr, gmx in xmlTags:
   if (globalflags.DetDescrVersion().startswith(geoTag)):
      print "preIncludes for ",layoutDescr, " layout"
      from InDetRecExample.InDetJobProperties import InDetFlags
      include('InDetSLHC_Example/preInclude.SLHC.SiliconOnly.Reco.py')
      include('InDetSLHC_Example/preInclude.SLHC_Setup_'+layoutDescr+'.py')
      if gmx=="GMX":
         print "preIncludes for GMX strip layout"
         include('InDetSLHC_Example/preInclude.SLHC_Setup_Strip_GMX.py')
         if geoTag=="ATLAS-P2-ITK-10-00-00" or geoTag=="ATLAS-P2-ITK-09-00-00" :
            include('InDetSLHC_Example/SLHC_Setup_Reco_TrackingGeometry.py')
         else:
            include('InDetSLHC_Example/SLHC_Setup_Reco_TrackingGeometry_GMX.py')
      else : 
         include('InDetSLHC_Example/SLHC_Setup_Reco_TrackingGeometry.py')
      break


# Just turn on the detector components we need
from AthenaCommon.DetFlags import DetFlags
DetFlags.detdescr.all_setOff() 
DetFlags.detdescr.SCT_setOn() 
DetFlags.detdescr.BField_setOn() 
DetFlags.detdescr.pixel_setOn() 

# Set up geometry and BField 
include("RecExCond/AllDet_detDescr.py")

from InDetSLHC_Example.SLHC_JobProperties import SLHC_Flags
SLHC_Flags.SLHC_Version = ''

         
from AthenaCommon.GlobalFlags import jobproperties
DetDescrVersion = jobproperties.Global.DetDescrVersion()

for geoTag, layoutDescr,gmx in xmlTags:
   if (globalflags.DetDescrVersion().startswith(geoTag)):
      print "postInclude for ",layoutDescr, " layout"
      include('InDetSLHC_Example/postInclude.SLHC_Setup_'+layoutDescr+'.py')
      break

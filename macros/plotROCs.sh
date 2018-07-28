#export PROD="LArHitAug17"
#export PROD="LArHitSep9_CrabKissing"
#export PROD="LArHitSep15_JetCore"
#export PROD="LArHitOct4"
#export PROD="LArHitOct4_Core02Sub"
#export PROD="LArHitOct4_Core03Sub"
#export PROD="LArHitNov5"

###core scan
#export PROD="LArHitNov10_10_1040"
#export PROD="LArHitNov10_15_1540"
#export PROD="LArHitNov10_20_2040"
#export PROD="LArHitNov10_25_2540"
#export PROD="LArHitNov10_30_3040"

###dt binning scan
#export PROD="LArHitNov13_1sigma"
#export PROD="LArHitNov13_2sigma"
#export PROD="LArHitNov13_3sigma"

###timing scans
#export PROD="LArHitNov11"

#export PROD="LArHitDec3_dr20"
#export PROD="LArHitDec3_dr25"
#export PROD="LArHitDec3_dr30"

#export PROD="LArHitDec13IDR"

#export PROD="LArHitDec14_dRdT"
#export PROD="LArHitDec14_dRdT_2sigmaT"

#export PROD="LArHitFeb2_JetShape"

#export PROD="LArHitNewITKJetsApril1"
#export PROD="LArHitNewITKJetsApril4"
#export PROD="LArHitNewITKJetsApril25"
export PROD="LArHitNewITKJetsJune12"

#export TAG="LArHit_July26"
#export SAMPLE="mu200/mc15_13TeV.301399.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_ZZ4nu.evgen.EVNT.e4397"

export TAG="InclBrlNominal"
export SAMPLE="mu200/mc15_14TeV.301399.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_ZZ4nu.recon.AOD.e4956_s3072_s3059_r9073"

export PRESHOWER=$1
if [ "$PRESHOWER" != "" ]; then
echo "preshower=${PRESHOWER}="
export TAG=${TAG}_${PRESHOWER}
export SAMPLE="mu200/mc15_14TeV.301399.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_ZZ4nu.recon.AOD.e4956_s3069_s3058_r9072"
fi


export SAMPLEPATH=${PROD}/${TAG}_${SAMPLE}
echo "Plotting : ${SAMPLEPATH}"

#root -b HGTDAnalysisTools/macros/plotROCPtEta.C\(\"${SAMPLEPATH}_Window45\",\"dTFitCoreNcell\"\) & 
#root -b HGTDAnalysisTools/macros/plotROCPtEta.C\(\"${SAMPLEPATH}_Window90\",\"dTFitCoreNcell\"\) & 

#root -b HGTDAnalysisTools/macros/plotROCQCDStoc.C\(\"${SAMPLEPATH}_Window45\",\"dTFitCoreNcell\"\) & 
#root -b HGTDAnalysisTools/macros/plotROCQCDStoc.C\(\"${SAMPLEPATH}_Window90\",\"dTFitCoreNcell\"\) & 

### 1mm + 3mm 
#root -b HGTDAnalysisTools/macros/plotROCSummer2017.C\(\"${SAMPLEPATH}_Window45\",\"dTFitCoreNcell\"\) & 
#root -b HGTDAnalysisTools/macros/plotROCSummer2017.C\(\"${SAMPLEPATH}_Window90\",\"dTFitCoreNcell\"\) & 

### 1mm 
root -b HGTDAnalysisTools/macros/plotROCSummer2017.C\(\"${SAMPLEPATH}_Window30\",\"dTFitCoreNcell\"\) & 



#root -b HGTDAnalysisTools/macros/plotROCPtEta.C\(\"${SAMPLEPATH}\",\"dTFit\"\) & 
#root -b HGTDAnalysisTools/macros/plotROCPtEta.C\(\"${SAMPLEPATH}\",\"dTFitCoreTrue\"\) & 
#root -b HGTDAnalysisTools/macros/plotROCPtEta.C\(\"${SAMPLEPATH}\",\"dTFitCore\"\) & 
#root -b HGTDAnalysisTools/macros/plotROCPtEta.C\(\"${SAMPLEPATH}\",\"dTFitCoreHGTrk\"\) & 
#root -b HGTDAnalysisTools/macros/plotROCPtEta.C\(\"${SAMPLEPATH}\",\"dTFitCoreBkgSub\"\) & 
#root -b HGTDAnalysisTools/macros/plotROCPtEta.C\(\"${SAMPLEPATH}\",\"dRdTFit\"\) &  
#root -b HGTDAnalysisTools/macros/plotROCPtEta.C\(\"${SAMPLEPATH}\",\"dRRatio3020\"\) & 
#root -b HGTDAnalysisTools/macros/plotROCPtEta.C\(\"${SAMPLEPATH}\",\"dRRatio35over25\"\) &
#root -b HGTDAnalysisTools/macros/plotROCPtEta.C\(\"${SAMPLEPATH}\",\"dRslope\"\) &  
#root -b HGTDAnalysisTools/macros/plotROCPtEta.C\(\"${SAMPLEPATH}\",\"dRslopeCore\"\) &  
#root -b HGTDAnalysisTools/macros/plotROCPtEta.C\(\"${SAMPLEPATH}\",\"dRslopeFit\"\) &  
#root -b HGTDAnalysisTools/macros/plotROCPtEta.C\(\"${SAMPLEPATH}\",\"dRslopeSigFit\"\) &  


#for D in dRDensity35over20 dRDensity35over25 dREdge35 dRCore000510 dRCore00051015 dRDensity000510over20 dRslopeFit
#do
#echo $D
#root -b HGTDAnalysisTools/macros/plotROCPtEta.C\(\"${SAMPLEPATH}\",\"${D}\"\)
#done



#export PROD="LArHitAug17/LArHit_July26"
#export PROD="LArHitSep9_CrabKissing/LArHit_July26"
#export PROD="LArHitSep15_JetCore/LArHit_July26"
#export PROD="LArHitSep15_JetCore02/LArHit_July26"
#export PROD="LArHitOct4/LArHit_July26"
#export PROD="LArHitOct4_Core02Sub/LArHit_July26"
#export PROD="LArHitOct4_Core03Sub/LArHit_July26"

###jet core scan
#export PROD="LArHitNov10_10_1040/LArHit_July26"
#export PROD="LArHitNov10_15_1540/LArHit_July26"
#export PROD="LArHitNov10_20_2040/LArHit_July26"
#export PROD="LArHitNov10_25_2540/LArHit_July26"
#export PROD="LArHitNov10_30_3040/LArHit_July26"

##bin size scan
#export PROD="LArHitNov13_1sigma/LArHit_July26"
#export PROD="LArHitNov13_2sigma/LArHit_July26"
#export PROD="LArHitNov13_3sigma/LArHit_July26"

#export PROD="LArHitNov11/LArHit_July26"

###dR scan with bkg subtraction
#export PROD="LArHitDec3_dr20/LArHit_July26"
#export PROD="LArHitDec3_dr25/LArHit_July26"
#export PROD="LArHitDec3_dr30/LArHit_July26"

#export PROD="LArHitDec10_dRdTFit/LArHit_July26"

#export PROD="LArHitDec13IDR/LArHit_July26"

#export PROD="LArHitDec14_dRdT/LArHit_July26"
#export PROD="LArHitDec14_dRdT_2sigmaT/LArHit_July26"

#export PROD="LArHitDec25Trks/LArHit_July26"

#export PROD="LArHitJan26_JetClusters/LArHit_July26"

#export PROD="LArHitFeb2_JetShape/LArHit_July26"

#export PROD="LArHitNewITKJetsApril1/InclBrlNominal"
#export PROD="LArHitNewITKJetsApril4/InclBrlNominal"

#export PROD="LArHitNewITKJetsApril25/InclBrlNominal"
export PROD="LArHitNewITKJetsJune12/InclBrlNominal"

#export SAMPLE="mu200/mc15_13TeV.301399.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_ZZ4nu.evgen.EVNT.e4397"
export SAMPLE="mu200/mc15_14TeV.301399.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_ZZ4nu.recon.AOD.e4956_s3072_s3059_r9073"

if [ "$1" != "" ]; then
export PROD=${PROD}_${1}
echo $PROD
export SAMPLE="mu200/mc15_14TeV.301399.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_ZZ4nu.recon.AOD.e4956_s3069_s3058_r9072"
fi

export SAMPLEPATH=${PROD}_${SAMPLE}

### 1mm + 3mm cells
#root -b HGTDAnalysisTools/macros/makeROCs.C\(\"${SAMPLEPATH}\",\"45\"\) &
#root -b HGTDAnalysisTools/macros/makeROCs.C\(\"${SAMPLEPATH}\",\"90\"\) &

### 1mm cells
root -b HGTDAnalysisTools/macros/makeROCs.C\(\"${SAMPLEPATH}\",\"30\"\) &
root -b HGTDAnalysisTools/macros/makeROCs.C\(\"${SAMPLEPATH}\",\"30\"\) &

INFILE=$1

HITSMerge_tf.py \
--inputHITSFile=$INFILE \
--outputHITS_MRGFile=HITS.pool.root  \
--DBRelease=current  \
--postInclude 'all:PyJobTransforms/UseFrontier.py,InDetSLHC_Example/postInclude.SLHC_Setup_ExtBrl_4.py' \
--preInclude 'all:InDetSLHC_Example/preInclude.SLHC_Setup_ExtBrl_4.py,HGTDAnalysisTools/preInclude.HGTD.py,InDetSLHC_Example/preInclude.SLHC.py,InDetSLHC_Example/preInclude.NoTRT.py,InDetSLHC_Example/preInclude.SLHC.py,InDetSLHC_Example/SLHC_Setup_Reco_TrackingGeometry.py' 

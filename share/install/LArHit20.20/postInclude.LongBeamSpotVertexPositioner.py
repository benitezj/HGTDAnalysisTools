ToolSvc.ISF_GenEventVertexPositioner.VertexShifters=[getPublicTool('ISF_LongBeamspotVertexPositioner')]
from AthenaCommon.CfgGetter import getPublicTool
getPublicTool("ISF_LongBeamspotVertexPositioner").SimpleTimeSmearing = True
getPublicTool("ISF_LongBeamspotVertexPositioner").LParameter = 50. # 50mm
getPublicTool("ISF_LongBeamspotVertexPositioner").TimeType = "GAUSS"
getPublicTool("ISF_LongBeamspotVertexPositioner").TimeWidth = .2 # 0.2ns

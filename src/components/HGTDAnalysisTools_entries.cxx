
#include "GaudiKernel/DeclareFactoryEntries.h"

#include "HGTDAnalysisTools/LArHitRead.h"
DECLARE_ALGORITHM_FACTORY( LArHitRead )

#include "HGTDAnalysisTools/LArHitTupleDetGeo.h"
DECLARE_ALGORITHM_FACTORY( LArHitTupleDetGeo )

DECLARE_FACTORY_ENTRIES( HGTDAnalysisTools ) 
{

  DECLARE_ALGORITHM( LArHitRead );
  DECLARE_ALGORITHM( LArHitTupleDetGeo );
}

#include "HitAnalysis/CaloHitAnalysis.h"
#include "HitAnalysis/HGTDHitAnalysis.h"

#include "GaudiKernel/DeclareFactoryEntries.h"

DECLARE_ALGORITHM_FACTORY( CaloHitAnalysis )
DECLARE_ALGORITHM_FACTORY( HGTDHitAnalysis )

DECLARE_FACTORY_ENTRIES( HitAnalysis ) {
  DECLARE_ALGORITHM( CaloHitAnalysis )
  DECLARE_ALGORITHM( HGTDHitAnalysis )
}


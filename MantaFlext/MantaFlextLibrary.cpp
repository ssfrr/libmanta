#include <flext.h>
#include "MantaFlextCentroid.h"
#include "MantaFlext.h"

static void mantaLibSetup(void)
{
   /* call the objects' setup functions, which also registers them
    * as part of the library (I think) */
   FLEXT_SETUP(manta);
   FLEXT_SETUP(MantaFlextCentroid);
   FLEXT_SETUP(MantaFlextMaximum);
}

FLEXT_LIB_SETUP(manta, mantaLibSetup)

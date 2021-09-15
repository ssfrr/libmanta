#include <flext.h>
#include "MantaFlextCentroid.h"
#include "MantaFlext.h"
#include "MantaFlextVersion.h"
#include "../core/MantaVersion.h"

static void mantaLibSetup(void)
{
   /* call the objects' setup functions, which also registers them
    * as part of the library (I think) */
   FLEXT_SETUP(manta);
   FLEXT_SETUP(MantaFlextCentroid);
   FLEXT_SETUP(MantaFlextMaximum);
   FLEXT_SETUP(MantaFlextPoly);
   post("manta: MantaFlext V %02d.%02d",
         MANTAFLEXT_MAJOR_VERSION,MANTAFLEXT_MINOR_VERSION);
   post("manta: PD / Max object by Spencer Russell, with Jeff Snyder, R. Luke Dubois, and Angie Hugeback. Using Flext by Thomas Grill");
}

FLEXT_LIB_SETUP(manta, mantaLibSetup)

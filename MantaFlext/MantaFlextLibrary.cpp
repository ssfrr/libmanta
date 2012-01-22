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
   post("manta: PD / Max object by Spencer Russell");
   post("manta: MantaFlext V %02d.%02d, libmanta V %02d.%02d",
         MANTAFLEXT_MAJOR_VERSION,MANTAFLEXT_MINOR_VERSION, 
         LIBMANTA_MAJOR_VERSION,LIBMANTA_MINOR_VERSION);
   post("manta: Centroid Algorithm by R. Luke Dubois");
   post("manta: Velocity Algorithm by Angie Hugeback");
   post("manta: Much thanks to Jeff Snyder at Snyderphonics");
   post("manta: Built on Flext by Thomas Grill");
}

FLEXT_LIB_SETUP(manta, mantaLibSetup)

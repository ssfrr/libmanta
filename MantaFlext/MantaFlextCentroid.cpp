#include "MantaFlextCentroid.h"

FLEXT_LIB("manta.centroid", MantaFlextCentroid);

MantaFlextCentroid::MantaFlextCentroid()
{
   AddInAnything();
   FLEXT_ADDMETHOD_V(0, padFrameHandler);
}

void MantaFlextCentroid::padFrameHandler(int argc, t_atom *argv)
{
   post("manta.centroid: frame received");
}

#include "MantaFlextCentroid.h"

FLEXT_LIB("manta.centroid", MantaFlextCentroid);

MantaFlextCentroid::MantaFlextCentroid() :
   dormant(true)
{
   AddInAnything();
   AddOutList();
   FLEXT_ADDMETHOD_V(0, padFrameHandler);
}

/* Centroid Algorithm by R. Luke Dubois */
void MantaFlextCentroid::padFrameHandler(int argc, t_atom *argv)
{
	int i, j;
	int thenormalmap[8][6][2];
	int themultmap[8][6][2];
	int ourmap[8][6];
	float sumx, sumy;
	float mapsum, centx, centy;
	
	centx = 0.;
	centy = 0.;
	mapsum = 0.;
	sumx = 0.;
	sumy = 0.;
	// step one - make a normal map and fill up ourmap
	for(i = 0; i < 8; i++)
	{
		for(j = 0; j < 6; j++)
		{
			thenormalmap[i][j][0] = i;
			thenormalmap[i][j][1] = j;
			
			ourmap[i][j] = GetFloat(argv[(i+(j*8))]);

         /* currently we're not supporting Raw */
#if 0
			if(x->israw) {
				ourmap[i][j] = ourmap[i][j] - x->threshmap[(i+(j*8))];
			}
#endif
			
			themultmap[i][j][0] = thenormalmap[i][j][0]*ourmap[i][j];
			themultmap[i][j][1] = thenormalmap[i][j][1]*ourmap[i][j];
			
			mapsum +=ourmap[i][j];
			sumx+=themultmap[i][j][0];
			sumy+=themultmap[i][j][1];
		}	
	}
	
	if (mapsum > 0)
	{
      t_atom outVals[3];
      centx = sumx / mapsum;
      centy = sumy / mapsum;

      dormant = false;
		SetFloat(outVals[0], centx);
		SetFloat(outVals[1], centy);
      SetFloat(outVals[2], mapsum);
      ToOutList(0, sizeof(outVals) / sizeof(t_atom), outVals);
	}
   else if(! dormant)
	{
      t_atom outVals[3];

      dormant = true;
		SetFloat(outVals[0], -1);
		SetFloat(outVals[1], -1);
      SetFloat(outVals[2], -1);
      ToOutList(0, sizeof(outVals) / sizeof(t_atom), outVals);
	}
}

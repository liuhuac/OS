#include "sim.h"

/*
 * Random number generator
 */
float xrand(unsigned *seed) {
   return((float)rand_r(seed) / (float) RAND_MAX);
}

/*
 * Print distribution of block sizes 
 */
void scanBlocks() {
   int sizeCounts[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
   int size;
   int ndx;

   for (ndx=0; ndx<(totalMemory/1024); ndx++) {
       sizeCounts[shadowTable[ndx].freeNdx]++;
   }

   size=1024;
   printf("DISTRIBUTION OF SUPERBLOCKS BETWEEN SIZES\n");
   printf("    SIZE  COUNT\n");
   for (ndx=0; ndx<7; ndx++) {
       printf("    %4d %6d\n", size, sizeCounts[ndx]);
       size >>=1;
   }
}


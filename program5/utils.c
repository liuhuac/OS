#include "vmsim.h"

/** pageTableInit **/
/*  allocate and initialize page table */
pageTableEntry_t *pageTableInit(int numPageFrames) {
   int ndx;
   pageTableEntry_t *pageTable;

   pageTable = 
       (pageTableEntry_t *)malloc(numPageFrames*sizeof(pageTableEntry_t));
 
   /* Initialize the page table */
   for (ndx=0; ndx<numPageFrames; ndx++) {
       pageTable[ndx].valid = 0;
       pageTable[ndx].modified = 0;
   }

   return(pageTable);
}

/** inMemory **/
/*  determine if virtual page is currently in the page table. */
int inMemory(unsigned vmPageNum) {
   int frameNdx;
   for (frameNdx = 0; frameNdx < numPageFrames; frameNdx++) {
       if ((vmPageNum == pageTable[frameNdx].vmPage) && 
           (pageTable[frameNdx].valid == 1)) {
          return(frameNdx);
       }
   }
   /* Virtual page not in memory -- return failure */
   return(-1);
} /* End inMemory */

/** freeFrame **/
/*  If virtual page is still in phyiscal memory, free frame */
void freeFrame(unsigned logicalAddress) {
   unsigned vmPageNum = logicalAddress >> 10;
   int frameNdx;
   if ((frameNdx = inMemory(vmPageNum)) >= 0) {
      /* Clear frame's valid bit entry */
      pageTable[frameNdx].valid = 0; 
   } 
}

#include "vmsim.h"

/** MMU **/
/*  Emulate the operation of the hardware MMU */
int MMU(unsigned logicalAddress, int refType) {
   unsigned vmPageNum = logicalAddress >> 10;
   int frameNdx;

   if ((frameNdx = inMemory(vmPageNum)) >= 0) {
      /* Page is in physical memory, update PMT entry */
      pageTable[frameNdx].referenced = 1;
      if (refType == 1) {
         pageTable[frameNdx].modified = 1;
      }
      return(1);
   }

   /* Page fault */
   return(0);
}

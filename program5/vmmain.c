#include "vmsim.h"

/** main **/
int main(int argc, char *argv[]) {
   FILE *pageRefFile;        /* Page reference file               */
   unsigned refType;         /* Page reference type               */
   unsigned logicalAddress;  /* Page reference number             */

   /* Statistics */
   int refCount = 0;         /* Number of references              */
   int pageFaults = 0;       /* Number of page faults             */
   int stats[3] = {0, 0, 0}; /* Stats on placement/replacement    */
   int code;                 /* Placement or replacement action   */

   lastLoaded = 0;

   if (argc < 3) {
      printf("Usage: vmSim page_trace_filename number_of_page_frames\n");
      exit(1);
   }
 
   /* Open trace file and get number of page frames from command line */
   assert((pageRefFile = fopen(argv[1], "r")) != NULL);
   assert((numPageFrames = atoi(argv[2])) > 0); 
 
   printf("VM simulation using %d page frames:\n", numPageFrames);
 
   /* Allocate space for page table */
   pageTable = pageTableInit(numPageFrames);

   /* MAIN LOOP -- Process the page reference files */
   while (fscanf(pageRefFile, "%u%u", &refType, &logicalAddress) == 2) {
      switch (refType) {
         case MEMREAD:
         case MEMWRITE:
              refCount++;
              /* Run address through the MMU */
              if (MMU(logicalAddress, refType) == 0) {
                 /* PAGE FAULT */
                 pageFaults++;
            
                 code = pageFault(logicalAddress);
                 stats[code]++;  // Update stats

                 /* Now "retry" reference -- must work this time */
                 assert(MMU(logicalAddress, refType) == 1);
              } 
              break;
         case FRAMEFREE:
              /* Free page frame associated with logical address */
              freeFrame(logicalAddress);
              break;
         default: 
              printf("Error: unknown memory reference type %d\n", refType);
              exit(1);
      }
   } /* End read loop */

   /* Print summary statistics */
   printf("\nSTATISTICS\n");
   printf("   References:           %8d\n", refCount);
   printf("   Page Faults:          %8d\n", pageFaults);
   printf("   Fault Handler Statistics\n");
   printf("      Placements:        %8d\n", stats[0]);
   printf("      Clean Replaced:    %8d\n", stats[1]);
   printf("      Modified Replaced: %8d\n", stats[2]);

   exit(0);
}

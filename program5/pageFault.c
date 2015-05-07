
#include "vmsim.h"

/** pageFault **/
/*  Process page fault */
int pageFault(unsigned logicalAddress) {
    int pageNumber = logicalAddress >> 10;

    /** STUBBED -- just do random placement/replacement
    
        This is an implementation of random replacement.  The drand48()
        function returns a "random" value between 0 and 1.

        No attempt to do "placement" is done in this version.  An empty
        frame is only allocated if the random number generates the index
        of an empty frame (valid == 0).

        Also this stubbed version always returns the code for replacing
        a "dirty" page -- even though the replaced page may have been
        modified.
    
    ***/

    //int frameNdx = drand48()*numPageFrames; /*Generate a random page frame # */
    int frameNdx;

    //placement
    for (frameNdx = 0; frameNdx < numPageFrames; frameNdx++) {
        if (pageTable[frameNdx].valid == 0) {
           pageTable[frameNdx].vmPage = pageNumber;
           pageTable[frameNdx].valid = 1;
           pageTable[frameNdx].modified = 0;
           pageTable[frameNdx].referenced = 0;
           lastLoaded = frameNdx;
           return(0);
       }
    }

    //replacement
    frameNdx = (lastLoaded + 1) % numPageFrames;
    while (1) {
        if (pageTable[frameNdx].referenced == 0) {
           pageTable[frameNdx].vmPage = pageNumber;
           pageTable[frameNdx].valid = 1;
           lastLoaded = frameNdx;
           if (pageTable[frameNdx].modified == 0) {
              return(1);
           } else {
              pageTable[frameNdx].modified = 0;
              return(2);
           }
        }
        pageTable[frameNdx].referenced = 0;
        frameNdx = (frameNdx + 1) % numPageFrames;
    }

    /* Assign the page to this page frame */
    //pageTable[frameNdx].vmPage = pageNumber;
    //pageTable[frameNdx].valid = 1;

    /* Always return code for a "dirty" page */
    //return(2);
}

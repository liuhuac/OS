/*********************************************************************
 *  DO NOT change code below this point (for sumbitted version)      *
 ********************************************************************/

#include "sim.h"

/* This is the code for each thread -- its main function in life is
   ... */
void *memSim(void *parmPtr) 
{
#define PARMPTR ((struct threadParms *) parmPtr)
    int ndx2, ndxStart, ndxLoop;
    int blockSize;
    float stateRate;
    void *allocPtr;
    
    /* Main Simulation Loop
       Simulate memory block allocations/deallocations 
    */
    stateRate = WARMUP_RATE;
    for (ndxLoop=0; ndxLoop<PARMPTR->loops; ndxLoop++) {
        /* Allocate or deallocate? */
        if (xrand(&(PARMPTR->randomSeed)) < stateRate) {
            /**************/
            /* DEALLOCATE */
            /**************/
            ndxStart = MAXALLOCS;
            while (ndxStart == MAXALLOCS) {
              ndxStart = xrand(&(PARMPTR->randomSeed)) * MAXALLOCS;
            }
            /* Find an allocated slot (if not continue) */
            for (ndx2=(ndxStart+1) % MAXALLOCS; ndx2!=ndxStart;
                ndx2=(ndx2+1)%MAXALLOCS) {
                if (allocBlockHandle[ndx2][PARMPTR->simID] != NULL) {

                    /* Block to be freed should start with "magic" number */
                    if (*(int *)allocBlockHandle[ndx2][PARMPTR->simID]!=MAGIC) {
                       printf("ERROR [thread %d, loop %d]: block being "
                              "freed does not start with magic number\n"
                              "probably overwritten by stray pointer\n",
                              PARMPTR->simID, ndxLoop);
                       printf("Aborting simulation\n");
                       exit(0);
                    }
                    /* Clear start of block */
                    blockSize = 
                      *(int *)(allocBlockHandle[ndx2][PARMPTR->simID]+4);
                    memset(allocBlockHandle[ndx2][PARMPTR->simID],0,blockSize);

                    /* Free block and mark slot free */
                    myFree(allocBlockHandle[ndx2][PARMPTR->simID]);
                    allocBlockHandle[ndx2][PARMPTR->simID] = NULL;

                    /* Record statistics */
                    PARMPTR->deallocations++;
                    PARMPTR->allocatedBlocks--;
                    break;
                }
            }
        } /* End deallocation simulation */
        else {
            /************/
            /* ALLOCATE */
            /************/
            blockSize = 0;
            while (blockSize == 0) {
              blockSize = xrand(&(PARMPTR->randomSeed)) * 
                             xrand(&(PARMPTR->randomSeed)) *
                             xrand(&(PARMPTR->randomSeed)) * maxBlock;
            }
            ndxStart = MAXALLOCS;
            while (ndxStart == MAXALLOCS) {
               ndxStart = xrand(&(PARMPTR->randomSeed)) * MAXALLOCS;
            }
            /* Find an available slot (if none don't allocate) */
            for (ndx2=(ndxStart+1) % MAXALLOCS; ndx2!=ndxStart;
                ndx2=(ndx2+1)%MAXALLOCS) {
                if (allocBlockHandle[ndx2][PARMPTR->simID] == NULL) {

                    /* Allocate block and remember size */
                    if ((allocPtr = myAlloc(blockSize)) == NULL) {
                       printf("Ran out of memory: "
                              "thread %d, loop %d, request size %d\n",
                              PARMPTR->simID, ndxLoop, blockSize);
                       break;
                    }

                    /* Sanity checks -- returned block must be within
                       allocated space, and must be at least aligned
                       on an 8-byte boundary.
                    */
                    assert((allocPtr>=memStart) && 
                           (allocPtr < memStart+totalMemory));
                    assert(((long int)allocPtr&0x7) == 0);

                    /* Remember where this block is */
                    allocBlockHandle[ndx2][PARMPTR->simID] = allocPtr;

                    /* Set values in allocated area for purposes used
                       by simuation */
                    memset(allocPtr, 0x1f, blockSize);
                    *(int *)allocPtr = MAGIC;
                    *(int *)(allocPtr+4) = blockSize;

                    /* Record statistics */
                    PARMPTR->totalMemoryRequest+=blockSize;
                    PARMPTR->allocations++;
                    PARMPTR->allocatedBlocks++;
                    break;
                }
            }
        }  /* End allocation simulation */
        /* After initial "warmup" period, shift to lower allocation
           rate */
	    if (ndxLoop == WARMUP_LOOPS)
            stateRate = STEADY_RATE;

    } /* End main simulation loop */

    /* Done with thread */
    return(NULL);
}
        

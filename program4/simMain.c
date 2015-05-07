/* CpSc 322 -- linear memory usage simulator */

/* This is a "test harness" to test linear memory allocation and
   free routines.
*/

/*********************************************************************
 *  DO NOT change code below this point (for sumbitted version)      *
 ********************************************************************/

#include "sim.h"


int main(int argc, char *argv[]) {
    int ndxThread;
    int ndx2;
    pthread_t *threadID;
    struct threadParms parameterBlock[MAXTHREADS];
    clock_t startTime, endTime;
    struct tms timeBuf;
    int allocationsTotal=0;
    int deallocationsTotal=0;
    int memoryTotal=0;
    int cpuAllocatedRemaining=0;
    int totalAllocatedRemaining=0;

    /* Process command line arguements */
    if (argc < 5) {
       fprintf(stderr, 
          "Usage: ./memSim numThreads maxMemory maxLoops maxBlock\n");
       exit(1);
    }

    totalMemory=1000*1024;
    if (((numThreads=atoi(argv[1]))<=0) ||
        ((totalMemory=atoi(argv[2]))<=0) ||
        ((maxLoops = atoi(argv[3]))<=0) ||
        ((maxBlock = atoi(argv[4]))<= 0)) {
        fprintf(stderr, "Could not convert command line arguement\n");
        fprintf(stderr, 
           "Usage: ./memSim numThreads maxMemory maxLoops maxBlock\n");
        exit(1);
    }
    if (numThreads > MAXTHREADS) {
       fprintf(stderr, "Number of threads must be between 1 and 4\n");
       fprintf(stderr, 
          "Usage: ./memSim numThreads maxMemory maxLoops maxBlock\n");
       exit(1);
    }

    threadID = (pthread_t *)malloc(numThreads*sizeof(pthread_t));

    printf("\nMemory simulation starting\n");
    printf("   Linear Memory Size:   %8d\n"
           "   Number of Threads:    %8d\n"
           "   Sim Loops Per Thread: %8d\n"
           "   Maximum Block Size:   %8d\n",
           totalMemory, numThreads, maxLoops, maxBlock);

    /* Initialize simulation variables */
    startTime = times(&timeBuf);
    for (ndxThread=0; ndxThread<numThreads; ndxThread++) {
        parameterBlock[ndxThread].simID = ndxThread;
        parameterBlock[ndxThread].loops = maxLoops;
        parameterBlock[ndxThread].allocations=0;
        parameterBlock[ndxThread].deallocations=0;
        parameterBlock[ndxThread].totalMemoryRequest=0;
        parameterBlock[ndxThread].allocatedBlocks=0;
        parameterBlock[ndxThread].randomSeed = (ndxThread+231) * 11;
        for (ndx2=0; ndx2<MAXALLOCS; ndx2++) {
            allocBlockHandle[ndx2][ndxThread] = NULL;
        }
    }

    /* Provide opportunity to do initialize memory manager */
    myMemInit();

    /* Create the threads to simulate memory requests */
    for (ndxThread=0;ndxThread<numThreads;ndxThread++) {
        if (pthread_create(&threadID[ndxThread], NULL, memSim,
               (void *)&parameterBlock[ndxThread]) != 0) {
            perror("Thread create");
            printf("Creation of thread %d failed\n", ndxThread);
            exit(1);
        }
    }

    /* Wait for threads to complete */
    for (ndxThread=0;ndxThread<numThreads;ndxThread++) 
        pthread_join(threadID[ndxThread],NULL);
    
    /* Print statistics */
    printf("\nTHR  %12.12s  %12.12s  %12.12s  %12.12s  %12.12s\n", 
      "Allocations", "Deallocations", "Total Memory","Remain Alloc", 
      "Remain Blks");
    for (ndxThread=0; ndxThread<numThreads; ndxThread++) {
        cpuAllocatedRemaining = 0;
        for (ndx2=0; ndx2<MAXALLOCS; ndx2++) {
            if (allocBlockHandle[ndx2][ndxThread] != NULL) {
                cpuAllocatedRemaining += 
                   *(int *)(allocBlockHandle[ndx2][ndxThread]+4);
            }
        }
        totalAllocatedRemaining += cpuAllocatedRemaining;
        printf("%3d  %12d  %12d  %12d  %12d  %12d\n",
           ndxThread,
           parameterBlock[ndxThread].allocations,
           parameterBlock[ndxThread].deallocations,
           parameterBlock[ndxThread].totalMemoryRequest,
           cpuAllocatedRemaining,
           parameterBlock[ndxThread].allocatedBlocks);
        allocationsTotal += parameterBlock[ndxThread].allocations;
        deallocationsTotal += parameterBlock[ndxThread].deallocations;
        memoryTotal += parameterBlock[ndxThread].totalMemoryRequest;
    }
    printf("TOTAL%12d  %12d  %12d  %12d\n",
        allocationsTotal, deallocationsTotal, memoryTotal,
        totalAllocatedRemaining);

    /* And finally print distribution of block sizes */
    scanBlocks();

    endTime = times(&timeBuf);
    printf("\nExecution time: Real=%d.%ds, user=%d.%ds, system=%d.%d\n\n", 
        (int)(endTime-startTime)/100, (int)(endTime-startTime)%100,
        (int)timeBuf.tms_utime/100, (int)timeBuf.tms_utime%100, 
        (int)timeBuf.tms_stime/100, (int)timeBuf.tms_stime%100);

    exit(0);
}

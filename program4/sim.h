/**************************************************************************
* INCLUDES                                                                * 
**************************************************************************/
#define _REENTRANT
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/times.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/**************************************************************************
* SIMULATION PARAMETERS                                                   *
**************************************************************************/
#define MAXTHREADS 8          /* Maximum number of threads */
#define MAXALLOCS 2000        /* For simulation    */
#define WARMUP_LOOPS 1000     /* For simulation */
#define WARMUP_RATE .2        /* For simulation */
#define STEADY_RATE .48       /* For simulation */
#define MAGIC 0x1a3b2f6e      /* Debugging      */

/**************************************************************************
* STRUCTURE DEFINITIONS AND GLOBAL DATA                                   *
**************************************************************************/
/* The following are only used by my simulation */
struct threadParms {
    int loops;
    int simID;
    unsigned randomSeed;
    int allocations;
    int deallocations;
    int totalMemoryRequest;
    int allocatedBlocks;
};

/* Simulation control data */
int numThreads;   /* Number of threads to run concurrently */
int maxLoops;     /* Number of simulation loops per thread */
int maxBlock;     /* Maximum block size request            */

/* "Handles" of allocated blocks, by thread (used by my 
    simulation only)  */
void *allocBlockHandle[MAXALLOCS][MAXTHREADS];

#include "mem.h"

/**********************************************************************
 *  PROTOTYPE STATEMENTS (simulator routines)                         *
 *********************************************************************/
void *memSim(void *parmPtr);
float xrand(unsigned *seed);
void scanBlocks();


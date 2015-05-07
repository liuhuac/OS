/** Linear memory management routines **/

#include "mem.h"

/** Global data **/

/** myMemInit **/
/** Memory manager initialization routine **/
void myMemInit() {
    int ndx;
    int numSuperBlocks;

    /* initialize semaphore */
    sem_init(&mutex, 0, 1);

    /* Allocate space for linear memory block that will be managed */
    memStart = (void *)memalign(4096, totalMemory);

    /* Initialize head pointers for blocks of size 16, 32, ..., 512, 1024 */
    for (ndx=0; ndx<7; ndx++) {
        freeList[ndx].next = NULL;
    }
    
    /* Create initial linked list of 1024 bytes free blocks */
    splitBlock(0, memStart, totalMemory);

    /* Allocate and initialize shadow table */
    numSuperBlocks=totalMemory/1024;
    shadowTable = 
       (superBlockInfo *)malloc(numSuperBlocks*sizeof(superBlockInfo));
    for (ndx=0; ndx<numSuperBlocks; ndx++) {
       shadowTable[ndx].freeNdx = 0;         /* in 1,024 list     */
       shadowTable[ndx].numAllocated = 0;    /* Nothing allocated */
    }
} /* End myMemInit() */
    
/** myAlloc **/
/** Memory allocation routine **/
void *myAlloc(int requestSize) {
    void *allocPtr;           /* Pointer to allocated block  */
    blockNode *superBlockPtr; /* Pointer to free super block */
    int listNdx;              /* Free list index             */
    int blockSize;            /* Rounded-up size             */
    int blockNdx;             /* Free list index             */
   
    /* Based on request size, determine the index of the free list to check */
    blockSize = 16;
    for (listNdx=6; requestSize > blockSize; listNdx--) {
        blockSize = blockSize << 1;  /* Left shift same as multiply by 2 */
    }
    assert(listNdx >= 0);

    sem_wait(&mutex);
    /* Is the list populated? */
    if ((allocPtr=getBlock(listNdx)) == NULL) {

       /* The list for the requested size is empty                         */
       if (listNdx == 0) {
          sem_post(&mutex);
          /* If this is a request for the 1024 list, then return NULL 
             (there are no larger blocks to split)                         */
          return(NULL);
       }

       /* Otherwise try to populate the list from the 1024 list            */
       if ((superBlockPtr = getBlock(0)) == NULL) {
          sem_post(&mutex);
          /* No luck -- nothing in the 1024 list */
          return(NULL);
       }

       /* We've got a superblock off of the 1024 list.                     */

       /* First determine its superblock index in shadow table, and
          update its shadow table data.                                    */
       blockNdx = ((void *)superBlockPtr-memStart)/1024;
       //sem_wait(&mutex); 
       shadowTable[blockNdx].freeNdx = listNdx;  /* Update free list index */
       shadowTable[blockNdx].numAllocated = 0;   /* None allocated yet     */
       //sem_post(&mutex);

       /* Next, split the 1,024 block into blocks of the required size,
          creating the linked list of free blocks as we go                 */
       //sem_wait(&mutex);
       splitBlock(listNdx, superBlockPtr, 1024);
       //sem_post(&mutex);

       /* And get block from list just created                             */
       //sem_wait(&mutex);
       allocPtr = getBlock(listNdx);
       //sem_post(&mutex);
    }
    //sem_post(&mutex);

    /* At this point allocPtr should be pointing to the block we will
       allocate. Update number allocated and return block pointer          */
    blockNdx = ((void *)allocPtr-memStart)/1024; 
    //sem_wait(&mutex);
    shadowTable[blockNdx].numAllocated++;
    sem_post(&mutex);

    /* Sanity check */
    assert(allocPtr >= memStart && allocPtr<memStart+totalMemory); 

    //sem_post(&mutex);

    return(allocPtr);
}  /* End myAlloc() */


/** myFree **/
/** Memory free routine **/
void myFree(void *freeBlockPtr) {
    /** STUBBED **/
    int blockNdx = (freeBlockPtr-memStart)/1024;
    int listNdx;
    void *superBlockStart = memStart + blockNdx * 1024;
    void *superBlockEnd = superBlockStart + 1024; 
    blockNode *preBlock;
    blockNode *subBlock;
sem_wait(&mutex);
    //sem_wait(&mutex);
    listNdx = shadowTable[blockNdx].freeNdx;
    //sem_post(&mutex);

    //sem_wait(&mutex);
    freeBlock(freeBlockPtr, listNdx);
    //sem_post(&mutex);

    //sem_wait(&mutex);
    shadowTable[blockNdx].numAllocated--;
    //sem_post(&mutex);

    //sem_wait(&mutex);
    if(shadowTable[blockNdx].numAllocated == 0) {
        //sem_wait(&mutex);
        subBlock = freeList[listNdx].next;
        preBlock = &freeList[listNdx];
        while(subBlock != NULL) {
           if(superBlockStart <= (void*)subBlock && (void*)subBlock < superBlockEnd) {
               preBlock->next = subBlock->next;
               subBlock = subBlock->next;
           } else {
               subBlock = subBlock->next;
               preBlock = preBlock->next;
           }
        }
       //sem_post(&mutex);

        freeBlock(superBlockStart, 0);
        //sem_wait(&mutex);
        shadowTable[blockNdx].freeNdx = 0;
        //sem_post(&mutex);
    }
    //sem_post(&mutex);
sem_post(&mutex);
}


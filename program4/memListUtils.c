#include "mem.h"

/** getBlock **/
/* Get block from "listNdx" free list */
blockNode *getBlock(int listNdx) {
   blockNode *allocPtr;    /* Pointer to allocated block           */

   /* Now allocate first block off of list */
   //sem_wait(&mutex);
   allocPtr = (void *)freeList[listNdx].next;
   //sem_post(&mutex);
   if (allocPtr == NULL) {
     //sem_post(&mutex);
     /* List is currently empty */
     return(NULL);
   }
 
   /* Remove block from free list */
   //sem_wait(&mutex);
   freeList[listNdx].next =  freeList[listNdx].next->next;
   //sem_post(&mutex);

   /* Update number of allocated blocks in superblock */
   return(allocPtr);
} /* End getBlock */


/** freeBlock **/
/* Link freed block back onto head of appropriate free list */
void freeBlock(blockNode *freeBlockPtr, int listNdx) {

   /* Link block back into appropriate free list */
   //sem_wait(&mutex);
   ((blockNode *)freeBlockPtr)->next = freeList[listNdx].next;
   freeList[listNdx].next = (blockNode *)freeBlockPtr;
   //sem_post(&mutex);

} /* End freeBlock */


/** splitBlock **/
/* Create a linked list of free blocks out of a larger block */
void splitBlock(int listNdx, blockNode *blockStart, int superBlockSize) {
    int       subBlockSize;    /* Size of sub-block     */
    blockNode *subBlock;       /* Pointer to sub-block  */
    blockNode *nextSubBlock;   /* Pointer to next block */
    int numBlocks;             /* Number of sub-blocks  */
    int blockCount;            /* loop counter          */

    subBlockSize = 1024 >> listNdx;
    assert(subBlockSize <= 1024);;
    /* Determine number of sub-blocks to create */
    numBlocks = superBlockSize/subBlockSize;
    blockCount=1;
    subBlock = blockStart;

    //sem_wait(&mutex);
    /* Create a linked list of sub-blocks in the super block */
    while (blockCount < numBlocks) {
       /* Computer address of next sub-block */
       nextSubBlock = (void *)subBlock + subBlockSize;
       /* Link it to list */
       subBlock->next = nextSubBlock;
       /* Move on to next block */
       subBlock = nextSubBlock;
       blockCount++;
    }
    subBlock->next = NULL;

    //sem_wait(&mutex);
    freeList[listNdx].next = blockStart;
    //sem_post(&mutex);

    return;
} /* End splitBlock */

/**********************************************************************
 *  Structures, global data and prototype statements for memory       *
 *    management routines                                             *
 *                                                                    *
 *  You may add additional global data and structure definitions      *
 *  and prototype statements to this include file.                    *
 *********************************************************************/

#define _REENTRANT
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>
#include <semaphore.h>

/** Structure and typedef definitions **/
typedef struct blockNode {
     struct blockNode *next;         /* Pointer to next node on free list */
} blockNode;

typedef struct info {
   int freeNdx;      /* Index of free list that contains blocks of this 
                        size                                             */
   int numAllocated; /* Number of allocated blocks in superblock         */
} superBlockInfo;

/** Global Data **/
int totalMemory;                     /* Size of linear memory            */
void *memStart;                      /* Start address of linear memory area 
                                        being managed                    */
blockNode freeList[8];               /* Dummy header nodes               */
superBlockInfo *shadowTable;         /* Pointer to malloc'd shadow table */
int length;

sem_t mutex;

/**  PROTOTYPE STATEMENTS **/
void myMemInit();
void *myAlloc(int requestSize);
void myFree(void *blockAddr);

void coalesceBlock(int blockNdx, int listNdx);

blockNode *getBlock(int listNdx);
void freeBlock(blockNode *freeBlockPtr, int listNdx);
void splitBlock(int listNdx, blockNode *blockStart, int superBlockSize);

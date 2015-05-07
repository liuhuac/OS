#ifndef VMSIM_H
#define VMSIM_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

/** reference string file action codes **/
#define MEMREAD   0
#define MEMWRITE  1
#define FRAMEFREE 9

/** Page table entry format **/
typedef struct pageTableEntry {
   unsigned vmPage;     /* Number of virtual page in this frame */
   int  valid;          /* Valid (present) bit                  */
   int  referenced;     /* Referenced (use) bit                 */
   int  modified;       /* Modified bit                         */
} pageTableEntry_t;

/** Global data **/
pageTableEntry_t *pageTable;  /* Pointer to base of page table array  */
int numPageFrames;            /* Number of page frames                */
int lastLoaded;

/** Prototype Statements **/
int   pageFault(unsigned logicalAddress);
int   inMemory(unsigned vmPageNum);
void  freeFrame(unsigned logicalAddress);
int   MMU(unsigned logicalAddress, int refType);
pageTableEntry_t *pageTableInit(int numPageFrames);

#endif

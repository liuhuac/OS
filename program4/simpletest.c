/* CpSc 322 -- linear memory usage simulator */

/* This is a "test harness" to test linear memory allocation and
   free routines.
*/

/*********************************************************************
 *  DO NOT change code below this point (for sumbitted version)      *
 ********************************************************************/

#include "sim.h"


int main(int argc, char *argv[]) {
    void *ptr, *ptr2, *ptr3;
    void *ptrarray[32];
    int  ndx;

    totalMemory=1000*1024;

    /* Provide opportunity to do initialize memory manager */
    myMemInit();
    printf("\nInitial Lists:\n");
    scanBlocks();

    ptr = myAlloc(10);
    memset(ptr, 0xac, 10);
    printf("\nLists after myAlloc(10):\n");
    scanBlocks();
    myFree(ptr);
    printf("\nLists after myFree() of previous block:\n");
    printf("Scan after free:\n");
    scanBlocks();

    ptr = myAlloc(1000);
    memset(ptr, 0xac, 1000);
    ptr2 = myAlloc(200);
    memset(ptr2, 0xac, 200);
    ptr3 = myAlloc(20);
    memset(ptr3, 0xac, 20);
    printf("\nLists after myAlloc()'s of 1000, 200 and 20:\n");
    scanBlocks();
    myFree(ptr3);
    myFree(ptr);
    printf("\nLists after myFree()'s of 1000 and 20 byte blocks:\n");
    scanBlocks();

    for (ndx=0; ndx<32; ndx++){
        ptrarray[ndx] = myAlloc(40);
        memset(ptrarray[ndx], 0xac, 40);
    }
    printf("\nLists after 32 myAlloc()'s of 40 byte blocks:\n");
    scanBlocks();

    for (ndx=0; ndx<32; ndx+=2){
        myFree(ptrarray[ndx]);
    }
    printf("\nLists after myFree()'s of every other 40 byte block:\n");
    scanBlocks();

    for (ndx=1; ndx<32; ndx+=2){
        myFree(ptrarray[ndx]);
    }
    printf("\nLists after myFree()'s rest of 40 byte block:\n");
    scanBlocks();

    ptr = myAlloc(50);
    memset(ptr, 0xac, 50);
    myFree(ptr);
    printf("\nLists after myAlloc() of 50 byte block, then myFree():\n");
    scanBlocks();

    printf("\nAnd finally free 200 byte block:\n");
    myFree(ptr2);
    scanBlocks();

    exit(0);
}

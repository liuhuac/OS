/** hw4

    Implementation of the homework 4 solution for stacks.

**/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "stack.h"
#include "userthread.h"

typedef struct parmType {
   int myID;            /* Thread's unique id                        */
   int numWords;        /* Number of words to push or pop            */
   mystack_t *stack;    /* Pointer to stack                          */
   int popcount;        /* If popper, count how many non-null return
                           pops occurred for this thread             */
} parm_t;

/* Global data */
parm_t parm[10];
tcb_t  *id[10];

/** pusher **/
void pusher() {
   int count;
   char wordbuf[20];
   int myid;

   /* Hokie and unreliable way to generate a unique id between 0 and 9 */
   tsnooze(1);
   for (myid=0; myid<10; myid++) {
       if (tid() == id[myid]) break;
   }
   assert(myid < 10);

   /* Manufacture data and place on stack */
   for (count=0; count<parm[myid].numWords; count++) {
       sprintf(wordbuf, "word%d.%d", myid, count);
       push(parm[myid].stack, strdup(wordbuf));
   }
   printf("Pusher Thread %d done\n", myid);
   texit();
}

/** popper **/
void popper() {
   int count;
   char *word;
   int myid;

   /* Hokie and unreliable way to generate a unique id between 0 and 9 */
   tsnooze(1);
   for (myid=0; myid<10; myid++) {
       if (tid() == id[myid]) break;
   }
   assert(myid < 10);

   /* Pop data from the stack */
   parm[myid].popcount = 0;
   for (count=0; count<parm[myid].numWords; count++) {
      word=pop(parm[myid].stack);
      /* Count number of successful pops */
      if (word != NULL) {
         free(word);
         parm[myid].popcount++;
      }
   }

   printf("Popper Thread %d done\n", myid);
   texit();
}
       
/** main **/
int main() {
   int ndx;
   mystack_t *stack1;
   mystack_t *stack2;

   startThreadSystem();

   /* Create stack1 */
   stack1 = newStack(100);
   /* And exercise it by spawning 4 threads */
   for (ndx=0; ndx<4; ndx++) {
       parm[ndx].myID = ndx;
       parm[ndx].numWords = 30000;
       parm[ndx].stack = stack1;
       if ((ndx%2) == 0) {
          id[ndx] = newThread(pusher);
       }
       else {
          id[ndx] = newThread(popper);
       }
   }

   /* Create stack2 */
   stack2 = newStack(5000);
   /* And exercise it by spawning 6 threads */
   for (; ndx<10; ndx++) {
       parm[ndx].myID = ndx;
       parm[ndx].numWords = 9000;
       parm[ndx].stack = stack2;
       if ((ndx%2) == 0) {
          id[ndx] = newThread(pusher);
       }
       else {
          id[ndx] = newThread(popper);
       }
   }

   /* Wait for everyone to end */
   for (ndx=0; ndx<10; ndx++) {
       twait(id[ndx]);
       if ((ndx % 2) == 1) {
          printf("Popper %d count = %d\n", ndx, parm[ndx].popcount);
       }
   }

   printf("Main thread done\n");
   exit(0);
}


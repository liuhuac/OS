/** CpSc 322

    Semaphore test using semaphore for mutual exclusion

**/
#include "userthread.h"
#include "semaphore.h"

/* Global data */
int         count = 0;   /* Shared data                                */
semaphore_t lock;        /* semaphore used to enforce mutual exclusion */
tcb_t       *child[4];   /* Thread ids                                 */      

extern tcb_t *runningTCB;

/** inc **/
void inc() {
  int i;
  int tmp;
  int delay;
  int myid;

  /* Hokie and unreliable way to generate a unique id between 0 and 3 */
  tsnooze(1);
  for (myid=0; myid<4; myid++) {
      if (tid() == child[myid]) break;
  }
  assert(myid < 4);

  printf("Incrementer %d starting\n", myid);
  for (i=0; i<100000; i++) {
      if (i%2000 == 0) {
          printf("%d[%d],", myid, tpriority()); fflush(stdout);
      }
    
      /* Start of critical section */
      semWait(&lock);
      tmp = count;
      for (delay=0; delay<1000; delay++) /* do nothing */;
      count = tmp + 1;
      semPost(&lock);
      /* End of critical section */

      for (delay=0; delay<10000; delay++) /* do nothing */;
  }
  printf("\nIncrementer %d ending\n", myid);
  texit();
}

/** main **/
int main(int argc, char *argv[]) {
   int   ndx;

   startThreadSystem();

   semInit(&lock, 1);
   for (ndx=0; ndx<4; ndx++) {
      child[ndx] = newThread(inc);
   }
   for (ndx=0; ndx<4; ndx++) {
      twait(child[ndx]);
   }

   printf("\ncount=%d\n", count);
   printf("Main done\n");
   exit(0);
}

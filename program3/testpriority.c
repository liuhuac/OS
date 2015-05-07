/** testpriority

    Tests cases where low priority thread is holding the
    spin lock and high priority thread is trying to acquire
    the lock.

**/

#include "userthread.h"
#include "semaphore.h"

semaphore_t lock;
long        count=0;

/* sub1: low priority thread */
void sub1() {
  int ndx;
  int ndx2;
  tsetrange(LEVELS-2, LEVELS-2);

  for (ndx=0; ndx<100; ndx++) {
     for (ndx2=0; ndx2<2000000; ndx2++) {
      semWait(&lock);
      count++;
      semPost(&lock);
    }
    printf("1"); fflush(stdout);
  }
  printf("sub1 done, priority=%d\n", tpriority());
  
  texit();
}

/* sub2: high priority thread */
void sub2() {
  int ndx;
  tsetrange(0,0);

  for (ndx=0; ndx<20; ndx++) {
      semWait(&lock);
      count++;
      semPost(&lock);
      tsnooze(1);
      printf("2\n"); fflush(stdout);
  }
  printf("sub2 done, priority=%d\n", tpriority());
  texit();
}


/** main thread -- parent **/
int main() {
   tcb_t *tid1, *tid2;
   startThreadSystem();

   semInit(&lock, 1);

   /* Create user threads */
   tid1= newThread(sub1);
   tid2= newThread(sub2);

   twait(tid1);
   twait(tid2);

   printf("Main: count=%ld\n", count);

   printf("Main done\n");
   exit(0);
}

#include "userthread.h"

/* sub1: Test child thread */
void sub1() {
  int i, delay;;
  for (i=0; i<20; i++) {
   printf("Hello from sub 1, loop %d\n", i);
   for (delay=0; delay<50000; delay++) /* Do nothing delay loop */;
   tyield();
  }
  printf(">>>sub 1 ending\n");
  texit();
}

/* sub1: Test child thread */
void sub2() {
  int i, delay;
  for (i=0; i<15; i++) {
   printf("Hello from sub 2, loop %d\n",i);
   for (delay=0; delay<50000; delay++) /* Do nothing delay loop */;
   tyield();
  }
  printf(">>>sub 2 ending\n");
  texit();
}

/** main thread -- parent **/
int main() {
   tcb_t *tid1, *tid2;
   startThreadSystem();
   /* Create user threads */
   tid1= newThread(sub1);
   tid2= newThread(sub2);

   printf("Waiting on tid1\n"); fflush(stdout);
   twait(tid1);
   printf("Waiting on tid2\n"); fflush(stdout);
   twait(tid2);

   printf("Main done\n");
   exit(0);
}

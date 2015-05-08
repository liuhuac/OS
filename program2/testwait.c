#include "userthread.h"

/* sub1: Test child thread */
void sub1() {
  int i, delay;;
  for (i=0; i<20; i++) {
   printf("[1, %d],", i);
   for (delay=0; delay<50000; delay++) /* Do nothing delay loop */;
   tyield();
  }
  printf("\n>>>sub 1 ending\n");
  texit();
}

void sub1Parent() {
   tcb_t *tid1, *tid2;
   printf("sub1Parent starting\n");
   tid1 = newThread(sub1);
   tid2 = newThread(sub1);
   twait(tid1);
   twait(tid2);
   printf("sub1Parent ending\n");
   texit();
}

/* sub2: Test child thread */
void sub2() {
  int i, delay;
  for (i=0; i<15; i++) {
   printf("[2, %d],", i);
   for (delay=0; delay<50000; delay++) /* Do nothing delay loop */;
   tyield();
  }
  printf("\n>>>sub 2 ending\n");
  texit();
}

void sub2Parent() {
   tcb_t *tid1, *tid2;
   printf("sub2Parent starting\n");
   tid1 = newThread(sub2);
   tid2 = newThread(sub2);
   twait(tid1);
   twait(tid2);
   printf("sub2Parent ending\n");
   texit();
}

/** main thread -- parent **/
int main() {
   tcb_t *tid1, *tid2;
   int waitcode;
   startThreadSystem();
   /* Create user threads */
   tid1= newThread(sub1Parent);
   tid2= newThread(sub2Parent);
   tyield();
   tyield();

   waitcode = twait(tid1);
   printf("\nMain parent: waitcode=%d\n", waitcode);
   waitcode = twait(tid2);
   printf("\nMain parent: waitcode=%d\n", waitcode);

   printf("Main done\n");
   exit(0);
}

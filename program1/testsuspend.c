#include "userthread.h"

/* Save thread tids in global data */
tcb_t *tid1, *tid2;

/* sub1: Test child thread */
void sub1() {
  int i, delay;;
  for (i=0; i<40; i++) {
   printf("sub 1: I'm alive loop %d\n", i);
   for (delay=0; delay<5000000; delay++) /* Do nothing delay loop */;
   tyield();
  }
  printf(">>>sub 1 ending\n");
  texit();
}

/* sub2: Test child thread */
void sub2() {
  int code;
  int i, delay;
  for (i=15; i>=0; i--) {
   printf("sub 2: countdown to suspend: %d\n",i);
   for (delay=0; delay<500000; delay++) /* Do nothing delay loop */;
   tyield();
  }
  printf("sub2: suspending sub1\n");
  code=tsuspend(tid1);
  printf("sub2: now sub1 should be suspended\n");
  printf("Kill code 1=%d\n", code);
  printf("sub2: try a bad suspend...\n");
  code = tsuspend((tcb_t *)999);
  printf("Kill code 2=%d\n", code);

  /* Delay ending for a while... */
  for (i=15; i>=0; i--) {
   printf("sub 2: countdown to resume: %d\n",i);
   for (delay=0; delay<500000; delay++) /* Do nothing delay loop */;
   tyield();
  }
  /* Now resume sub1 */
  printf("sub2: Resuming sub1\n");
  code = tresume(tid1);
  printf("sub2: return code from resume: %d\n", code);
  printf(">>>sub 2 ending\n");
  texit();
}

void sub3() {
   tcb_t *tid4;
   printf("sub3: creating sub1 child\n");
   tid4 = newThread(sub1);
   printf("sub3: and wait for sub1 to complete\n");
   twait(tid4);
   printf("sub3: sub1 has completed, done\n");
   texit();
}

/** main thread -- parent **/
int main() {
   tcb_t *tid3;
   int i, delay;

   startThreadSystem();

   /* Create user threads */
   tid1 = newThread(sub1);
   tid2 = newThread(sub2);

   twait(tid1);
   printf("Main: thread 1 ended\n");
   twait(tid2);
   printf("Main: thread 2 ended\n");

   /* Test suspending a waiting thread */
   tid3 = newThread(sub3);
   for (i=0; i<5; i++) {
      printf("main: I'm alive loop %d\n", i);
      for (delay=0; delay<5000000; delay++) /* Do nothing delay loop */;
      tyield();
   }
   printf("main: now suspend sub3\n");
   assert(tsuspend(tid3));
   for (i=0; i<5; i++) {
      printf("main: I'm alive loop %d\n", i);
      for (delay=0; delay<5000000; delay++) /* Do nothing delay loop */;
      tyield();
   }
   printf("main: now resume sub3\n");
   assert(tresume(tid3));
   printf("main: and wait for sub3 to end\n");
   twait(tid3);

   /* All child threads complete */
   printf("Main done\n");
   exit(0);
}

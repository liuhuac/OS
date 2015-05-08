#include "userthread.h"

/* sub1: Test child thread */
void sub1() {
  int i,delay;
  for (i=0; i<20; i++) {
   printf("Hello from sub 1, loop %d\n", i); fflush(stdout);
   for (delay=0; delay<5000000; delay++) /*Do nothing delay loop */;
   for (delay=0; delay<5000000; delay++) /*Do nothing delay loop */;
   for (delay=0; delay<5000000; delay++) /*Do nothing delay loop */;
   for (delay=0; delay<5000000; delay++) /*Do nothing delay loop */;
   for (delay=0; delay<5000000; delay++) /*Do nothing delay loop */;
   for (delay=0; delay<5000000; delay++) /*Do nothing delay loop */;
   for (delay=0; delay<5000000; delay++) /*Do nothing delay loop */;
  }
  printf(">>>sub 1 ending\n");
  texit();
}

/* sub2: Test child thread */
void sub2() {
  int i,delay;
  for (i=0; i<15; i++) {
   printf("Hello from sub 2, loop %d\n",i); fflush(stdout);
   for (delay=0; delay<5000000; delay++) /*Do nothing delay loop */;
   for (delay=0; delay<5000000; delay++) /*Do nothing delay loop */;
   for (delay=0; delay<5000000; delay++) /*Do nothing delay loop */;
   for (delay=0; delay<5000000; delay++) /*Do nothing delay loop */;
   for (delay=0; delay<5000000; delay++) /*Do nothing delay loop */;
   for (delay=0; delay<5000000; delay++) /*Do nothing delay loop */;
   for (delay=0; delay<5000000; delay++) /*Do nothing delay loop */;
   for (delay=0; delay<5000000; delay++) /*Do nothing delay loop */;
   for (delay=0; delay<5000000; delay++) /*Do nothing delay loop */;
   for (delay=0; delay<5000000; delay++) /*Do nothing delay loop */;
   for (delay=0; delay<5000000; delay++) /*Do nothing delay loop */;
  }
  printf(">>>sub 2 ending\n");
  texit();
}

/* sub3: Test child thread */
void sub3() {
  int i,delay;
  for (i=0; i<25; i++) {
   printf("Hello from sub 3, loop %d\n",i); fflush(stdout);
   for (delay=0; delay<1000000; delay++) /*Do nothing delay loop */;
   for (delay=0; delay<1000000; delay++) /*Do nothing delay loop */;
   for (delay=0; delay<1000000; delay++) /*Do nothing delay loop */;
   for (delay=0; delay<1000000; delay++) /*Do nothing delay loop */;
   for (delay=0; delay<1000000; delay++) /*Do nothing delay loop */;
   for (delay=0; delay<1000000; delay++) /*Do nothing delay loop */;
   for (delay=0; delay<1000000; delay++) /*Do nothing delay loop */;
  }
  printf(">>>sub 3 ending\n");
  texit();
}

/* sub4: this thread also voluntarily releases the CPU with a
   tyield() call.
*/
void sub4() {
  int delay;
  while (tactive() > 2) {
      printf("sub4: tid=%p\n", tid());
      for (delay=0; delay<5000000; delay++) /*Do nothing delay loop */;
      for (delay=0; delay<5000000; delay++) /*Do nothing delay loop */;
      for (delay=0; delay<5000000; delay++) /*Do nothing delay loop */;
      for (delay=0; delay<5000000; delay++) /*Do nothing delay loop */;
      tyield();
  }
  printf(">>>sub 4 ending\n");
  texit();
}

/** Main thread **/
int main() {
   tcb_t *tid1, *tid2, *tid3, *tid4;

   /* Start the child threads running, and wait for them to complete */
   startThreadSystem();

   tid1 = newThread(sub1);
   tid2 = newThread(sub2);
   tid3 = newThread(sub3);
   tid4 = newThread(sub4);

   twait(tid1);
   twait(tid2);
   twait(tid3);
   twait(tid4);
   
   /* All threads have finished */
   printf("Main done\n");
   exit(0);
}

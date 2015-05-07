#include "userthread.h"
#include "msg.h"

/* Global data */
long count = 0;
mailbox_t *lock;

extern tcb_t *runningTCB;

void inc() {
  int i;
  long tmp;
  int delay;
  int *token;
  printf("Incrementer %d starting\n", tid());
  for (i=0; i<4000000; i++) {
      if (i%100000 == 0) {
         printf("<%d,%d>,", tid(), tpriority());
         fflush(stdout);
      }
      token = receive(lock);
      tmp = count;
      for (delay=0; delay<10; delay++) /* do nothing */;
      count = tmp + 1;
      send(lock, token);
  }
  printf("Incrementer %d ending\n", tid());
  texit();
}

int main(int argc, char *argv[]) {
   int tid1, tid2, tid3, tid4;
   clock_t startTime, endTime;
   struct tms timeBuf;
   startTime = times(&timeBuf);

   startThreadSystem();

   lock = newMailbox();
   tid1 = newThread(inc);
   tid2 = newThread(inc);
   tid3 = newThread(inc);
   tid4 = newThread(inc);

   send(lock , "thekey");

   printf("Main waiting\n");
   childWait(tid1);
   childWait(tid2);
   childWait(tid3);
   childWait(tid4);

   printf("\ncount=%ld\n", count);
   printf("Main done\n");
   endTime = times(&timeBuf);
   printf("\nExecution time: Real=%d.%ds, user=%d.%ds, system=%d.%d\n\n",
       (int)(endTime-startTime)/100, (int)(endTime-startTime)%100,
       (int)timeBuf.tms_utime/100, (int)timeBuf.tms_utime%100,
       (int)timeBuf.tms_stime/100, (int)timeBuf.tms_stime%100);

   exit(0);
}

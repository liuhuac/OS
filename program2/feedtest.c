#include "userthread.h"

/** Global data **/
extern tcbList_t *liveList;
extern tcb_t     *runningTCB;
extern float     alpha;

int makeid(tcb_t *tcb);

/** snoozer **/
void snoozer() {
   int loopCnt;
   int myid = makeid(tid());

   printf("Starting snoozer%2d\n", myid); fflush(stdout);
   for (loopCnt = 0; loopCnt < 50; loopCnt++) {
       tsnooze(30);
       fprintf(stdout, "snoozer%2d  %5d - priority %2d (tau=%6.4f)\n", 
                 myid, loopCnt, tpriority(), runningTCB->tau); 
      fflush(stdout);
   }
   
   fprintf(stdout, "snooze%2d ending - priority %d\n", myid,
           tpriority()); fflush(stdout);
   texit();
}

/** looper **/
void looper() {
   int loopCnt;
   int loopCnt2;
   int myid = makeid(tid());

   printf("Starting looper%2d\n", myid); fflush(stdout);
   for (loopCnt=0; loopCnt < 35; loopCnt++) {
       /* CPU Loop */
       for (loopCnt2=0; loopCnt2<10000000; loopCnt2++) {
           /* Do nothing */
       }
       for (loopCnt2=0; loopCnt2<10000000; loopCnt2++) {
           /* Do nothing */
       }
       for (loopCnt2=0; loopCnt2<10000000; loopCnt2++) {
           /* Do nothing */
       }
       for (loopCnt2=0; loopCnt2<10000000; loopCnt2++) {
           /* Do nothing */
       }
       fprintf(stdout, "looper%2d   %5d - priority %2d (tau=%6.4f)\n", 
                 myid, loopCnt, tpriority(), runningTCB->tau); 
       fflush(stdout);
   }
   tsnooze(1);
   fprintf(stdout, "Looper%2d ending - priority %d\n", myid,
           tpriority()); fflush(stdout);
   texit();
}

/** schizoid **/
void schizoid() {
   int outerLoop;
   int loopCnt;
   int loopCnt2;
   int outCnt=1;
   int myid = makeid(tid());

   printf("Starting schizoid%2d\n", myid); fflush(stdout);
   for (outerLoop=0; outerLoop<4; outerLoop++) {
      for (loopCnt=0; loopCnt < 5; loopCnt++) {
          /* CPU Loop */
          for (loopCnt2=0; loopCnt2<10000000; loopCnt2++) {
              /* Do nothing */
          }
          for (loopCnt2=0; loopCnt2<10000000; loopCnt2++) {
              /* Do nothing */
          }
          for (loopCnt2=0; loopCnt2<10000000; loopCnt2++) {
              /* Do nothing */
          }
          fprintf(stdout, "schizoid%2d %5d - priority %2d (tau=%6.4f)[cpu]\n",
                    myid, outCnt++, tpriority(), runningTCB->tau); 
          fflush(stdout);
      }
      for (loopCnt = 0; loopCnt < 5; loopCnt++) {
          tsnooze(50);
          fprintf(stdout, "schizoid%2d %5d - priority %2d (tau=%6.4f)"
                          "[snooze]\n",
                    myid, outCnt++, tpriority(), runningTCB->tau); 
          fflush(stdout);
      }
   }
   fprintf(stdout, "schizoid%2d ending - priority %d\n", myid,
           tpriority()); fflush(stdout);
   texit();
}


/** MAIN **/
int main(int argc, char *argv[]) {
   clock_t startTime, endTime;
   struct tms timeBuf;
   tcb_t *childids[20];
   int   ndx;
   int   children=0;

   if (argc == 2) {
      alpha = atof(argv[1]);
   }

   printf("FEEDTEST: alpha = %f\n", alpha);
   for (ndx=0; ndx<sizeof(childids)/sizeof(tcb_t); ndx++) {
       childids[ndx] = 0;
   }

   startTime = times(&timeBuf);

   startThreadSystem();

   /* Create threads */
   for (ndx=0; ndx<3; ndx++) {
      childids[children++] = newThread(snoozer);
   }
   for (ndx=0; ndx<5; ndx++) {
      childids[children++] = newThread(looper);
   }
   for (ndx=0; ndx<3; ndx++) {
      childids[children++] = newThread(schizoid);
   }

   for (ndx=0; ndx<children; ndx++) {
       twait(childids[ndx]);
   }

   fprintf(stdout, "Main done\n");

   endTime = times(&timeBuf);
   printf("\nExecution time: Real=%d.%ds, user=%d.%ds, system=%d.%d\n\n",
       (int)(endTime-startTime)/100, (int)(endTime-startTime)%100,
       (int)timeBuf.tms_utime/100, (int)timeBuf.tms_utime%100,
       (int)timeBuf.tms_stime/100, (int)timeBuf.tms_stime%100);

   tsnooze(1);

   exit(0);
}

/** makeid -- helper function **/
int makeid(tcb_t *tcb) {
   /* Create a "short" id based on position in live list.  */
   /* Kludge -- might not be unique if a thread ends first */
   tcb_t *curr = liveList->tail->liveprev;
   int id = 1;

   /* Search liveList backwards for thread with specified tid */
   while (curr != liveList->head && curr != tcb) {
      curr = curr->liveprev;
      id++;
   }
   if (curr != NULL) {
      /* Found it */
      return(id);
   }
   /* We shouldn't get here */
   abort();
}


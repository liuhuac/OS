#include "userthread.h"

/** Global Data **/
tcbList_t *readyList[LEVELS];       /* Ready list             */
tcbList_t *suspendReadyList = NULL;/* Ready but suspended list   */
tcbList_t *liveList = NULL;        /* List of live tcb's         */

tcb_t *runningTCB;                 /* Currently running thread   */
tcb_t mainTCB;                     /* Starting threads's TCB     */
int   numThreads=0;                /* Number of active threads   */
struct sigaction vtAlrmAction;     /* SIGVTALRM action           */
int    vtSigNum;                   /* SIGVTALRM signal number    */
int    quantum = QUANTUM;          /* Thread time quantum        */
tcbList_t *zombieList = NULL;      /* Zombie list                */
tcbList_t *sleepList = NULL;       /* Queue of snoozing threads  */
float alpha = 0.5;                 /* Alpha used to computer tau */


/** timeslice **/
void timeslice(int status) {
   runningTCB->slices++;
   if(runningTCB->slices == TIMESLICE) {
	   /* tau should be recomputed */
	   runningTCB->tau = talpha() * runningTCB->slices + (1 - talpha()) * runningTCB->tau;
	   /* The "slices" value should be reset back to zero */
	   runningTCB->slices = 0;

	   runningTCB->priority = (int)(runningTCB->tau+0.5);
	   /* checked everytime a thread's priority is changed */
	   tsetrange(0, LEVELS-2);

	   /* current thread is preempted */
	   tyield();
   } else {
	   /* a new time quantum is set */
	   setalarm(quantum);
   }

   //tyield();                // Switch to next thread on ready list
}

/** tid **/
tcb_t *tid() {
    /* Return current thread tid */
    return(runningTCB);
}

/** tpriority **/
int tpriority() {
    return(runningTCB->priority);
}

/** tsetrange **/
void tsetrange(int minval, int maxval) {
   /** STUBBED */
	runningTCB->priorityMinVal = minval;
	runningTCB->priorityMaxVal = maxval;

	if(runningTCB->priority > maxval) {
		runningTCB->priority = maxval;
	} else if (runningTCB->priority < minval) {
		runningTCB->priority = minval;
	}
}

/** tsetalpha **/
void tsetalpha(float alf) {
   alpha = alf;
}

/** talpha **/
float talpha() {
   return(alpha);
}

/** enable **/
void enable() {
   /* Enable "interrupts" -- tyield() implicitely resets the timer 
       which is the only "interrupt" in this system.
   */
   tyield();
}

/** disable **/
void disable() {
   /* The timer is the only "interrut" -- so setting the timeout to
      zero effectively disables "interrupts".
   */
   setalarm(0);
}

/** newThread **/
tcb_t *newThread(void (*func)()) {
   unsigned int *sp;                      // Stack pointer

   numThreads++;     // Keep track of number of active threads

   /* Now create a new thread control block and thread stack */
   tcb_t *newTCB = malloc(sizeof(tcb_t)); // Allocate space for new tcb

   /* Initialize the new scheduling related fields in the tcb_t */
   newTCB->priority = 0;
   newTCB->tau = 0;
   newTCB->slices = 0;
   newTCB->priorityMinVal = 0;
   newTCB->priorityMaxVal = LEVELS - 2;

   newTCB->stackBase = malloc(STACKSIZE); // And give thread its own stack

   /* Set stack pointer to end of stack (stack grows down) */
   sp = (unsigned int *)((void *)(newTCB->stackBase) + STACKSIZE - 4);

   /* Initialized the "saved" registers on the stack */
   *sp = (unsigned int) func;      // Set saved IP to function address
   sp--; *sp = (unsigned int) sp;  // EBP
   sp--; *sp = 0;                  // FLAGS register
   sp--; *sp = 0;                  // EDI
   sp--; *sp = 0;                  // ESI
   sp--; *sp = 0;                  // EDX
   sp--; *sp = 0;                  // ECX
   sp--; *sp = 0;                  // EBX
   sp--; *sp = 0;                  // EAX

   newTCB->stackPtr = sp;          // Save stack pointer in TCB
   newTCB->state = READY;          // Set thread's state

   /* Disable interrupts before messing with global lists */
   disable();

   /* Add thread to "live" list */
   addLive(newTCB);

   /* Link thread onto end of ready list */
   queue(readyList[newTCB->priority], newTCB);

   enable();   // Re-enable interrupts

   /* Return new thread's ID */
   return(newTCB);
}

/** tyield **/
void tyield() {
   tcb_t *lastTCB;

   disable();   // disable interrupts

   /* Link current thread to end of ready list */
   queue(readyList[runningTCB->priority], runningTCB);
   
   /* Change state of current thread */
   lastTCB = runningTCB;
   lastTCB->state = READY;

   /* Dispatch next thread */
   dispatch();  // Dispatch re-enables interrupts
}

/** texit **/
void texit() {

   disable();                          // Disable interrupts

   runningTCB->state = ZOMBIE;         // Change state to "zombie"

   queue(zombieList, runningTCB);      // Queue on zombie list

   /* Remove this thread from live list */
   removeLive(runningTCB);

   /* Ready parent thread if waiting for this thread to end */
   if (runningTCB->waitingParent != NULL) {
      if (runningTCB->waitingParent->state == WAIT) {
         queue(readyList[runningTCB->priority],
               runningTCB->waitingParent);
         runningTCB->waitingParent->state = READY;
      }
      else 
      if (runningTCB->waitingParent->state == SUSPENDWAIT) {
         queue(suspendReadyList, runningTCB->waitingParent);
         runningTCB->waitingParent->state = SUSPENDREADY;
      }
      else {
         printf("OOPS: waiting parent in neither WAIT or SUSPENDWAIT STATE\n");
         exit(1);
      }
      runningTCB->waitingParent = NULL;
   }

   /* Dispatch next thread */
   dispatch();
}

/** dispatch **/
void dispatch() {
   tcb_t *previousThread;

   disable();   // Disable interrupts

   /* First see if anyone should be woken up */
   checkSleepers();

   /* Remember pointer to thread being switch off CPU */
   previousThread = runningTCB;

   /* Pick thread at head of ready list as first thread to dispatch */
   int i;
   for(i=0; i<LEVELS; i++){
	   if ((runningTCB = dequeue(readyList[i])) != NULL){
		   break;
	   }
   }

   assert(runningTCB != NULL);

   /* Switch to selected thread */
   doSwitch(runningTCB, previousThread);
}

/** startThreadSystem **/
void startThreadSystem() {
   runningTCB = &mainTCB;

   /* Create the ready lists and the live list */
   //if (readyList == 0) {
    int i;
    for(i=0; i<LEVELS; i++) {
        readyList[i] = createList();
    }



   if (liveList == NULL) {
      liveList = createList();
   }
   if (suspendReadyList == NULL) {
      suspendReadyList = createList();
   }
   if (sleepList == NULL) {
      sleepList = createList();
   }

   /* Initialize action structure for vt alarm signal */
   vtSigNum = SIGVTALRM;
   vtAlrmAction.sa_handler = timeslice;
   sigemptyset(&vtAlrmAction.sa_mask);
   vtAlrmAction.sa_flags = SA_NODEFER;

   /* Setup the tcleaner thread */
   zombieList = createList();
   newThread(tcleaner);
}

/** tactive: return number of active threads **/
int tactive() {
    return(numThreads);
}

/** twait **/
int twait(tcb_t *child) {
    disable();   // Disable interrupts
    /* Make sure child exists -- it may have finished already or the
       arguement may be wrong 
    */
    if (findAlive(child)) {
       /* We're going into a wait state.  */
       child->waitingParent = runningTCB;
       runningTCB->state = WAIT;

       /* tau should be recomputed */
       runningTCB->tau = talpha() * runningTCB->slices + (1 - talpha()) * runningTCB->tau;
       /* The "slices" value should be reset back to zero */
       runningTCB->slices = 0;

       runningTCB->priority = (int)(runningTCB->tau+0.5);

       /* checked everytime a thread's priority is changed */
       tsetrange(0, LEVELS-2);

       /* Dispatch another thread */
       dispatch();  // dispatch() implicitly re-enables interrupts
       return(1);
    }
    else {
       /* Requested thread is no longer active */
       enable();  // Re-enable interrupts
       return(0);
    }
}

int tsuspend(tcb_t *tcb) {
    disable();
    if (!findAlive(tcb)) {
       printf("suspend: thread %p not alive\n", tcb);
       enable();
       return(0);
    }
    if (tcb->state == READY) {
       removeTCB(tcb);
       queue(suspendReadyList, tcb);
       tcb->state = SUSPENDREADY;
   }
   else 
   if (tcb->state == WAIT) {
      tcb->state = SUSPENDWAIT;
   }
   else {
      printf("suspend: thread %p not in READY or WAIT state\n", tcb);
      enable();
      return(0);
   }
   enable();
   return(1);
}

int tresume(tcb_t *tcb) {
    disable();
    if (!findAlive(tcb)) {
       printf("resume: thread %p not alive\n", tcb);
       enable();
       return(0);
    }
    if (tcb->state == SUSPENDWAIT) {
       tcb->state = WAIT;
    }
    else 
    if (tcb->state == SUSPENDREADY) {
       removeTCB(tcb);
       queue(readyList[tcb->priority], tcb);
       tcb->state = READY;
    }
    else {
       printf("resume: thread %p not in SUSPEND state\n", tcb);
       enable();
       return(0);
    }
    enable();
    return(1);
}

/** tsnooze **/
void tsnooze(int sleepTime) {


   /* The return from times() is in 10 millisecond units */
   struct tms timeBuf;
   clock_t currentTime;

   disable();             // Turn off timer interrupt

   currentTime = times(&timeBuf);

   /* Place this thread on the sleep queue */
   runningTCB->wakeup = currentTime + sleepTime;
   /* Place tcb on sleep queue in ascending order of wakeup time */
   runningTCB->state = WAIT;

   /* tau should be recomputed */
   runningTCB->tau = talpha() * runningTCB->slices + (1 - talpha()) * runningTCB->tau;

   /* The "slices" value should be reset back to zero */
   runningTCB->slices = 0;

   runningTCB->priority = (int)(runningTCB->tau+0.5);

   /* checked everytime a thread's priority is changed */
   tsetrange(0, LEVELS-2);

   insert(sleepList, runningTCB, sleepOrder);

   /* Dispatch next thread on ready list */
   dispatch();
}

/** checkSleepers -- helper function **/
void checkSleepers() {
   struct tms timeBuf;
   clock_t currentTime;
   tcb_t *tcbPtr;
   tcb_t *tcbWakeup;

   /* Is anyone snoozing and need to be woken up? */
   tcbPtr = sleepList->head->next;
   currentTime = times(&timeBuf);
   while ((tcbPtr != sleepList->tail) && (tcbPtr->wakeup < currentTime)) {
      /* Dequeue thread from sleep list */
      tcbPtr = tcbPtr->next; // move past node we're about to dequeue
      tcbWakeup = dequeue(sleepList);
      if (tcbWakeup->state == SUSPENDWAIT) {
         /* Move thread to SUSPENDREADY state */
         tcbWakeup->state = SUSPENDREADY;
         queue(suspendReadyList, tcbWakeup);
      }
      else {
         /* Move thread to READY state */
         tcbWakeup->state = READY;
         /* Wake thread up (i.e. move to ready list) */
         queue(readyList[tcbWakeup->priority], tcbWakeup);
      }

      /* On to next on list */
   }
}

/** sleepOrder  -- used to determine where to place thread
                   on sleepList
**/
int sleepOrder(tcb_t *thread1, tcb_t *thread2) {
    if (thread1->wakeup < thread2->wakeup) {
       return(-1);
    }
    else
    if (thread1->wakeup == thread2->wakeup) {
       return(0);
    }
    else {
       return(1);
    }
}

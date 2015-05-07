#include "userthread.h"

/** Global Data **/
/* readyList array -- one for each priority level */
tcbList_t *readyList = NULL;       /* Ready list                 */
tcbList_t *suspendReadyList = NULL;/* Ready but suspended list   */
tcbList_t *liveList = NULL;        /* List of live tcb's         */

tcb_t *runningTCB;                 /* Currently running thread   */
tcb_t mainTCB;                     /* Starting threads's TCB     */
int   numThreads=0;                /* Number of active threads   */
struct sigaction vtAlrmAction;     /* SIGVTALRM action           */
int    vtSigNum;                   /* SIGVTALRM signal number    */
int    quantum = QUANTUM;          /* Thread time quantum        */
tcbList_t *zombieList = NULL;      /* Zombie list                */

/** timeslice **/
void timeslice(int status) {
   tyield();                // Switch to next thread on ready list
}

/** tid **/
tcb_t *tid() {
    /* Return current thread tid */
    return(runningTCB);
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
   queue(readyList, newTCB);

   enable();   // Re-enable interrupts

   /* Return new thread's ID */
   return(newTCB);
}

/** tyield **/
void tyield() {
   tcb_t *lastTCB;

   disable();   // disable interrupts

   /* Link current thread to end of ready list */
   queue(readyList, runningTCB);
   
   /* Change state of current thread */
   lastTCB = runningTCB;
   lastTCB->state = READY;

   /* Dispatch next thread */
   dispatch();  // Dispatch re-enables interrupts
}

/** texit **/
void texit() {

   disable();                          // Disable interrupts

   /** STUBBED -- current code just terminates everything **/
   printf("STUBBED VERSION OF texit(): SYSTEM TERMINATING\n");
   exit(0);
}

/** dispatch **/
void dispatch() {
   tcb_t *previousThread;

   disable();   // Disable interrupts

   /* Remember pointer to thread being switch off CPU */
   previousThread = runningTCB;

   /* Pick thread at head of ready list as first thread to dispatch */
   runningTCB = dequeue(readyList);
   assert(runningTCB != NULL);

   /* Switch to selected thread */
   doSwitch(runningTCB, previousThread);
}

/** startThreadSystem **/
void startThreadSystem() {
   runningTCB = &mainTCB;

   /* If create the ready and live lists */
   if (readyList == NULL) {
      readyList = createList();
   }
   if (liveList == NULL) {
      liveList = createList();
   }
   if (suspendReadyList == NULL) {
      suspendReadyList = createList();
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
    /** STUBBED -- CODE DOESN'T DO ANYTHING **/
    enable();
    return(1);
}

int tresume(tcb_t *tcb) {
    disable();
    /** STUBBED -- CODE DOESN'T DO ANYTHING **/
    enable();
    return(1);
}

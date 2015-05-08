/* userthread.h */

#ifndef USERTHREAD_H
#define USERTHREAD_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <assert.h>
#include <sys/times.h>
#include <limits.h>

#define STACKSIZE 4*4096

#define LEVELS      7        // Number ready lists

#define QUANTUM     1000     // Thread time quantum
#define TIMESLICE   5        // Number of quantums before pre-emption

#define FIXED         0
#define SRTF          1

/* Thread states */
#define RUNNING       1
#define READY         2
#define SUSPENDREADY  3
#define WAIT          4
#define SUSPENDWAIT   5
#define ZOMBIE       99

typedef struct tcbType {
   unsigned int *stackPtr;    // Save area for hardware stack pointer.
   unsigned int *stackBase;   // Start address of the hardware stack
   int  state;                // Thread's state

   /* New fields SRTF scheduling */
   float tau;                 // Thread's current TAU value
   int   priority;            // Thread's curent priority
   int   priorityMinVal;      // Thread's minimum numerical priority value
   int   priorityMaxVal;      // Thread's maximum numerical priority value
   int   slices;              // Number of quantums expired

   struct tcbType *waitingParent; // tcb of waiting parent, or NULL if none
   clock_t wakeup;            // Time at which to wakeup thread from snooze

   struct tcbType *next;      // Pointer to next thread on list
   struct tcbType *prev;      // Pointer to previous thread on list

   struct tcbType *livenext;  // Pointer to next live TCB
   struct tcbType *liveprev;  // Pointer to previous live TCB
} tcb_t;


/** Prototype statements **/
/* Thread management functions */

/* User thread API functions */
void  startThreadSystem();
tcb_t *newThread(void (*func)());
void  tyield();
void  texit();
int   twait(tcb_t *id);
tcb_t *tid();
int   tpriority();
void  tsetpriority(int priority);
void  tsetpolicy(int policy);
void  tsetrange(int high, int low);
float talpha();
void  tsetalpha(float alpha);
int   tactive();

int   tsuspend(tcb_t *tcb);
int   tresume(tcb_t *tcb);

void  tsnooze(int sleepTime);

void  tcleaner();

/* Internal thread library functions */
void  dispatch();
void  doSwitch(tcb_t *prevTCB, tcb_t *newTCB);

void  checkSleepers();
int   sleepOrder(tcb_t *thread1, tcb_t *thread2);

int   setalarm(int quantum);
void  enable();
void  disable();

#include "list.h"

#endif 

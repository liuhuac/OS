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

#define QUANTUM     1000     // Thread time quantum

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

   struct tcbType *waitingParent; // tcb of waiting parent, or NULL if none

   struct tcbType *next;      // Pointer to next thread on list
   struct tcbType *prev;      // Pointer to previous thread on list

   struct tcbType *livenext;  // Pointer to next live TCB
   struct tcbType *liveprev;  // Pointer to previous live TCB
} tcb_t;


/** Prototype statements **/
/* Thread management functions */
void  startThreadSystem();
tcb_t *newThread(void (*func)());
void  tyield();
void  texit();
int   twait(tcb_t *id);
tcb_t *tid();
int   tactive();

int   tsuspend(tcb_t *tcb);
int   tresume(tcb_t *tcb);

void  tcleaner();

void  dispatch();
void  doSwitch(tcb_t *prevTCB, tcb_t *newTCB);

void  checkSleepers();

int   setalarm(int quantum);
void  enable();
void  disable();

#include "list.h"

#endif 

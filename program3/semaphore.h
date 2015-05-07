#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "userthread.h"

typedef struct semaphore {
   int count;            /* Semaphore's count value      */
   int locallock;        /* Spin lock                    */
   tcbList_t *waitlist;  /* Wait list                    */
} semaphore_t;

/* Prototype statements */
void semInit(semaphore_t *semaphore, int inititalValue);
void semWait(semaphore_t *semaphore);
void semPost(semaphore_t *semaphore);

#endif


/** CPSC/ECE 322 Semaphore Routines
    semInit()
    semWait()
    semPost()
**/

#include "userthread.h"
#include "semaphore.h"
#include <assert.h>

/** Global Data **/
extern tcb_t *runningTCB;
extern tcbList_t *readyList[];

/** semInit **/
void semInit(semaphore_t *semaphore, int initialValue) {
   /** STUBBED **/
   semaphore->locallock = 0;
   semaphore->count = initialValue;
   if( semaphore->waitlist == NULL ) {
      semaphore->waitlist = createList();
   }
}

/** semWait **/
void semWait(semaphore_t *semaphore) {
   /** STUBBED **/
   runningTCB->inLock = 1;

   while (testandset(&(semaphore->locallock)) != 0) {
      tyield();
   }

   semaphore->count--;
   if( semaphore->count < 0 ) {
      queue(semaphore->waitlist, runningTCB); 
      semaphore->locallock = 0;
      runningTCB->inLock = 0;
      dispatch();
   } else {
      semaphore->locallock = 0;
      runningTCB->inLock = 0;
   }
}

/** semSignal **/
void semPost(semaphore_t *semaphore) {
   /** STUBBED **/
   runningTCB->inLock = 1;

   while (testandset(&(semaphore->locallock)) != 0) {
      tyield();
   }

   semaphore->count++;
   if( semaphore->count <= 0 ) {
      queue(readyList[0],dequeue(semaphore->waitlist)); 
   }

   semaphore->locallock = 0;

   runningTCB->inLock = 0;
}

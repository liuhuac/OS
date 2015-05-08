#include "userthread.h"

/** Global Data **/
extern int   numThreads;        /* Number of active threads   */
extern tcbList_t *zombieList;   /* Zombie list                */

/** tcleaner  thread code **/
void tcleaner() {
     tcb_t *zombie;

     /* Limit tcleaner to lowest priority level */
     tsetrange(LEVELS-1, LEVELS-1);

     /* Forever, look for zombie threads */
     while(1) {
        /* Cleanup zombie's on zombie list */
        while ((zombie = dequeue(zombieList)) != NULL) {
           fprintf(stdout, "tcleaner: cleaning up thread %p\n",
                   zombie);
           fflush(stdout);
           free(zombie->stackBase);
           free(zombie);
           numThreads--;
           printf("tcleaner: priority = %d\n", tpriority());
        }
        tyield();  /* Go on to next thread */
     }
}

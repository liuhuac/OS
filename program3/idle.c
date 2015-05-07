#include "userthread.h"

/** "idle"  thread code 
    This thread initiated in startThreadSystem() ensures that there will
    always be something on the readyList.  This eliminates a special
    case in the dispatcher of having to deal with an empty readyList.

    The thread runs at lowest priority so that it doesn't compete with
    useful threads.
**/
void idle() {
printf("\"idle\" thread starting tid=%p\n", tid()); fflush(stdout);
   tsetrange(LEVELS-1, LEVELS-1);
   /* "Do nothing" loop just so there is always something to dispatch */
   while (1) {
      tyield();
   }
}

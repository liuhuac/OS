#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <assert.h>

/** setalarm **/
int setalarm(int quantum) {
   struct itimerval newval;
   struct itimerval oldval;

   /* Set a timer interrupt at "quantum" time units in the future */
   newval.it_interval.tv_sec = 0;
   newval.it_interval.tv_usec = 0;
   newval.it_value.tv_sec = 0;
   newval.it_value.tv_usec = quantum;

   assert(setitimer(ITIMER_VIRTUAL, &newval, &oldval) == 0);

   return(oldval.it_value.tv_usec);
}

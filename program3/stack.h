#ifndef STACK_H
#define STACK_H
#include "semaphore.h"

typedef struct stackType {
   char **stackArray;
   int  top;
   int  size;

   /** Semaphores **/
   semaphore_t lock;        /* Critical section protection */
   semaphore_t full;        /* Data in stack               */
   semaphore_t empty;       /* Empty space in stack        */
} mystack_t;


/* Prototype statements */
mystack_t *newStack(int size);
char      *pop(mystack_t *stack);
int       push(mystack_t *stack, char *word);

#endif


/** CpSc 322 -- homework 4 sample solution

    stack.c

    Note that each stack has its own semaphore.  This allows
      each stack to be manipulated independently (i.e. different
      threads can be pushing or popping on different stacks at the
      same time).
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#include "stack.h"
#include "semaphore.h"

/** stack_init **/
mystack_t *newStack(int size) {
   mystack_t *stack;            /* New stack pointer                        */

   stack = malloc(sizeof(mystack_t));
   stack->stackArray = malloc(size * sizeof(char *));
   stack->top = 0;
   stack->size = size;
   semInit(&stack->lock,  1);    /* Semaphore to protect this stack's   */
                                 /* critical sections                   */
   semInit(&stack->full, 0);     /* Sempahore to indicate data in stack */
   semInit(&stack->empty, size); /* Semaphore to indicate space in stack */
   return(stack);
}

/** push **/
int push(mystack_t *stack, char *word) {

    semWait(&stack->empty);          /* Wait for space in stack */

    /* Essentially the whole routine is a critical section.
       We must prevent "top" from being changed by another thread
       during any of these operations.
    */
    semWait(&stack->lock);

    /* Add word pointer to stack (with "empty" semaphore no need to worry
       about top being greater than stack size) */
    stack->stackArray[stack->top] = word;
    stack->top++;
    assert(stack->top <= stack->size);

    semPost(&stack->full);          /* Signal data in stack  */
    semPost(&stack->lock);          /* And release lock      */
    return(0);
}

/** pop **/
char *pop(mystack_t *stack) {
    char *word;
 
    semWait(&stack->full);          /* Wait for data in stack  */

    /* Essentially the whole routine is a critical section.
       We must prevent "top" from being changed by another thread
       during any of these operations.
    */
    semWait(&stack->lock);

    /* Remove word pointer from stack (no need to worry about top less
       than zero because of "full" semaphore) */
    stack->top--;
    assert(stack->top >= 0);
    word = stack->stackArray[stack->top];

    semPost(&stack->empty);        /* Signal space in stack  */
    semPost(&stack->lock);         /* And release lock       */
 
    return(word);
}

#include "userthread.h"

extern tcbList_t *liveList;

/*** Linked list utilities

     This is an implementation of a doubly linked list of
     TCBs with a dummy head and tail node.
***/

/** restart **/
void restart(tcbList_t *list) {
   list->current = list->head->next;
}

/** getNext **/
tcb_t *getNext(tcbList_t *list) {
   tcb_t *tcb;
   if (list->current == list->tail) {
      return(NULL);
   }
   tcb = list->current;
   list->current = list->current->next;
   return(tcb);
}

/** removeTCB **/
void removeTCB(tcb_t *tcb) {
   tcb->prev->next = tcb->next;
   tcb->next->prev = tcb->prev;
}

/** isEmptyList **/
int isEmptyList(tcbList_t *list) {
   if (list->head->next == list->tail) {
      return(1);
   }
   return(0);
}

/** createList **/
tcbList_t *createList() {
   /* Create doubly linked list with dummy head/tail nodes */
   tcbList_t *newList = malloc(sizeof(tcbList_t));
   tcb_t *headNode = malloc(sizeof(tcb_t));
   tcb_t *tailNode = malloc(sizeof(tcb_t));

   /* Link dummy head and tail nodes into list, and to each other */
   headNode->next = tailNode;
   tailNode->prev = headNode;
   newList->head = headNode;
   newList->tail = tailNode;
   //headNode->tid = -98;
   //tailNode->tid = -99;

   /* extra links for liveList (singly linked ) */
   headNode->livenext = tailNode;
   tailNode->liveprev = headNode;

   return(newList);
}

/** queue **/
void queue(tcbList_t *list, tcb_t *thread) {
   tcb_t *currentLast;

   /* Add to end of list (before dummy tail node) */
   currentLast = list->tail->prev;
   currentLast->next = thread;
   thread->prev = currentLast;
   thread->next = list->tail;
   list->tail->prev = thread;
}

/** dequeue **/
tcb_t *dequeue(tcbList_t *list) {
   tcb_t *thread;

   /* Test for empty list */
   if (isEmptyList(list)) {
      /* List is empty (only dummy header/tail nodes) */
      return(NULL);
   }
   /* Dequeue from head of list */
   thread = list->head->next;
   thread->next->prev = list->head;
   list->head->next = thread->next;

   return(thread);
}

/** insert (add into list based on "order" criteria **/
void insert(tcbList_t *list, tcb_t *thread,
            int (*order)(tcb_t *thread, tcb_t *curr)) {
   tcb_t *curr;
   tcb_t *prev;

   /* Figure out where to insert this node */
   curr = list->head->next;
   while ((curr != list->tail) &&
          (order(thread, curr) >= 0)) {
          curr = curr->next;
   }

   /* Add to in front of node pointed to by curr */
   prev = curr->prev;
   thread->next = curr;
   thread->prev = prev;
   curr->prev = thread;
   prev->next = thread;
}

/****************************************************************
   liveList routines
****************************************************************/

/** addLive **/
void addLive(tcb_t *tcb) {
   /* Add to head of list */
   tcb_t *head = liveList->head;
   tcb->livenext = head->livenext;
   tcb->liveprev = head;
   head->livenext->liveprev = tcb;
   head->livenext = tcb;
}

/** removeLive **/
void removeLive(tcb_t *tcb) {
   tcb->liveprev->livenext = tcb->livenext;
   tcb->livenext->liveprev = tcb->liveprev;
}

/** findAlive **/
int findAlive(tcb_t *tcb) {
   tcb_t *curr = liveList->head->livenext;
   /* Search liveList for thread with specified tid */
   while (curr != NULL && curr != tcb) {
      curr = curr->livenext;
   }
   if (curr != NULL) {
      /* Found it */
      return(1);
   }
   else {
      /* Not on list */
      return(0);
   }
}

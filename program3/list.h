#ifndef LIST_H
#define LIST_H

typedef struct tcbListType {
   tcb_t *head;               // Head of ready list
   tcb_t *tail;               // Tail of ready list
   tcb_t *current;            // Position in list used by getNext()
} tcbList_t;


/* List function prototype statements */
tcbList_t *createList();
void      queue(tcbList_t *list, tcb_t *tcb);
tcb_t     *dequeue(tcbList_t *list);
int       isEmptyList(tcbList_t *list);

void      restart(tcbList_t *list);
tcb_t     *getNext(tcbList_t *list);
void      removeTCB(tcb_t *tcb);

void insert(tcbList_t *list, tcb_t *thread,
            int (*order)(tcb_t *thread, tcb_t *curr));

void dumpList(tcbList_t *list);

/** liveList routines */
void      addLive(tcb_t *tcb);
void      removeLive(tcb_t *tcb);
int       findAlive(tcb_t *tcb);

#endif

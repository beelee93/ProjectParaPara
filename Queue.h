//////////////////////////////////////////
// Queue.h
// Defines function prototypes to
// manipulate a queue of pointers
//////////////////////////////////////////
#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#include <stdlib.h>
#include <stddef.h>

typedef struct __queuenode {
    void* ptr;
    struct __queuenode* next;
} QueueNode;

typedef struct {
    QueueNode* head;
    QueueNode* tail;
    int size;
} PtrQueue;

void BL_QueueCreate(PtrQueue* queue);
void BL_QueueEnqueue(PtrQueue* queue, void* ptr);
void* BL_QueueDequeue(PtrQueue* queue);
int BL_QueueIsEmpty(PtrQueue* queue);

#endif // QUEUE_H_INCLUDED

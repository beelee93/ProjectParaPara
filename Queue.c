//////////////////////////////////////////
// Queue.c
// Implements a queue of pointers to be used
// by Game Object Manager
//////////////////////////////////////////
#include "Queue.h"

void BL_QueueCreate(PtrQueue* queue)
{
    if(!queue) return;
    queue->head = queue->tail = NULL;
    queue->size = 0;
}

void BL_QueueEnqueue(PtrQueue* queue, void* ptr)
{
    QueueNode* temp;

    if(!queue) return;
    if(!queue->head)
    {
        // first element in queue
        temp = queue->head = (QueueNode*)malloc(sizeof(QueueNode));
        temp->ptr = ptr;
        temp->next = NULL;

        queue->tail = temp;
        queue->size=1;
    }
    else
    {
        // we enqueue it at the tail's end
        temp = queue->tail->next = (QueueNode*)malloc(sizeof(QueueNode));
        temp->next = NULL;
        temp->ptr = ptr;

        // we set the new tail
        queue->tail = temp;
        queue->size++;
    }
}

void* BL_QueueDequeue(PtrQueue* queue)
{
    QueueNode* temp;
    void* resPtr;

    if(!queue) return NULL;
    if(queue->size==0) return NULL;

    // we get the head
    temp = queue->head;
    resPtr = temp->ptr;

    // move the head pointer down
    queue->head = temp->next;
    queue->size--;

    // free the old head
    free(temp);
    return resPtr;
}

int BL_QueueIsEmpty(PtrQueue *queue)
{
    return (!queue || queue->size==0);
}

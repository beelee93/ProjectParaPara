//////////////////////////////////////////
// RenderQueue.h
// Definitions for a sorted render queue to be
// used by the GOM
//////////////////////////////////////////
#ifndef RENDERQUEUE_H_INCLUDED
#define RENDERQUEUE_H_INCLUDED

#include "GameObject.h"
#include <stdlib.h>

typedef struct __queuenode
{
    BL_GameObject* item;
    struct __queuenode* next;
} QueueNode;

// As more nodes are added, capacity would increase
// appropriately. Dequeueing the queue would send
// the item over to the inactive queue, ready to be
// recycled, to reduce memallocs.
#define ACTIVE 0
#define INACTIVE 1
class BL_RenderQueue
{
public:
    BL_RenderQueue();
    ~BL_RenderQueue();

    // removes all alloc-ed nodes from memory
    void Purge();

    // Dequeues the active queue for rendering
    void Render(double secs, SDL_Renderer* renderer);

    // Queues an object for rendering based on its depth
    void QueueObject(BL_GameObject* obj);

protected:
    void Enqueue(QueueNode** head);
    QueueNode* Dequeue(QueueNode** head);
    QueueNode* CreateNode();

private:
    QueueNode* queueHeads[2];
    QueueNode* activeQueueTail;
};

#endif // RENDERQUEUE_H_INCLUDED

//////////////////////////////////////////
// RenderQueue.cpp
// Implements a sorted render queue
//////////////////////////////////////////

#include "RenderQueue.h"

BL_RenderQueue::BL_RenderQueue()
{
    queueHeads[ACTIVE] = NULL;
    queueHeads[INACTIVE] = NULL;

    activeQueueTail = NULL;

    // let's allocate some to inactive queue
    int i;
    QueueNode* node = NULL;
    node = queueHeads[INACTIVE] = CreateNode();
    for(i=1;i<10;i++)
    {
        node->next = CreateNode();
        node = node->next;
    }
}

QueueNode* BL_RenderQueue::CreateNode()
{
    QueueNode* node = (QueueNode*)malloc(sizeof(QueueNode));
    node->next = NULL;
    node->item = NULL;
	return node;
}

void BL_RenderQueue::QueueObject(BL_GameObject* obj)
{
    if(!obj) return; // do not accept NULL objects

    // check if head of inactive is NULL
    QueueNode* node = NULL;
    if(!queueHeads[INACTIVE]) // no more available nodes
    {
        // we create a new node
        node = CreateNode();
    }
    else
    {
        // we recycle inactive nodes from its head node
        node = queueHeads[INACTIVE];
        node->next = NULL;

        // move the queue head down the line
        queueHeads[INACTIVE] = node->next;
    }
    node->item = obj;

    if(!queueHeads[ACTIVE]) // empty queue
    {
        queueHeads[ACTIVE] = node;
        activeQueueTail = node;
    }
    else
    {
        // non-empty. we search for appropriate position
        // -descending order
        if(obj->depth<=activeQueueTail->item->depth) // lowest possible -> put at tail
        {
            activeQueueTail->next = node;
            activeQueueTail = node;
            node->next = NULL;
        }
        else if(obj->depth >= queueHeads[ACTIVE]->item->depth)
        {
            // highest - we put at head
            node->next = queueHeads[ACTIVE];
            queueHeads[ACTIVE] = node;
        }
        else
        {
            QueueNode* pre = queueHeads[ACTIVE];
            while(pre)
            {
                if(obj->depth < pre->item->depth)
                {
                    // we place it after pre
                    node->next = pre->next;
                    pre->next = node;
                    break;
                }
                pre = pre->next;
            }
        }
    }
}

void BL_RenderQueue::Render(double secs)
{
    if(!queueHeads[ACTIVE]) return; // nothing to render
    QueueNode* node = queueHeads[ACTIVE];

    // traverse down and render
    while(node)
    {
        node->item->OnRender(secs);

        // dequeue
        queueHeads[ACTIVE] = node->next;

        // move this node over to inactive queue
        node->item = NULL;
        node->next = queueHeads[INACTIVE];
        queueHeads[INACTIVE] = node;

        // next node
        node = queueHeads[ACTIVE];
    }
    activeQueueTail = NULL;
}

void BL_RenderQueue::Purge()
{
    QueueNode* node = queueHeads[INACTIVE];
    QueueNode* temp;
    while(node)
    {
        temp = node->next;
        free(node);
        node = temp;
    }

    node = queueHeads[ACTIVE];
    while(node)
    {
        temp = node->next;
        free(node);
        node = temp;
    }

    queueHeads[INACTIVE] = queueHeads[ACTIVE] = NULL;
}

BL_RenderQueue::~BL_RenderQueue()
{
    Purge();
}

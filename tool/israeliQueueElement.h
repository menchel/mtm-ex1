#ifndef ISRAELI_QUEUE_ELEMENT_H
#define ISRAELI_QUEUE_ELEMENT_H

typedef struct IsraeliQueueElement
{
    void* m_item;
    int m_numOfFriends;
    int m_numOfRivals;
}IsraeliQueueElement;
//creates new queue item
IsraeliQueueElement* createIsraeliQueueElement(void*item);

//destroy the queueItem
void destroyIsraeliQueueElement(IsraeliQueueElement* queueElement);

#endif


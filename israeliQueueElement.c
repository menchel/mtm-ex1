#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "israeliQueueElement.h"
IsraeliQueueElement* createIsraeliQueueElement(void*item)
{
    IsraeliQueueElement* newElement=(IsraeliQueueElement*)malloc(sizeof(IsraeliQueueElement));
    if(newElement==NULL)
    {
        return NULL;
    }
    newElement->m_item=item;
    newElement->m_numOfFriends=0;
    newElement->m_numOfRivals=0;
    return newElement;
}

void destroyIsraeliQueueElement(IsraeliQueueElement* queueElement)
{
    if(queueElement==NULL)
    {
        return;
    }
    free(queueElement);
}


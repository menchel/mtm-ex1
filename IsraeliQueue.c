#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "IsraeliQueue.h"
#include "node.h"
#include "israeliQueueElement.h"
#include "IsraeliQueueHelper.h"
struct IsraeliQueue_t
{
   FriendshipFunction * m_friendshipFunctionArray;
   int m_friendshipFunctionArraySize;
   ComparisonFunction m_comparisonFunc;
   int m_friendship_th;
   int m_rivalry_th;
   Node* m_first;
   Node* m_last;
};

//The function will move the elements of the array from to to from
static void moveElemetsToBack(IsraeliQueueElement** elementsArray,bool* elementsUsed,int from,int to);
//The function will move the first element that was not moved to the furthest position he can get
//The function will also mark him as used in the elementUsed array
static void moveFirstNonUsedElement(IsraeliQueue queue,IsraeliQueueElement** elementsArray,bool* elementUsed,int size);

//The function will create a new functions array according to the array of israeli queues
static void createNewFriendshipFunctionsArray(IsraeliQueue* queues, FriendshipFunction * newArray);

//The function will calculate the new friendship and rivalry thershold for the queue
static void calculateFriendshipAndRivalry(IsraeliQueue* queues,int*friendShip,int*rivalry);

//The function will move the elements of the array from to to from
static void moveElements(IsraeliQueueElement** elementsArray,bool* elementsUsed,int from,int to);

//The function will get two items and check if they are considered friends
static bool checkIfFriends(IsraeliQueue queue,void* item1,void*item2);

//The function will get two items and check if they are considered enemies
static bool checkIfEnemies(IsraeliQueue queue,void* item1,void*item2);

//The function will calculate the length of all the function arrays
//in the given array of queues
static int getSizeOfAllFunctionsArrays(IsraeliQueue* queues);

static void moveElemetsToBack(IsraeliQueueElement** elementsArray,bool* elementsUsed,int from,int to)
{
    for(int i=from-1;i>=to;i--)
    {
        elementsArray[i+1]=elementsArray[i];
        elementsUsed[i+1]=elementsUsed[i];
    }
}

static bool checkIfFriends(IsraeliQueue queue,void* item1,void*item2)
{
    for(int i=0;i<queue->m_friendshipFunctionArraySize;i++)
    {
        if(queue->m_friendshipFunctionArray[i](item1,item2)>queue->m_friendship_th)
        {
            return true;
        }
    }
    return false;
}

static bool checkIfEnemies(IsraeliQueue queue,void* item1,void*item2)
{
    if(checkIfFriends(queue,item1,item2))
    {
        return false;
    }
    int sum=0;
    for(int i=0;i<queue->m_friendshipFunctionArraySize;i++)
    {
        sum+=queue->m_friendshipFunctionArray[i](item1,item2);
    }
    sum=((double)sum)/queue->m_friendshipFunctionArraySize;
    return sum<queue->m_rivalry_th;
}

static int getSizeOfAllFunctionsArrays(IsraeliQueue* queues)
{
    int index=0;
    int size=0;
    while(queues[index]!=NULL)
    {
        size+=queues[index]->m_friendshipFunctionArraySize;
        index++;
    }
    return index;
}

static void moveElements(IsraeliQueueElement** elementsArray,bool* elementsUsed,int from,int to)
{
    for(int i=from;i<to;i++)
    {
        elementsArray[i]=elementsArray[i+1];
        elementsUsed[i]=elementsUsed[i+1];
    }
}

static void moveFirstNonUsedElement(IsraeliQueue queue,IsraeliQueueElement** elementsArray,bool* elementUsed,int size)
{
    int firstNonUsed=size-1;//find first non used
    for(int i=0;i<size-1;i++)
    {
        if(!elementUsed[i])
        {
            firstNonUsed=i;
            break;
        }
    }
    bool changed=false;
    for(int j=size-1;j>=0;j--)
    {
        if(j==firstNonUsed)
            continue;
        if(elementsArray[j]->m_numOfFriends<FRIEND_QUOTA && checkIfFriends(queue,elementsArray[j]->m_item,elementsArray[firstNonUsed]->m_item))//friends
        {
            bool enemyFound=false;
            for(int enemy=j-1;enemy>firstNonUsed;enemy--)
            {
                if(elementsArray[enemy]->m_numOfRivals<RIVAL_QUOTA && checkIfEnemies(queue,elementsArray[enemy]->m_item,elementsArray[firstNonUsed]->m_item))
                {
                    elementsArray[enemy]->m_numOfRivals++;
                    enemyFound=true;
                    j=enemy;//because he blocked the others
                    break;
                }
            }
            if(!enemyFound)//need to pass
            {
                IsraeliQueueElement* elementToChange=elementsArray[firstNonUsed];
                if(firstNonUsed<j-1)//move further
                {
                    moveElements(elementsArray,elementUsed,firstNonUsed,j-1);
                    elementsArray[j-1]=elementToChange;
                    elementUsed[j-1]=true;
                    elementsArray[j]->m_numOfFriends++;
                }
                else//move to before
                {
                    elementsArray[j]->m_numOfFriends++;
                    moveElemetsToBack(elementsArray,elementUsed,firstNonUsed,j);
                    elementsArray[j]=elementToChange;
                    elementUsed[j]=true;
                }
                changed=true;
                break;
            }
        }
    }
    if(!changed)//move to end
    {
        IsraeliQueueElement* elementToChange=elementsArray[firstNonUsed];
        moveElemetsToBack(elementsArray,elementUsed,size-1,0);
        elementsArray[0]=elementToChange;
        elementUsed[0]=true;
    }
}
static void calculateFriendshipAndRivalry(IsraeliQueue* queues,int*friendShip,int*rivalry)
{
    int rivalryMulti=1;
    int friendshipSum=0;
    int count=0;
    int index=0;
    while(queues[index]!=NULL)
    {
        rivalryMulti*=queues[index]->m_rivalry_th;
        friendshipSum+=queues[index]->m_friendship_th;
        count++;
        index++;
    }
    if(count==0)
    {
        return;
    }
    double power=((double)1)/count;
    *rivalry=ceil(pow(rivalryMulti,power));
    *friendShip=ceil(((double)friendshipSum)/count);
}

static void createNewFriendshipFunctionsArray(IsraeliQueue* queues, FriendshipFunction * newArray)
{
    int indexForQueues=0;
    int innerIndex=0;
    int arrayIndex=0;
    while(queues[indexForQueues]!=NULL)
    {
        while(queues[indexForQueues]->m_friendshipFunctionArray[innerIndex]!=NULL)
        {
            newArray[arrayIndex]=queues[indexForQueues]->m_friendshipFunctionArray[innerIndex];
            arrayIndex++;
            innerIndex++;
        }
        indexForQueues++;
        innerIndex=0;
    }
    newArray[arrayIndex]=NULL;
}
IsraeliQueue IsraeliQueueCreate(FriendshipFunction * friendshipFunc, ComparisonFunction comparisonFunc, int friendship_th, int rivalry_th)
{
    if(friendshipFunc==NULL || comparisonFunc==NULL)
    {
        return NULL;
    }
    IsraeliQueue newIsraeliQueue=(IsraeliQueue)malloc(sizeof(struct IsraeliQueue_t));
    if(newIsraeliQueue==NULL)
    {
        return NULL;
    }

    int friendshipArrayLength=0;
    while(friendshipFunc[friendshipArrayLength]!=NULL)
    {
        friendshipArrayLength++;
    }
    newIsraeliQueue->m_friendshipFunctionArraySize=friendshipArrayLength;

    newIsraeliQueue->m_friendshipFunctionArray=(FriendshipFunction*)malloc(sizeof(FriendshipFunction)*(friendshipArrayLength+1));
    if(newIsraeliQueue->m_friendshipFunctionArray==NULL)
    {
        return NULL;
    }
    int index=0;
    while(friendshipFunc[index]!=NULL)
    {
        newIsraeliQueue->m_friendshipFunctionArray[index]=friendshipFunc[index];
        index++;
    }
    newIsraeliQueue->m_friendshipFunctionArray[index]=NULL;
    newIsraeliQueue->m_comparisonFunc=comparisonFunc;
    newIsraeliQueue->m_friendship_th=friendship_th;
    newIsraeliQueue->m_rivalry_th=rivalry_th;
    newIsraeliQueue->m_first=NULL;
    newIsraeliQueue->m_last=NULL;
    return newIsraeliQueue;
}
IsraeliQueue IsraeliQueueClone(IsraeliQueue q)
{
    //not confident
    if(q==NULL)
    {
        return NULL;
    }
    IsraeliQueue newIsraeliQueue=IsraeliQueueCreate(q->m_friendshipFunctionArray,q->m_comparisonFunc,q->m_friendship_th,q->m_rivalry_th);
    if(newIsraeliQueue==NULL)
    {
        return NULL;
    }
    Node* elementsToCopy=q->m_first;
    IsraeliQueueElement*temp=NULL;
    if(q->m_first==NULL)//empty
    {
        return newIsraeliQueue;
    }
    temp=createIsraeliQueueElement(((IsraeliQueueElement*)elementsToCopy->m_element)->m_item);
    if(temp==NULL)
    {
        IsraeliQueueDestroy(newIsraeliQueue);
        return NULL;
    }
    newIsraeliQueue->m_first=createNode(temp);
    if(newIsraeliQueue->m_first==NULL)
    {
        IsraeliQueueDestroy(newIsraeliQueue);
        return NULL;
    }
    Node*current=newIsraeliQueue->m_first;
    elementsToCopy=elementsToCopy->m_next;
    while(elementsToCopy!=NULL)
    {
        temp=createIsraeliQueueElement(((IsraeliQueueElement*)elementsToCopy->m_element)->m_item);
        if(temp==NULL)
        {
           IsraeliQueueDestroy(newIsraeliQueue);
           return NULL;
        }
        temp->m_numOfFriends=((IsraeliQueueElement*)elementsToCopy->m_element)->m_numOfFriends;
        temp->m_numOfRivals=((IsraeliQueueElement*)elementsToCopy->m_element)->m_numOfRivals;
        current->m_next=createNode(temp);
        if(current==NULL)
        {
            IsraeliQueueDestroy(newIsraeliQueue);
            return NULL;
        }
        newIsraeliQueue->m_last=current;
        current=current->m_next;
        elementsToCopy=elementsToCopy->m_next;
    }
    return newIsraeliQueue;
}
IsraeliQueueError IsraeliQueueEnqueue(IsraeliQueue queue, void *item)
{
    if(queue==NULL)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    if(queue->m_first==NULL)
    {
        queue->m_first=createNode(createIsraeliQueueElement(item));
        queue->m_last=queue->m_first;
        return ISRAELIQUEUE_SUCCESS;
    }
    Node* current=queue->m_first;
    Node* currentFindRivals=NULL;
    bool rivalFound=false;
    while(current!=NULL)
    {
        if(((IsraeliQueueElement*)current->m_element)->m_numOfFriends<FRIEND_QUOTA && checkIfFriends(queue,item,((IsraeliQueueElement*)current->m_element)->m_item))//friends
        {
            currentFindRivals=current->m_next;
            while(currentFindRivals!=NULL)
            {
                if(((IsraeliQueueElement*)currentFindRivals->m_element)->m_numOfRivals<RIVAL_QUOTA && checkIfEnemies(queue,item,((IsraeliQueueElement*)currentFindRivals->m_element)->m_item))//enemies
                {
                    rivalFound=true;
                    ((IsraeliQueueElement*)currentFindRivals->m_element)->m_numOfRivals++;
                    break;
                }
                currentFindRivals=currentFindRivals->m_next;
            }
            if(!rivalFound)//can put here
            {
                Node*temp=createNode(createIsraeliQueueElement(item));
                temp->m_next=current->m_next;
                current->m_next=temp;
                if(temp->m_next==NULL)
                {
                    queue->m_last=temp;
                }
                ((IsraeliQueueElement*)current->m_element)->m_numOfFriends++;
                return ISRAELIQUEUE_SUCCESS;
            }
            rivalFound=false;
            current=currentFindRivals->m_next;
        }
        else
            current=current->m_next;
    }
    //put in last
    queue->m_last->m_next=createNode(createIsraeliQueueElement(item));
    queue->m_last=queue->m_last->m_next;
    return ISRAELIQUEUE_SUCCESS;
}
int IsraeliQueueSize(IsraeliQueue israeliQueue)
{
    if(israeliQueue==NULL)
    {
        return 0;
    }
    int count=0;
    Node* current=israeliQueue->m_first;
    while(current!=NULL)
    {
        count++;
        current=current->m_next;
    }
    return count;
}
IsraeliQueueError IsraeliQueueAddFriendshipMeasure(IsraeliQueue israQueue, FriendshipFunction friendshipFunc)
{
    //not confident at all
    if(israQueue==NULL || friendshipFunc==NULL)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    FriendshipFunction*newFriendShipArrray=(FriendshipFunction*)malloc(sizeof(FriendshipFunction)*(israQueue->m_friendshipFunctionArraySize+2));
    if(newFriendShipArrray==NULL)
    {
        return ISRAELIQUEUE_ALLOC_FAILED;
    }
    int index=0;
    while(israQueue->m_friendshipFunctionArray[index]!=NULL)
    {
        newFriendShipArrray[index]=israQueue->m_friendshipFunctionArray[index];
        index++;
    }
    newFriendShipArrray[index]=friendshipFunc;
    newFriendShipArrray[index+1]=NULL;

    free(israQueue->m_friendshipFunctionArray);//we did the malloc

    israQueue->m_friendshipFunctionArray=newFriendShipArrray;
    israQueue->m_friendshipFunctionArraySize++;
    return ISRAELIQUEUE_SUCCESS;
}
void IsraeliQueueDestroy(IsraeliQueue queueToDestroy)
{
    if(queueToDestroy==NULL)
    {
        return;
    }
    Node*current=queueToDestroy->m_first;
    while(current!=NULL)
    {
        destroyIsraeliQueueElement(current->m_element);
        current=current->m_next;
    }
    destroyNodeList(queueToDestroy->m_first);
    free(queueToDestroy->m_friendshipFunctionArray);
    free(queueToDestroy);
}
void* IsraeliQueueDequeue(IsraeliQueue queueToDequeue)
{
    if(queueToDequeue==NULL)
    {
        return NULL;
    }
    if(!(queueToDequeue->m_first))
    {
        return NULL;
    }
    void* result = ((IsraeliQueueElement*)queueToDequeue->m_first->m_element)->m_item;
    destroyIsraeliQueueElement(queueToDequeue->m_first->m_element);
    Node*temp=queueToDequeue->m_first;
    queueToDequeue->m_first = queueToDequeue->m_first->m_next;
    free(temp);
    if(queueToDequeue->m_first == NULL)
    {
        queueToDequeue->m_last = NULL;
    }
    return result;
}
IsraeliQueueError IsraeliQueueUpdateRivalryThreshold(IsraeliQueue queueToUpdate, int newRivalryThreshold)
{
    if(queueToUpdate==NULL)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    queueToUpdate->m_rivalry_th = newRivalryThreshold;
    return ISRAELIQUEUE_SUCCESS;
}
IsraeliQueueError IsraeliQueueUpdateFriendshipThreshold(IsraeliQueue queueToUpdate, int newFriendShipThreshold)
{
    if(queueToUpdate==NULL)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    queueToUpdate->m_friendship_th=newFriendShipThreshold;
    return ISRAELIQUEUE_SUCCESS;
}

bool IsraeliQueueContains(IsraeliQueue queue, void* object)
{
    if(queue==NULL)
    {
        return false;
    }
    Node* curr = queue->m_first;
    while(curr)
    {
        if(queue->m_comparisonFunc(((IsraeliQueueElement*)curr->m_element)->m_item, object))
        {
            return true;
        }
        curr = curr->m_next;
    }
    return false;
}
IsraeliQueueError IsraeliQueueImprovePositions(IsraeliQueue queue)
{
    if(queue==NULL)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    int size=IsraeliQueueSize(queue);
    IsraeliQueueElement** elementsArray=(IsraeliQueueElement**)malloc(sizeof(IsraeliQueueElement*)*size);
    if(elementsArray==NULL)
    {
        return ISRAELIQUEUE_ALLOC_FAILED;
    }
    bool* elementUsed=(bool*)malloc(sizeof(bool)*size);
    if(elementUsed==NULL)
    {
        return ISRAELIQUEUE_ALLOC_FAILED;
    }
    Node*temp=queue->m_first;
    for(int i=size-1;i>=0;i--)
    {
        elementsArray[i]=((IsraeliQueueElement*)temp->m_element);
        elementUsed[i]=false;
        temp=temp->m_next;
    }
    for(int times=0;times<size;times++)//will happen times times
    {
        moveFirstNonUsedElement(queue,elementsArray,elementUsed,size);
    }
    temp=queue->m_first;
    for(int i=size-1;i>=0;i--)
    {
        temp->m_element=elementsArray[i];
        temp=temp->m_next;
    }
    free(elementsArray);
    free(elementUsed);
    return ISRAELIQUEUE_SUCCESS;
}
IsraeliQueue IsraeliQueueMerge(IsraeliQueue* queues,ComparisonFunction comapreFunc)
{
    //the function is good if no need to save the queues
    if(queues==NULL || comapreFunc==NULL)
    {
        return NULL;
    }
    int rivalry_th=0;
    int friendship_th=0;
    calculateFriendshipAndRivalry(queues,&friendship_th,&rivalry_th);
    //insert friendship functions
    int sizeOfFunctionArray=getSizeOfAllFunctionsArrays(queues)+1;//with null at end
    FriendshipFunction* temp=(FriendshipFunction*)malloc(sizeof(FriendshipFunction)*sizeOfFunctionArray);
    if(temp==NULL)
    {
        return NULL;
    }
    createNewFriendshipFunctionsArray(queues,temp);
    IsraeliQueue newQueue=IsraeliQueueCreate(temp,comapreFunc,friendship_th,rivalry_th);
    if(newQueue==NULL)
    {
        return NULL;
    }
    //insert to new queue
    while(true)
    {
        int countExistingQueues=0;
        int index=0;
        while(queues[index]!=NULL)
        {
            if(queues[index]->m_first!=NULL)
            {
                IsraeliQueueEnqueue(newQueue,IsraeliQueueDequeue(queues[index]));
                countExistingQueues++;
            }
            index++;
        }
        if(countExistingQueues==0)
        {
            break;
        }
        index=0;
    }
    return newQueue;
}

IsraeliQueueError enqueueRegular(IsraeliQueue queue,void* item)
{
     if(queue==NULL || item==NULL)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    if(queue->m_first==NULL)
    {
        queue->m_first=createNode(createIsraeliQueueElement(item));
        queue->m_last=queue->m_first;
        return ISRAELIQUEUE_SUCCESS;
    }
    //put in last
    queue->m_last->m_next=createNode(createIsraeliQueueElement(item));
    queue->m_last=queue->m_last->m_next;
    return ISRAELIQUEUE_SUCCESS;
}

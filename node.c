#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "node.h"

Node* createNode(void* element)
{
    Node* newNode=(Node*)malloc(sizeof(Node));
    if(newNode==NULL)
    {
        return NULL;
    }
    newNode->m_element=element;
    newNode->m_next=NULL;
    return newNode;
}

void destroyNodeList(Node* node)
{
    Node*currNode=node;
    Node*nextPointer=NULL;
    while(currNode!=NULL)
    {
        nextPointer=currNode->m_next;
        free(currNode);
        currNode=nextPointer;
    }
}
Node* addToList(Node*nodeList,Node*nodeToAdd)
{
    if(nodeList==NULL)
    {
        return nodeToAdd;
    }
    nodeToAdd->m_next=nodeList;
    return nodeToAdd;
}
void addNodeAfter(Node*placeToAdd,Node*nodeToAdd)
{
    nodeToAdd->m_next=placeToAdd->m_next;
    placeToAdd->m_next=nodeToAdd;
}
Node* addToEndOfList(Node*nodeList,Node*nodeToAdd)
{
    if(nodeList==NULL)
    {
        return nodeToAdd;
    }
    Node*temp=nodeList;
    while(temp->m_next!=NULL)
    {
        temp=temp->m_next;
    }
    temp->m_next=nodeToAdd;
    return nodeList;
}

int getListLength(Node*nodeList)
{
    int count=0;
    while(nodeList!=NULL)
    {
        nodeList=nodeList->m_next;
        count++;
    }
    return count;
}



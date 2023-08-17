#ifndef NODE_H
#define NODE_H
typedef struct Node{
    void* m_element;
    struct Node* m_next;
}Node;

//Creates a new Node
Node* createNode(void* item);

//Destroies a list
void destroyNodeList(Node* node);

//Adds a given node to the list
Node* addToList(Node*nodeList,Node*nodeToAdd);

//Add a gicen node after a given node
void addNodeAfter(Node*placeToAdd,Node*nodeToAdd);

//Add node to the end of the list
Node* addToEndOfList(Node*nodeList,Node*nodeToAdd);

//Gets the length of the list
int getListLength(Node*nodeList);

#endif // NODE_H


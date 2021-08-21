//LinkedList.h
#ifndef _LINKEDLIST_H
#define _LINKEDLIST_H
#include<stddef.h>//size_t
typedef signed long Long;
typedef struct _node Node;
typedef struct _node {
	Node* previous;
	//void* object;
	Node* next;
}Node;
typedef struct _linkedList {
	Node* head;
	Node* tail;
	Long length;
	Node* current;
}LinkedList;

void LinkedList_Create(LinkedList* linkedList);
Node* LinkedList_InsertBefore(LinkedList* linkedList, Node* index, void* object, size_t size);
Node* LinkedList_InsertAfter(LinkedList* linkedList, Node* index, void* object, size_t size);
Node* LinkedList_AppendFromHead(LinkedList* linkedList, void* object, size_t size);
Node* LinkedList_AppendFromTail(LinkedList* linkedList, void* object, size_t size);
Node* LinkedList_Delete(LinkedList* linkedList, Node* index);
Node* LinkedList_DeleteFromHead(LinkedList* linkedList);
Node* LinkedList_DeleteFromTail(LinkedList* linkedList);
void LinkedList_DeleteAllItems(LinkedList* linkedList);
Node* LinkedList_LinearSearchUnique(LinkedList* linkedList, void* key, int(*compare)(void*, void*));
void LinkedList_LinearSearchDuplicate(LinkedList* linkedList, void* key, Node** (*indexes), Long *count,
	int(*compare)(void*, void*));
Node* LinkedList_First(LinkedList* linkedList);
Node* LinkedList_Previous(LinkedList* linkedList);
Node* LinkedList_Next(LinkedList* linkedList);
Node* LinkedList_Last(LinkedList* linkedList);
void LinkedList_GetAt(LinkedList* linkedList, Node* index, void* object, size_t size);
void LinkedList_Destroy(LinkedList* linkedList);

#endif//_LINKEDLIST_H

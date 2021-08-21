//IndexBinder.h
#ifndef _INDEXBINDER_H
#define _INDEXBINDER_H
#include"BinaryTree.h"

typedef struct _index Index;
typedef struct _businessCard BusinessCard;

typedef struct _indexBinder {
	BinaryTree indexes;
	Long length;
}IndexBinder;

void IndexBinder_Create(IndexBinder* indexBinder);
Index* IndexBinder_Put(IndexBinder* indexBinder, BusinessCard* index);
Index* IndexBinder_Find(IndexBinder* indexBinder, char(*companyName));
Index* IndexBinder_PutOut(IndexBinder* indexBinder, char(*companyName), BusinessCard* index);
void IndexBinder_Arrange(IndexBinder* indexBinder);
void MakeList(IndexBinder* indexBinder, Index* (*indexes), Long* count);
void IndexBinder_Destroy(IndexBinder* indexBinder);
int CompareCompanyNames(void* one, void* other);
void Node_Arrange(BinaryNode* node);

#endif//_INDEXBINDER_H

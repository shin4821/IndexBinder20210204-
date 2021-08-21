//Index.h
#ifndef _INDEX_H
#define _INDEX_H
#include"array.h"
#include"BusinessCardBinder.h"
typedef struct _index {
	char companyName[32];
	Array businessCards;
	Long capacity;
	Long length;
}Index;

void Index_Create(Index* index, Long capacity);
Long Index_Put(Index* index, BusinessCard* key);
Long Index_Find(Index* index, BusinessCard* key);
Long Index_PutOut(Index* index, Long index_);
void Index_Arrange(Index* index);
void Index_Destroy(Index* index);
int CompareBusinessCards(void* one, void* other);
int CompareBusinessCardNames(void* one, void* other);
int CompareArray(void* one, void* other);
BusinessCard* Index_GetAt(Index* index, Long index_);

#endif//INDEX_H






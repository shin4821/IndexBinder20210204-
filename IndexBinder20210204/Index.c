//Index.c
#include"Index.h"
#include<stdlib.h>
#include<string.h>

void Index_Create(Index* index, Long capacity) {
	Create(&index->businessCards, capacity, sizeof(Long));
	index->capacity = capacity;
	index->length = 0;
}

Long Index_Put(Index* index, BusinessCard* key) {
	Long index_;//첨자

	//1. key(BusinessCard*)를 입력받는다.
	//2. capacity가 length보다 큰 경우, store을 호출한다.
	if (index->capacity > index->length) {
		index_ = index->length;
		Store(&index->businessCards, index_, &key, sizeof(BusinessCard*));
	}
	//3. capacity가 length보다 작거나 같은 경우, AppendForRear를 호출한다.
	else {
		AppendForRear(&index->businessCards, &key, sizeof(BusinessCard*));
		index->capacity++;
	}
	index->length++;
	//4. 첨자를 출력한다.
	return index_;
	//5. 끝내다.
}

Long Index_Find(Index* index, BusinessCard* key) {
	Long i;

	//1. key를 입력받는다.
	//2. LinearSearchUnique를 호출한다.
	i = LinearSearchUnique(&index->businessCards, key, sizeof(BusinessCard*), CompareBusinessCards);
	//3. i를 출력한다.
	return i;
	//4. 끝내다.
}

Long Index_PutOut(Index* index, Long index_) {
	//1. 첨자를 입력받는다.
	//2. Delete를 호출한다.
	Delete(&index->businessCards, index_, sizeof(BusinessCard*));
	index->capacity--;
	index->length--;
	//3. 끝내다.
}

void Index_Arrange(Index* index) {
	SelectionSort(&index->businessCards, sizeof(BusinessCard*), CompareArray);
}

void Index_Destroy(Index* index) {
	Destroy(&index->businessCards);
}

int CompareBusinessCards(void* one, void* other) {
	BusinessCard**one_ = (BusinessCard**)one;
	BusinessCard* other_ = (BusinessCard*)other;
	int ret;

	if (*one_ == other_) {
		ret = 0;
	}
	else {
		ret = -1;
	}
	return ret;
}

int CompareBusinessCardNames(void* one, void* other) {
	BusinessCard** one_ = (BusinessCard**)one;
	BusinessCard* other_ = (BusinessCard*)other;
	int ret;

	ret = strcmp((*one_)->personal.name, other_->personal.name);
	return ret;
}

int CompareArray(void* one, void* other) {
	BusinessCard** one_ = (BusinessCard**)one;
	BusinessCard** other_ = (BusinessCard**)other;
	int ret;

	ret = strcmp((*one_)->personal.name, (*other_)->personal.name);
	return ret;
}


BusinessCard* Index_GetAt(Index* index, Long index_) {
	BusinessCard* object = NULL;

	//1. index(첨자)를 입력받는다.
	//2. GetAt을 호출한다.
	GetAt(&index->businessCards, index_, &object, sizeof(BusinessCard*));
	//3. 명함을 출력한다.
	return object;
	//4. 끝내다.
}


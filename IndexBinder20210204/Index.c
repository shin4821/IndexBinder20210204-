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
	Long index_;//÷��

	//1. key(BusinessCard*)�� �Է¹޴´�.
	//2. capacity�� length���� ū ���, store�� ȣ���Ѵ�.
	if (index->capacity > index->length) {
		index_ = index->length;
		Store(&index->businessCards, index_, &key, sizeof(BusinessCard*));
	}
	//3. capacity�� length���� �۰ų� ���� ���, AppendForRear�� ȣ���Ѵ�.
	else {
		AppendForRear(&index->businessCards, &key, sizeof(BusinessCard*));
		index->capacity++;
	}
	index->length++;
	//4. ÷�ڸ� ����Ѵ�.
	return index_;
	//5. ������.
}

Long Index_Find(Index* index, BusinessCard* key) {
	Long i;

	//1. key�� �Է¹޴´�.
	//2. LinearSearchUnique�� ȣ���Ѵ�.
	i = LinearSearchUnique(&index->businessCards, key, sizeof(BusinessCard*), CompareBusinessCards);
	//3. i�� ����Ѵ�.
	return i;
	//4. ������.
}

Long Index_PutOut(Index* index, Long index_) {
	//1. ÷�ڸ� �Է¹޴´�.
	//2. Delete�� ȣ���Ѵ�.
	Delete(&index->businessCards, index_, sizeof(BusinessCard*));
	index->capacity--;
	index->length--;
	//3. ������.
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

	//1. index(÷��)�� �Է¹޴´�.
	//2. GetAt�� ȣ���Ѵ�.
	GetAt(&index->businessCards, index_, &object, sizeof(BusinessCard*));
	//3. ������ ����Ѵ�.
	return object;
	//4. ������.
}


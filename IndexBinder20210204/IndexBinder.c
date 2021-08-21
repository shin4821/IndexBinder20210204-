//IndexBinder.c
#include"IndexBinder.h"
#include"Index.h"
#include"BusinessCardBinder.h"
#include<stdlib.h>
#include<string.h>
#pragma warning(disable:4996) //string.h와 세트.

void IndexBinder_Create(IndexBinder* indexBinder) {
	BinaryTree_Create(&indexBinder->indexes);
	indexBinder->length = 0;
}

#if 0
// 손 보기 전의 IndexBinder_Put
Index* IndexBinder_Put(IndexBinder* indexBinder, BusinessCard* index) {
	BinaryNode* node = NULL;
	Index indexCard;
	Index* index_;
	Long i;
	Long capacity = 10;

	//1. 명함 put후의 current를 입력 받는다.
	//2. key(indexCard)를 만든다.
    strcpy(indexCard.companyName, index->company.name);
	//3. Search를 호출하여 같은 key값이 있는지 확인한다.
	node = Search(&indexBinder->indexes, &indexCard, CompareCompanyNames);
	//4. node가 null이면,
	if (node == NULL) {
		//4.1 indexCard에서 배열을 create한다.
		Index_Create(&indexCard, capacity);
		//4.2 Insert를 호출한다.
		node = BinaryTree_Insert(&indexBinder->indexes, &indexCard, sizeof(Index), CompareCompanyNames);
		index_ = (Index*)(node + 1);
		//4.3 index_배열의 첫번째 위치에 put한다.
		i = 0;
		Index_Put(index_, i, index);
		//4.4 length를 1씩 증가시킨다.
		indexBinder->length++;
	}
	//5. node가 null이 아닐 경우,
	else {
		index_ = (Index*)(node + 1);
		//5.1 index_배열의 위치를 찾는다.
		i = index_->length;
		//5.2 index_배열의 해당 위치에 PUT한다.
		Index_Put(index_, i, index);
	}
	//6. indexCard의 배열을 할당해제한다.
	Index_Destroy(&indexCard);
	//7. current를 출력한다.
	return index_;
	//8. 끝내다.
}
#endif

Index* IndexBinder_Put(IndexBinder* indexBinder, BusinessCard* index) {
	BinaryNode* node = NULL;
	Index indexCard;
	Index* index_;
	Long i;
	Long capacity = 10;

	//1. 명함 put후의 current를 입력 받는다.
	//2. key(indexCard)를 만든다.
	strcpy(indexCard.companyName, index->company.name);
	//3. Search를 호출하여 같은 key값이 있는지 확인한다.
	node = Search(&indexBinder->indexes, &indexCard, CompareCompanyNames);
	//4. node가 null이면,
	if (node == NULL) {
		//4.1 indexCard에서 배열을 create한다.
		Index_Create(&indexCard, capacity);
		//4.2 Insert를 호출한다.
		node = BinaryTree_Insert(&indexBinder->indexes, &indexCard, sizeof(Index), 
			CompareCompanyNames);
		//4.3 length를 1씩 증가시킨다.
		indexBinder->length++;
	}
	//5. 배열에 Put한다.
	index_ = (Index*)(node + 1);
	Index_Put(index_, index);

	//7. current를 출력한다.
	return index_;
	//8. 끝내다.
}

Index* IndexBinder_Find(IndexBinder* indexBinder, char(*companyName)) {
	Index indexCard;
	Index* index;
	BinaryNode* node;

	//1. indexCard를 만든다.
	strcpy(indexCard.companyName, companyName);
	//2. Search를 호출한다.
	node = Search(&indexBinder->indexes, &indexCard, CompareCompanyNames);
	//3. node가 null이 아니면 index에 치환한다.
	if (node != NULL) {
		index = (Index*)(node + 1);
	}
	else {
		index = NULL;
	}
	//4. index를 출력한다.
	return index;
	//5. 끝내다.
}

Index* IndexBinder_PutOut(IndexBinder* indexBinder, char(*companyName), BusinessCard* index){
	Index* index_;
	Index indexCard;
	Long i = 0;
	BinaryNode* node;

	//1. 상호, 명함의 위치를 입력받는다.
	//2. indexCard를 만든다.
	strcpy(indexCard.companyName, companyName);
	//3. Search를 호출한다.
	node = Search(&indexBinder->indexes, &indexCard, CompareCompanyNames);
	index_ = (Index*)(node + 1);

	//4. 뽑은 명함의 배열 위치를 찾는다.
	i = Index_Find(index_, index);
	//5. 해당 배열을 뽑는다.
	Index_PutOut(index_, i);

	//6. length가 0인 경우,
	if (index_->length <= 0) {
		//6.1 색인을 뽑는다.
		BinaryTree_Delete(&indexBinder->indexes, &indexCard, sizeof(Index), CompareCompanyNames);
		//6.2 length를 1씩 줄인다.
		indexBinder->length--;
		index_ = NULL;
	}

	//7. index_를 출력한다.
	return index_;

	//8. 끝내다.
}

void Node_Arrange(BinaryNode* node) {
	Index* index;

	//1. node를 읽는다.
	//2. node가 null이 아니면,
	if (node != NULL) {
		//2.1 node의 left부터 1-3 반복한다.
		Node_Arrange(node->left);
		//2.2 node의 배열을 Arrange한다.
		index = (Index*)(node + 1);
		Index_Arrange(index);
		//2.3 node의 right부터 1-3 반복한다.
		Node_Arrange(node->right);
	}
	//3. 끝낸다.
}

void IndexBinder_Arrange(IndexBinder* indexBinder) {
	MakeBalance(&indexBinder->indexes, sizeof(Index));
	Node_Arrange(indexBinder->indexes.root);

}

void MakeList(IndexBinder* indexBinder, Index* (*indexes), Long* count) {
	//1. MakeKeys를 호출한다.
	MakeKeys(&indexBinder->indexes, indexes, count, sizeof(Index));
	//2. indexes, count를 출력한다.
	//3. 끝내다.
}

int CompareCompanyNames(void* one, void* other) {
	Index* one_ = (Index*)one;
	Index* other_ = (Index*)other;
	int ret;

	ret = strcmp(one_->companyName, other_->companyName);
	return ret;
}

void IndexBinder_Destroy(IndexBinder* indexBinder) {
	BinaryTree_Destroy(&indexBinder->indexes);
}







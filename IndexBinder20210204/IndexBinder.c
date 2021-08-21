//IndexBinder.c
#include"IndexBinder.h"
#include"Index.h"
#include"BusinessCardBinder.h"
#include<stdlib.h>
#include<string.h>
#pragma warning(disable:4996) //string.h�� ��Ʈ.

void IndexBinder_Create(IndexBinder* indexBinder) {
	BinaryTree_Create(&indexBinder->indexes);
	indexBinder->length = 0;
}

#if 0
// �� ���� ���� IndexBinder_Put
Index* IndexBinder_Put(IndexBinder* indexBinder, BusinessCard* index) {
	BinaryNode* node = NULL;
	Index indexCard;
	Index* index_;
	Long i;
	Long capacity = 10;

	//1. ���� put���� current�� �Է� �޴´�.
	//2. key(indexCard)�� �����.
    strcpy(indexCard.companyName, index->company.name);
	//3. Search�� ȣ���Ͽ� ���� key���� �ִ��� Ȯ���Ѵ�.
	node = Search(&indexBinder->indexes, &indexCard, CompareCompanyNames);
	//4. node�� null�̸�,
	if (node == NULL) {
		//4.1 indexCard���� �迭�� create�Ѵ�.
		Index_Create(&indexCard, capacity);
		//4.2 Insert�� ȣ���Ѵ�.
		node = BinaryTree_Insert(&indexBinder->indexes, &indexCard, sizeof(Index), CompareCompanyNames);
		index_ = (Index*)(node + 1);
		//4.3 index_�迭�� ù��° ��ġ�� put�Ѵ�.
		i = 0;
		Index_Put(index_, i, index);
		//4.4 length�� 1�� ������Ų��.
		indexBinder->length++;
	}
	//5. node�� null�� �ƴ� ���,
	else {
		index_ = (Index*)(node + 1);
		//5.1 index_�迭�� ��ġ�� ã�´�.
		i = index_->length;
		//5.2 index_�迭�� �ش� ��ġ�� PUT�Ѵ�.
		Index_Put(index_, i, index);
	}
	//6. indexCard�� �迭�� �Ҵ������Ѵ�.
	Index_Destroy(&indexCard);
	//7. current�� ����Ѵ�.
	return index_;
	//8. ������.
}
#endif

Index* IndexBinder_Put(IndexBinder* indexBinder, BusinessCard* index) {
	BinaryNode* node = NULL;
	Index indexCard;
	Index* index_;
	Long i;
	Long capacity = 10;

	//1. ���� put���� current�� �Է� �޴´�.
	//2. key(indexCard)�� �����.
	strcpy(indexCard.companyName, index->company.name);
	//3. Search�� ȣ���Ͽ� ���� key���� �ִ��� Ȯ���Ѵ�.
	node = Search(&indexBinder->indexes, &indexCard, CompareCompanyNames);
	//4. node�� null�̸�,
	if (node == NULL) {
		//4.1 indexCard���� �迭�� create�Ѵ�.
		Index_Create(&indexCard, capacity);
		//4.2 Insert�� ȣ���Ѵ�.
		node = BinaryTree_Insert(&indexBinder->indexes, &indexCard, sizeof(Index), 
			CompareCompanyNames);
		//4.3 length�� 1�� ������Ų��.
		indexBinder->length++;
	}
	//5. �迭�� Put�Ѵ�.
	index_ = (Index*)(node + 1);
	Index_Put(index_, index);

	//7. current�� ����Ѵ�.
	return index_;
	//8. ������.
}

Index* IndexBinder_Find(IndexBinder* indexBinder, char(*companyName)) {
	Index indexCard;
	Index* index;
	BinaryNode* node;

	//1. indexCard�� �����.
	strcpy(indexCard.companyName, companyName);
	//2. Search�� ȣ���Ѵ�.
	node = Search(&indexBinder->indexes, &indexCard, CompareCompanyNames);
	//3. node�� null�� �ƴϸ� index�� ġȯ�Ѵ�.
	if (node != NULL) {
		index = (Index*)(node + 1);
	}
	else {
		index = NULL;
	}
	//4. index�� ����Ѵ�.
	return index;
	//5. ������.
}

Index* IndexBinder_PutOut(IndexBinder* indexBinder, char(*companyName), BusinessCard* index){
	Index* index_;
	Index indexCard;
	Long i = 0;
	BinaryNode* node;

	//1. ��ȣ, ������ ��ġ�� �Է¹޴´�.
	//2. indexCard�� �����.
	strcpy(indexCard.companyName, companyName);
	//3. Search�� ȣ���Ѵ�.
	node = Search(&indexBinder->indexes, &indexCard, CompareCompanyNames);
	index_ = (Index*)(node + 1);

	//4. ���� ������ �迭 ��ġ�� ã�´�.
	i = Index_Find(index_, index);
	//5. �ش� �迭�� �̴´�.
	Index_PutOut(index_, i);

	//6. length�� 0�� ���,
	if (index_->length <= 0) {
		//6.1 ������ �̴´�.
		BinaryTree_Delete(&indexBinder->indexes, &indexCard, sizeof(Index), CompareCompanyNames);
		//6.2 length�� 1�� ���δ�.
		indexBinder->length--;
		index_ = NULL;
	}

	//7. index_�� ����Ѵ�.
	return index_;

	//8. ������.
}

void Node_Arrange(BinaryNode* node) {
	Index* index;

	//1. node�� �д´�.
	//2. node�� null�� �ƴϸ�,
	if (node != NULL) {
		//2.1 node�� left���� 1-3 �ݺ��Ѵ�.
		Node_Arrange(node->left);
		//2.2 node�� �迭�� Arrange�Ѵ�.
		index = (Index*)(node + 1);
		Index_Arrange(index);
		//2.3 node�� right���� 1-3 �ݺ��Ѵ�.
		Node_Arrange(node->right);
	}
	//3. ������.
}

void IndexBinder_Arrange(IndexBinder* indexBinder) {
	MakeBalance(&indexBinder->indexes, sizeof(Index));
	Node_Arrange(indexBinder->indexes.root);

}

void MakeList(IndexBinder* indexBinder, Index* (*indexes), Long* count) {
	//1. MakeKeys�� ȣ���Ѵ�.
	MakeKeys(&indexBinder->indexes, indexes, count, sizeof(Index));
	//2. indexes, count�� ����Ѵ�.
	//3. ������.
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







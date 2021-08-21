//BinaryTree.c
#include"BinaryTree.h"
#include<stdlib.h>
#include<memory.h>

void BinaryTree_Create(BinaryTree* binaryTree) {
	binaryTree->root = NULL;
	binaryTree->length = 0;
	binaryTree->balance = 0;
}

BinaryNode* BinaryTree_Insert(BinaryTree* binaryTree, void* key, size_t size, int(*compare)(void*, void*)) {
	BinaryNode* index;
	BinaryNode* node;
	BinaryNode* parent = NULL;

	//1. key의 값을 입력받는다.
	//2. index를 힙에 할당한다.
	index = (BinaryNode*)malloc(sizeof(BinaryNode) + size);
	memcpy(index + 1, key, size);
	index->left = NULL;
	index->right = NULL;
	node = binaryTree->root;

	//3. 부모를 찾는다.
	while (node != NULL) {
		parent = node;
		if (compare(node + 1, key) > 0) {
			node = node->left;
		}
		else {
			node = node->right;
		}
	}

	//4. 부모와 index를 잇는다.
	if (parent != NULL) {
		if (compare(parent + 1, key) > 0) {
			parent->left = index;
		}
		else {
			parent->right = index;
		}
	}
	else {
		binaryTree->root = index;
	}

	//5. length를 1씩 증가시킨다.
	binaryTree->length++;

	//6. 부모의 key와 key를 비교하여 balance를 조절한다.
	if (compare(binaryTree->root + 1, key) > 0) {
		binaryTree->balance--;
	}
	else if(compare(binaryTree->root + 1, key) < 0){
		binaryTree->balance++;
	}

	//7. index를 출력한다.
	return index;

	//8. 끝낸다.
}

BinaryNode* Search(BinaryTree* binaryTree, void* key, int(*compare)(void*, void*)) {
	BinaryNode* it;
	BinaryNode* index = NULL;

	//1. key를 입력받는다.
	//2. it이 null이 아니고, it의 key값이 아닌동안 반복한다.
	it = binaryTree->root;
	while (it != NULL && compare(it + 1, key) != 0) {
		if (compare(it + 1, key) > 0) {
			it = it->left;
		}
		else {
			it = it->right;
		}
	}
	index = it;
	//3. index를 출력한다.
	return index;
	//4. 끝내다.
}


void BinaryTree_GetAt(BinaryTree* binaryTree, BinaryNode* index, void* key, size_t size) {
	//1. index를 입력받는다.
	//2. index의 key값을 key에 적는다.
	memcpy(key, index + 1, size);
	//3. key를 출력한다.
	//4.끝낸다.
}

BinaryNode* BinaryTree_Delete(BinaryTree* binaryTree, void* key, size_t size, int(*compare)(void*, void*)) {
	BinaryNode* index = NULL;
	BinaryNode* parent = NULL;
	BinaryNode* replace = NULL;
	BinaryNode* replaceKid = NULL;
	BinaryNode* replaceParent = NULL;
	BinaryNode* it = NULL;
	size_t rootKey;
	BinaryNode* right = NULL;

	//1. key값을 입력받는다.
	//2. key의 위치를 찾는다.
	it = binaryTree->root;
	while (compare(it + 1, key) != 0) {
		parent = it;
		if (compare(it + 1, key) > 0) {
			it = it->left;
		}
		else {
			it = it->right;
		}
	}
	index = it;
	right = index->right;

	//3. replace를 찾는다.
	it = index->right;
	while (it != NULL) {
		replace = it;
		it = it->left;
	}
	if (right == NULL) { //3.1 index의 오른쪽 값이 없다면,
		it = index->left;
		while (it != NULL) {
			replace = it;
			it = it->right;
		}
	}
	//4. replaceParent의 위치를 찾는다.
	it = index;
	while (replace != NULL && it != replace) {
		replaceParent = it;
		if (compare(it + 1, replace + 1) > 0) {
			it = it->left;
		}
		else {
			it = it->right;
		}
	}
	//5. replaceParent와 replaceKid의 관계를 설정한다.
	if (replace != NULL && right != NULL && replaceParent != index) {
		replaceKid = replace->right;
		replaceParent->left = replaceKid;
	}
	else if (replace != NULL && right == NULL && replaceParent != index) {
		replaceKid = replace->left;
		replaceParent->right = replaceKid;
	}
	//6. replace의 관계를 설정한다.
	if (replace != NULL && right != NULL && replaceParent != index) {
		replace->left = index->left;
		replace->right = index->right;
	}
	else if (replace != NULL && right == NULL && replaceParent != index) {
		replace->left = index->left;
	}
	else if (replace != NULL && right != NULL && replaceParent == index) {
		replace->left = index->left;
	}
	//7. parent와의 관계를 설정한다.
	rootKey = malloc(size);
	memcpy(rootKey, binaryTree->root + 1, size);

	if (parent != NULL) {
		if (compare(parent + 1, index + 1) > 0) {
			parent->left = replace;
		}
		else {
			parent->right = replace;
		}
	}
	else {
		binaryTree->root = replace;
	}
	//8. index를 지운다.
	if (index != NULL) {
		free(index);
		index = NULL;
	}
	//9. length를 1씩 뺀다.
	binaryTree->length--;
	//10. balance를 계산한다.
	if (compare(rootKey, key) > 0) {
		binaryTree->balance++;
	}
	else if (compare(rootKey, key) < 0) {
		binaryTree->balance--;
	}
	else if (replace != NULL) {
		if (compare(rootKey, replace + 1) > 0) {
			binaryTree->balance++;
		}
		else {
			binaryTree->balance--;
		}
	}

	//11. index를 출력한다.
	return index;
	//12. 끝내다.
	if (rootKey != NULL) {
		free(rootKey);
		rootKey = NULL;
	}
}

void MakeBalance(BinaryTree* binaryTree, size_t size) {
	void(*keys);
	Long count;

	//1. MakeKeys를 호출한다.
	MakeKeys(binaryTree, &keys, &count, size);
	//2. DeleteAllItems를 호출한다.
	DeleteAllItems(binaryTree);
	//3. MakeTree를 호출한다.
	MakeTree(binaryTree, keys, count, size);
	//4. 끝낸다.
}

void Node_MakeKeys(BinaryNode* node, void(*keys), Long* count, Long* i, size_t size) {
	//1. 노드를 읽는다.
	//2. 노드가 null이 아닐 경우,
	if (node != NULL) {
		//2.1 노드의 left부터 1~4를 반복한다.
		Node_MakeKeys(node->left, keys, count, i, size); // 이 값은 MakeKeys로 반환되는 것이다.
		//2.2 keys에 노드의 key를 적는다.
		memcpy(((char(*))keys) + ((*i) * size), node + 1, size);
		//2.3 개수를 센다.
		(*count)++;
		(*i)++;
		//2.4 노드의 right부터 1~4를 반복한다.
		Node_MakeKeys(node->right, keys, count, i, size);
	}
	//3. keys, i, count를 출력한다.
	//4. 끝내다.
}

void MakeKeys(BinaryTree* binaryTree, void* (*keys), Long* count, size_t size) {
	Long i = 0;
	*count = 0;

	//1. keys에 length만큼 배열을 할당한다.
	*keys = (void(*))calloc(binaryTree->length, size);
	//2. Node_MakeKeys를 호출한다.
	Node_MakeKeys(binaryTree->root, *keys, count, &i, size);
	//3. keys, count를 출력한다.
	//4. 끝내다.
}

void Node_DeleteAllItems(BinaryNode* node) {
	//1. 노드를 읽는다.
	//2. 노드가 null이 아닌 경우,
	if (node != NULL) {
		//2.1 노드의 left부터 1~3과정을 반복한다.
		Node_DeleteAllItems(node->left);
		//2.2 노드의 right부터 1~3과정을 반복한다.
		Node_DeleteAllItems(node->right);
		//2.3 node를 없앤다.
		if (node != NULL) {
			free(node);
			node = NULL;
		}
	}
	//3. 끝낸다.
}

void DeleteAllItems(BinaryTree* binaryTree) {
	//1. Node_DeleteAllItems를 호출한다.
	Node_DeleteAllItems(binaryTree->root);
	//2. root를 null 처리한다.
	binaryTree->root = NULL;
	//3. length를 0으로 초기화한다.
	binaryTree->length = 0;
	//4. balance를 0으로 초기화한다.
	binaryTree->balance = 0;
	//5. 끝낸다.
}

BinaryNode* Node_MakeTree(void(*keys), Long low, Long high, size_t size) {
	BinaryNode* node = NULL;
	Long middle;

	//1. keys, low, high를 입력받는다.
	//2. low가 high보다 적거나 같으면,
	if (low <= high) {
		node = (BinaryNode*)malloc(sizeof(BinaryNode) + size);
		middle = (low + high) / 2;
		memcpy(node + 1, ((char(*))keys) + (middle * size), size);
		node->left = NULL;
		node->right = NULL;

		node->left = Node_MakeTree(keys, low, middle - 1, size);
		node->right = Node_MakeTree(keys, middle + 1, high, size);
	}
	//3. node를 출력한다.
	return node;
	//4. 끝내다.
}

void MakeTree(BinaryTree* binaryTree, void(*keys), Long count, size_t size) {
	BinaryTree* node;
	Long low ;
	Long high;
	Long remainder;

	low = 0;
	high = count - 1;

	//1. Node_MakeTree를 호출한다.
	node = Node_MakeTree(keys, low, high, size);
	//2. root와 node를 잇는다.
	binaryTree->root = node;
	//3. length에 count를 적는다.
	binaryTree->length = count;
	//4. count가 짝수인지 홀수인지 판단 후, balance를 정한다.
	remainder = count - (count / 2) * 2;
	if (remainder >= 1) {
		binaryTree->balance = 0;
	}
	else {
		binaryTree->balance = 1;
	}
	//5. 끝낸다.
}

void Node_Destroy(BinaryNode* node) {
	//1. 노드를 입력받는다.
	//2. 노드가 null이 아닐 경우,
	if (node != NULL) {
		//2.1 노드의 left를 읽은 후, 1~3 과정을 반복한다.
		Node_Destroy(node->left);
		//2.2 노드의 right을 읽은 후, 1~3 과정을 반복한다.
		Node_Destroy(node->right);
		//2.3 노드를 지운다.
		if (node != NULL) {
			free(node);
			node = NULL;
		}
	}
	//3.끝낸다.
}

void BinaryTree_Destroy(BinaryTree* binaryTree) {
	//1. Node_Destroy를 호출한다.
	Node_Destroy(binaryTree->root);
	//2. 끝낸다.
}







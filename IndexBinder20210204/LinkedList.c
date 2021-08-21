//LinkedList.c
#include"LinkedList.h"
#include<memory.h>
#include<stdlib.h>

void LinkedList_Create(LinkedList* linkedList) {
	//1. head를 null 처리한다.
	linkedList->head = NULL;
	//2. tail을 null 처리한다.
	linkedList->tail = NULL;
	//3. length를 0으로 초기화한다.
	linkedList->length = 0;
	//4. current를 null 처리한다.
	linkedList->current = NULL;
	//5. 끝내다.
}

Node* LinkedList_AppendFromHead(LinkedList* linkedList, void* object, size_t size) {
	//1. 값을 입력받는다.
	//2. node를 맨 앞에 꽂는다.
	linkedList->current = (Node*)malloc(sizeof(Node) + size);
	memcpy(linkedList->current + 1, object, size);
	linkedList->current->previous = linkedList->current;
	if (linkedList->head != NULL) {
		linkedList->current->next = linkedList->head;
		linkedList->head->previous = linkedList->current;
	}
	else {
		linkedList->current->next = linkedList->current;
		linkedList->tail = linkedList->current;
	}
	linkedList->head = linkedList->current;
	//3. 개수를 센다.
	linkedList->length++;
	//4. current를 출력한다.
	return linkedList->current;
	//5. 끝내다.
}

Node* LinkedList_InsertBefore(LinkedList* linkedList, Node* index, void* object, size_t size) {
	//1. 위치, 값을 입력받는다.
	//2. 위치 앞에 node를 끼운다.
	linkedList->current = (Node*)malloc(sizeof(Node) + size);
	memcpy(linkedList->current + 1, object, size);
	linkedList->current->next = index;
	if (linkedList->head != index) {
		linkedList->current->previous = index->previous;
		index->previous->next = linkedList->current;
	}
	else {
		linkedList->current->previous = linkedList->current;
		linkedList->head = linkedList->current;
	}
	index->previous = linkedList->current;
	//3. 개수를 센다.
	linkedList->length++;
	//4. current를 출력한다.
	return linkedList->current;
	//5. 끝내다.
}

Node* LinkedList_InsertAfter(LinkedList* linkedList, Node* index, void* object, size_t size) {
	//1. 위치, 값을 입력받는다.
	//2. 위치 뒤에 node를 끼운다.
	linkedList->current = (Node*)malloc(sizeof(Node) + size);
	memcpy(linkedList->current + 1, object, size);
	linkedList->current->previous = index;
	if (linkedList->tail != index) {
		linkedList->current->next = index->next;
		index->next->previous = linkedList->current;
	}
	else {
		linkedList->current->next = linkedList->current;
		linkedList->tail = linkedList->current;
	}
	index->next = linkedList->current;
	//3. 개수를 센다.
	linkedList->length++;
	//4. current를 출력한다.
	return linkedList->current;
	//5. 끝내다.
}


Node* LinkedList_AppendFromTail(LinkedList* linkedList, void* object, size_t size) {
	//1. 값을 입력받는다.
	//2. Node를 맨 뒤에 꽂는다.
	linkedList->current = (Node*)malloc(sizeof(Node) + size);
	memcpy(linkedList->current + 1, object, size);
	linkedList->current->next = linkedList->current;
	if (linkedList->tail != NULL) {
		linkedList->current->previous = linkedList->tail;
		linkedList->tail->next = linkedList->current;
	}
	else {
		linkedList->current->previous = linkedList->current;
		linkedList->head = linkedList->current;
	}
	linkedList->tail = linkedList->current;
	//3. 개수를 센다.
	linkedList->length++;
	//4. current를 출력한다.
	return linkedList->current;
	//5. 끝내다.
}

Node* LinkedList_Delete(LinkedList* linkedList, Node* index) {
	//1. 위치를 입력받는다.
	//2. 위치가 head와 tail 모두 아닌 경우, 이전 이후 카드를 잇고 current를 찾는다.
	if (linkedList->head != index && linkedList->tail != index) {
		index->next->previous = index->previous;
		index->previous->next = index->next;
		linkedList->current = index->next;
	}
	//2.1 위치가 head인 경우, 이전 이후 카드를 잇고 current를 찾는다.
	else if (linkedList->head == index && linkedList->tail != index) {
		index->next->previous = index->next;
		linkedList->head = index->next;
		linkedList->current = index->next;
	}
	//2.2 위치가 tail인 경우, 이전 이후 카드를 잇고 current를 찾는다.
	else if (linkedList->tail == index && linkedList->head != index) {
		index->previous->next = index->previous;
		linkedList->tail = index->previous;
		linkedList->current = index->previous;
	}
	//2.3 위치가 head와 tail 모두인 경우, 모두 null 처리한다.
	else {
		linkedList->head = NULL;
		linkedList->tail = NULL;
		linkedList->current = NULL;
	}
	//3. 위치 node를 지운다.
	if (index != NULL) {
		free(index);
		index = NULL;
	}
	//4. 개수를 1씩 뺀다.
	linkedList->length--;
	//5. 위치를 출력한다.
	return index;
	//6. 끝낸다.
}

Node* LinkedList_DeleteFromHead(LinkedList* linkedList) {
	Node* index;

	//1. index를 head로 옮긴다.
	index = linkedList->head;
	//2. current의 위치를 설정한다.
	if (linkedList->tail != index) {
		index->next->previous = index->next;
		linkedList->head = index->next;
		linkedList->current = index->next;
	}
	else {
		linkedList->head = NULL;
		linkedList->tail = NULL;
		linkedList->current = NULL;
	}
	//3. index를 지운다.
	if (index != NULL) {
		free(index);
		index = NULL;
	}
	//4. 개수를 1씩 뺀다.
	linkedList->length--;
	//5. index를 출력한다.
	return index;
	//6. 끝내다.
}

Node* LinkedList_DeleteFromTail(LinkedList* linkedList) {
	Node* index;

	//1. index를 tail로 옮긴다.
	index = linkedList->tail;
	//2. current의 위치를 설정한다.
	if (linkedList->head != index) {
		index->previous->next = index->previous;
		linkedList->tail = index->previous;
		linkedList->current = index->previous;
	}
	else {
		linkedList->head = NULL;
		linkedList->tail = NULL;
		linkedList->current = NULL;
	}
	//3. index를 지운다.
	if (index != NULL) {
		free(index);
		index = NULL;
	}
	//4. 개수를 1씩 뺀다.
	linkedList->length--;
	//5. current를 출력한다.
	return index;
	//6. 끝내다.
}

void LinkedList_DeleteAllItems(LinkedList* linkedList) {
	Node* it;
	Node* previous = NULL;

	//1. 노드의 처음부터 끝까지 반복한다.
	it = linkedList->head;
	while (it != previous) {
		linkedList->current = it->next;
		previous = it;

		//1.1  노드를 지운다.
		if (it != NULL) {
			free(it);
			it = NULL;
		}
		it = linkedList->current;
	}
	//2. head를 null 처리한다.
	linkedList->head = NULL;
	//3. tail을 null 처리한다.
	linkedList->tail = NULL;
	//4. current를 null 처리한다.
	linkedList->current = NULL;
	//5. length를 0으로 초기화한다.
	linkedList->length = 0;
	//6. 끝내다.
}

Node* LinkedList_LinearSearchUnique(LinkedList* linkedList, void* key, int(*compare)(void*, void*)) {
	Node* index = NULL;
	Node* it;
	Node* previous = NULL;

	//1. key를 입력받는다.
	//2. key의 값이 아니고, 노드의 끝이 아닌 동안 반복한다.
	it = linkedList->head;
	while (it != previous && compare(it + 1, key) != 0) {
		previous = it;
		it = it->next;
	}
	if (it != previous) {
		index = it;
	}
	//3. index를 출력한다.
	return index;
	//4. 끝내다.
}

void LinkedList_LinearSearchDuplicate(LinkedList* linkedList, void* key, Node** (*indexes), Long* count,
	int(*compare)(void*, void*)) {
	Node* it;
	Node* previous = NULL;
	Long i = 0;
	*count = 0;

	//1. key를 입력받는다.
	//2. 노드의 끝이 아닌동안 반복한다.
	*indexes = (Node * (*))calloc(linkedList->length, sizeof(Node*));
	it = linkedList->head;
	while (it != previous) {
		//2.1 object가 key값인 경우,
		if (compare(it + 1, key) == 0) {
			//2.1.1. indexes에 적는다.
			(*indexes)[i] = it;
			//2.1.2. 개수를 센다.
			(*count)++;
			i++;
		}
		previous = it;
		it = it->next;
	}
	//2. indexes, count를 출력한다.
	//3. 끝내다.
}

Node* LinkedList_First(LinkedList* linkedList) {
	//1. current를 이전 노드로 옮긴다.
	linkedList->current = linkedList->head;
	//2. current를 출력한다.
	return linkedList->current;
	//3. 끝내다.
}

Node* LinkedList_Previous(LinkedList* linkedList) {
	//1. current를 이전 노드로 옮긴다.
	linkedList->current = linkedList->current->previous;
	//2. current를 출력한다.
	return linkedList->current;
	//3. 끝내다.
}

Node* LinkedList_Next(LinkedList* linkedList) {
	//1. current를 다음 노드로 옮긴다.
	linkedList->current = linkedList->current->next;
	//2. current를 출력한다.
	return linkedList->current;
	//3. 끝내다.
}

Node* LinkedList_Last(LinkedList* linkedList) {
	//1. current를 tail로 옮긴다.
	linkedList->current = linkedList->tail;
	//2. current를 출력한다.
	return linkedList->current;
	//3. 끝낸다.
}

void LinkedList_GetAt(LinkedList* linkedList, Node* index, void* object, size_t size) {
	//1. 위치를 입력받는다.
	//2. 위치의 값을 object에 적는다.
	memcpy(object, index + 1, size);
	//3. object를 출력한다.
	//4. 끝낸다.
}

void LinkedList_Destroy(LinkedList* linkedList) {
	Node* it;
	Node* previous = NULL;

	//1. 노드의 끝이 아닌동안 반복한다.
	it = linkedList->head;
	while (it != previous) {
		linkedList->current = it->next;
		previous = it;
		//1.1 노드를 지운다.
		if (it != NULL) {
			free(it);
			it = NULL;
		}
		it = linkedList->current;
	}
	//2. 끝낸다.
}




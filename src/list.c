#include "list.h"

#include <stdlib.h>

void list_init(List* list) {
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
}

void list_add(List* list, void* data) {
	ListNode* node = malloc(sizeof(ListNode));
	node->data = data;
	node->next = NULL;

	/* If the list is empty, the new node is both the head and the tail */
	if (list->head == NULL) {
		list->head = node;
		list->tail = node;
	}
	/* Otherwise, insert the new node at the tail */
	else {
		list->tail->next = node;
		list->tail = node;
	}

	list->size++;
}

void list_insert(List* list, void* data, int index) {
	int i;
	ListNode *prev, *node = malloc(sizeof(ListNode));
	node->data = data;

	if (index == 0) {
		node->next = list->head;
		list->head = node;
	} else {
		prev = list->head;
		for (i = 0; i < index - 1; i++) {
			prev = prev->next;
		}
		node->next = prev->next;
		prev->next = node;
	}

	list->size++;
}

void list_remove(List* list, int index) {
	ListNode* node = list->head;
	ListNode* prev = NULL;
	int i = 0;

	if (index < 0 || index >= list->size) {
		return;
	}

	while (i < index) {
		prev = node;
		node = node->next;
		i++;
	}

	if (prev == NULL) {
		list->head = node->next;
	} else {
		prev->next = node->next;
	}

	free(node);
	list->size--;
}

void* list_get(List* list, int index) {
	ListNode* node = list->head;
	int i = 0;

	while (node != NULL) {
		if (i == index)
			return node->data;

		node = node->next;
		i++;
	}

	return NULL;
}

void list_destroy(List* list) {
	ListNode* node = list->head;
	ListNode* next;

	while (node != NULL) {
		next = node->next;
		free(node->data);
		free(node);
		node = next;
	}

	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
}
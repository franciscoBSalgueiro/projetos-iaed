#include "proj1.h"

#include <stdlib.h>

void list_init(List* list) {
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
}

void list_add(List* list, void* data) {
	ListNode* node = custom_alloc(sizeof(ListNode));
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

void list_insert(List* list, void* data, ListNode* node) {
	ListNode *new_node = custom_alloc(sizeof(ListNode));
	new_node->data = data;
	if (node == NULL) {
		new_node->next = list->head;
		list->head = new_node;
	} else {
		new_node->next = node->next;
		node->next = new_node;
	}

	list->size++;
}

void list_remove(List* list, ListNode* node, ListNode* prev) {
	if (node == NULL) return;

	if (prev == NULL) {
		list->head = node->next;
	} else {
		prev->next = node->next;
	}

	list->size--;
	free(node->data);
	free(node);
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
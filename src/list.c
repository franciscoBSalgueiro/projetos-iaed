/*
 * File:  list.c
 * Author:  Francisco Salgueiro
 * Description: List implementation
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "proj1.h"

void list_init(List* list) {
	list->head = NULL;
	list->tail = NULL;
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
}

void list_insert(List* list, void* data, ListNode* node) {
	ListNode* new_node = custom_alloc(sizeof(ListNode));
	new_node->data = data;
	if (node == NULL) {
		new_node->next = list->head;
		list->head = new_node;
	} else {
		new_node->next = node->next;
		node->next = new_node;
	}
}

void list_remove(List* list, ListNode* node, ListNode* prev) {
	if (node == NULL) return;

	if (prev == NULL)
		list->head = node->next;
	else
		prev->next = node->next;

	free(node);
}

void list_destroy(List* list) {
	ListNode* node = list->head;
	ListNode* next;

	while (node != NULL) {
		next = node->next;
		free(node);
		node = next;
	}
}

/* Sort a list of Reservations alphabetically */
void list_sort(List* list) {
	ListNode* node, *next, *prev;
	int ended = FALSE;

	while (!ended) {
		ended = TRUE;
		for(prev = NULL, node = list->head; node != NULL; prev = node, node = next) {
			next = node->next;
			if (next != NULL && strcmp(((Reservation*)node->data)->id,
				((Reservation*)next->data)->id) > 0) {
				ended = FALSE;
				node->next = next->next;
				
				next->next = node;
				if (prev == NULL)
					list->head = next;
				else
					prev->next = next;
				if(next == list->tail)
					list->tail = node;
			}
		}
	}
}
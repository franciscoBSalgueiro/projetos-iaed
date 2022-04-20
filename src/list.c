/*
 * File:  list.c
 * Author:  Francisco Salgueiro
 * Description: List implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "proj2.h"

/* Creates a new List */
void list_init(List* list) {
	list->head = NULL;
	list->tail = NULL;
}

/* Adds a new node to the end of the list */
void list_add(List* list, void* data) {
	ListNode* node = custom_alloc(sizeof(ListNode));
	node->data = data;
	node->next = NULL;

	if (list->head == NULL) {
		list->head = node;
		list->tail = node;
	} else {
		list->tail->next = node;
		list->tail = node;
	}
}

/* Inserts a new node in the list after the provided node, or at the beggining
 * if node is NULL */
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

/* Removes a node from the list */
void list_remove(List* list, ListNode* node, ListNode* prev) {
	if (node == NULL) return;

	if (prev == NULL)
		list->head = node->next;
	else
		prev->next = node->next;

	free(node);
}

/* Deletes all nodes from a list */
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
	ListNode *node, *next, *prev;
	int ended = FALSE;

	while (!ended) {
		ended = TRUE;
		for (prev = NULL, node = list->head; node != NULL;
			 prev = node, node = next) {
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
				if (next == list->tail) list->tail = node;
			}
		}
	}
}
/*
 * File:  list.c
 * Author:  Francisco Salgueiro
 * Description: Hashtable implementation for storing Reservations
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "proj1.h"

int hash(char* v) {
	int h, a = 31415, b = 27183;
	for (h = 0; *v != '\0'; v++, a = a * b % (HASH_TABLE_SIZE - 1))
		h = (a * h + *v) % HASH_TABLE_SIZE;
	return h;
}

/* Creates a new HashTable */
HashTable* hashtable_create() {
	int i;
	HashTable* ht = custom_alloc(sizeof(HashTable));

	for (i = 0; i < HASH_TABLE_SIZE; i++) {
		list_init(&ht->table[i]);
	}

	return ht;
}

/* Destroys a HashTable */
void hashtable_destroy(HashTable* ht) {
	int i;
	for (i = 0; i < HASH_TABLE_SIZE; i++) {
		ListNode* node = ht->table[i].head;
		while (node != NULL) {
			ListNode* next = node->next;
			free(((Reservation*)node->data)->id);
			free(node->data);
			free(node);
			node = next;
		}
	}
	free(ht);
}

/* Removes string from HashTable */
void hashtable_remove(HashTable* ht, Reservation* r) {
	int index = hash(r->id);
	ListNode* node = ht->table[index].head;
	ListNode* prev = NULL;
	while (node != NULL) {
		if (node->data == r) {
			free(((Reservation*)node->data)->id);
			free(node->data);
			list_remove(&ht->table[index], node, prev);
			return;
		}
		prev = node;
		node = node->next;
	}
}

/* Adds a new string to the HashTable */
void hashtable_add(HashTable* ht, Reservation* r) {
	int index = hash(r->id);
	list_add(&ht->table[index], r);
}

/* Returns true if the HashTable contains the reservation with that id */
int hashtable_contains(HashTable* ht, char* str) {
	int index = hash(str);
	ListNode* node = ht->table[index].head;

	while (node != NULL) {
		if (strcmp(((Reservation*)node->data)->id, str) == 0) return TRUE;
		node = node->next;
	}
	return FALSE;
}

/* Returns the flight which contains the reservation id */
Flight* hashtable_get(HashTable* ht, char* id) {
	int index = hash(id);
	ListNode* node = ht->table[index].head;

	while (node != NULL) {
		if (strcmp(((Reservation*)node->data)->id, id) == 0)
			return ((Reservation*)node->data)->flight;
		node = node->next;
	}
	return NULL;
}

/* Implementation of HashTable for storing a list of strings */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "proj1.h"

int hash(char* v) {
	int h = 0, a = 127;
	for (; *v != '\0'; v++) {
		h = (a * h + *v) % HASH_TABLE_SIZE;
	}
	return h;
}

/* Creates a new HashTable */
HashTable* hashtable_create() {
	int i;
	HashTable* ht = custom_alloc(sizeof(HashTable));
	ht->size = 0;

	for (i = 0; i < HASH_TABLE_SIZE; i++) list_init(&ht->table[i]);

	return ht;
}

/* Destroys a HashTable */
void hashtable_destroy(HashTable* ht) {
	int i;
	for (i = 0; i < HASH_TABLE_SIZE; i++) {
		ListNode* node = ht->table[i].head;
		while (node != NULL) {
			ListNode* next = node->next;
			free(node->data);
			free(node);
			node = next;
		}
	}
	free(ht);
}

/* Removes string from HashTable */
void hashtable_remove(HashTable* ht, char* key) {
	int index = hash(key);
	ListNode* node = ht->table[index].head;
	ListNode* prev = NULL;
	while (node != NULL) {
		if (strcmp(node->data, key) == 0) {
			list_remove(&ht->table[index], node, prev);
			return;
		}
		prev = node;
		node = node->next;
	}
}

/* Adds a new string to the HashTable */
void hashtable_add(HashTable* ht, char* str) {
	int index = hash(str);
	list_add(&ht->table[index], str);
	ht->size++;
}

/* Returns true if the HashTable contains the string */
int hashtable_contains(HashTable* ht, char* str) {
	int index = hash(str);
	ListNode* node = ht->table[index].head;

	while (node != NULL) {
		if (strcmp(node->data, str) == 0) return TRUE;
		node = node->next;
	}
	return FALSE;
}
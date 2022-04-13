#include <stdlib.h>
#include "list.h"

void list_init(List* list) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void list_add(List* list, void* data) {
    ListNode* node = (ListNode*) malloc(sizeof(ListNode));
    node->data = data;
    node->next = NULL;

    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
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
        if (i == index) {
            return node->data;
        }

        node = node->next;
        i++;
    }

    return NULL;
}
    
void list_clear(List* list) {
    ListNode* node = list->head;
    ListNode* next;

    while (node != NULL) {
        next = node->next;
        free(node);
        node = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void list_destroy(List* list) {
    list_clear(list);
    free(list);
}
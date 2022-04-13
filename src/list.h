
typedef struct ListNode {
    void* data;
    struct ListNode* next;
} ListNode;

typedef struct List {
    ListNode* head;
    ListNode* tail;
    int size;
} List;

void list_init(List* list);
void list_add(List* list, void* data);
void list_remove(List* list, int index);
int list_contains(List* list, void* data);
void* list_get(List* list, int index);
void list_clear(List* list);
void list_destroy(List* list);
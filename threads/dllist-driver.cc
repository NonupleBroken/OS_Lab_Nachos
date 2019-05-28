#include "dllist.h"

char items[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

// random generate a list with N elements
void RandomGenerateList(DLList *list, int N, int which) {
    int key;
    int item_index;
    for (int i = 0; i < N; i++) {
        key = Random() % 1001;
        item_index = Random() % 26;
        list->SortedInsert(&items[item_index], key);
        printf("Thread %d inserted item: %c with key=%d.\n", which, items[item_index], key);
    }
}

// remove N items starting from the head of the list and cout removed item
void RemoveFromHead(DLList *list, int N, int which) {
    void *item;
    int key;
    for (int i = 0; i < N; i++) {
        item = list->Remove(&key);
        if (item != NULL)
            printf("Thread %d removed item: %c with key=%d.\n", which, *((char*)item), key);
        else {
            printf("List is empty.\n");
            break;
        }
    }
}

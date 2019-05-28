#include "dllist.h"

// initialize a list element
DLLElement::DLLElement(void *itemPtr, int sortKey) {
    this->item = itemPtr;
    this->key = sortKey;
    this->next = NULL;
    this->prev = NULL;
}

// initialize the list
DLList::DLList() {
    this->first = NULL;
    this->last = NULL;
    lock = new Lock("list lock");
}

// de-allocate the list
DLList::~DLList() {
    while (this->Remove(NULL) != NULL);
    delete lock;
}

// add to head of list (set key = min_key-1)
void DLList::Prepend(void *item) {
    lock->Acquire();
    DLLElement *new_element = new DLLElement(item, 0);
    if (this->IsEmpty()) {
        this->first = new_element;
        this->last = new_element;
    }
    else {
        new_element->key = this->first->key - 1;
        new_element->next = this->first;
        new_element->prev = NULL;
        this->first->prev = new_element;
        this->first = new_element;
    }
    // printf("Prepend element with key=%d\n", this->first->key);
    lock->Release();
}

// add to tail of list (set key = max_key+1)
void DLList::Append(void *item) {
    lock->Acquire();
    DLLElement *new_element = new DLLElement(item, 0);
    if (IsEmpty()) {
        this->first = new_element;
        this->last = new_element;
    }
    else {
        new_element->key = this->last->key + 1;
        new_element->next = NULL;
        new_element->prev = this->last;
        this->last->next = new_element;
        this->last = new_element;
    }
    // printf("Append element with key=%d\n", this->last->key);
    lock->Release();
}

// remove from head of list
void *DLList::Remove(int *keyPtr) {
    lock->Acquire();
    if (IsEmpty()) {
        // printf("Remove element with key=%d failed. Empty list.\n", *keyPtr);
        lock->Release();
        return NULL;
    }
    DLLElement *new_element = this->first;
    *keyPtr = this->first->key;
    void *new_element_item = new_element->item;
    if (this->first == this->last) {
        this->first = NULL;
        this->last = NULL;
    }
    else {
        this->first = new_element->next;
        this->first->prev = NULL;
    }
    // printf("Remove element with key=%d\n", *keyPtr);
    lock->Release();
    return new_element_item;
}        

// return true if list has no element
bool DLList::IsEmpty() {
    if (this->first == NULL && this->last == NULL)
        return true;
    else if (this->first != NULL && this->last != NULL)
        return false;
    else {
        printf("Error! Either the first or the last is NULL of the List!\n");
        return false;
    }
}

void DLList::SortedInsert(void *item, int sortKey) {
    lock->Acquire();
    DLLElement *new_element = new DLLElement(item, sortKey);
    DLLElement *ptr = this->first;
    if (IsEmpty()) {
        this->first = new_element;
        this->last = new_element;
    }
    else {
        for (; ptr != NULL; ptr = ptr->next)
            if (ptr->key > sortKey) break;
        if (ptr == NULL) {
            new_element->prev = this->last;
            this->last->next = new_element;
            this->last = new_element;
            new_element->next = NULL;
        }
        else if (ptr == this->first) {
            new_element->next = this->first;
            this->first->prev = new_element;
            this->first = new_element;
            this->first->prev = NULL;
        }
        else {
            ptr->prev->next = new_element;
            new_element->prev = ptr->prev;
            new_element->next = ptr;
            ptr->prev = new_element;
        }
    }
    // printf("SortedInsert element with key=%d\n", sortKey);
    lock->Release();
}
        
void *DLList::SortedRemove(int sortKey) {
    lock->Acquire();
    DLLElement *ptr = this->first;
    if (IsEmpty()) {
        printf("SortedRemove element with key=%d failed. Empty list.\n", sortKey);
        lock->Release();
        return NULL;
    }
    for (; ptr != NULL; ptr = ptr->next)
        if (ptr->key == sortKey) break;
    if (ptr == NULL) {
        printf("SortedRemove element with key=%d failed. No such element with this sortKey.\n", sortKey);
        lock->Release();
        return NULL;
    }
    else if (ptr == this->first) {
        this->first = this->first->next;
        this->first->prev = NULL;
    }
    else if(ptr == this->last) {
        this->last = this->last->prev;
        this->last->next = NULL;
    }
    else {
        ptr->prev->next = ptr->next;
        ptr->next->prev = ptr->prev;
    }
    // printf("SortedRemove element with key=%d\n", sortKey);
    lock->Release();
    return ptr->item;
}

void DLList::ShowElements() {
    DLLElement *ptr = this->first;
    if (!IsEmpty()) {
        printf("Traversing the list:\n");
        for (; ptr != NULL; ptr = ptr->next)
            printf("item: %c    key: %d\n", *((char*)ptr->item), ptr->key);
    }
    else printf("Empty list.\n");
}
void DLList::ShowElementsReverse() {
    DLLElement *ptr = this->last;
    if (!IsEmpty()) {
        printf("Traversing the list order by desc:\n");
        for (; ptr != NULL; ptr = ptr->prev)
            printf("item: %c    key: %d\n", *((char*)ptr->item), ptr->key);
    }
    else printf("Empty list.\n");
}

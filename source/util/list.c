//
// MIT License
//
// Copyright (c) 2023 Elias Engelbert Plank
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <stdlib.h>

#include "list.h"

/// Allocates a new node instance
ListNode *node_new(void *data) {
    ListNode *self = malloc(sizeof(ListNode));
    self->prev = NULL;
    self->next = NULL;
    self->data = data;
    return self;
}

/// Frees all associated data with the node except the data
void node_free(ListNode *self) {
    free(self);
}

/// Allocates a new list instance
LinkedList *linked_list_new(void) {
    LinkedList *self = malloc(sizeof(LinkedList));
    self->head = NULL;
    self->tail = NULL;
    self->length = 0;
    self->capacity = 0;
    return self;
}

/// Frees all data associated with the list
void linked_list_free(LinkedList *self) {
    linked_list_clear(self);
    free(self);
}

/// Clears the list
void linked_list_clear(LinkedList *self) {
    ListNode *it = self->head;

    while (it != NULL) {
        ListNode *tmp = it;
        it = it->next;
        node_free(tmp);
    }
    self->head = NULL;
    self->tail = NULL;
    self->length = 0;
}

/// Appends the data to the list
void linked_list_append(LinkedList *self, void *data) {
    ListNode *node = node_new(data);

    if (self->head == NULL) {
        self->head = node;
        self->tail = node;
        node->prev = NULL;
        self->length++;
        return;
    }

    ListNode *temp = self->tail;
    temp->next = node;
    self->tail = node;
    self->tail->prev = temp;
    self->tail->next = NULL;
    self->length++;
}

/// Sets the first node of the list
void linked_list_set_head(LinkedList *self, void *data) {
    ListNode *node = node_new(data);

    node->next = self->head;
    node->prev = NULL;

    if (self->head != NULL) {
        self->head->prev = node;
    }

    self->head = node;
    self->length++;
}

/// Alias to linked_list_append
void linked_list_set_tail(LinkedList *self, void *data) {
    linked_list_append(self, data);
}

/// Inserts a node at the specified index
void linked_list_insert(LinkedList *self, usize idx, void *data) {
    if (self->head == NULL) {
        linked_list_set_head(self, data);
        return;
    }

    ListNode *node = node_new(data);
    ListNode *temp = self->head;

    while (--idx) {
        temp = temp->next;
    }

    node->next = temp->next;
    node->prev = temp;

    temp->next = node;
    self->length++;
}

/// Retrieves the data of the node at the specified index in the list
void *linked_list_at(LinkedList const *self, usize const idx) {
    if (self->head == NULL || idx >= self->length) {
        return NULL;
    }

    usize iterator;
    usize iterator_limit;
    ListNode *temp;
    if (idx < (self->length - idx)) {
        temp = self->head;
        iterator_limit = idx;
        for (iterator = 0; temp != NULL && iterator < iterator_limit; ++iterator) {
            temp = temp->next;
        }
    } else {
        temp = self->tail;
        iterator_limit = self->length - idx - 1;
        for (iterator = 0; temp != NULL && iterator < iterator_limit; ++iterator) {
            temp = temp->prev;
        }
    }
    return temp != NULL && iterator == iterator_limit ? temp->data : NULL;
}

/// Tries to find the specified data using the specified equal function
ListNode *linked_list_find(LinkedList const *self, void const *data, ListEqualFunction const equal) {
    if (self->head == NULL || data == NULL || equal == NULL) {
        return NULL;
    }

    for (ListNode *it = self->head; it != NULL; it = it->next) {
        if (equal(it->data, data)) {
            return it;
        }
    }
    return NULL;
}

/// Tries to remove the specified data using the specified equal function
void linked_list_remove(LinkedList *self, void const *data, ListEqualFunction const equal) {
    if (self->head == NULL || data == NULL || equal == NULL) {
        return;
    }

    for (ListNode *it = self->head; it != NULL; it = it->next) {
        if (equal(it->data, data)) {
            if (it->prev) {
                it->prev->next = it->next;
            } else {
                self->head = it->next;
            }

            if (it->next) {
                it->next->prev = it->prev;
            } else {
                self->tail = it->prev;
            }

            node_free(it);
            self->length--;
            return;
        }
    }
}

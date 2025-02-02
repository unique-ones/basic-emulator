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

#include <stdio.h>
#include <stdlib.h>

#include "list.h"

/// Allocates a new node instance
node_t *node_new(void *data) {
    node_t *self = (node_t *) malloc(sizeof(node_t));
    self->prev = NULL;
    self->next = NULL;
    self->data = data;
    return self;
}

/// Frees all associated data with the node except the data
void node_free(node_t *self) {
    free(self);
}

/// Allocates a new list instance
list_t *list_new(void) {
    list_t *self = (list_t *) malloc(sizeof(list_t));
    self->head = NULL;
    self->tail = NULL;
    self->length = 0;
    self->capacity = 0;
    return self;
}

/// Frees all data associated with the list
void list_free(list_t *self) {
    list_clear(self);
    free(self);
}

/// Clears the list
void list_clear(list_t *self) {
    node_t *it = self->head;
    node_t *tmp;

    while (it != NULL) {
        tmp = it;
        it = it->next;
        node_free(tmp);
    }
    self->head = NULL;
    self->tail = NULL;
    self->length = 0;
}

/// Appends the data to the list
void list_append(list_t *self, void *data) {
    node_t *node = node_new(data);

    if (self->head == NULL) {
        self->head = node;
        self->tail = node;
        node->prev = NULL;
        self->length++;
        return;
    }

    node_t *temp = self->tail;
    temp->next = node;
    self->tail = node;
    self->tail->prev = temp;
    self->tail->next = NULL;
    self->length++;
}

/// Sets the first node of the list
void list_set_head(list_t *self, void *data) {
    node_t *node = node_new(data);

    node->next = self->head;
    node->prev = NULL;

    if (self->head != NULL) {
        self->head->prev = node;
    }

    self->head = node;
    self->length++;
}

/// Alias to list_append
void list_set_tail(list_t *self, void *data) {
    list_append(self, data);
}

/// Inserts a node at the specified index
void list_insert(list_t *self, u32 idx, void *data) {
    if (self->head == NULL) {
        list_set_head(self, data);
        return;
    }

    node_t *node = node_new(data);
    node_t *temp = self->head;

    while (--idx) {
        temp = temp->next;
    }

    node->next = temp->next;
    node->prev = temp;

    temp->next = node;
    self->length++;
}

/// Retrieves the data of the node at the specified index in the list
void *list_at(list_t *self, u32 idx) {
    if (self->head == NULL || idx >= self->length) {
        return NULL;
    }

    u32 iterator;
    u32 iterator_limit;
    node_t *temp;
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
node_t *list_find(list_t *self, void *data, list_equal_function_t equal) {
    if (self->head == NULL || data == NULL || equal == NULL) {
        return NULL;
    }

    for (node_t *it = self->head; it != NULL; it = it->next) {
        if (equal(it->data, data)) {
            return it;
        }
    }
    return NULL;
}

/// Tries to remove the specified data using the specified equal function
void list_remove(list_t *self, void *data, list_equal_function_t equal) {
    if (self->head == NULL || data == NULL || equal == NULL) {
        return;
    }

    for (node_t *it = self->head; it != NULL; it = it->next) {
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

#include <stdio.h>
#include <stdlib.h>

#include "list.h"

node_t* node_new(void* data) {
    node_t* self = (node_t*) malloc(sizeof(node_t));
    self->prev = NULL;
    self->next = NULL;
    self->data = data;
    return self;
}

void node_free(node_t* self) {
    free(self);
}

void* node_data(node_t* self) {
    return self->data;
}

list_t* list_new() {
    list_t* self = (list_t*) malloc(sizeof(list_t));
    self->head = NULL;
    self->tail = NULL;
    self->length = 0;
    self->capacity = 0;
    return self;
}

void list_free(list_t* self) {
    node_t* it = self->head;
    node_t* tmp;

    while (it != NULL) {
        tmp = it;
        it = it->next;
        node_free(tmp);
    }

    free(self);
}

void list_append(list_t* self, void* data) {
    node_t* node = node_new(data);

    if (self->head == NULL) {
        self->head = node;
        node->prev = NULL;
        self->length++;
        return;
    }

    node_t* temp = self->head;

    while (temp->next != NULL) {
        temp = temp->next;
    }

    temp->next = node;
    node->prev = temp;
    self->tail = node;
    self->length++;
}

void list_set_head(list_t* self, void* data) {
    node_t* node = node_new(data);

    node->next = self->head;
    node->prev = NULL;

    if (self->head != NULL) {
        self->head->prev = node;
    }

    self->head = node;
    self->length++;
}

void list_set_tail(list_t* self, void* data) {
    list_append(self, data);
}

void list_insert(list_t* self, u32 idx, void* data) {
    if (self->head == NULL) {
        list_set_head(self, data);
        return;
    }

    node_t* node = node_new(data);
    node_t* temp = self->head;

    while (--idx) {
        temp = temp->next;
    }

    node->next = temp->next;
    node->prev = temp;

    temp->next = node;
    self->length++;
}

void* list_at(list_t* self, u32 idx) {
    if (self->head == NULL || idx >= self->length) {
        return NULL;
    }

    u32 iterator;
    u32 iterator_limit;
    node_t* temp;
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

node_t* list_find(list_t* self, void* data, list_equal_function_t equal) {
    if (self->head == NULL || data == NULL || equal == NULL) {
        return NULL;
    }

    for (node_t* it = self->head; it != NULL; it = it->next) {
        if (equal(it->data, data)) {
            return it;
        }
    }
    return NULL;
}
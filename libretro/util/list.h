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

#ifndef RETRO_COLLECTIONS_H
#define RETRO_COLLECTIONS_H

#include "types.h"

typedef struct node {
    struct node* prev;
    struct node* next;
    void* data;
} node_t;

/**
 * @brief allocate a new node instance
 * @param data data to be wrapped in a node
 * @return reference to node
 */
node_t* node_new(void* data);

/**
 * @brief free all associated data with the node except the data
 * @param self reference to node
 */
void node_free(node_t* self);

typedef struct list {
    node_t* head;
    node_t* tail;
    u32 length;
    u32 capacity;// pre-allocation?
} list_t;

typedef bool (*list_equal_function_t)(const void*, const void*);

/**
 * @brief allocate a new list instance
 * @return reference to list
 */
list_t* list_new(void);

/**
 * @brief free all data associated with the list and call node_free on each node
 * @param self reference to list
 */
void list_free(list_t* self);

/**
 * @brief insert a node at the specified index
 * @param self reference to list
 * @param idx index to insert node at
 * @param data data which is to be inserted
 */
void list_insert(list_t* self, u32 idx, void* data);

/**
 * @brief get the data of the node at the specified index in the list
 * @param self reference to the list
 * @param idx index of node
 * @return data of the node at the specified index
 */
void* list_at(list_t* self, u32 idx);

/**
 * @brief append data to the list
 * @param self reference to the list
 * @param data reference to the data which is to be appended
 */
void list_append(list_t* self, void* data);

/**
 * @brief set the first node of the list
 * @param self reference to the list
 * @param data reference to the data
 */
void list_set_head(list_t* self, void* data);

/**
 * @brief alias to list_append
 * @param self reference to the list
 * @param data reference to the data which is set as the tail (appended)
 */
void list_set_tail(list_t* self, void* data);

/**
 * @brief tries to find the specified data using the specified compare function
 * @param self reference to the list
 * @param data reference to the data which shall be found
 * @param compare compare function
 * @return node of the found data, NULL on failure
 */
node_t* list_find(list_t* self, void* data, list_equal_function_t equal);

#endif// RETRO_COLLECTIONS_H

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

#ifndef RETRO_UTIL_LIST_H
#define RETRO_UTIL_LIST_H

#include "types.h"

/// Forward declare node
typedef struct ListNode ListNode;

typedef struct ListNode {
    ListNode *prev;
    ListNode *next;
    void *data;
} ListNode;

/// Allocates a new node instance
/// @param data The data to be wrapped in a node
/// @return A reference to node
ListNode *node_new(void *data);

/// Frees all associated data with the node except the data
/// @param self A reference to node
void node_free(ListNode *self);

typedef struct LinkedList {
    ListNode *head;
    ListNode *tail;
    u32 length;
    u32 capacity;// pre-allocation?
} LinkedList;

typedef bool (*ListEqualFunction)(const void *, const void *);

/// Allocates a new list instance
/// @return A new list instance
LinkedList *linked_list_new(void);

/// Frees all data associated with the list
/// @param self The list handle
void linked_list_free(LinkedList *self);

/// Clears the list
/// @param self The list handle
void linked_list_clear(LinkedList *self);

/// Inserts a node at the specified index
/// @param self The list handle
/// @param idx The index to insert node at
/// @param data The data which is to be inserted
void linked_list_insert(LinkedList *self, u32 idx, void *data);

/// Retrieves the data of the node at the specified index in the list
/// @param self The list handle
/// @param idx The index of node
/// @return The data of the node at the specified index, or NULL
void *linked_list_at(LinkedList *self, u32 idx);

/// Appends the data to the list
/// @param self The list handle
/// @param data The data which is to be appended
void linked_list_append(LinkedList *self, void *data);

/// Sets the first node of the list
/// @param self The list handle
/// @param data The data
void linked_list_set_head(LinkedList *self, void *data);

/// Alias to linked_list_append
/// @param self The list handle
/// @param data The data which is set as the tail (appended)
void linked_list_set_tail(LinkedList *self, void *data);

/// Tries to find the specified data using the specified equal function
/// @param self The list handle
/// @param data The data which shall be found
/// @param equal The equality check function
/// @return The node of the found data or NULL on failure
ListNode *linked_list_find(LinkedList *self, void *data, ListEqualFunction equal);

/// Tries to remove the specified data using the specified equal function
/// @param self The list handle
/// @param data The data which shall be removed
/// @param equal The equality check function
void linked_list_remove(LinkedList *self, void *data, ListEqualFunction equal);

#endif// RETRO_UTIL_LIST_H

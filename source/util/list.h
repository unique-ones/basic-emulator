// Copyright (c) 2025 Elias Engelbert Plank

#ifndef RETRO_UTIL_LIST_H
#define RETRO_UTIL_LIST_H

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
static ListNode *node_new(void *data);

/// Frees all associated data with the node except the data
/// @param self A reference to node
static void node_free(ListNode *self);

typedef struct LinkedList {
    ListNode *head;
    ListNode *tail;
    usize length;
    usize capacity;// pre-allocation?
} LinkedList;

typedef b32 (*ListEqualFunction)(const void *, const void *);

/// Allocates a new list instance
/// @return A new list instance
static LinkedList *linked_list_new(void);

/// Frees all data associated with the list
/// @param self The list handle
static void linked_list_free(LinkedList *self);

/// Clears the list
/// @param self The list handle
static void linked_list_clear(LinkedList *self);

/// Inserts a node at the specified index
/// @param self The list handle
/// @param idx The index to insert node at
/// @param data The data which is to be inserted
static void linked_list_insert(LinkedList *self, usize idx, void *data);

/// Retrieves the data of the node at the specified index in the list
/// @param self The list handle
/// @param idx The index of node
/// @return The data of the node at the specified index, or NULL
static void *linked_list_at(LinkedList const *self, usize idx);

/// Appends the data to the list
/// @param self The list handle
/// @param data The data which is to be appended
static void linked_list_append(LinkedList *self, void *data);

/// Sets the first node of the list
/// @param self The list handle
/// @param data The data
static void linked_list_set_head(LinkedList *self, void *data);

/// Alias to linked_list_append
/// @param self The list handle
/// @param data The data which is set as the tail (appended)
static void linked_list_set_tail(LinkedList *self, void *data);

/// Tries to find the specified data using the specified equal function
/// @param self The list handle
/// @param data The data which shall be found
/// @param equal The equality check function
/// @return The node of the found data or NULL on failure
static ListNode *linked_list_find(LinkedList const *self, void const *data, ListEqualFunction equal);

/// Tries to remove the specified data using the specified equal function
/// @param self The list handle
/// @param data The data which shall be removed
/// @param equal The equality check function
static void linked_list_remove(LinkedList *self, void const *data, ListEqualFunction equal);

#endif// RETRO_UTIL_LIST_H

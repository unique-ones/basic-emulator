#ifndef RETRO_COLLECTIONS_LIST_H
#define RETRO_COLLECTIONS_LIST_H

#include <stddef.h>

typedef struct node {
    struct node *prev;
    struct node *next;
    void *data;
} node_t;

/**
 * @brief allocate a new node instance
 * @param data data to be wrapped in a node
 * @return reference to node
 */
node_t *node_new(void *data);

/**
 * @brief free all associated data with the node except the data 
 * @param reference to node
 */
void node_free(node_t *self);

/**
 * @brief get a reference to the data of the given node
 * @param reference to node
 * @return reference to data
 */
void *node_data(node_t *self);

typedef struct list {
    node_t *head;
    node_t *tail;
    size_t length;
    size_t capacity; // preallocation?
} list_t;

/**
 * @brief allocate a new list instance
 * @return reference to list
 */
list_t *list_new(void);

/**
 * @brief free all data associated with the list and call node_free on each node 
 * @param self reference to list
 */
void list_free(list_t *self);

/**
 * @brief insert a node at the specified index
 * @param self reference to list
 * @param idx index to insert node at
 * @param data data which is to be inserted
 */
void list_insert(list_t *self, size_t idx, void *data);

/**
 * @brief get the data of the node at the specified index in the list
 * @param self reference to the list
 * @param idx index of node
 * @return data of the node at the specified index
 */
void *list_at(list_t *self, size_t idx);

/**
 * @brief append data to the list
 * @param self reference to the list
 * @param data reference to the data which is to be appended
 */
void list_append(list_t *self, void *data);

/**
 * @brief set the first node of the list
 * @param self reference to the list
 * @param data reference to the data
 */
void list_set_head(list_t *self, void *data);

/**
 * @brief alias to list_append
 */
void list_set_tail(list_t *self, void *data);

#endif // RETRO_COLLECTIONS_LIST_H
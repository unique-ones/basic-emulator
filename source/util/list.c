// Copyright (c) 2025 Elias Engelbert Plank

/// Allocates a new node instance
static ListNode *node_new(void *data) {
    ListNode *self = malloc(sizeof(ListNode));
    self->prev = NULL;
    self->next = NULL;
    self->data = data;
    return self;
}

/// Frees all associated data with the node except the data
static void node_free(ListNode *self) {
    free(self);
}

/// Allocates a new list instance
static LinkedList *linked_list_new(void) {
    LinkedList *self = malloc(sizeof(LinkedList));
    self->head = NULL;
    self->tail = NULL;
    self->length = 0;
    self->capacity = 0;
    return self;
}

/// Frees all data associated with the list
static void linked_list_free(LinkedList *self) {
    linked_list_clear(self);
    free(self);
}

/// Clears the list
static void linked_list_clear(LinkedList *self) {
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
static void linked_list_append(LinkedList *self, void *data) {
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
static void linked_list_set_head(LinkedList *self, void *data) {
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
static void linked_list_set_tail(LinkedList *self, void *data) {
    linked_list_append(self, data);
}

/// Inserts a node at the specified index
static void linked_list_insert(LinkedList *self, usize idx, void *data) {
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
static void *linked_list_at(LinkedList const *self, usize const idx) {
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
static ListNode *linked_list_find(LinkedList const *self, void const *data, ListEqualFunction const equal) {
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
static void linked_list_remove(LinkedList *self, void const *data, ListEqualFunction const equal) {
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

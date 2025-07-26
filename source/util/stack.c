// Copyright (c) 2025 Elias Engelbert Plank

/// Allocates a new stack
Stack *stack_new(usize const capacity) {
    Stack *self = malloc(sizeof(Stack));
    self->capacity = capacity;
    self->size = 0;
    self->data = (void **) malloc(capacity * sizeof(void *));
    return self;
}

/// Grows the stack's capacity
void stack_grow(Stack *self, usize const capacity) {
    void **new_data = realloc(self->data, capacity * sizeof(void *));
    if (new_data) {
        self->data = new_data;
        self->capacity = capacity;
    } else {
        assert(false && "realloc failed");
    }
}

/// Frees the stack entries except for the data itself
void stack_free(Stack *self) {
    free(self->data);
    self->capacity = 0;
    self->size = 0;
    free(self);
}

/// Pushes the specified data onto the stack
void stack_push(Stack *self, void *data) {
    if (self->size == self->capacity) {
        // If we run out of memory, allocate twice the capacity
        stack_grow(self, self->capacity * 2);
    }
    self->data[self->size++] = data;
}

/// Pops data from the stack
void *stack_pop(Stack *self) {
    if (self->size == 0) {
        return NULL;
    }
    return self->data[--(self->size)];
}

/// Peeks the top element
void *stack_peek(Stack const *self) {
    if (self->size == 0) {
        return NULL;
    }
    return self->data[self->size - 1];
}

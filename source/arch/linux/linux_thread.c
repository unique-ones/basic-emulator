// Copyright (c) 2025 Elias Engelbert Plank

/// Creates a new thread with the specified runner
static Thread thread_create(ThreadRunner runner, void *arg) {
    pthread_t thread;
    pthread_create(&thread, NULL, runner, arg);
    return (Thread) thread;
}

typedef struct Mutex {
    pthread_mutex_t handle;
} Mutex;

/// Creates a new mutex
static Mutex *mutex_new(void) {
    Mutex *self = (Mutex *) malloc(sizeof(Mutex));
    pthread_mutex_init(&self->handle, NULL);
    return self;
}

/// Frees the mutex
static void mutex_free(Mutex *self) {
    pthread_mutex_destroy(&self->handle);
    free(self);
}

/// Exclusively locks the mutex
static void mutex_lock(Mutex *self) {
    pthread_mutex_lock(&self->handle);
}

/// Unlocks the mutex
static void mutex_unlock(Mutex *self) {
    pthread_mutex_unlock(&self->handle);
}

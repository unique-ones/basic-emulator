#include <pthread.h>
#include <stdlib.h>

#include "../thread.h"

thread_t thread_create(thread_runner runner, void* arg) {
    pthread_t thread;
    pthread_create(&thread, NULL, runner, arg);
    return (thread_t) thread;
}

typedef struct mutex {
    pthread_mutex_t handle;
} mutex_t;

mutex_t* mutex_new(void) {
    mutex_t* self = (mutex_t*) malloc(sizeof(mutex_t));
    pthread_mutex_init(&self->handle, NULL);
    return self;
}

void mutex_free(mutex_t* self) {
    pthread_mutex_destroy(&self->handle);
    free(self);
}

void mutex_lock(mutex_t* self) {
    pthread_mutex_lock(&self->handle);
}

void mutex_unlock(mutex_t* self) {
    pthread_mutex_unlock(&self->handle);
}

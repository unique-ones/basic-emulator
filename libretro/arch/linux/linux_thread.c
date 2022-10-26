#include <pthread.h>

#include "../thread.h"

typedef struct thread {
    pthread_t handle;
} thread_t;

typedef struct mutex {
    pthread_mutex_t handle;
} mutex_t;

thread_t* thread_new() {
    thread_t* thread = (thread_t*) malloc(sizeof(thread_t));
    thread->handle = 0;
    return thread;
}

void thread_free(thread_t* self) {
    free(self);
}
#include <Windows.h>

#include "../thread.h"

thread_t thread_create(thread_runner runner, void* arg) {
    thread_t thread = CreateThread(NULL, 0, runner, arg, 0, NULL);
    return thread;
}

typedef struct mutex {
    HANDLE handle;
} mutex_t;

mutex_t* mutex_new(void) {
    mutex_t* self = (mutex_t*) malloc(sizeof(mutex_t));
    self->handle = CreateMutexA(NULL, FALSE, NULL);
    return self;
}

void mutex_free(mutex_t* self) {
    CloseHandle(self->handle);
    self->handle = INVALID_HANDLE_VALUE;
    free(self);
}

void mutex_lock(mutex_t* self) {
    WaitForSingleObject(self->handle, INFINITE);
}

void mutex_unlock(mutex_t* self) {
    ReleaseMutex(self->handle);
}

// Copyright (c) 2025 Elias Engelbert Plank

#include <Windows.h>

#include "../thread.h"

/// Creates a new thread with the specified runner
Thread thread_create(ThreadRunner runner, void *arg) {
    Thread thread = CreateThread(NULL, 0, runner, arg, 0, NULL);
    return thread;
}

typedef struct Mutex {
    HANDLE handle;
} Mutex;

/// Creates a new mutex
Mutex *mutex_new(void) {
    Mutex *self = (Mutex *) malloc(sizeof(mutex_t));
    self->handle = CreateMutexA(NULL, FALSE, NULL);
    return self;
}

/// Frees the mutex
void mutex_free(Mutex *self) {
    CloseHandle(self->handle);
    self->handle = INVALID_HANDLE_VALUE;
    free(self);
}

/// Exclusively locks the mutex
void mutex_lock(Mutex *self) {
    WaitForSingleObject(self->handle, INFINITE);
}

/// Unlocks the mutex
void mutex_unlock(Mutex *self) {
    ReleaseMutex(self->handle);
}

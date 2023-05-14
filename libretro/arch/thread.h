#ifndef RETRO_ARCH_THREAD_H
#define RETRO_ARCH_THREAD_H

#include "../types.h"

typedef u32 thread_id;
typedef void* (*thread_runner)(void*);

/**
 * create a new thread with the specified runner
 * @param runner runner for the thread
 * @param arg arguments that are passed to the runner
 * @return thread id
 */
thread_id thread_create(thread_runner runner, void* arg);

typedef struct mutex mutex_t;

mutex_t* mutex_new(void);
void mutex_free(mutex_t* self);
void mutex_lock(mutex_t* self);
void mutex_unlock(mutex_t* self);

#endif// RETRO_ARCH_THREAD_H
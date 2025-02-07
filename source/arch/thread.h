// Copyright (c) 2025 Elias Engelbert Plank

#ifndef RETRO_ARCH_THREAD_H
#define RETRO_ARCH_THREAD_H

#include "core/types.h"

#ifdef LIBRETRO_PLATFORM_WIN32

typedef void *Thread;
typedef unsigned long (*ThreadRunner)(void *);

#else

typedef u32 Thread;
typedef void *(*ThreadRunner)(void *);

#endif

/// Creates a new thread with the specified runner
/// @param runner The runner function for the thread
/// @param arg The argument that is passed to the runner
/// @return A thread handle
Thread thread_create(ThreadRunner runner, void *arg);

typedef struct Mutex Mutex;

/// Creates a new mutex
/// @return A new mutex
Mutex *mutex_new(void);

/// Frees the mutex
/// @param self The mutex handle
void mutex_free(Mutex *self);

/// Exclusively locks the mutex
/// @param self The mutex handle
void mutex_lock(Mutex *self);

/// Unlocks the mutex
/// @param self The mutex handle
void mutex_unlock(Mutex *self);

#endif// RETRO_ARCH_THREAD_H

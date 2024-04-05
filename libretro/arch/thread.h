//
// MIT License
//
// Copyright (c) 2024 Elias Engelbert Plank
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef RETRO_ARCH_THREAD_H
#define RETRO_ARCH_THREAD_H

#include "../types.h"

#ifdef LIBRETRO_PLATFORM_WIN32

typedef void *thread_t;
typedef unsigned long (*thread_runner)(void *);

#else

typedef u32 thread_t;
typedef void *(*thread_runner)(void *);

#endif

/// Creates a new thread with the specified runner
/// @param runner The runner function for the thread
/// @param arg The argument that is passed to the runner
/// @return A thread handle
thread_t thread_create(thread_runner runner, void *arg);

typedef struct mutex mutex_t;

/// Creates a new mutex
/// @return A new mutex
mutex_t *mutex_new(void);

/// Frees the mutex
/// @param self The mutex handle
void mutex_free(mutex_t *self);

/// Exclusively locks the mutex
/// @param self The mutex handle
void mutex_lock(mutex_t *self);

/// Unlocks the mutex
/// @param self The mutex handle
void mutex_unlock(mutex_t *self);

#endif// RETRO_ARCH_THREAD_H

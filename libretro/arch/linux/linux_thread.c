//
// MIT License
//
// Copyright (c) 2023 Elias Engelbert Plank
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

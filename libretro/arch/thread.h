#ifndef RETRO_ARCH_THREAD_H
#define RETRO_ARCH_THREAD_H

#include "../types.h"

/**
 * forward declarations of platform specific handles
*/
typedef struct thread thread_t;
typedef struct mutex mutex_t;

/**
 * @brief allocate a platform specific thread instance
 * @return reference to thread instance
*/
thread_t* thread_new();

/**
 * @brief free the thread instance
 * @param self reference to thread instance
*/
void thread_free(thread_t* self);

#endif // RETRO_ARCH_THREAD_H
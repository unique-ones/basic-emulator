// Copyright (c) 2025 Elias Engelbert Plank

#ifndef RETRO_ARCH_TIME_H
#define RETRO_ARCH_TIME_H

#include "core/types.h"

/// Sends the current thread of execution to sleep for the specified time
/// @param milliseconds The time in milliseconds
void time_sleep(u32 milliseconds);

#endif// RETRO_ARCH_TIME_H

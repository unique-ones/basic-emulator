// Copyright (c) 2025 Elias Engelbert Plank

#ifndef RETRO_UTIL_RANDOM_H
#define RETRO_UTIL_RANDOM_H

/// Seeds the random number generator
/// @param seed The seed value
void random_seed(u64 seed);

/// Retrieves an unsigned 64-bit random number
/// @return The random number
u64 random_u64(void);

#endif// RETRO_UTIL_RANDOM_H

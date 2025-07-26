// Copyright (c) 2025 Elias Engelbert Plank

static u64 random_state = 42;

/// Seeds the random number generator
void random_seed(u64 const seed) {
    random_state = seed;
    random_u64();
}

/// Retrieves an unsigned 64-bit random number
u64 random_u64(void) {
    u64 x = random_state;
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    random_state = x;
    return random_state;
}

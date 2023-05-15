#include "random.h"

static u64 random_state = 42;

void random_seed(u64 seed) {
    random_state = seed;
    random_u64();
}

u64 random_u64(void) {
    u64 x = random_state;
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    random_state = x;
    return random_state;
}

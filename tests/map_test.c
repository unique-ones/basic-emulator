// Copyright (c) 2025 Elias Engelbert Plank

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <libretro/gpu/buffer.h>
#include <libretro/util/map.h>

typedef struct Uniform {
    u32 location;
    ShaderType type;
} Uniform;

bool uniform_equal(Uniform *a, Uniform *b) {
    if (a->location != b->location) {
        return false;
    }
    return a->type == b->type;
}

static void map_test(void) {
    Uniform *scale = &(Uniform){ .location = 0, .type = FLOAT4 };
    Uniform *curvature = &(Uniform){ .location = 1, .type = FLOAT2 };

    HashMap *map = hash_map_new();
    hash_map_insert(map, "uniform_scale", scale);
    hash_map_insert(map, "uniform_curvature", curvature);

    Uniform *scale_find = hash_map_find(map, "uniform_scale");
    assert(scale_find != NULL && uniform_equal(scale_find, scale) && "map_find mismatch");

    Uniform *altered_scale = &(Uniform){ .location = 10, .type = FLOAT4 };
    hash_map_insert(map, "uniform_scale", altered_scale);

    scale_find = hash_map_find(map, "uniform_scale");
    assert(scale_find != NULL && uniform_equal(scale_find, altered_scale) && "map_find mismatch");

    Uniform *curvature_find = hash_map_find(map, "uniform_curvature");
    assert(curvature_find != NULL && uniform_equal(curvature_find, curvature) && "map_find mismatch");

    hash_map_free(map);
}

int main(int argc, char **argv) {
    map_test();
    return 0;
}
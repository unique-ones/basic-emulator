#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <libretro/buffer.h>
#include <libretro/map.h>

typedef struct uniform {
    u32 location;
    shader_type_t type;
} uniform_t;

bool uniform_equal(uniform_t* a, uniform_t* b) {
    if (a->location != b->location) {
        return false;
    }
    return a->type == b->type;
}

static void map_test() {
    uniform_t* scale = &(uniform_t){ .location = 0, .type = FLOAT4 };
    uniform_t* curvature = &(uniform_t){ .location = 1, .type = FLOAT2 };

    map_t* map = map_new();
    map_insert(map, "uniform_scale", scale);
    map_insert(map, "uniform_curvature", curvature);

    uniform_t* scale_find = map_find(map, "uniform_scale");
    assert(scale_find != NULL && uniform_equal(scale_find, scale) && "map_find mismatch");

    uniform_t* altered_scale = &(uniform_t){ .location = 10, .type = FLOAT4 };
    map_insert(map, "uniform_scale", altered_scale);

    scale_find = map_find(map, "uniform_scale");
    assert(scale_find != NULL && uniform_equal(scale_find, altered_scale) && "map_find mismatch");

    uniform_t* curvature_find = map_find(map, "uniform_curvature");
    assert(curvature_find != NULL && uniform_equal(curvature_find, curvature) && "map_find mismatch");

    map_free(map);
}

int main(int argc, char** argv) {
    map_test();
    return 0;
}
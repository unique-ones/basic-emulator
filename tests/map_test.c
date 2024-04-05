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

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <libretro/gpu/buffer.h>
#include <libretro/util/map.h>

typedef struct uniform {
    u32 location;
    shader_type_t type;
} uniform_t;

bool uniform_equal(uniform_t *a, uniform_t *b) {
    if (a->location != b->location) {
        return false;
    }
    return a->type == b->type;
}

static void map_test(void) {
    uniform_t *scale = &(uniform_t){ .location = 0, .type = FLOAT4 };
    uniform_t *curvature = &(uniform_t){ .location = 1, .type = FLOAT2 };

    map_t *map = map_new();
    map_insert(map, "uniform_scale", scale);
    map_insert(map, "uniform_curvature", curvature);

    uniform_t *scale_find = map_find(map, "uniform_scale");
    assert(scale_find != NULL && uniform_equal(scale_find, scale) && "map_find mismatch");

    uniform_t *altered_scale = &(uniform_t){ .location = 10, .type = FLOAT4 };
    map_insert(map, "uniform_scale", altered_scale);

    scale_find = map_find(map, "uniform_scale");
    assert(scale_find != NULL && uniform_equal(scale_find, altered_scale) && "map_find mismatch");

    uniform_t *curvature_find = map_find(map, "uniform_curvature");
    assert(curvature_find != NULL && uniform_equal(curvature_find, curvature) && "map_find mismatch");

    map_free(map);
}

int main(int argc, char **argv) {
    map_test();
    return 0;
}
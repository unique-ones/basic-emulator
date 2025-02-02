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
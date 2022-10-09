#include "math.h"

s32 s32_max(s32 a, s32 b) {
    return a > b ? a : b;
}

s32 s32_min(s32 a, s32 b) {
    return a < b ? a : b;
}

s32 s32_clamp(s32 n, s32 min, s32 max) {
    return s32_min(max, s32_max(n, min));
}

void f32mat4_create_identity(f32mat4_t* matrix) {
    matrix->value[0].x = 1.0f;
    matrix->value[0].y = 0.0f;
    matrix->value[0].z = 0.0f;
    matrix->value[0].w = 0.0f;
    matrix->value[1].x = 0.0f;
    matrix->value[1].y = 1.0f;
    matrix->value[1].z = 0.0f;
    matrix->value[1].w = 0.0f;
    matrix->value[2].x = 0.0f;
    matrix->value[2].y = 0.0f;
    matrix->value[2].z = 1.0f;
    matrix->value[2].w = 0.0f;
    matrix->value[3].x = 0.0f;
    matrix->value[3].y = 0.0f;
    matrix->value[3].z = 0.0f;
    matrix->value[3].w = 1.0f;
}

void f32mat4_create_ortho(f32mat4_t* matrix, f32 left, f32 right, f32 bottom, f32 top) {
    f32mat4_create_identity(matrix);
    matrix->value[0].x = 2.0f / (right - left);
    matrix->value[1].y = 2.0f / (top - bottom);
    matrix->value[2].z = -1.0f;
    matrix->value[3].x = -(right + left) / (right - left);
    matrix->value[3].y = -(top + bottom) / (top - bottom);
}
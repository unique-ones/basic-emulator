//
// MIT License
//
// Copyright (c) 2023 Elias Engelbert Plank
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

#include "math.h"

/// Returns the greater of the two passed variables
s32 s32_max(s32 a, s32 b) {
    return a > b ? a : b;
}

/// Returns the smaller of the two passed variables
s32 s32_min(s32 a, s32 b) {
    return a < b ? a : b;
}

/// Clamps the specified value to the given bounds
s32 s32_clamp(s32 n, s32 min, s32 max) {
    return s32_min(max, s32_max(n, min));
}

/// Creates an identity matrix
void f32mat4_create_identity(f32mat4_t* self) {
    self->value[0].x = 1.0f;
    self->value[0].y = 0.0f;
    self->value[0].z = 0.0f;
    self->value[0].w = 0.0f;
    self->value[1].x = 0.0f;
    self->value[1].y = 1.0f;
    self->value[1].z = 0.0f;
    self->value[1].w = 0.0f;
    self->value[2].x = 0.0f;
    self->value[2].y = 0.0f;
    self->value[2].z = 1.0f;
    self->value[2].w = 0.0f;
    self->value[3].x = 0.0f;
    self->value[3].y = 0.0f;
    self->value[3].z = 0.0f;
    self->value[3].w = 1.0f;
}

/// Creates an orthogonal projection matrix
void f32mat4_create_orthogonal(f32mat4_t* self, f32 left, f32 right, f32 bottom, f32 top) {
    f32mat4_create_identity(self);
    self->value[0].x = 2.0f / (right - left);
    self->value[1].y = 2.0f / (top - bottom);
    self->value[2].z = -1.0f;
    self->value[3].x = -(right + left) / (right - left);
    self->value[3].y = -(top + bottom) / (top - bottom);
}

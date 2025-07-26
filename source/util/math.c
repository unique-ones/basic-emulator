// Copyright (c) 2025 Elias Engelbert Plank

/// Returns the greater of the two passed variables
static s32 s32_max(s32 const a, s32 const b) {
    return a > b ? a : b;
}

/// Returns the smaller of the two passed variables
static s32 s32_min(s32 const a, s32 const b) {
    return a < b ? a : b;
}

/// Clamps the specified value to the given bounds
static s32 s32_clamp(s32 const n, s32 const min, s32 const max) {
    return s32_min(max, s32_max(n, min));
}

/// Returns the greater of the two passed variables
static u32 u32_max(u32 const a, u32 const b) {
    return a > b ? a : b;
}

/// Returns the smaller of the two passed variables
static u32 u32_min(u32 const a, u32 const b) {
    return a < b ? a : b;
}

/// Returns the greater of the two passed variables
static s64 s64_max(s64 const a, s64 const b) {
    return a > b ? a : b;
}

/// Returns the smaller of the two passed variables
static s64 s64_min(s64 const a, s64 const b) {
    return a < b ? a : b;
}

/// Clamps the specified value to the given bounds
static s64 s64_clamp(s64 const n, s64 const min, s64 const max) {
    return s64_min(max, s64_max(n, min));
}

/// Creates an identity matrix
static void f32mat4_create_identity(F32Mat4 *self) {
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
static void f32mat4_create_orthogonal(F32Mat4 *self, f32 const left, f32 const right, f32 const bottom, f32 const top) {
    f32mat4_create_identity(self);
    self->value[0].x = 2.0f / (right - left);
    self->value[1].y = 2.0f / (top - bottom);
    self->value[2].z = -1.0f;
    self->value[3].x = -(right + left) / (right - left);
    self->value[3].y = -(top + bottom) / (top - bottom);
}

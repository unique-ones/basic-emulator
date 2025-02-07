// Copyright (c) 2025 Elias Engelbert Plank

#ifndef RETRO_TYPES_H
#define RETRO_TYPES_H

#include <stdint.h>

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef u64 usize;
typedef s64 ssize;

typedef u8 bool;

typedef float f32;
typedef double f64;

#define true 1
#define false 0

#define STACK_ARRAY_SIZE(stack_array) (sizeof stack_array / sizeof stack_array[0])

#define ASSERT(x, ...)                \
    if (!(x)) {                       \
        fprintf(stderr, __VA_ARGS__); \
        abort();                      \
    }

typedef struct S32Vector2 {
    s32 x;
    s32 y;
} S32Vector2;

typedef struct S32Vector3 {
    s32 x;
    s32 y;
    s32 z;
} S32Vector3;

typedef struct S32Vector4 {
    s32 x;
    s32 y;
    s32 z;
    s32 w;
} S32Vector4;

typedef struct U32Vector2 {
    u32 x;
    u32 y;
} U32Vector2;

typedef struct U32Vector3 {
    u32 x;
    u32 y;
    u32 z;
} U32Vector3;

typedef struct U32Vector4 {
    u32 x;
    u32 y;
    u32 z;
    u32 w;
} U32Vector4;

typedef struct F32Vector2 {
    f32 x;
    f32 y;
} F32Vector2;

typedef struct F32Vector3 {
    f32 x;
    f32 y;
    f32 z;
} F32Vector3;

typedef struct F32Vector4 {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} F32Vector4;

typedef struct F32Mat4 {
    F32Vector4 value[4];
} F32Mat4;

typedef struct Vertex {
    F32Vector3 position;
    F32Vector3 color;
    F32Vector2 texture;
} Vertex;

typedef struct BinaryBuffer {
    char *data;
    u32 size;
} BinaryBuffer;

#endif// RETRO_TYPES_H

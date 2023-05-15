#ifndef RETRO_TYPES_H
#define RETRO_TYPES_H

#include <stdint.h>

#include <glad/glad.h>
#include <stddef.h>

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

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

typedef struct s32vec2 {
    s32 x;
    s32 y;
} s32vec2_t;

typedef struct s32vec3 {
    s32 x;
    s32 y;
    s32 z;
} s32vec3_t;

typedef struct s32vec4 {
    s32 x;
    s32 y;
    s32 z;
    s32 w;
} s32vec4_t;

typedef struct f32vec2 {
    f32 x;
    f32 y;
} f32vec2_t;

typedef struct f32vec3 {
    f32 x;
    f32 y;
    f32 z;
} f32vec3_t;

typedef struct f32vec4 {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} f32vec4_t;

typedef struct f32mat4 {
    f32vec4_t value[4];
} f32mat4_t;

typedef struct vertex {
    f32vec3_t position;
    f32vec3_t color;
    f32vec2_t texture;
} vertex_t;

typedef struct binary_buffer {
    char* data;
    u32 size;
} binary_buffer_t;

#endif// RETRO_TYPES_H

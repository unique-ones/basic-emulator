#ifndef RETRO_TEXT_H
#define RETRO_TEXT_H

#include "types.h"

typedef struct text_cursor {
    char* data;
    s32 fill;
    s32 cursor;
    u32 capacity;
    bool submit;
} text_cursor_t;

/**
 * @brief creates an input buffer
 * @param self buffer handle
 * @param capacity capacity of the input buffer
 */
void text_cursor_create(text_cursor_t* self, u32 capacity);

/**
 * @brief destroys the input buffer and frees its data
 * @param self buffer handle
 */
void text_cursor_destroy(text_cursor_t* self);

/**
 * @brief inserts a character into the input buffer
 * @param self buffer handle
 * @param data data
 * @return bool
 */
bool text_cursor_emplace(text_cursor_t* self, char data);

/**
 * @brief removes data at the cursor, reorders buffer to be continuous in memory
 * @param self buffer handle
 * @return bool
 */
bool text_cursor_remove(text_cursor_t* self);

/**
 * @brief advances the cursor by the specified offset
 * @param self input buffer handle
 * @param offset offset
 */
void text_cursor_advance(text_cursor_t* self, s32 offset);

/**
 * @brief checks if the input buffer is full
 * @return bool
 */
bool text_cursor_is_full(text_cursor_t* self);

/**
 * @brief clears the text queue
 * @param self text queue instance
 */
void text_cursor_clear(text_cursor_t* self);

typedef struct text_entry {
    struct text_entry* prev;
    struct text_entry* next;
    char* data;
    u32 length;
} text_entry_t;

text_entry_t* text_entry_new(char* data, u32 length);
void text_entry_free(text_entry_t* self);

typedef struct text_queue {
    text_entry_t* begin;
    text_entry_t* end;
    u32 entries;
} text_queue_t;

text_queue_t* text_queue_new(void);
void text_queue_free(text_queue_t* self);
void text_queue_clear(text_queue_t* self);
void text_queue_push(text_queue_t* self, char* data, u32 length);
void text_queue_push_format(text_queue_t* self, const char* fmt, ...);

#endif// RETRO_TEXT_H

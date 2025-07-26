// Copyright (c) 2025 Elias Engelbert Plank

#ifndef RETRO_UTIL_TEXT_H
#define RETRO_UTIL_TEXT_H

typedef struct TextCursor {
    char *data;
    ssize fill;
    ssize cursor;
    ssize capacity;
    b32 submit;
} TextCursor;

/// Creates a text cursor with the specified capacity
/// @param self The text cursor handle
/// @param capacity The capacity of the text cursor
static void text_cursor_create(TextCursor *self, ssize capacity);

/// Destroys the text cursor and frees its data
/// @param self The text cursor handle
static void text_cursor_destroy(TextCursor const *self);

/// Inserts a character into the text cursor buffer at the cursor position
/// @param self The text cursor handle
/// @param data The character
/// @return A b32ean value that indicates whether the character could be emplaced
static b32 text_cursor_emplace(TextCursor *self, char data);

/// Removes data at the cursor, reorders buffer to be continuous in memory
/// @param self The text cursor handle
/// @return A b32ean value that indicates whether a character could be removed
static b32 text_cursor_remove(TextCursor *self);

/// Advances the cursor by the specified offset
/// @param self The text cursor handle
/// @param offset The offset
static void text_cursor_advance(TextCursor *self, ssize offset);

/// Checks if the text cursor buffer is full
/// @return A b32ean value that indicates whether the buffer is full
static b32 text_cursor_is_full(TextCursor const *self);

/// Clears the buffer of the text cursor
/// @param self The text cursor handle
static void text_cursor_clear(TextCursor *self);

typedef struct TextEntry {
    struct TextEntry *prev;
    struct TextEntry *next;
    char *data;
    usize length;
} TextEntry;

/// Creates a new text entry
/// @param data The text data
/// @param length The length of the text
/// @return A new text entry
static TextEntry *text_entry_new(char const *data, usize length);

/// Frees the text entry
/// @param self The text entry handle
static void text_entry_free(TextEntry *self);

typedef struct TextQueue {
    TextEntry *begin;
    TextEntry *end;
    usize entries;
} TextQueue;

/// Creates a new text queue
/// @return A new text queue
static TextQueue *text_queue_new(void);

/// Frees the text queue
/// @param self The text queue handle
static void text_queue_free(TextQueue *self);

/// Clears the text queue
/// @param self The text queue handle
static void text_queue_clear(TextQueue *self);

/// Pushes data to the text queue
/// @param self The text queue handle
/// @param data The text data
/// @param length The text length
static void text_queue_push(TextQueue *self, char const *data, usize length);

/// Pushes format strings to the text queue
/// @param self The text queue handle
/// @param fmt The text format string
/// @param ... The variadic arguments
static void text_queue_push_format(TextQueue *self, const char *fmt, ...);

#endif// RETRO_UTIL_TEXT_H

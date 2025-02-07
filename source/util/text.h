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

#ifndef RETRO_UTIL_TEXT_H
#define RETRO_UTIL_TEXT_H

#include "core/types.h"

typedef struct TextCursor {
    char *data;
    ssize fill;
    ssize cursor;
    ssize capacity;
    bool submit;
} TextCursor;

/// Creates a text cursor with the specified capacity
/// @param self The text cursor handle
/// @param capacity The capacity of the text cursor
void text_cursor_create(TextCursor *self, ssize capacity);

/// Destroys the text cursor and frees its data
/// @param self The text cursor handle
void text_cursor_destroy(TextCursor const *self);

/// Inserts a character into the text cursor buffer at the cursor position
/// @param self The text cursor handle
/// @param data The character
/// @return A boolean value that indicates whether the character could be emplaced
bool text_cursor_emplace(TextCursor *self, char data);

/// Removes data at the cursor, reorders buffer to be continuous in memory
/// @param self The text cursor handle
/// @return A boolean value that indicates whether a character could be removed
bool text_cursor_remove(TextCursor *self);

/// Advances the cursor by the specified offset
/// @param self The text cursor handle
/// @param offset The offset
void text_cursor_advance(TextCursor *self, ssize offset);

/// Checks if the text cursor buffer is full
/// @return A boolean value that indicates whether the buffer is full
bool text_cursor_is_full(TextCursor const *self);

/// Clears the buffer of the text cursor
/// @param self The text cursor handle
void text_cursor_clear(TextCursor *self);

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
TextEntry *text_entry_new(char const *data, usize length);

/// Frees the text entry
/// @param self The text entry handle
void text_entry_free(TextEntry *self);

typedef struct TextQueue {
    TextEntry *begin;
    TextEntry *end;
    usize entries;
} TextQueue;

/// Creates a new text queue
/// @return A new text queue
TextQueue *text_queue_new(void);

/// Frees the text queue
/// @param self The text queue handle
void text_queue_free(TextQueue *self);

/// Clears the text queue
/// @param self The text queue handle
void text_queue_clear(TextQueue *self);

/// Pushes data to the text queue
/// @param self The text queue handle
/// @param data The text data
/// @param length The text length
void text_queue_push(TextQueue *self, char const *data, usize length);

/// Pushes format strings to the text queue
/// @param self The text queue handle
/// @param fmt The text format string
/// @param ... The variadic arguments
void text_queue_push_format(TextQueue *self, const char *fmt, ...);

#endif// RETRO_UTIL_TEXT_H

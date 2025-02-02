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

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

/// Creates a new token instance
token_t *token_new(token_type_t type, char *lexeme, u32 length) {
    token_t *self = malloc(sizeof(token_t));
    self->prev = NULL;
    self->next = NULL;
    self->type = type;
    self->lexeme = (char *) malloc(length);
    self->length = length;
    memcpy(self->lexeme, lexeme, length);
    return self;
}

/// Frees the specified token instance
void token_free(token_t *self) {
    self->prev = NULL;
    self->next = NULL;
    free(self->lexeme);
    self->length = 0;
    free(self);
}

/// Creates a new token list instance
token_list_t *token_list_new(void) {
    token_list_t *self = malloc(sizeof(token_list_t));
    self->begin = NULL;
    self->end = NULL;
    self->tokens = 0;
    return self;
}

/// Clears the specified token list (i.e. deletes all tokens)
void token_list_clear(token_list_t *self) {
    token_t *it = self->begin;
    token_t *tmp;

    while (it != NULL) {
        tmp = it;
        it = it->next;
        token_free(tmp);
    }
    self->begin = NULL;
    self->end = NULL;
    self->tokens = 0;
}

/// Frees the specified token list (i.e. deletes tokens and free list)
void token_list_free(token_list_t *self) {
    token_list_clear(self);
    free(self);
}

/// Pushes a token to the specified token list
void token_list_push(token_list_t *self, token_type_t type, char *lexeme, u32 length) {
    token_t *token = token_new(type, lexeme, length);
    if (self->begin == NULL) {
        self->begin = token;
        self->end = token;
        token->prev = NULL;
        self->tokens++;
        return;
    }

    token_t *tmp = self->begin;
    while (tmp->next != NULL) {
        tmp = tmp->next;
    }

    tmp->next = token;
    token->prev = tmp;
    self->end = token;
    self->tokens++;
}

typedef struct string_iterator {
    char *base;
    u32 length;
    u32 index;
} string_iterator_t;

/// Creates a string iterator
static void string_iterator_create(string_iterator_t *self, char *base, u32 length) {
    self->base = base;
    self->length = length;
    self->index = 0;
}

/// Advances the string iterator by one
static void string_iterator_advance(string_iterator_t *self) {
    if (self->index < self->length - 1) {
        self->index++;
    }
}

/// Retrieves the current char of the iterator
static char string_iterator_current(string_iterator_t *self) {
    return self->base[self->index];
}

/// Retrieves the next char of the iterator
static char string_iterator_next(string_iterator_t *self) {
    return self->base[self->index + 1];
}

/// Checks if the iterator is at the end
static bool string_iterator_end(string_iterator_t *self) {
    return self->index == self->length - 1;
}

/// Checks if the specified char is a trivial token
static bool tokenize_is_trivial_token(char current) {
    switch (current) {
        case ':':
        case '(':
        case ')':
        case '?':
        case '=':
        case '+':
        case '-':
        case '*':
        case '/':
        case ',':
        case '>':
        case '<':
        case '^':
        case '$':
            return true;
        default:
            return false;
    }
}

/// Converts a trivial token to the corresponding token type
static token_type_t tokenize_trivial_to_type(char current) {
    switch (current) {
        case ':':
            return TOKEN_COLON;
        case '(':
            return TOKEN_LEFT_PARENTHESIS;
        case ')':
            return TOKEN_RIGHT_PARENTHESIS;
        case '?':
            return TOKEN_QUESTION_MARK;
        case '=':
            return TOKEN_EQUAL_SIGN;
        case '+':
            return TOKEN_PLUS;
        case '-':
            return TOKEN_MINUS;
        case '*':
            return TOKEN_ASTERISK;
        case '/':
            return TOKEN_SLASH;
        case ',':
            return TOKEN_COMMA;
        case '>':
            return TOKEN_GREATER_THAN;
        case '<':
            return TOKEN_LESS_THAN;
        case '^':
            return TOKEN_CIRCUMFLEX;
        case '$':
            return TOKEN_DOLLAR;
        default:
            return TOKEN_INVALID;
    }
}

/// Checks if two string views are equal
static bool string_view_equal(const char *first, u32 first_size, const char *second, u32 second_size) {
    if (first_size != second_size) {
        return false;
    }
    return strncmp(first, second, first_size) == 0;
}

/// Tokenizes the specified data
token_list_t *tokenize(char *data, u32 length) {
    token_list_t *list = token_list_new();
    string_iterator_t iterator;
    string_iterator_create(&iterator, data, length);
    while (!string_iterator_end(&iterator)) {
        // ignore whitespaces
        if (isspace(string_iterator_current(&iterator))) {
            string_iterator_advance(&iterator);
            continue;
        }

        // check if we have a trivial token (i.e. token of length 1)
        char current_trivial = string_iterator_current(&iterator);
        if (tokenize_is_trivial_token(current_trivial)) {
            token_type_t type = tokenize_trivial_to_type(current_trivial);
            token_list_push(list, type, &current_trivial, 1);
            string_iterator_advance(&iterator);
            continue;
        }

        // string-literals
        if (string_iterator_current(&iterator) == '"') {
            string_iterator_advance(&iterator);
            char *lexeme = iterator.base + iterator.index;
            u32 begin_index = iterator.index;

            char previous = 0;
            while (string_iterator_current(&iterator) != '"' || previous == '\\') {
                previous = string_iterator_current(&iterator);
                string_iterator_advance(&iterator);
            }
            u32 end_index = iterator.index;
            u32 lexeme_length = end_index - begin_index;
            token_list_push(list, TOKEN_STRING, lexeme, lexeme_length);
        }

        // alphabetic characters
        if (isalpha(string_iterator_current(&iterator))) {
            char *lexeme = iterator.base + iterator.index;
            u32 begin_index = iterator.index;
            while (isalpha(string_iterator_current(&iterator))) {
                string_iterator_advance(&iterator);
            }
            u32 end_index = iterator.index;
            u32 lexeme_length = end_index - begin_index;

            if (string_view_equal(lexeme, lexeme_length, "PRINT", 5)) {
                token_list_push(list, TOKEN_PRINT, lexeme, lexeme_length);
            } else if (string_view_equal(lexeme, lexeme_length, "FN", 2)) {
                token_list_push(list, TOKEN_FN, lexeme, lexeme_length);
            } else if (string_view_equal(lexeme, lexeme_length, "DEF", 3)) {
                token_list_push(list, TOKEN_DEF, lexeme, lexeme_length);
            } else if (string_view_equal(lexeme, lexeme_length, "LET", 3)) {
                token_list_push(list, TOKEN_LET, lexeme, lexeme_length);
            } else if (string_view_equal(lexeme, lexeme_length, "RUN", 3)) {
                token_list_push(list, TOKEN_RUN, lexeme, lexeme_length);
            } else if (string_view_equal(lexeme, lexeme_length, "EXIT", 4)) {
                token_list_push(list, TOKEN_EXIT, lexeme, lexeme_length);
            } else if (string_view_equal(lexeme, lexeme_length, "CLEAR", 5)) {
                token_list_push(list, TOKEN_CLEAR, lexeme, lexeme_length);
            } else {
                token_list_push(list, TOKEN_IDENTIFIER, lexeme, lexeme_length);
            }
            continue;
        }

        // numbers
        if (isdigit(string_iterator_current(&iterator))) {
            char *lexeme = iterator.base + iterator.index;
            token_type_t type = TOKEN_NUMBER;
            u32 begin_index = iterator.index;
            while (isdigit(string_iterator_current(&iterator))) {
                string_iterator_advance(&iterator);
            }
            if (string_iterator_current(&iterator) == '.' && isdigit(string_iterator_next(&iterator))) {
                type = TOKEN_NUMBER_FLOAT;
                do {
                    string_iterator_advance(&iterator);
                } while (isdigit(string_iterator_current(&iterator)));
            }
            token_list_push(list, type, lexeme, iterator.index - begin_index);
            continue;
        }

        string_iterator_advance(&iterator);
    }
    return list;
}

/// Checks if the token iterator reached the end
bool token_iterator_end(token_iterator_t *self) {
    return self->current == self->end;
}

/// Returns an invalid token
token_t *token_iterator_invalid(void) {
    static token_t token = { 0 };
    token.type = TOKEN_INVALID;
    token.lexeme = "";
    token.length = 0;
    token.prev = NULL;
    token.next = NULL;
    return &token;
}

/// Retrieves the current token
token_t *token_iterator_current(token_iterator_t *self) {
    if (self->current) {
        return self->current;
    }
    return token_iterator_invalid();
}

/// Retrieves the next token
token_t *token_iterator_next(token_iterator_t *self) {
    if (!token_iterator_end(self)) {
        return self->current->next;
    }
    return token_iterator_invalid();
}

/// Advances the token cursor by one
void token_iterator_advance(token_iterator_t *self) {
    if (!token_iterator_end(self)) {
        self->current = self->current->next;
    } else {
        self->current = token_iterator_invalid();
    }
}

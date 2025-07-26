// Copyright (c) 2025 Elias Engelbert Plank

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

/// Creates a new token instance
Token *token_new(TokenType type, char const *lexeme, usize const length) {
    Token *self = malloc(sizeof(Token));
    self->prev = NULL;
    self->next = NULL;
    self->type = type;
    self->lexeme = (char *) malloc(length);
    self->length = length;
    memcpy(self->lexeme, lexeme, length);
    return self;
}

/// Frees the specified token instance
void token_free(Token *self) {
    self->prev = NULL;
    self->next = NULL;
    free(self->lexeme);
    self->length = 0;
    free(self);
}

/// Creates a new token list instance
TokenList *token_list_new(void) {
    TokenList *self = malloc(sizeof(TokenList));
    self->begin = NULL;
    self->end = NULL;
    self->tokens = 0;
    return self;
}

/// Clears the specified token list (i.e. deletes all tokens)
void token_list_clear(TokenList *self) {
    Token *it = self->begin;

    while (it != NULL) {
        Token *tmp = it;
        it = it->next;
        token_free(tmp);
    }
    self->begin = NULL;
    self->end = NULL;
    self->tokens = 0;
}

/// Frees the specified token list (i.e. deletes tokens and free list)
void token_list_free(TokenList *self) {
    token_list_clear(self);
    free(self);
}

/// Pushes a token to the specified token list
void token_list_push(TokenList *self, TokenType const type, char const *lexeme, usize const length) {
    if (self == NULL) {
        return;
    }

    Token *token = token_new(type, lexeme, length);
    if (self->begin == NULL) {
        self->begin = token;
        self->end = token;
        token->prev = NULL;
        self->tokens++;
        return;
    }

    Token *tmp = self->begin;
    while (tmp->next != NULL) {
        tmp = tmp->next;
    }

    tmp->next = token;
    token->prev = tmp;
    self->end = token;
    self->tokens++;
}

typedef struct StringIterator {
    char *base;
    usize length;
    usize index;
} StringIterator;

/// Creates a string iterator
static void string_iterator_create(StringIterator *self, char *base, usize const length) {
    self->base = base;
    self->length = length;
    self->index = 0;
}

/// Advances the string iterator by one
static void string_iterator_advance(StringIterator *self) {
    if (self->index < self->length - 1) {
        self->index++;
    }
}

/// Retrieves the current char of the iterator
static char string_iterator_current(StringIterator const *self) {
    return self->base[self->index];
}

/// Retrieves the next char of the iterator
static char string_iterator_next(StringIterator const *self) {
    return self->base[self->index + 1];
}

/// Checks if the iterator is at the end
static bool string_iterator_end(StringIterator const *self) {
    return self->index == self->length - 1;
}

/// Checks if the specified char is a trivial token
static bool tokenize_is_trivial_token(char const current) {
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
static TokenType tokenize_trivial_to_type(char const current) {
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
static bool string_view_equal(const char *first, usize const first_size, const char *second, usize const second_size) {
    if (first_size != second_size) {
        return false;
    }
    return strncmp(first, second, first_size) == 0;
}

/// Tokenizes the specified data
TokenList *tokenize(char *data, usize const length) {
    TokenList *list = token_list_new();
    StringIterator iterator;
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
            TokenType const type = tokenize_trivial_to_type(current_trivial);
            token_list_push(list, type, &current_trivial, 1);
            string_iterator_advance(&iterator);
            continue;
        }

        // string-literals
        if (string_iterator_current(&iterator) == '"') {
            string_iterator_advance(&iterator);
            char const *lexeme = iterator.base + iterator.index;
            usize const begin_index = iterator.index;

            char previous = 0;
            while (string_iterator_current(&iterator) != '"' || previous == '\\') {
                previous = string_iterator_current(&iterator);
                string_iterator_advance(&iterator);
            }
            usize const end_index = iterator.index;
            usize const lexeme_length = end_index - begin_index;
            token_list_push(list, TOKEN_STRING, lexeme, lexeme_length);
        }

        // alphabetic characters
        if (isalpha(string_iterator_current(&iterator))) {
            char const *lexeme = iterator.base + iterator.index;
            usize const begin_index = iterator.index;

            while (isalnum(string_iterator_current(&iterator))) {
                string_iterator_advance(&iterator);
            }

            usize const end_index = iterator.index;
            usize const lexeme_length = end_index - begin_index;

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
            char const *lexeme = iterator.base + iterator.index;
            TokenType type = TOKEN_NUMBER;
            usize const begin_index = iterator.index;
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
bool token_iterator_end(TokenIterator const *self) {
    return self->current == self->end;
}

/// Returns an invalid token
Token *token_iterator_invalid(void) {
    static Token token = { 0 };
    token.type = TOKEN_INVALID;
    token.lexeme = "";
    token.length = 0;
    token.prev = NULL;
    token.next = NULL;
    return &token;
}

/// Retrieves the current token
Token *token_iterator_current(TokenIterator const *self) {
    if (self->current) {
        return self->current;
    }
    return token_iterator_invalid();
}

/// Retrieves the next token
Token *token_iterator_next(TokenIterator const *self) {
    if (!token_iterator_end(self)) {
        return self->current->next;
    }
    return token_iterator_invalid();
}

/// Advances the token cursor by one
void token_iterator_advance(TokenIterator *self) {
    if (!token_iterator_end(self)) {
        self->current = self->current->next;
    } else {
        self->current = token_iterator_invalid();
    }
}

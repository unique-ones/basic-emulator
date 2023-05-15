#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

token_t* token_new(token_type_t type, char* lexeme, u32 length) {
    token_t* self = (token_t*) malloc(sizeof(token_t));
    self->prev = NULL;
    self->next = NULL;
    self->type = type;
    self->lexeme = (char*) malloc(length);
    self->length = length;
    memcpy(self->lexeme, lexeme, length);
    return self;
}

void token_free(token_t* self) {
    self->prev = NULL;
    self->next = NULL;
    free(self->lexeme);
    self->length = 0;
    free(self);
}

token_list_t* token_list_new(void) {
    token_list_t* self = (token_list_t*) malloc(sizeof(token_list_t));
    self->begin = NULL;
    self->end = NULL;
    self->tokens = 0;
    return self;
}

void token_list_clear(token_list_t* self) {
    token_t* it = self->begin;
    token_t* tmp;

    while (it != NULL) {
        tmp = it;
        it = it->next;
        token_free(tmp);
    }
    self->begin = NULL;
    self->end = NULL;
    self->tokens = 0;
}
void token_list_free(token_list_t* self) {
    token_list_clear(self);
    free(self);
}

void token_list_push(token_list_t* self, token_type_t type, char* lexeme, u32 length) {
    token_t* token = token_new(type, lexeme, length);
    if (self->begin == NULL) {
        self->begin = token;
        token->prev = NULL;
        self->tokens++;
        return;
    }

    token_t* tmp = self->begin;
    while (tmp->next != NULL) {
        tmp = tmp->next;
    }

    tmp->next = token;
    token->prev = tmp;
    self->end = token;
    self->tokens++;
}

typedef struct string_iterator {
    char* base;
    u32 length;
    u32 index;
} string_iterator_t;

static void string_iterator_create(string_iterator_t* self, char* base, u32 length) {
    self->base = base;
    self->length = length;
    self->index = 0;
}

static void string_iterator_advance(string_iterator_t* self) {
    if (self->index < self->length - 1) {
        self->index++;
    }
}

static char string_iterator_current(string_iterator_t* self) {
    return self->base[self->index];
}

static char string_iterator_next(string_iterator_t* self) {
    return self->base[self->index + 1];
}

static bool string_iterator_end(string_iterator_t* self) {
    return self->index == self->length - 1;
}

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
        case '"':
            return true;
        default:
            return false;
    }
}

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
        case '"':
            return TOKEN_QUOTE;
        default:
            return TOKEN_INVALID;
    }
}

token_list_t* tokenize(char* data, u32 length) {
    token_list_t* list = token_list_new();
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

        // alphabetic characters
        if (isalpha(string_iterator_current(&iterator))) {
            char* lexeme = iterator.base + iterator.index;
            u32 begin_index = iterator.index;
            while (isalpha(string_iterator_current(&iterator))) {
                string_iterator_advance(&iterator);
            }
            u32 end_index = iterator.index;
            token_list_push(list, TOKEN_TEXT, lexeme, end_index - begin_index);
            continue;
        }

        // numbers
        if (isdigit(string_iterator_current(&iterator))) {
            char* lexeme = iterator.base + iterator.index;
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
            u32 end_index = iterator.index;
            token_list_push(list, type, lexeme, end_index - begin_index);
            continue;
        }

        string_iterator_advance(&iterator);
    }
    return list;
}

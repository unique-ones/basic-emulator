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

#ifndef RETRO_LEXER_H
#define RETRO_LEXER_H

#include "types.h"

typedef enum token_type {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_NUMBER_FLOAT,
    TOKEN_QUOTE,
    TOKEN_COLON,
    TOKEN_LEFT_PARENTHESIS,
    TOKEN_RIGHT_PARENTHESIS,
    TOKEN_QUESTION_MARK,
    TOKEN_EQUAL_SIGN,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_ASTERISK,
    TOKEN_SLASH,
    TOKEN_COMMA,
    TOKEN_GREATER_THAN,
    TOKEN_LESS_THAN,
    TOKEN_CIRCUMFLEX,
    TOKEN_DOLLAR,
    TOKEN_INVALID,

    // keywords
    TOKEN_CLEAR,
    TOKEN_LET,
} token_type_t;

typedef struct token {
    struct token* prev;
    struct token* next;
    token_type_t type;
    char* lexeme;
    u32 length;
} token_t;

/**
 * @brief creates a new token instance
 * @param type type of the token
 * @param lexeme string of the token
 * @param length length of the lexeme
 * @return token instance
 */
token_t* token_new(token_type_t type, char* lexeme, u32 length);

/**
 * @brief free the specified token instance
 * @param self token instance
 */
void token_free(token_t* self);

typedef struct token_list {
    token_t* begin;
    token_t* end;
    u32 tokens;
} token_list_t;

/**
 * @brief create a new token list instance
 * @return token list
 */
token_list_t* token_list_new(void);

/**
 * @brief clear the specified token list (i.e. delete all tokens)
 * @param self token list instance
 */
void token_list_clear(token_list_t* self);

/**
 * @brief free the specified token list (i.e. delete tokens and free list)
 * @param self token list instance
 */
void token_list_free(token_list_t* self);

/**
 * @brief push a token to the specified token list
 * @param self token list instance
 * @param type type of the token
 * @param lexeme string of the token
 * @param length length of the lexeme
 */
void token_list_push(token_list_t* self, token_type_t type, char* lexeme, u32 length);

/**
 * @brief tokenize the specified data
 * @param data string that shall be tokenized
 * @param length length of the string
 * @return token list
 */
token_list_t* tokenize(char* data, u32 length);

#endif// RETRO_LEXER_H

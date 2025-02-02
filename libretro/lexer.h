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

#ifndef RETRO_LEXER_H
#define RETRO_LEXER_H

#include "types.h"

typedef enum TokenType {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_NUMBER_FLOAT,
    TOKEN_QUOTE,
    TOKEN_STRING,
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

    // Keywords
    TOKEN_LET,
    TOKEN_CLEAR,
    TOKEN_PRINT,
    TOKEN_DEF,
    TOKEN_FN,

    // Emulator commands
    TOKEN_RUN,
    TOKEN_EXIT
} TokenType;

typedef struct Token {
    struct Token *prev;
    struct Token *next;
    TokenType type;
    char *lexeme;
    u32 length;
} Token;

/// Creates a new token instance
/// @param type The type of the token
/// @param lexeme The string of the token
/// @param length The length of the lexeme
/// @return A new token instance
Token *token_new(TokenType type, char *lexeme, u32 length);

/// Frees the specified token instance
/// @param self The token instance
void token_free(Token *self);

typedef struct TokenList {
    Token *begin;
    Token *end;
    u32 tokens;
} TokenList;

/// Creates a new token list instance
/// @return A new token list
TokenList *token_list_new(void);

/// Clears the specified token list (i.e. deletes all tokens)
/// @param self The token list instance
void token_list_clear(TokenList *self);

/// Frees the specified token list (i.e. deletes tokens and free list)
/// @param self The token list instance
void token_list_free(TokenList *self);

/// Pushes a token to the specified token list
/// @param self The token list instance
/// @param type The type of the token
/// @param lexeme The string of the token
/// @param length The length of the lexeme
void token_list_push(TokenList *self, TokenType type, char *lexeme, u32 length);

/// Tokenizes the specified data
/// @param data The string that shall be tokenized
/// @param length The length of the string
/// @return A new token list
TokenList *tokenize(char *data, u32 length);

typedef struct TokenIterator {
    Token *current;
    Token *end;
} TokenIterator;

/// Checks if the token iterator reached the end
/// @param self The token iterator
/// @return A boolean that indicates whether the iterator is at the end
bool token_iterator_end(TokenIterator *self);

/// Returns an invalid token
/// @return An invalid token
Token *token_iterator_invalid(void);

/// Retrieves the current token
/// @param self The token iterator
/// @return The current token
Token *token_iterator_current(TokenIterator *self);

/// Retrieves the next token
/// @param self The token iterator
/// @return The next token
Token *token_iterator_next(TokenIterator *self);

/// Advances the token cursor by one
/// @param self The token iterator
void token_iterator_advance(TokenIterator *self);

#endif// RETRO_LEXER_H

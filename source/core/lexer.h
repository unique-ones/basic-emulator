// Copyright (c) 2025 Elias Engelbert Plank

#ifndef RETRO_LEXER_H
#define RETRO_LEXER_H

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
    usize length;
} Token;

/// Creates a new token instance
/// @param type The type of the token
/// @param lexeme The string of the token
/// @param length The length of the lexeme
/// @return A new token instance
static Token *token_new(TokenType type, char const *lexeme, usize length);

/// Frees the specified token instance
/// @param self The token instance
static void token_free(Token *self);

typedef struct TokenList {
    Token *begin;
    Token *end;
    usize tokens;
} TokenList;

/// Creates a new token list instance
/// @return A new token list
static TokenList *token_list_new(void);

/// Clears the specified token list (i.e. deletes all tokens)
/// @param self The token list instance
static void token_list_clear(TokenList *self);

/// Frees the specified token list (i.e. deletes tokens and free list)
/// @param self The token list instance
static void token_list_free(TokenList *self);

/// Pushes a token to the specified token list
/// @param self The token list instance
/// @param type The type of the token
/// @param lexeme The string of the token
/// @param length The length of the lexeme
static void token_list_push(TokenList *self, TokenType type, char const *lexeme, usize length);

/// Tokenizes the specified data
/// @param data The string that shall be tokenized
/// @param length The length of the string
/// @return A new token list
static TokenList *tokenize(char *data, usize length);

typedef struct TokenIterator {
    Token *current;
    Token *end;
} TokenIterator;

/// Checks if the token iterator reached the end
/// @param self The token iterator
/// @return A b32ean that indicates whether the iterator is at the end
static b32 token_iterator_end(TokenIterator const *self);

/// Returns an invalid token
/// @return An invalid token
static Token *token_iterator_invalid(void);

/// Retrieves the current token
/// @param self The token iterator
/// @return The current token
static Token *token_iterator_current(TokenIterator const *self);

/// Retrieves the next token
/// @param self The token iterator
/// @return The next token
static Token *token_iterator_next(TokenIterator const *self);

/// Advances the token cursor by one
/// @param self The token iterator
static void token_iterator_advance(TokenIterator *self);

#endif// RETRO_LEXER_H

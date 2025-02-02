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

#ifndef RETRO_STMT_H
#define RETRO_STMT_H

#include "expr.h"
#include "lexer.h"
#include "prog.h"

typedef struct statement statement_t;

typedef struct let_statement {
    expression_t *variable;
    expression_t *initializer;
} let_statement_t;

/// Creates a new let statement
/// @param arena The arena for allocations
/// @param line The line of the statement
/// @param variable The variable
/// @param initializer The initializer value of the variable
/// @return A new let statement
statement_t *let_statement_new(arena_t *arena, u32 line, expression_t *variable, expression_t *initializer);

/// Creates a new clear statement
/// @param arena The arena for allocations
/// @param line The line of the statement
/// @return A new clear statement
statement_t *clear_statement_new(arena_t *arena, u32 line);

typedef struct print_statement {
    expression_t *printable;
} print_statement_t;

/// Creates a new print statement
/// @param arena The arena for allocations
/// @param line The line of the statement
/// @param printable The printable expression
/// @return A new print statement
statement_t *print_statement_new(arena_t *arena, u32 line, expression_t *printable);

/// Creates a new run statement
/// @param arena The arena for allocations
/// @return A new run statement
statement_t *run_statement_new(arena_t *arena);

typedef enum statement_type {
    // Variable Control
    STATEMENT_CLEAR,
    STATEMENT_LET,

    // Emulator commands
    STATEMENT_PRINT,
    STATEMENT_RUN
} statement_type_t;

typedef struct statement {
    u32 line;
    statement_type_t type;
    union {
        let_statement_t let;
        print_statement_t print;
    };
} statement_t;

typedef enum result_type {
    RESULT_OK,
    RESULT_ERROR,
} result_type_t;

typedef struct statement_result_t {
    result_type_t type;
    union {
        statement_t *statement;
        const char *error;
    };
} statement_result_t;

/// Compiles a statement from a list of tokens
/// @param arena The arena for allocations
/// @param begin The first token in the statement
/// @param end The last token in the statement
/// @return The statement or nil if the statement where invalid
statement_result_t statement_compile(arena_t *arena, token_t *begin, token_t *end);

/// Executes the statement
/// @param self The statement
/// @param program The program state
void statement_execute(statement_t *self, program_t *program);

#endif// RETRO_STMT_H

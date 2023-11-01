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

#ifndef RETRO_STMT_H
#define RETRO_STMT_H

#include "expr.h"
#include "lexer.h"
#include "prog.h"

typedef struct statement statement_t;

typedef struct line_statement {
    u32 line;
    statement_t* statement;
} line_statement_t;

/// Creates a new line statement
/// @param line The line for the statement
/// @param statement The statement after the line
/// @return A new statement
statement_t* line_statement_new(u32 line, statement_t* statement);

/// Frees the line statement
/// @param self The line statement
void line_statement_free(statement_t* self);

typedef struct let_statement {
    token_t name;
    expression_t* initializer;
} let_statement_t;

/// Creates a new let statement
/// @param name The name of the variable
/// @param initializer The initializer value of the variable
/// @return A new statement
statement_t* let_statement_new(token_t name, expression_t* initializer);

/// Frees the let statement
/// @param self The let statement
void let_statement_free(statement_t* self);

typedef struct clear_statement {
    token_t name;
} clear_statement_t;

typedef enum statement_type {
    STATEMENT_LINE,
    STATEMENT_LET,
    STATEMENT_CLEAR,
} statement_type_t;

typedef struct statement {
    statement_type_t type;
    union {
        line_statement_t line;
        let_statement_t let;
        clear_statement_t clear;
    };
} statement_t;

/// Compiles a statement from a list of tokens
/// @param begin The first token in the statement
/// @param end The last token in the statement
/// @return The statement or nil if the statement where invalid
statement_t* statement_compile(token_t* begin, token_t* end);

/// Frees the statement
/// @param self The statement
void statement_free(statement_t* self);

/// Executes the statement
/// @param self The statement
/// @param program The program state
void statement_execute(statement_t* self, program_t* program);

#endif// RETRO_STMT_H

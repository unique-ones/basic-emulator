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
/// @param arena The arena for allocations
/// @param line The line for the statement
/// @param statement The statement after the line
/// @return A new statement
statement_t* line_statement_new(arena_t* arena, u32 line, statement_t* statement);

typedef struct let_statement {
    expression_t* variable;
    expression_t* initializer;
} let_statement_t;

/// Creates a new let statement
/// @param arena The arena for allocations
/// @param variable The variable
/// @param initializer The initializer value of the variable
/// @return A new statement
statement_t* let_statement_new(arena_t* arena, expression_t* variable, expression_t* initializer);

typedef struct print_statement {
    expression_t* printable;
} print_statement_t;

/// Creates a new print statement
/// @param arena The arena for allocations
/// @param printable The printable expression
/// @return A new statement
statement_t* print_statement_new(arena_t* arena, expression_t* printable);

typedef enum statement_type {
    STATEMENT_LINE,
    STATEMENT_LET,
    STATEMENT_PRINT,
} statement_type_t;

typedef struct statement {
    statement_type_t type;
    union {
        line_statement_t line;
        let_statement_t let;
        print_statement_t print;
    };
} statement_t;

/// Compiles a statement from a list of tokens
/// @param arena The arena for allocations
/// @param begin The first token in the statement
/// @param end The last token in the statement
/// @return The statement or nil if the statement where invalid
statement_t* statement_compile(arena_t* arena, token_t* begin, token_t* end);

/// Executes the statement
/// @param self The statement
/// @param program The program state
void statement_execute(statement_t* self, program_t* program);

#endif// RETRO_STMT_H

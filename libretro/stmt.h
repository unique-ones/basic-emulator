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

/**
 * @brief creates a new line statement
 * @param line the line for the statement
 * @param statement the statement after the line
 * @return a new statement
 */
statement_t* line_statement_new(u32 line, statement_t* statement);

/**
 * @brief frees the line statement
 * @param self line statement
 */
void line_statement_free(statement_t* self);

typedef struct let_statement {
    token_t name;
    expression_t* initializer;
} let_statement_t;

/**
 * @brief creates a new let statement
 * @param name the name of the variable
 * @param initializer the initializer value of the variable
 * @return a new statement
 */
statement_t* let_statement_new(token_t name, expression_t* initializer);

/**
 * @brief frees the let statement
 * @param self let statement
 */
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

/**
 * @brief compile a statement from a list of tokens
 * @param begin the first token in the statement
 * @param end the last token in the statement
 * @return the statement or nil if the statement where invalid
 */
statement_t* statement_compile(token_t* begin, token_t* end);

/**
 * @brief free the statement
 * @param self the statement
 */
void statement_free(statement_t* self);

/**
 * @brief executes the statement
 * @param self the statement
 * @param program the program state
 */
void statement_execute(statement_t* self, program_t* program);

#endif// RETRO_STMT_H

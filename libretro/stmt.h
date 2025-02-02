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

typedef struct Statement Statement;

typedef struct LetStatement {
    Expression *variable;
    Expression *initializer;
} LetStatement;

/// Creates a new let statement
/// @param arena The arena for allocations
/// @param line The line of the statement
/// @param variable The variable
/// @param initializer The initializer value of the variable
/// @return A new let statement
Statement *let_statement_new(MemoryArena *arena, u32 line, Expression *variable, Expression *initializer);

/// Creates a new clear statement
/// @param arena The arena for allocations
/// @param line The line of the statement
/// @return A new clear statement
Statement *clear_statement_new(MemoryArena *arena, u32 line);

typedef struct DefFnStatement {
    Expression *name;
    Expression *variable;
    Expression *body;
} DefFnStatement;

/// Creates a new def fn statement
/// @param arena The arena for allocations
/// @param line The line of the statement
/// @param name The name of the function
/// @param variable The variable that is used
/// @param body The body of the function
/// @return A new def fn statement
Statement *def_fn_statement_new(MemoryArena *arena, u32 line, Expression *name, Expression *variable, Expression *body);

typedef struct PrintStatement {
    Expression *printable;
} PrintStatement;

/// Creates a new print statement
/// @param arena The arena for allocations
/// @param line The line of the statement
/// @param printable The printable expression
/// @return A new print statement
Statement *print_statement_new(MemoryArena *arena, u32 line, Expression *printable);

/// Creates a new run statement
/// @param arena The arena for allocations
/// @return A new run statement
Statement *run_statement_new(MemoryArena *arena);

typedef enum StatementType {
    // Variable Control
    STATEMENT_CLEAR,
    STATEMENT_LET,
    STATEMENT_DEF_FN,

    // Emulator commands
    STATEMENT_PRINT,
    STATEMENT_RUN
} StatementType;

typedef struct Statement {
    u32 line;
    StatementType type;
    union {
        LetStatement let;
        DefFnStatement def_fn;
        PrintStatement print;
    };
} Statement;

typedef enum ResultType {
    RESULT_OK,
    RESULT_ERROR,
} ResultType;

typedef struct StatementResult {
    ResultType type;
    union {
        Statement *statement;
        const char *error;
    };
} StatementResult;

/// Compiles a statement from a list of tokens
/// @param arena The arena for allocations
/// @param begin The first token in the statement
/// @param end The last token in the statement
/// @return The statement or nil if the statement where invalid
StatementResult statement_compile(MemoryArena *arena, Token *begin, Token *end);

/// Executes the statement
/// @param self The statement
/// @param program The program state
void statement_execute(Statement *self, Program *program);

#endif// RETRO_STMT_H

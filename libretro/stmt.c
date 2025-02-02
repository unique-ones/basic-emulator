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

#include "stmt.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

/// Creates a new let statement
statement_t *let_statement_new(arena_t *arena, u32 line, expression_t *variable, expression_t *initializer) {
    statement_t *self = arena_alloc(arena, sizeof(statement_t));
    self->line = line;
    self->type = STATEMENT_LET;
    self->let.variable = variable;
    self->let.initializer = initializer;
    return self;
}

/// Creates a new clear statement
statement_t *clear_statement_new(arena_t *arena, u32 line) {
    statement_t *self = arena_alloc(arena, sizeof(statement_t));
    self->line = line;
    self->type = STATEMENT_CLEAR;
    return self;
}

/// Creates a new print statement
statement_t *print_statement_new(arena_t *arena, u32 line, expression_t *printable) {
    statement_t *self = arena_alloc(arena, sizeof(statement_t));
    self->line = line;
    self->type = STATEMENT_PRINT;
    self->print.printable = printable;
    return self;
}

/// Creates a new run statement
statement_t *run_statement_new(arena_t *arena) {
    statement_t *self = arena_alloc(arena, sizeof(statement_t));
    self->line = 0;
    self->type = STATEMENT_RUN;
    return self;
}

/// Checks if the given token type matches the current token in the state
static bool match(token_iterator_t *state, token_type_t type) {
    return token_iterator_current(state)->type == type;
}

/// Checks if the given token type matches the next token in the state
static bool match_next(token_iterator_t *state, token_type_t type) {
    return token_iterator_next(state)->type == type;
}

/// Creates a successful statement result
static statement_result_t statement_result_make(statement_t *statement) {
    statement_result_t result = { 0 };
    result.type = RESULT_OK;
    result.statement = statement;
    return result;
}

/// Creates a statement result with an error message
static statement_result_t statement_result_make_error(const char *error) {
    statement_result_t result = { 0 };
    result.type = RESULT_ERROR;
    result.error = error;
    return result;
}

/// Compiles a statement from the token state
static statement_result_t statement_compile_internal(arena_t *arena, u32 line, token_iterator_t *state);

/// Compiles a let statement from the token state
static statement_result_t statement_compile_let(arena_t *arena, u32 line, token_iterator_t *state) {
    if (match(state, TOKEN_LET)) {
        token_iterator_advance(state);
    }
    if (match(state, TOKEN_IDENTIFIER) && match_next(state, TOKEN_EQUAL_SIGN)) {
        token_t *identifier_token = token_iterator_current(state);
        token_iterator_advance(state);
        token_iterator_advance(state);

        expression_t *initializer = expression_compile(arena, state->current, state->end);
        if (initializer == NULL) {
            return statement_result_make_error("LET statement has invalid initializer");
        }
        expression_t *variable = variable_expression_new(arena, identifier_token->lexeme, identifier_token->length);
        return statement_result_make(let_statement_new(arena, line, variable, initializer));
    }
    return statement_result_make_error("LET statement must take form of [ LET ] <identifier> = <initializer>");
}

/// Compiles a clear statement
static statement_result_t statement_compile_clear(arena_t *arena, u32 line, token_iterator_t *state) {
    token_iterator_advance(state);
    return statement_result_make(clear_statement_new(arena, line));
}

/// Compiles a print statement
static statement_result_t statement_compile_print(arena_t *arena, u32 line, token_iterator_t *state) {
    token_iterator_advance(state);
    expression_t *printable = expression_compile(arena, state->current, state->end);
    if (printable == NULL) {
        return statement_result_make_error("Invalid expression after PRINT statement");
    }
    return statement_result_make(print_statement_new(arena, line, printable));
}

static statement_result_t statement_compile_run(arena_t *arena) {
    return statement_result_make(run_statement_new(arena));
}

/// Compiles a statement from the token state
static statement_result_t statement_compile_internal(arena_t *arena, u32 line, token_iterator_t *state) {
    // Clear all variables
    if (match(state, TOKEN_CLEAR)) {
        return statement_compile_clear(arena, line, state);
    }

    // Assignment
    if (match(state, TOKEN_IDENTIFIER) || match(state, TOKEN_LET)) {
        return statement_compile_let(arena, line, state);
    }

    // Printing
    if (match(state, TOKEN_PRINT)) {
        return statement_compile_print(arena, line, state);
    }
    return statement_result_make_error("Encountered invalid token");
}

/// Compiles a statement from a list of tokens
statement_result_t statement_compile(arena_t *arena, token_t *begin, token_t *end) {
    token_iterator_t state = { 0 };
    state.current = begin;
    state.end = end;

    if (match(&state, TOKEN_RUN)) {
        return statement_compile_run(arena);
    }

    if (match(&state, TOKEN_EXIT)) {
        exit(0);
    }

    // Any other statement should be preceeded by a number
    if (!match(&state, TOKEN_NUMBER)) {
        return statement_result_make_error("Statement is missing line number");
    }

    token_t *line_token = token_iterator_current(&state);
    token_iterator_advance(&state);

    char *line_begin = line_token->lexeme;
    char *line_end = line_begin + line_token->length;
    return statement_compile_internal(arena, strtoul(line_begin, &line_end, 10), &state);
}

/// Executes a line statement
static void statement_execute_let(statement_t *self, program_t *program) {
    expression_t *var = self->let.variable;
    if (expression_is_arithmetic(self->let.initializer)) {
        // TODO(elias): This here is actually a memory leak since the old unevaluated expression
        // still lives inside the arena. Imagine having a loop that computes an expression over
        // and over again. I don't really care about it at the moment but maybe this should be
        // handled sometime in the future.
        f64 result = expression_evaluate(self->let.initializer, program->symbols);
        self->let.initializer = number_expression_new(&program->objects, result);
    }
    map_insert(program->symbols, var->variable.name, self->let.initializer);
    program->no_wait = true;
}

static void statement_execute_clear(statement_t *self, program_t *program) {
    map_clear(program->symbols);
    program->no_wait = true;
}

/// Executes a line statement
static void statement_execute_print(statement_t *self, program_t *program) {
    expression_t *printable = self->print.printable;
    if (expression_is_arithmetic(printable)) {
        f64 result = expression_evaluate(printable, program->symbols);
        program_print_format(program, "%lf", result);
    } else {
        assert(printable->type == EXPRESSION_STRING && "printable must be arithmetic or string");
        program_print_format(program, "%.*s", printable->string.length, printable->string.data);
    }
    program->no_wait = false;
}

/// Executes the statement
void statement_execute(statement_t *self, program_t *program) {
    switch (self->type) {
        case STATEMENT_LET:
            statement_execute_let(self, program);
            break;
        case STATEMENT_CLEAR:
            statement_execute_clear(self, program);
            break;
        case STATEMENT_PRINT:
            statement_execute_print(self, program);
            break;
        default:
            break;
    }
}

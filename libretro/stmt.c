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

#include "stmt.h"

#include <stdlib.h>

statement_t* line_statement_new(u32 line, statement_t* statement) {
    statement_t* self = (statement_t*) malloc(sizeof(statement_t));
    self->type = STATEMENT_LINE;
    self->line.line = line;
    self->line.statement = statement;
    return self;
}

void line_statement_free(statement_t* self) {
    statement_free(self->line.statement);
    free(self);
}

statement_t* let_statement_new(token_t name, expression_t* initializer) {
    statement_t* self = (statement_t*) malloc(sizeof(statement_t));
    self->type = STATEMENT_LET;
    self->let.name = name;
    self->let.initializer = initializer;
    return self;
}

void let_statement_free(statement_t* self) {
    expression_free(self->let.initializer);
    free(self);
}

static bool match(token_state_t* state, token_type_t type) {
    return token_state_current(state)->type == type;
}

static bool match_next(token_state_t* state, token_type_t type) {
    return token_state_next(state)->type == type;
}

static statement_t* statement(token_state_t* state);

static statement_t* statement_line(token_state_t* state) {
    if (match_next(state, TOKEN_NUMBER)) {
        return NULL;
    }

    token_t* line_token = token_state_current(state);
    token_state_advance(state);

    char* line_begin = line_token->lexeme;
    char* line_end = line_begin + line_token->length;
    u32 line = strtoul(line_begin, &line_end, 10);
    return line_statement_new(line, statement(state));
}

static statement_t* statement_let(token_state_t* state) {
    if (match(state, TOKEN_LET)) {
        token_state_advance(state);
    }
    if (match(state, TOKEN_IDENTIFIER) && match_next(state, TOKEN_EQUAL_SIGN)) {
        token_t* identifier_token = token_state_current(state);
        token_state_advance(state);
        token_state_advance(state);

        expression_t* initializer = expression_compile(state->current, state->end);
        if (initializer == NULL) {
            return NULL;
        }
        return let_statement_new(*identifier_token, initializer);
    }
    return NULL;
}

static statement_t* statement(token_state_t* state) {
    if (match(state, TOKEN_NUMBER)) {
        return statement_line(state);
    }
    if (match(state, TOKEN_IDENTIFIER) || match(state, TOKEN_LET)) {
        return statement_let(state);
    }
    return NULL;
}

statement_t* statement_compile(token_t* begin, token_t* end) {
    token_state_t state = { 0 };
    state.current = begin;
    state.end = end;
    return statement(&state);
}

void statement_free(statement_t* self) {
    switch (self->type) {
        case STATEMENT_LINE:
            line_statement_free(self);
            break;
        case STATEMENT_LET:
            let_statement_free(self);
            break;
        case STATEMENT_CLEAR:
            break;
    }
}

void statement_execute(statement_t* self, program_t* program) { }

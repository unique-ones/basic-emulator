// Copyright (c) 2025 Elias Engelbert Plank

#include "stmt.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "expr.h"
#include "lexer.h"

/// Creates a new let statement
Statement *let_statement_new(MemoryArena *arena, usize line, Expression *variable, Expression *initializer) {
    Statement *self = arena_alloc(arena, sizeof(Statement));
    self->line = line;
    self->type = STATEMENT_LET;
    self->let.variable = variable;
    self->let.initializer = initializer;
    return self;
}

/// Creates a new clear statement
Statement *clear_statement_new(MemoryArena *arena, usize line) {
    Statement *self = arena_alloc(arena, sizeof(Statement));
    self->line = line;
    self->type = STATEMENT_CLEAR;
    return self;
}

/// Creates a new def fn statement
Statement *def_fn_statement_new(MemoryArena *arena,
                                usize line,
                                Expression *name,
                                Expression *variable,
                                Expression *body) {
    Statement *self = arena_alloc(arena, sizeof(Statement));
    self->line = line;
    self->type = STATEMENT_DEF_FN;
    self->def_fn.name = name;
    self->def_fn.variable = variable;
    self->def_fn.body = body;
    return self;
}

/// Creates a new print statement
Statement *print_statement_new(MemoryArena *arena, usize line, Expression *printable) {
    Statement *self = arena_alloc(arena, sizeof(Statement));
    self->line = line;
    self->type = STATEMENT_PRINT;
    self->print.printable = printable;
    return self;
}

/// Creates a new run statement
Statement *run_statement_new(MemoryArena *arena) {
    Statement *self = arena_alloc(arena, sizeof(Statement));
    self->line = 0;
    self->type = STATEMENT_RUN;
    return self;
}

/// Checks if the given token type matches the current token in the state
static bool match(TokenIterator *state, TokenType type) {
    return token_iterator_current(state)->type == type;
}

/// Checks if the given token type matches the next token in the state
static bool match_next(TokenIterator *state, TokenType type) {
    return token_iterator_next(state)->type == type;
}

/// Creates a successful statement result
static StatementResult statement_result_make(Statement *statement) {
    StatementResult result = { 0 };
    result.type = RESULT_OK;
    result.statement = statement;
    return result;
}

/// Creates a statement result with an error message
static StatementResult statement_result_make_error(const char *error) {
    StatementResult result = { 0 };
    result.type = RESULT_ERROR;
    result.error = error;
    return result;
}

/// Compiles a statement from the token state
static StatementResult statement_compile_internal(MemoryArena *arena, usize line, TokenIterator *state);

/// Compiles a let statement from the token state
static StatementResult statement_compile_let(MemoryArena *arena, usize line, TokenIterator *state) {
    if (match(state, TOKEN_LET)) {
        token_iterator_advance(state);
    }
    if (match(state, TOKEN_IDENTIFIER) && match_next(state, TOKEN_EQUAL_SIGN)) {
        Token *identifier_token = token_iterator_current(state);
        token_iterator_advance(state);
        token_iterator_advance(state);

        Expression *initializer = expression_compile(arena, state->current, state->end);
        if (initializer == NULL) {
            return statement_result_make_error("LET statement has invalid initializer");
        }
        Expression *variable = variable_expression_new(arena, identifier_token->lexeme, identifier_token->length);
        return statement_result_make(let_statement_new(arena, line, variable, initializer));
    }
    return statement_result_make_error("LET statement must take form of [ LET ] <identifier> = <initializer>");
}

static StatementResult statement_compile_def_fn(MemoryArena *arena, usize line, TokenIterator *state) {
    static const char *form_err = "DEF FN statement must take form of DEF FN <name>(<var>) = <body>";
    token_iterator_advance(state);
    if (!match(state, TOKEN_FN)) {
        return statement_result_make_error(form_err);
    }
    token_iterator_advance(state);

    if (!match(state, TOKEN_IDENTIFIER)) {
        return statement_result_make_error(form_err);
    }
    Token *name_token = token_iterator_current(state);
    token_iterator_advance(state);

    if (!match(state, TOKEN_LEFT_PARENTHESIS)) {
        return statement_result_make_error(form_err);
    }
    token_iterator_advance(state);

    if (!match(state, TOKEN_IDENTIFIER)) {
        return statement_result_make_error(form_err);
    }
    Token *variable_token = token_iterator_current(state);
    token_iterator_advance(state);

    if (!match(state, TOKEN_RIGHT_PARENTHESIS) || !match_next(state, TOKEN_EQUAL_SIGN)) {
        return statement_result_make_error(form_err);
    }
    token_iterator_advance(state);
    token_iterator_advance(state);

    Expression *body = expression_compile(arena, state->current, state->end);
    if (body == NULL) {
        return statement_result_make_error("LET statement has invalid initializer");
    }

    Expression *name = variable_expression_new(arena, name_token->lexeme, name_token->length);
    Expression *var = variable_expression_new(arena, variable_token->lexeme, variable_token->length);
    return statement_result_make(def_fn_statement_new(arena, line, name, var, body));
}

/// Compiles a clear statement
static StatementResult statement_compile_clear(MemoryArena *arena, usize line, TokenIterator *state) {
    token_iterator_advance(state);
    return statement_result_make(clear_statement_new(arena, line));
}

/// Compiles a print statement
static StatementResult statement_compile_print(MemoryArena *arena, usize line, TokenIterator *state) {
    token_iterator_advance(state);
    Expression *printable = expression_compile(arena, state->current, state->end);
    if (printable == NULL) {
        return statement_result_make_error("Invalid expression after PRINT statement");
    }
    return statement_result_make(print_statement_new(arena, line, printable));
}

static StatementResult statement_compile_run(MemoryArena *arena) {
    return statement_result_make(run_statement_new(arena));
}

/// Compiles a statement from the token state
static StatementResult statement_compile_internal(MemoryArena *arena, usize line, TokenIterator *state) {
    // Clear all variables
    if (match(state, TOKEN_CLEAR)) {
        return statement_compile_clear(arena, line, state);
    }

    // Assignment
    if (match(state, TOKEN_IDENTIFIER) || match(state, TOKEN_LET)) {
        return statement_compile_let(arena, line, state);
    }

    // Single variable function
    if (match(state, TOKEN_DEF)) {
        return statement_compile_def_fn(arena, line, state);
    }

    // Printing
    if (match(state, TOKEN_PRINT)) {
        return statement_compile_print(arena, line, state);
    }
    return statement_result_make_error("Encountered invalid token");
}

/// Compiles a statement from a list of tokens
StatementResult statement_compile(MemoryArena *arena, Token *begin, Token *end) {
    TokenIterator state = { 0 };
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

    Token *line_token = token_iterator_current(&state);
    token_iterator_advance(&state);

    char *line_begin = line_token->lexeme;
    char *line_end = line_begin + line_token->length;
    return statement_compile_internal(arena, strtoull(line_begin, &line_end, 10), &state);
}

/// Executes a line statement
static void statement_execute_let(Statement *self, Program *program) {
    Expression *var = self->let.variable;
    if (expression_is_arithmetic(self->let.initializer)) {
        // TODO(elias): This here is actually a memory leak since the old unevaluated expression
        // still lives inside the arena. Imagine having a loop that computes an expression over
        // and over again. I don't really care about it at the moment but maybe this should be
        // handled sometime in the future.
        f64 result = expression_evaluate(self->let.initializer, program->symbols);
        self->let.initializer = number_expression_new(&program->objects, result);
    }
    hash_map_insert(program->symbols, var->variable.name, self->let.initializer);
    program->no_wait = true;
}

/// Executes a clear statement
static void statement_execute_clear(Statement *self, Program *program) {
    hash_map_clear(program->symbols);
    program->no_wait = true;
}

/// Executes a custom function definition statement
static void statement_execute_def_fn(Statement *self, Program *program) {
    FunctionDefinition *definition = arena_alloc(&program->objects, sizeof(FunctionDefinition));
    definition->type = FUNCTION_DEFINITION_DYNAMIC;
    definition->name = self->def_fn.name->variable.name;
    definition->variable.variable = self->def_fn.variable;
    definition->variable.body = self->def_fn.body;
    hash_map_insert(program->symbols, definition->name, definition);
}

/// Executes a line statement
static void statement_execute_print(Statement *self, Program *program) {
    Expression *printable = self->print.printable;
    if (expression_is_arithmetic(printable)) {
        f64 result = expression_evaluate(printable, program->symbols);
        program_print_format(program, "%lf\n", result);
    } else {
        assert(printable->type == EXPRESSION_STRING && "printable must be arithmetic or string");
        program_print_format(program, "%.*s\n", printable->string.length, printable->string.data);
    }
    program->no_wait = false;
}

/// Executes the statement
void statement_execute(Statement *self, Program *program) {
    switch (self->type) {
        case STATEMENT_LET:
            statement_execute_let(self, program);
            break;
        case STATEMENT_CLEAR:
            statement_execute_clear(self, program);
            break;
        case STATEMENT_DEF_FN:
            statement_execute_def_fn(self, program);
            break;
        case STATEMENT_PRINT:
            statement_execute_print(self, program);
            break;
        default:
            break;
    }
}

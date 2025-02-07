// Copyright (c) 2025 Elias Engelbert Plank

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "expr.h"

#include <stdio.h>

/// Creates a new unary expression instance
Expression *unary_expression_new(MemoryArena *arena, Operator const operator, Expression * expression) {
    Expression *self = arena_alloc(arena, sizeof(Expression));
    self->type = EXPRESSION_UNARY;
    self->unary.operator= operator;
    self->unary.expression = expression;
    return self;
}

/// Evaluates the unary expression
f64 unary_expression_evaluate(Expression const *self, HashMap const *symbol_table) {
    f64 const value = expression_evaluate(self->unary.expression, symbol_table);
    return self->unary.operator== OPERATOR_ADD ? value : - 1.0 * value;
}

/// Creates a new binary expression instance
Expression *binary_expression_new(MemoryArena *arena, Expression *left, Expression *right, Operator const operator) {
    Expression *self = arena_alloc(arena, sizeof(Expression));
    self->type = EXPRESSION_BINARY;
    self->binary.left = left;
    self->binary.right = right;
    self->binary.operator= operator;
    return self;
}

/// Evaluates the binary expression
f64 binary_expression_evaluate(Expression const *self, HashMap const *symbol_table) {
    f64 const left = expression_evaluate(self->binary.left, symbol_table);
    f64 const right = expression_evaluate(self->binary.right, symbol_table);
    switch (self->binary.operator) {
        case OPERATOR_ADD:
            return left + right;
        case OPERATOR_SUB:
            return left - right;
        case OPERATOR_MUL:
            return left * right;
        case OPERATOR_DIV:
            return left / right;
    }
    return 0.0;
}

/// Creates a new variable expression instance
Expression *variable_expression_new(MemoryArena *arena, char const *name, usize const length) {
    Expression *self = arena_alloc(arena, sizeof(Expression));
    self->type = EXPRESSION_VARIABLE;
    memset(self->variable.name, 0, sizeof self->variable.name);
    memcpy(self->variable.name, name, length);
    return self;
}

/// Evaluates the variable expression
f64 variable_expression_evaluate(Expression const *self, HashMap const *symbol_table) {
    Expression *initializer = hash_map_find(symbol_table, self->variable.name);
    if (initializer) {
        return expression_evaluate(initializer, symbol_table);
    }
    return 0;
}

/// Creates a new function parameter instance
FunctionParameter *function_parameter_new(MemoryArena *arena, Expression *expression) {
    FunctionParameter *self = arena_alloc(arena, sizeof(FunctionParameter));
    self->expression = expression;
    self->prev = NULL;
    self->next = NULL;
    return self;
}

/// Creates a new function expression instance
Expression *function_expression_new(MemoryArena *arena, char const *name, usize const length) {
    Expression *self = arena_alloc(arena, sizeof(Expression));
    self->type = EXPRESSION_FUNCTION;

    memset(self->function.name, 0, sizeof self->function.name);
    memcpy(self->function.name, name, length);

    self->function.first_parameter = NULL;
    self->function.last_parameter = NULL;
    self->function.parameter_count = 0;

    return self;
}

/// Pushes a parameter to the specified function expression
void function_expression_push(MemoryArena *arena, Expression *self, Expression *parameter) {
    FunctionParameter *entry = function_parameter_new(arena, parameter);
    FunctionExpression *function = &self->function;
    if (function->first_parameter == NULL) {
        function->first_parameter = entry;
        entry->prev = NULL;
        function->parameter_count++;
        return;
    }

    FunctionParameter *tmp = function->first_parameter;
    while (tmp->next != NULL) {
        tmp = tmp->next;
    }

    tmp->next = entry;
    entry->prev = tmp;
    function->last_parameter = entry;
    function->parameter_count++;
}

/// Retrieves the parameter at the specified index
FunctionParameter *function_expression_get_parameter(Expression const *self, usize const index) {
    FunctionExpression const *function = &self->function;
    if (function->first_parameter == NULL || index >= function->parameter_count) {
        return NULL;
    }

    usize iterator;
    usize iterator_limit;
    FunctionParameter *temp;
    if (index < (function->parameter_count - index)) {
        temp = function->first_parameter;
        iterator_limit = index;
        for (iterator = 0; temp != NULL && iterator < iterator_limit; ++iterator) {
            temp = temp->next;
        }
    } else {
        temp = function->last_parameter;
        iterator_limit = function->parameter_count - index - 1;
        for (iterator = 0; temp != NULL && iterator < iterator_limit; ++iterator) {
            temp = temp->prev;
        }
    }
    return temp != NULL && iterator == iterator_limit ? temp : NULL;
}

#define EXPR_PARAM(index) \
    (expression_evaluate(function_expression_get_parameter(self, index)->expression, symbol_table))

/// Evaluates the specified function expression
f64 function_expression_evaluate(Expression const *self, HashMap const *symbol_table) {
    FunctionExpression const *function = &self->function;
    FunctionDefinition const *definition = hash_map_find(symbol_table, function->name);
    if (definition == NULL) {
        return 0.0;
    }

    switch (definition->type) {
        case FUNCTION_DEFINITION_DYNAMIC: {
            // NOTE(elias): This is EXTREMELY bad as the arena allocates a 4 Kb block per default, which means
            // that for every invocation of a user defined function, we have to allocate 4 Kb. Maybe this is an
            // indicator to transition away from arenas...

            MemoryArena temporary = arena_identity(ALIGNMENT8);
            Expression *number = number_expression_new(&temporary, EXPR_PARAM(0));

            // TODO(elias): Use dynamic identifier prefix to avoid overwriting variables
            hash_map_insert(symbol_table, definition->variable.variable->variable.name, number);
            f64 result = expression_evaluate(definition->variable.body, symbol_table);
            hash_map_remove(symbol_table, definition->variable.variable->variable.name);
            arena_destroy(&temporary);
            return result;
        }
        case FUNCTION_DEFINITION_BUILTIN: {
            if (function->parameter_count == definition->builtin.parameter_count) {
                switch (function->parameter_count) {
                    case 0:
                        return definition->builtin.func0();
                    case 1:
                        return definition->builtin.func1(EXPR_PARAM(0));
                    case 2:
                        return definition->builtin.func2(EXPR_PARAM(0), EXPR_PARAM(1));
                    default:
                        break;
                }
            }
            break;
        }
        default:
            break;
    }
    return 0.0;
}

#undef EXPR_PARAM

/// Creates a new number expression instance
Expression *number_expression_new(MemoryArena *arena, f64 const number) {
    Expression *self = arena_alloc(arena, sizeof(Expression));
    self->type = EXPRESSION_NUMBER;
    self->number = number;
    return self;
}

/// Evaluates the specified number expression
f64 number_expression_evaluate(Expression const *self) {
    return self->number;
}

/// Creates a new exponential expression instance
Expression *exponential_expression_new(MemoryArena *arena, Expression *base, Expression *exponent) {
    Expression *self = arena_alloc(arena, sizeof(Expression));
    self->type = EXPRESSION_EXPONENTIAL;
    self->exponential.base = base;
    self->exponential.exponent = exponent;
    return self;
}

/// Evaluates the specified exponential expression
f64 exponential_expression_evaluate(Expression const *self, HashMap const *symbol_table) {
    return pow(expression_evaluate(self->exponential.base, symbol_table),
               expression_evaluate(self->exponential.exponent, symbol_table));
}

/// Creates a new string expression by storing the string in the provided arena
Expression *string_expression_new(MemoryArena *arena, char const *data, usize const length) {
    Expression *self = arena_alloc(arena, sizeof(Expression));
    self->type = EXPRESSION_STRING;
    self->string.data = arena_alloc(arena, length);
    self->string.length = length;
    memcpy(self->string.data, data, length);
    return self;
}


/// Parses an addition or subtraction expression
static Expression *expression_add_or_sub(MemoryArena *arena, TokenIterator *state);

/// Parses a unary-plus-or-minus expression
static Expression *expression_unary_plus_or_minus(MemoryArena *arena, TokenIterator *state);

/// Parses an exponential expression
static Expression *expression_exponential(MemoryArena *arena, TokenIterator *state, Expression *base) {
    if (token_iterator_current(state)->type == TOKEN_CIRCUMFLEX) {
        token_iterator_advance(state);
        return exponential_expression_new(arena, base, expression_unary_plus_or_minus(arena, state));
    }
    return base;
}

/// Parses a primary expression, that includes numbers, variables and functions
static Expression *expression_primary(MemoryArena *arena, TokenIterator *state) {
    if (token_iterator_current(state)->type == TOKEN_NUMBER ||
        token_iterator_current(state)->type == TOKEN_NUMBER_FLOAT) {
        Token const *number_token = token_iterator_current(state);
        token_iterator_advance(state);

        char *number_begin = number_token->lexeme;
        char *number_end = number_begin + number_token->length;
        f64 const value = strtod(number_begin, &number_end);
        Expression *number = number_expression_new(arena, value);
        return expression_exponential(arena, state, number);
    }
    if (token_iterator_current(state)->type == TOKEN_IDENTIFIER) {
        Token const *text_token = token_iterator_current(state);
        token_iterator_advance(state);

        // function expression
        if (token_iterator_current(state) && token_iterator_current(state)->type == TOKEN_LEFT_PARENTHESIS) {
            Expression *function = function_expression_new(arena, text_token->lexeme, text_token->length);
            token_iterator_advance(state);
            function_expression_push(arena, function, expression_add_or_sub(arena, state));
            while (token_iterator_current(state)->type == TOKEN_COMMA) {
                function_expression_push(arena, function, expression_add_or_sub(arena, state));
            }
            return expression_exponential(arena, state, function);
        }

        // variable expression
        Expression *variable = variable_expression_new(arena, text_token->lexeme, text_token->length);
        return expression_exponential(arena, state, variable);
    }
    if (token_iterator_current(state)->type == TOKEN_LEFT_PARENTHESIS) {
        token_iterator_advance(state);

        Expression *inner = expression_add_or_sub(arena, state);
        if (token_iterator_current(state)->type != TOKEN_RIGHT_PARENTHESIS) {
            // we need some sort of error callback here
            return NULL;
        }
        token_iterator_advance(state);
        return expression_exponential(arena, state, inner);
    }

    // end of input
    return NULL;
}

/// Parses a unary-plus-or-minus expression
static Expression *expression_unary_plus_or_minus(MemoryArena *arena, TokenIterator *state) {
    if (token_iterator_current(state)->type == TOKEN_PLUS || token_iterator_current(state)->type == TOKEN_MINUS) {
        Operator const operator= token_iterator_current(state)->type == TOKEN_PLUS ? OPERATOR_ADD : OPERATOR_SUB;
        token_iterator_advance(state);

        Expression *inner = expression_unary_plus_or_minus(arena, state);
        if (!inner) {
            return NULL;
        }

        return unary_expression_new(arena, operator, inner);
    }
    return expression_primary(arena, state);
}

/// Parses a multiplication or division expression
static Expression *expression_mul_or_div(MemoryArena *arena, TokenIterator *state) {
    // The first term in the multiplication or division can be an
    // expression of higher precedence, which are all contained by
    // unary plus or minus expressions
    Expression *left = expression_unary_plus_or_minus(arena, state);
    if (!left) {
        return NULL;
    }

    // In the next step, we try to consume either a plus or minus sign
    while (token_iterator_current(state)->type == TOKEN_ASTERISK ||
           token_iterator_current(state)->type == TOKEN_SLASH) {
        Operator const operator= token_iterator_current(state)->type == TOKEN_ASTERISK ? OPERATOR_MUL : OPERATOR_DIV;
        token_iterator_advance(state);

        // On the right side of the expression, we again try to parse an expression
        // of higher precedence
        Expression *right = expression_unary_plus_or_minus(arena, state);
        if (!right) {
            return NULL;
        }

        // Reassign the subexpression to the previous subexpression, as it then contains all previous subexpressions
        left = binary_expression_new(arena, left, right, operator);
    }
    return left;
}

/// Parses an addition or subtraction expression
static Expression *expression_add_or_sub(MemoryArena *arena, TokenIterator *state) {
    // The first term in the addition or subtraction can be an
    // expression of higher precedence, they are all handled by
    // multiplication or division as it is next in the
    // precedence hierarchy
    Expression *left = expression_mul_or_div(arena, state);
    if (!left) {
        return NULL;
    }

    // In the next step, we try to consume either a plus or minus sign
    while (token_iterator_current(state)->type == TOKEN_PLUS || token_iterator_current(state)->type == TOKEN_MINUS) {
        Operator const operator= token_iterator_current(state)->type == TOKEN_PLUS ? OPERATOR_ADD : OPERATOR_SUB;
        token_iterator_advance(state);

        // On the right side of the plus or minus sign, we again try to parse an expression of higher precedence
        Expression *right = expression_mul_or_div(arena, state);
        if (!right) {
            return NULL;
        }

        // Reassign the subexpression to the previous subexpression, as it then contains all previous subexpressions
        left = binary_expression_new(arena, left, right, operator);
    }
    return left;
}

static Expression *expression_arithmetic_or_final(MemoryArena *arena, TokenIterator *state) {
    Token const *current = token_iterator_current(state);
    if (current->type == TOKEN_STRING) {
        return string_expression_new(arena, current->lexeme, current->length);
    }
    return expression_add_or_sub(arena, state);
}

/// Compiles an expression from a list of tokens
Expression *expression_compile(MemoryArena *arena, Token *begin, Token *end) {
    TokenIterator state = { 0 };
    state.current = begin;
    state.end = end;
    return expression_arithmetic_or_final(arena, &state);
}

/// Evaluates the specified expression
f64 expression_evaluate(Expression const *self, HashMap const *symbol_table) {
    assert(expression_is_arithmetic(self) && "expression must be arithmetic for evaluation!");

    switch (self->type) {
        case EXPRESSION_BINARY:
            return binary_expression_evaluate(self, symbol_table);
        case EXPRESSION_NUMBER:
            return number_expression_evaluate(self);
        case EXPRESSION_VARIABLE:
            return variable_expression_evaluate(self, symbol_table);
        case EXPRESSION_FUNCTION:
            return function_expression_evaluate(self, symbol_table);
        case EXPRESSION_UNARY:
            return unary_expression_evaluate(self, symbol_table);
        case EXPRESSION_EXPONENTIAL:
            return exponential_expression_evaluate(self, symbol_table);
        default:
            break;
    }
    return 0.0;
}

/// Checks if an expression is arithmetic
bool expression_is_arithmetic(Expression const *self) {
    switch (self->type) {
        case EXPRESSION_STRING:
            return false;
        default:
            return true;
    }
}

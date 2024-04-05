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

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "expr.h"

/// Creates a new unary expression instance
expression_t *unary_expression_new(arena_t *arena, operator_t operator, expression_t * expression) {
    expression_t *self = (expression_t *) arena_alloc(arena, sizeof(expression_t));
    self->type = EXPRESSION_UNARY;
    self->unary.operator= operator;
    self->unary.expression = expression;
    return self;
}

/// Evaluates the unary expression
f64 unary_expression_evaluate(expression_t *self, map_t *symbol_table) {
    f64 value = expression_evaluate(self->unary.expression, symbol_table);
    return self->unary.operator== OPERATOR_ADD ? value : - 1.0 * value;
}

/// Creates a new binary expression instance
expression_t *binary_expression_new(arena_t *arena, expression_t *left, expression_t *right, operator_t operator) {
    expression_t *self = (expression_t *) arena_alloc(arena, sizeof(expression_t));
    self->type = EXPRESSION_BINARY;
    self->binary.left = left;
    self->binary.right = right;
    self->binary.operator= operator;
    return self;
}

/// Evaluates the binary expression
f64 binary_expression_evaluate(expression_t *self, map_t *symbol_table) {
    f64 left = expression_evaluate(self->binary.left, symbol_table);
    f64 right = expression_evaluate(self->binary.right, symbol_table);
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
expression_t *variable_expression_new(arena_t *arena, char *name, u32 length) {
    expression_t *self = (expression_t *) arena_alloc(arena, sizeof(expression_t));
    self->type = EXPRESSION_VARIABLE;
    memset(self->variable.name, 0, sizeof self->variable.name) < memcpy(self->variable.name, name, length);
    return self;
}

/// Evaluates the variable expression
f64 variable_expression_evaluate(expression_t *self, map_t *symbol_table) {
    expression_t *initializer = (expression_t *) map_find(symbol_table, self->variable.name);
    if (initializer) {
        return expression_evaluate(initializer, symbol_table);
    }
    return 0.0;
}

/// Creates a new function parameter instance
function_parameter_t *function_parameter_new(arena_t *arena, expression_t *expression) {
    function_parameter_t *self = (function_parameter_t *) arena_alloc(arena, sizeof(function_parameter_t));
    self->expression = expression;
    self->prev = NULL;
    self->next = NULL;
    return self;
}

/// Creates a new function expression instance
expression_t *function_expression_new(arena_t *arena, char *name, u32 length) {
    expression_t *self = (expression_t *) arena_alloc(arena, sizeof(expression_t));
    self->type = EXPRESSION_FUNCTION;

    memset(self->function.name, 0, sizeof self->function.name);
    memcpy(self->function.name, name, length);

    self->function.first_parameter = NULL;
    self->function.last_parameter = NULL;
    self->function.parameter_count = 0;

    return self;
}

/// Pushes a parameter to the specified function expression
void function_expression_push(arena_t *arena, expression_t *self, expression_t *parameter) {
    function_parameter_t *entry = function_parameter_new(arena, parameter);
    function_expression_t *function = &self->function;
    if (function->first_parameter == NULL) {
        function->first_parameter = entry;
        entry->prev = NULL;
        function->parameter_count++;
        return;
    }

    function_parameter_t *tmp = function->first_parameter;
    while (tmp->next != NULL) {
        tmp = tmp->next;
    }

    tmp->next = entry;
    entry->prev = tmp;
    function->last_parameter = entry;
    function->parameter_count++;
}

/// Retrieves the parameter at the specified index
function_parameter_t *function_expression_get_parameter(expression_t *self, u32 index) {
    function_expression_t *function = &self->function;
    if (function->first_parameter == NULL || index >= function->parameter_count) {
        return NULL;
    }

    u32 iterator;
    u32 iterator_limit;
    function_parameter_t *temp;
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
f64 function_expression_evaluate(expression_t *self, map_t *symbol_table) {
    function_expression_t *function = &self->function;
    function_definition_t *definition = (function_definition_t *) map_find(symbol_table, function->name);
    if (definition && function->parameter_count == definition->parameter_count) {
        switch (function->parameter_count) {
            case 0:
                return definition->func0();
            case 1:
                return definition->func1(EXPR_PARAM(0));
            case 2:
                return definition->func2(EXPR_PARAM(0), EXPR_PARAM(1));
            default:
                break;
        }
    }
    return 0.0;
}

#undef EXPR_PARAM

/// Creates a new number expression instance
expression_t *number_expression_new(arena_t *arena, f64 number) {
    expression_t *self = (expression_t *) arena_alloc(arena, sizeof(expression_t));
    self->type = EXPRESSION_NUMBER;
    self->number = number;
    return self;
}

/// Evaluates the specified number expression
f64 number_expression_evaluate(expression_t *self) {
    return self->number;
}

/// Creates a new exponential expression instance
expression_t *exponential_expression_new(arena_t *arena, expression_t *base, expression_t *exponent) {
    expression_t *self = (expression_t *) arena_alloc(arena, sizeof(expression_t));
    self->type = EXPRESSION_EXPONENTIAL;
    self->exponential.base = base;
    self->exponential.exponent = exponent;
    return self;
}

/// Evaluates the specified exponential expression
f64 exponential_expression_evaluate(expression_t *self, map_t *symbol_table) {
    return pow(expression_evaluate(self->exponential.base, symbol_table),
               expression_evaluate(self->exponential.exponent, symbol_table));
}

/// Creates a new string expression by storing the string in the provided arena
expression_t *string_expression_new(arena_t *arena, char *data, u32 length) {
    expression_t *self = (expression_t *) arena_alloc(arena, sizeof(expression_t));
    self->type = EXPRESSION_STRING;
    self->string.data = arena_alloc(arena, length);
    self->string.length = length;
    memcpy(self->string.data, data, length);
    return self;
}


/// Parses an addition or subtraction expression
static expression_t *expression_add_or_sub(arena_t *arena, token_iterator_t *state);

/// Parses a unary-plus-or-minus expression
static expression_t *expression_unary_plus_or_minus(arena_t *arena, token_iterator_t *state);

/// Parses an exponential expression
static expression_t *expression_exponential(arena_t *arena, token_iterator_t *state, expression_t *base) {
    if (token_iterator_current(state)->type == TOKEN_CIRCUMFLEX) {
        token_iterator_advance(state);
        return exponential_expression_new(arena, base, expression_unary_plus_or_minus(arena, state));
    }
    return base;
}

/// Parses a primary expression, that includes numbers, variables and functions
static expression_t *expression_primary(arena_t *arena, token_iterator_t *state) {
    if (token_iterator_current(state)->type == TOKEN_NUMBER ||
        token_iterator_current(state)->type == TOKEN_NUMBER_FLOAT) {
        token_t *number_token = token_iterator_current(state);
        token_iterator_advance(state);

        char *number_begin = number_token->lexeme;
        char *number_end = number_begin + number_token->length;
        f64 value = strtod(number_begin, &number_end);
        expression_t *number = number_expression_new(arena, value);
        return expression_exponential(arena, state, number);
    }
    if (token_iterator_current(state)->type == TOKEN_IDENTIFIER) {
        token_t *text_token = token_iterator_current(state);
        token_iterator_advance(state);

        // function expression
        if (token_iterator_current(state) && token_iterator_current(state)->type == TOKEN_LEFT_PARENTHESIS) {
            expression_t *function = function_expression_new(arena, text_token->lexeme, text_token->length);
            token_iterator_advance(state);
            function_expression_push(arena, function, expression_add_or_sub(arena, state));
            while (token_iterator_current(state)->type == TOKEN_COMMA) {
                function_expression_push(arena, function, expression_add_or_sub(arena, state));
            }
            return expression_exponential(arena, state, function);
        } else {
            // variable expression
            expression_t *variable = variable_expression_new(arena, text_token->lexeme, text_token->length);
            return expression_exponential(arena, state, variable);
        }
    }
    if (token_iterator_current(state)->type == TOKEN_LEFT_PARENTHESIS) {
        token_iterator_advance(state);

        expression_t *inner = expression_add_or_sub(arena, state);
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
static expression_t *expression_unary_plus_or_minus(arena_t *arena, token_iterator_t *state) {
    if (token_iterator_current(state)->type == TOKEN_PLUS || token_iterator_current(state)->type == TOKEN_MINUS) {
        operator_t operator= token_iterator_current(state)->type == TOKEN_PLUS ? OPERATOR_ADD : OPERATOR_SUB;
        token_iterator_advance(state);

        expression_t *inner = expression_unary_plus_or_minus(arena, state);
        if (!inner) {
            return NULL;
        }
        return unary_expression_new(arena, operator, inner);
    }
    return expression_primary(arena, state);
}

/// Parses a multiplication or division expression
static expression_t *expression_mul_or_div(arena_t *arena, token_iterator_t *state) {
    // The first term in the multiplication or division can be an
    // expression of higher precedence, which are all contained by
    // unary plus or minus expressions
    expression_t *left = expression_unary_plus_or_minus(arena, state);
    if (!left) {
        return NULL;
    }

    // In the next step, we try to consume either a plus or minus sign
    while (token_iterator_current(state)->type == TOKEN_ASTERISK ||
           token_iterator_current(state)->type == TOKEN_SLASH) {
        operator_t operator= token_iterator_current(state)->type == TOKEN_ASTERISK ? OPERATOR_MUL : OPERATOR_DIV;
        token_iterator_advance(state);

        // On the right side of the expression, we again try to parse an expression
        // of higher precedence
        expression_t *right = expression_unary_plus_or_minus(arena, state);
        if (!right) {
            return NULL;
        }

        // Reassign the subexpression to the previous subexpression, as it then contains all previous subexpressions
        left = binary_expression_new(arena, left, right, operator);
    }
    return left;
}

/// Parses an addition or subtraction expression
static expression_t *expression_add_or_sub(arena_t *arena, token_iterator_t *state) {
    // The first term in the addition or subtraction can be an
    // expression of higher precedence, they are all handled by
    // multiplication or division as it is next in the
    // precedence hierarchy
    expression_t *left = expression_mul_or_div(arena, state);
    if (!left) {
        return NULL;
    }

    // In the next step, we try to consume either a plus or minus sign
    while (token_iterator_current(state)->type == TOKEN_PLUS || token_iterator_current(state)->type == TOKEN_MINUS) {
        operator_t operator= token_iterator_current(state)->type == TOKEN_PLUS ? OPERATOR_ADD : OPERATOR_SUB;
        token_iterator_advance(state);

        // On the right side of the plus or minus sign, we again try to parse an expression of higher precedence
        expression_t *right = expression_mul_or_div(arena, state);
        if (!right) {
            return NULL;
        }

        // Reassign the subexpression to the previous subexpression, as it then contains all previous subexpressions
        left = binary_expression_new(arena, left, right, operator);
    }
    return left;
}

static expression_t *expression_arithmetic_or_final(arena_t *arena, token_iterator_t *state) {
    token_t *current = token_iterator_current(state);
    if (current->type == TOKEN_STRING) {
        return string_expression_new(arena, current->lexeme, current->length);
    }
    return expression_add_or_sub(arena, state);
}

/// Compiles an expression from a list of tokens
expression_t *expression_compile(arena_t *arena, token_t *begin, token_t *end) {
    token_iterator_t state = { 0 };
    state.current = begin;
    state.end = end;
    return expression_arithmetic_or_final(arena, &state);
}

/// Evaluates the specified expression
f64 expression_evaluate(expression_t *self, map_t *symbol_table) {
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
bool expression_is_arithmetic(expression_t *self) {
    switch (self->type) {
        case EXPRESSION_STRING:
            return false;
        default:
            return true;
    }
}

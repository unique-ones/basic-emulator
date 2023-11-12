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

#ifndef RETRO_EXPR_H
#define RETRO_EXPR_H

#include "lexer.h"
#include "types.h"
#include "util/arena.h"
#include "util/map.h"

typedef enum expression_type {
    EXPRESSION_BINARY,
    EXPRESSION_NUMBER,
    EXPRESSION_VARIABLE,
    EXPRESSION_FUNCTION,
    EXPRESSION_UNARY,
    EXPRESSION_EXPONENTIAL
} expression_type_t;

/// Forward declare expression
typedef struct expression expression_t;

typedef enum operator{ OPERATOR_ADD, OPERATOR_SUB, OPERATOR_MUL, OPERATOR_DIV } operator_t;

typedef struct unary_expression {
    operator_t operator;
    expression_t* expression;
} unary_expression_t;

/// Creates a new unary expression instance
/// @param arena The arena for the allocation
/// @param operator The operator
/// @param expression The expression
/// @return A new expression instance
expression_t* unary_expression_new(arena_t* arena, operator_t operator, expression_t * expression);

/// Evaluates the unary expression
/// @param self The expression instance
/// @param symbol_table The symbol table
/// @return The resulting value
f64 unary_expression_evaluate(expression_t* self, map_t* symbol_table);

typedef struct binary_expression {
    expression_t* left;
    expression_t* right;
    operator_t operator;
} binary_expression_t;

/// Creates a new binary expression instance
/// @param arena The arena for allocations
/// @param left left expression
/// @param right right expression
/// @param operator binary operator
/// @return A new expression instance
expression_t* binary_expression_new(arena_t* arena, expression_t* left, expression_t* right, operator_t operator);

/// Evaluates the binary expression
/// @param self The expression instance
/// @param symbol_table The symbol table
/// @return The resulting value
f64 binary_expression_evaluate(expression_t* self, map_t* symbol_table);

typedef struct variable_expression {
    char* name;
    u32 length;
} variable_expression_t;

/// Creates a new variable expression instance
/// @param arena The arena for allocations
/// @param name The name of the variable
/// @param length The length of the variable name
/// @return A new expression instance
expression_t* variable_expression_new(arena_t* arena, char* name, u32 length);

/// Evaluates the variable expression
/// @param self The expression instance
/// @param symbol_table The symbol table
/// @return The resulting value
f64 variable_expression_evaluate(expression_t* self, map_t* symbol_table);

typedef struct function_parameter {
    expression_t* expression;
    struct function_parameter* prev;
    struct function_parameter* next;
} function_parameter_t;

/// Creates a new function parameter instance
/// @param arena The arena for allocations
/// @param expression The expression
/// @return A new function parameter instance
function_parameter_t* function_parameter_new(arena_t* arena, expression_t* expression);

typedef struct function_expression {
    char* name;
    u32 length;
    function_parameter_t* first_parameter;
    function_parameter_t* last_parameter;
    u32 parameter_count;
} function_expression_t;

typedef struct function_definition {
    const char* name;
    u32 parameter_count;
    union {
        f64 (*func0)(void);
        f64 (*func1)(f64);
        f64 (*func2)(f64, f64);
    };
} function_definition_t;

/// Creates a new function expression instance
/// @param arena The arena for allocations
/// @param name The name of the function
/// @param length The length of the function name
/// @return A new expression instance
expression_t* function_expression_new(arena_t* arena, char* name, u32 length);

/// Pushes a parameter to the specified function expression
/// @param arena The arena for allocations
/// @param self The expression instance
/// @param parameter The parameter
void function_expression_push(arena_t* arena, expression_t* self, expression_t* parameter);

/// Retrieves the parameter at the specified index
/// @param self The expression instance
/// @param index The parameter index
function_parameter_t* function_expression_get_parameter(expression_t* self, u32 index);

/// Evaluates the specified function expression
/// @param self The function expression instance
/// @param symbol_table The symbol table
/// @return The resulting value
f64 function_expression_evaluate(expression_t* self, map_t* symbol_table);

/// Creates a new number expression instance
/// @param arena The arena for allocations
/// @param number The number
/// @return A new expression instance
expression_t* number_expression_new(arena_t* arena, f64 number);

/// Evaluates the specified number expression
/// @param self The expression instance
/// @return The resulting value
f64 number_expression_evaluate(expression_t* self);

typedef struct exponential_expression {
    expression_t* base;
    expression_t* exponent;
} exponential_expression_t;

/// Creates a new exponential expression instance
/// @param arena The arena for allocations
/// @param base The base of the exponential expression
/// @param exponent The exponent of the exponential expression
/// @return A new expression instance
expression_t* exponential_expression_new(arena_t* arena, expression_t* base, expression_t* exponent);

/// Evaluates the specified exponential expression
/// @param self The expression instance
/// @param symbol_table The symbol table
/// @return The resulting value
f64 exponential_expression_evaluate(expression_t* self, map_t* symbol_table);

/// What the fuck is this... we probably want to use shunting yard here at some point
typedef struct expression {
    expression_type_t type;
    union {
        unary_expression_t unary;
        binary_expression_t binary;
        variable_expression_t variable;
        function_expression_t function;
        exponential_expression_t exponential;
        f64 number;
    };
} expression_t;

/// Compiles an expression from a list of tokens
/// @param arena The arena for allocations
/// @param begin The first token
/// @param end The last token
/// @return The resulting expression
expression_t* expression_compile(arena_t* arena, token_t* begin, token_t* end);

/// Evaluates the specified expression
/// @param self The expression instance
/// @param symbol_table The symbol table
/// @return The resulting value
f64 expression_evaluate(expression_t* self, map_t* symbol_table);

#endif// RETRO_EXPR_H

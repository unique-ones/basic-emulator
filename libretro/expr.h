#ifndef RETRO_EXPR_H
#define RETRO_EXPR_H

#include "lexer.h"
#include "map.h"
#include "types.h"

typedef enum expression_type {
    EXPRESSION_BINARY,
    EXPRESSION_NUMBER,
    EXPRESSION_VARIABLE,
    EXPRESSION_FUNCTION,
    EXPRESSION_UNARY,
    EXPRESSION_EXPONENTIAL
} expression_type_t;

// forward declare expression
typedef struct expression expression_t;

typedef enum operator{ OPERATOR_ADD, OPERATOR_SUB, OPERATOR_MUL, OPERATOR_DIV } operator_t;

typedef struct unary_expression {
    operator_t operator;
    expression_t* expression;
} unary_expression_t;

/**
 * @brief creates a new unary expression instance
 * @param operator operator
 * @param expression expression
 * @return expression instance
 */
expression_t* unary_expression_new(operator_t operator, expression_t * expression);

/**
 * @brief frees the specified unary expression and its inner expression
 * @param self expression instance
 */
void unary_expression_free(expression_t* self);

/**
 * @brief evaluates the unary expression
 * @param self expression instance
 * @param symbol_table symbol table
 * @return value
 */
f64 unary_expression_evaluate(expression_t* self, map_t* symbol_table);

typedef struct binary_expression {
    expression_t* left;
    expression_t* right;
    operator_t operator;
} binary_expression_t;

/**
 * @brief creates a new binary expression instance
 * @param left left expression
 * @param right right expression
 * @param operator binary operator
 * @return expression instance
 */
expression_t* binary_expression_new(expression_t* left, expression_t* right, operator_t operator);

/**
 * @brief frees the specified binary expression and its inner expressions
 * @param self expression instance
 */
void binary_expression_free(expression_t* self);

/**
 * @brief evaluates the binary expression
 * @param self expression instance
 * @param symbol_table symbol table
 * @return value
 */
f64 binary_expression_evaluate(expression_t* self, map_t* symbol_table);

typedef struct variable_expression {
    char* name;
    u32 length;
} variable_expression_t;

/**
 * @brief creates a new variable expression instance
 * @param name name of the variable
 * @param length length of the variable name
 * @return expression instance
 */
expression_t* variable_expression_new(char* name, u32 length);

/**
 * @brief frees the specified variable expression
 * @param self expression instance
 */
void variable_expression_free(expression_t* self);

/**
 * @brief evaluates the variable expression
 * @param self expression instance
 * @param symbol_table symbol table
 * @return value
 */
f64 variable_expression_evaluate(expression_t* self, map_t* symbol_table);

typedef struct function_parameter {
    expression_t* expression;
    struct function_parameter* prev;
    struct function_parameter* next;
} function_parameter_t;

/**
 * @brief creates a new function parameter instance
 * @param expression expression
 * @return function parameter instance
 */
function_parameter_t* function_parameter_new(expression_t* expression);

/**
 * @brief frees the specified function parameter
 * @param self function parameter instance
 */
void function_parameter_free(function_parameter_t* self);

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

/**
 * @brief creates a new function expression instance
 * @param name name of the function
 * @param length length of the function name
 * @return expression instance
 */
expression_t* function_expression_new(char* name, u32 length);

/**
 * @brief frees the specified function expression
 * @param self expression instance
 */
void function_expression_free(expression_t* self);

/**
 * @brief pushes a parameter to the specified function expression
 * @param self expression instance
 * @param parameter parameter
 */
void function_expression_push(expression_t* self, expression_t* parameter);

/**
 * @brief retrieves the parameter at the specified index
 * @param self expression instance
 * @param index parameter index
 */
function_parameter_t* function_expression_get_parameter(expression_t* self, u32 index);

/**
 * @brief evaluates the specified function expression
 * @param self function expression instance
 * @param symbol_table symbol table
 * @return value
 */
f64 function_expression_evaluate(expression_t* self, map_t* symbol_table);

/**
 * @brief creates a new number expression instance
 * @param number number
 * @return expression instance
 */
expression_t* number_expression_new(f64 number);

/**
 * @brief frees the specified number expression instance
 * @param self expression instance
 */
void number_expression_free(expression_t* self);

/**
 * @brief evaluates the specified number expression
 * @param self expression instance
 * @return value
 */
f64 number_expression_evaluate(expression_t* self);

typedef struct exponential_expression {
    expression_t* base;
    expression_t* exponent;
} exponential_expression_t;

/**
 * @brief creates a new exponential expression instance
 * @param base the base of the exponential expression
 * @param exponent the exponent of the exponential expression
 * @return expression instance
 */
expression_t* exponential_expression_new(expression_t* base, expression_t* exponent);

/**
 * @brief frees the specified exponential expression
 * @param self expression instance
 */
void exponential_expression_free(expression_t* self);

/**
 * @brief evaluates the specified exponential expression
 * @param self expression instance
 * @param symbol_table symbol table
 * @return value
 */
f64 exponential_expression_evaluate(expression_t* self, map_t* symbol_table);

/**
 * @brief What the fuck is this... we probably want to use shunting yard here
 */
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

/**
 * @brief compiles an expression from a list of tokens
 * @param begin first token
 * @param end last token
 * @return expression
 */
expression_t* expression_compile(token_t* begin, token_t* end);

/**
 * @brief frees the specified expression instance
 * @param self expression instance
 */
void expression_free(expression_t* self);

/**
 * @brief evaluates the specified expression
 * @param self expression instance
 * @param symbol_table symbol table
 * @return value
 */
f64 expression_evaluate(expression_t* self, map_t* symbol_table);

#endif// RETRO_EXPR_H

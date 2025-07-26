// Copyright (c) 2025 Elias Engelbert Plank

#ifndef RETRO_EXPR_H
#define RETRO_EXPR_H

enum {
    EXPRESSION_IDENTIFIER_LENGTH = 64
};

typedef enum ExpressionType {
    EXPRESSION_BINARY,
    EXPRESSION_NUMBER,
    EXPRESSION_VARIABLE,
    EXPRESSION_FUNCTION,
    EXPRESSION_UNARY,
    EXPRESSION_EXPONENTIAL,
    EXPRESSION_STRING
} ExpressionType;

/// Forward declare expression
typedef struct Expression Expression;

typedef enum Operator {
    OPERATOR_ADD,
    OPERATOR_SUB,
    OPERATOR_MUL,
    OPERATOR_DIV
} Operator;

typedef struct UnaryExpression {
    Operator operator;
    Expression *expression;
} UnaryExpression;

/// Creates a new unary expression instance
/// @param arena The arena for the allocation
/// @param operator The operator
/// @param expression The expression
/// @return A new expression instance
static Expression *unary_expression_new(MemoryArena *arena, Operator operator, Expression * expression);

/// Evaluates the unary expression
/// @param self The expression instance
/// @param symbol_table The symbol table
/// @return The resulting value
static f64 unary_expression_evaluate(Expression const *self, HashMap const *symbol_table);

typedef struct BinaryExpression {
    Expression *left;
    Expression *right;
    Operator operator;
} BinaryExpression;

/// Creates a new binary expression instance
/// @param arena The arena for allocations
/// @param left left expression
/// @param right right expression
/// @param operator binary operator
/// @return A new expression instance
static Expression *binary_expression_new(MemoryArena *arena, Expression *left, Expression *right, Operator operator);

/// Evaluates the binary expression
/// @param self The expression instance
/// @param symbol_table The symbol table
/// @return The resulting value
static f64 binary_expression_evaluate(Expression const *self, HashMap const *symbol_table);

typedef struct VariableExpression {
    char name[EXPRESSION_IDENTIFIER_LENGTH];
} VariableExpression;

/// Creates a new variable expression instance
/// @param arena The arena for allocations
/// @param name The name of the variable
/// @param length The length of the variable name
/// @return A new expression instance
static Expression *variable_expression_new(MemoryArena *arena, char const *name, usize length);

/// Evaluates the variable expression
/// @param self The expression instance
/// @param symbol_table The symbol table
/// @return The resulting value
static f64 variable_expression_evaluate(Expression const *self, HashMap const *symbol_table);

typedef struct FunctionParameter {
    Expression *expression;
    struct FunctionParameter *prev;
    struct FunctionParameter *next;
} FunctionParameter;

/// Creates a new function parameter instance
/// @param arena The arena for allocations
/// @param expression The expression
/// @return A new function parameter instance
static FunctionParameter *function_parameter_new(MemoryArena *arena, Expression *expression);

typedef struct FunctionExpression {
    char name[EXPRESSION_IDENTIFIER_LENGTH];
    FunctionParameter *first_parameter;
    FunctionParameter *last_parameter;
    usize parameter_count;
} FunctionExpression;

typedef struct FunctionDefinitionBuiltin {
    enum {
        PARAMETER_COUNT_0 = 0,
        PARAMETER_COUNT_1 = 1,
        PARAMETER_COUNT_2 = 2
    } parameter_count;

    union {
        f64 (*func0)(void);
        f64 (*func1)(f64);
        f64 (*func2)(f64, f64);
    };
} FunctionDefinitionBuiltin;

typedef struct FunctionDefinitionDynamic {
    Expression *variable;
    Expression *body;
} FunctionDefinitionDynamic;

typedef enum FunctionDefinitionType {
    FUNCTION_DEFINITION_BUILTIN,
    FUNCTION_DEFINITION_DYNAMIC
} FunctionDefinitionType;

typedef struct FunctionDefinition {
    const char *name;
    FunctionDefinitionType type;
    union {
        FunctionDefinitionBuiltin builtin;
        FunctionDefinitionDynamic variable;
    };
} FunctionDefinition;

/// Creates a new function expression instance
/// @param arena The arena for allocations
/// @param name The name of the function
/// @param length The length of the function name
/// @return A new expression instance
static Expression *function_expression_new(MemoryArena *arena, char const *name, usize length);

/// Pushes a parameter to the specified function expression
/// @param arena The arena for allocations
/// @param self The expression instance
/// @param parameter The parameter
static void function_expression_push(MemoryArena *arena, Expression *self, Expression *parameter);

/// Retrieves the parameter at the specified index
/// @param self The expression instance
/// @param index The parameter index
static FunctionParameter *function_expression_get_parameter(Expression const *self, usize index);

/// Evaluates the specified function expression
/// @param self The function expression instance
/// @param symbol_table The symbol table
/// @return The resulting value
static f64 function_expression_evaluate(Expression const *self, HashMap const *symbol_table);

/// Creates a new number expression instance
/// @param arena The arena for allocations
/// @param number The number
/// @return A new expression instance
static Expression *number_expression_new(MemoryArena *arena, f64 number);

/// Evaluates the specified number expression
/// @param self The expression instance
/// @return The resulting value
static f64 number_expression_evaluate(Expression const *self);

typedef struct ExponentialExpression {
    Expression *base;
    Expression *exponent;
} ExponentialExpression;

/// Creates a new exponential expression instance
/// @param arena The arena for allocations
/// @param base The base of the exponential expression
/// @param exponent The exponent of the exponential expression
/// @return A new expression instance
static Expression *exponential_expression_new(MemoryArena *arena, Expression *base, Expression *exponent);

/// Evaluates the specified exponential expression
/// @param self The expression instance
/// @param symbol_table The symbol table
/// @return The resulting value
static f64 exponential_expression_evaluate(Expression const *self, HashMap const *symbol_table);

typedef struct StringExpression {
    char *data;
    usize length;
} StringExpression;

/// Creates a new string expression by storing the string in the provided arena
/// @param arena The arena for allocations
/// @param data A pointer to the string
/// @param length The length of the string
static Expression *string_expression_new(MemoryArena *arena, char const *data, usize length);

/// What the fuck is this... we probably want to use shunting yard here at some point
typedef struct Expression {
    ExpressionType type;
    union {
        // arithmetic expressions
        UnaryExpression unary;
        BinaryExpression binary;
        VariableExpression variable;
        FunctionExpression function;
        ExponentialExpression exponential;
        f64 number;

        // non-arithmetic expressions
        StringExpression string;
    };
} Expression;

/// Compiles an expression from a list of tokens
/// @param arena The arena for allocations
/// @param begin The first token
/// @param end The last token
/// @return The resulting expression
static Expression *expression_compile(MemoryArena *arena, Token *begin, Token *end);

/// Evaluates the specified expression
/// @param self The expression instance
/// @param symbol_table The symbol table
/// @return The resulting value
static f64 expression_evaluate(Expression const *self, HashMap const *symbol_table);

/// Checks if an expression is arithmetic
/// @param self The expression instance
/// @param A b32ean value that indicates whether the expression
///        is arithmetic or not
static b32 expression_is_arithmetic(Expression const *self);

#endif// RETRO_EXPR_H

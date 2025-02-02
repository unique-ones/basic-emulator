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
#include <stdio.h>
#include <string.h>

#include <libretro/util/stack.h>

typedef struct Employee {
    const char *name;
    u32 id;
} Employee;

bool employee_equal(Employee *a, Employee *b) {
    if (a->id != b->id) {
        return false;
    }
    return strcmp(a->name, b->name) == 0;
}

static void stack_test_push(void) {
    Employee *joseph = &(Employee){ .name = "Joseph", .id = 1 };
    Employee *adam = &(Employee){ .name = "Adam", .id = 2 };

    Stack *stack = stack_new(4);
    stack_push(stack, joseph);
    stack_push(stack, adam);
    assert(stack->size == 2 && "stack size mismatch");
    stack_free(stack);
}

static void stack_test_pop(void) {
    Employee *peter = &(Employee){ .name = "Peter", .id = 1 };
    Employee *michael = &(Employee){ .name = "Michael", .id = 10 };
    Employee *natalie = &(Employee){ .name = "Natalie", .id = 3 };

    Stack *stack = stack_new(4);
    stack_push(stack, peter);
    stack_push(stack, michael);
    stack_push(stack, natalie);

    Employee *popped = stack_pop(stack);
    assert(stack->size == 2 && "stack size mismatch");
    assert(employee_equal(popped, natalie) && "stack entry mismatch");
    stack_free(stack);
}

static void stack_test_peek(void) {
    Employee *mario = &(Employee){ .name = "Mario", .id = 11 };
    Employee *eva = &(Employee){ .name = "Eva", .id = 20 };
    Employee *alina = &(Employee){ .name = "Alina", .id = 17 };

    Stack *stack = stack_new(4);
    stack_push(stack, mario);
    stack_push(stack, eva);
    stack_push(stack, alina);

    Employee *top = stack_peek(stack);
    assert(stack->size == 3 && "stack size mismatch");
    assert(employee_equal(top, alina) && "stack entry mismatch");

    Employee *popped = stack_pop(stack);
    assert(stack->size == 2 && "stack size mismatch");
    assert(employee_equal(top, popped) && "stack entry mismatch");

    top = stack_peek(stack);
    assert(employee_equal(top, eva) && "stack entry mismatch");
    stack_free(stack);
}

static void stack_test_grow(void) {
    Employee *mario = &(Employee){ .name = "Mario", .id = 11 };
    Employee *eva = &(Employee){ .name = "Eva", .id = 20 };
    Employee *alina = &(Employee){ .name = "Alina", .id = 17 };
    Employee *peter = &(Employee){ .name = "Peter", .id = 1 };
    Employee *michael = &(Employee){ .name = "Michael", .id = 10 };
    Employee *natalie = &(Employee){ .name = "Natalie", .id = 3 };
    Employee *joseph = &(Employee){ .name = "Joseph", .id = 1 };

    Stack *stack = stack_new(3);
    stack_push(stack, mario);
    stack_push(stack, eva);
    stack_push(stack, alina);
    assert(stack->capacity == 3 && "stack capacity mismatch");

    stack_push(stack, peter);
    assert(stack->capacity == 6 && "stack capacity mismatch");

    stack_push(stack, michael);
    stack_push(stack, natalie);
    stack_push(stack, joseph);
    assert(stack->capacity == 12 && "stack capacity mismatch");

    stack_free(stack);
}

int main(int argc, char **argv) {
    stack_test_push();
    stack_test_pop();
    stack_test_peek();
    stack_test_grow();
    return 0;
}
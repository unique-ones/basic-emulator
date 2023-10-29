#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <libretro/util/stack.h>

typedef struct employee {
    const char* name;
    u32 id;
} employee_t;

bool employee_equal(employee_t* a, employee_t* b) {
    if (a->id != b->id) {
        return false;
    }
    return strcmp(a->name, b->name) == 0;
}

static void stack_test_push(void) {
    employee_t* joseph = &(employee_t){ .name = "Joseph", .id = 1 };
    employee_t* adam = &(employee_t){ .name = "Adam", .id = 2 };

    stack_t* stack = stack_new(4);
    stack_push(stack, joseph);
    stack_push(stack, adam);
    assert(stack->size == 2 && "stack size mismatch");
    stack_free(stack);
}

static void stack_test_pop(void) {
    employee_t* peter = &(employee_t){ .name = "Peter", .id = 1 };
    employee_t* michael = &(employee_t){ .name = "Michael", .id = 10 };
    employee_t* natalie = &(employee_t){ .name = "Natalie", .id = 3 };

    stack_t* stack = stack_new(4);
    stack_push(stack, peter);
    stack_push(stack, michael);
    stack_push(stack, natalie);

    employee_t* popped = stack_pop(stack);
    assert(stack->size == 2 && "stack size mismatch");
    assert(employee_equal(popped, natalie) && "stack entry mismatch");
    stack_free(stack);
}

static void stack_test_peek(void) {
    employee_t* mario = &(employee_t){ .name = "Mario", .id = 11 };
    employee_t* eva = &(employee_t){ .name = "Eva", .id = 20 };
    employee_t* alina = &(employee_t){ .name = "Alina", .id = 17 };

    stack_t* stack = stack_new(4);
    stack_push(stack, mario);
    stack_push(stack, eva);
    stack_push(stack, alina);

    employee_t* top = stack_peek(stack);
    assert(stack->size == 3 && "stack size mismatch");
    assert(employee_equal(top, alina) && "stack entry mismatch");

    employee_t* popped = stack_pop(stack);
    assert(stack->size == 2 && "stack size mismatch");
    assert(employee_equal(top, popped) && "stack entry mismatch");

    top = stack_peek(stack);
    assert(employee_equal(top, eva) && "stack entry mismatch");
    stack_free(stack);
}

static void stack_test_grow(void) {
    employee_t* mario = &(employee_t){ .name = "Mario", .id = 11 };
    employee_t* eva = &(employee_t){ .name = "Eva", .id = 20 };
    employee_t* alina = &(employee_t){ .name = "Alina", .id = 17 };
    employee_t* peter = &(employee_t){ .name = "Peter", .id = 1 };
    employee_t* michael = &(employee_t){ .name = "Michael", .id = 10 };
    employee_t* natalie = &(employee_t){ .name = "Natalie", .id = 3 };
    employee_t* joseph = &(employee_t){ .name = "Joseph", .id = 1 };

    stack_t* stack = stack_new(3);
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

int main(int argc, char** argv) {
    stack_test_push();
    stack_test_pop();
    stack_test_peek();
    stack_test_grow();
    return 0;
}
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libretro/util/list.h>

static void list_test_at(void) {
    list_t* list = list_new();

    char* data = "GOSUB 1000";
    for (u32 i = 0; i < strlen(data); i++) {
        list_append(list, data + i);
        assert(list->length == i + 1 && "list_append length mismatch");
    }

    for (u32 i = 0; i < strlen(data); i++) {
        char* node_data = list_at(list, i);
        assert(node_data && "list_at returned NULL data");
        assert(*node_data == *(data + i) && "list_at data mismatch");
    }
    list_free(list);
}

static void list_test_insert(void) {
    list_t* list = list_new();

    char* data = "foo_bar";
    for (u32 i = 0; i < strlen(data); ++i) {
        list_insert(list, i, data + i);
        assert(list->length == i + 1 && "list_insert length mismatch");
    }

    u32 i = 0;
    node_t* node = list->head;
    while (node != NULL) {
        assert(*((char*) (node->data)) == data[i++]);
        node = node->next;
    }

    list_free(list);
}

typedef enum animal_type { MAMMAL = 0, INSECT = 1 } animal_type_t;

typedef struct animal {
    const char* name;
    animal_type_t type;
} animal_t;

static bool animal_compare(const void* a, const void* b) {
    if (a == NULL || b == NULL) {
        return false;
    }
    const animal_t* first_animal = (const animal_t*) a;
    const animal_t* second_animal = (const animal_t*) b;
    if (first_animal->type != second_animal->type) {
        return false;
    }
    return strcmp(first_animal->name, second_animal->name) == 0;
}

static void list_test_find(void) {
    animal_t* bear = &(animal_t){ .name = "joe", .type = MAMMAL };
    animal_t* monkey = &(animal_t){ .name = "jeff", .type = MAMMAL };
    animal_t* bee = &(animal_t){ .name = "mia", .type = INSECT };
    animal_t* cat = &(animal_t){ .name = "rupert", .type = MAMMAL };
    animal_t* dog = &(animal_t){ .name = "sir peanut", .type = MAMMAL };

    list_t* list = list_new();
    node_t* empty_result = list_find(list, cat, animal_compare);
    assert(empty_result == NULL && "list_find returned invalid node");

    list_append(list, bear);
    list_append(list, monkey);
    list_append(list, bee);
    list_append(list, cat);
    list_append(list, dog);

    node_t* dog_result = list_find(list, dog, animal_compare);
    assert(dog_result != NULL && animal_compare(dog_result->data, dog) && "list_find did not find dog entry");

    node_t* cat_result = list_find(list, cat, animal_compare);
    assert(cat_result != NULL && animal_compare(cat_result->data, cat) && "list_find did not find dog entry");

    node_t* bee_result = list_find(list, bee, animal_compare);
    assert(bee_result != NULL && animal_compare(bee_result->data, bee) && "list_find did not find dog entry");

    node_t* monkey_result = list_find(list, monkey, animal_compare);
    assert(monkey_result != NULL && animal_compare(monkey_result->data, monkey) && "list_find did not find dog entry");

    node_t* bear_result = list_find(list, bear, animal_compare);
    assert(bear_result != NULL && animal_compare(bear_result->data, bear) && "list_find did not find dog entry");

    list_free(list);
}

int main(int argc, char** argv) {
    list_test_at();
    list_test_insert();
    list_test_find();
}
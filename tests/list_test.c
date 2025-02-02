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
#include <stdlib.h>
#include <string.h>

#include <libretro/util/list.h>

static void list_test_at(void) {
    LinkedList *list = linked_list_new();

    char *data = "GOSUB 1000";
    for (u32 i = 0; i < strlen(data); i++) {
        linked_list_append(list, data + i);
        assert(list->length == i + 1 && "list_append length mismatch");
    }

    for (u32 i = 0; i < strlen(data); i++) {
        char *node_data = linked_list_at(list, i);
        assert(node_data && "list_at returned NULL data");
        assert(*node_data == *(data + i) && "list_at data mismatch");
    }
    linked_list_free(list);
}

static void list_test_insert(void) {
    LinkedList *list = linked_list_new();

    char *data = "foo_bar";
    for (u32 i = 0; i < strlen(data); ++i) {
        linked_list_insert(list, i, data + i);
        assert(list->length == i + 1 && "list_insert length mismatch");
    }

    u32 i = 0;
    ListNode *node = list->head;
    while (node != NULL) {
        assert(*((char *) (node->data)) == data[i++]);
        node = node->next;
    }

    linked_list_free(list);
}

typedef enum AnimalType {
    MAMMAL = 0,
    INSECT = 1
} AnimalType;

typedef struct Animal {
    const char *name;
    AnimalType type;
} Animal;

static bool animal_compare(const void *a, const void *b) {
    if (a == NULL || b == NULL) {
        return false;
    }
    const Animal *first_animal = (const Animal *) a;
    const Animal *second_animal = (const Animal *) b;
    if (first_animal->type != second_animal->type) {
        return false;
    }
    return strcmp(first_animal->name, second_animal->name) == 0;
}

static void list_test_find(void) {
    Animal *bear = &(Animal){ .name = "joe", .type = MAMMAL };
    Animal *monkey = &(Animal){ .name = "jeff", .type = MAMMAL };
    Animal *bee = &(Animal){ .name = "mia", .type = INSECT };
    Animal *cat = &(Animal){ .name = "rupert", .type = MAMMAL };
    Animal *dog = &(Animal){ .name = "sir peanut", .type = MAMMAL };

    LinkedList *list = linked_list_new();
    ListNode *empty_result = linked_list_find(list, cat, animal_compare);
    assert(empty_result == NULL && "list_find returned invalid node");

    linked_list_append(list, bear);
    linked_list_append(list, monkey);
    linked_list_append(list, bee);
    linked_list_append(list, cat);
    linked_list_append(list, dog);

    ListNode *dog_result = linked_list_find(list, dog, animal_compare);
    assert(dog_result != NULL && animal_compare(dog_result->data, dog) && "list_find did not find dog entry");

    ListNode *cat_result = linked_list_find(list, cat, animal_compare);
    assert(cat_result != NULL && animal_compare(cat_result->data, cat) && "list_find did not find dog entry");

    ListNode *bee_result = linked_list_find(list, bee, animal_compare);
    assert(bee_result != NULL && animal_compare(bee_result->data, bee) && "list_find did not find dog entry");

    ListNode *monkey_result = linked_list_find(list, monkey, animal_compare);
    assert(monkey_result != NULL && animal_compare(monkey_result->data, monkey) && "list_find did not find dog entry");

    ListNode *bear_result = linked_list_find(list, bear, animal_compare);
    assert(bear_result != NULL && animal_compare(bear_result->data, bear) && "list_find did not find dog entry");

    linked_list_free(list);
}

int main(int argc, char **argv) {
    list_test_at();
    list_test_insert();
    list_test_find();
}
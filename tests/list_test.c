#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libretro/list.h>

static void list_test_at() {
    list_t* list = list_new();

    char* data = "GOSUB 1000";
    for (u32 i = 0; i < strlen(data); i++) {
        list_append(list, data + i);
    }

    for (u32 i = 0; i < strlen(data); i++) {
        char* node_data = list_at(list, i);
        assert(*node_data == *(data + i) && "list_at mismatch");
    }
    list_free(list);
}

static void list_test_insert() {
    list_t* list = list_new();

    char* data = "foo_bar";
    for (u32 i = 0; i < strlen(data); ++i) {
        list_insert(list, i, data + i);
    }

    u32 i = 0;
    node_t* node = list->head;
    while (node != NULL) {
        assert(*((char*) (node->data)) == data[i++]);
        node = node->next;
    }

    list_free(list);
}

int main(int argc, char** argv) {
    list_test_at();
    list_test_insert();
}
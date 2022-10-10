#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libretro/list.h>

void list_test_insert() {
    list_t* l = list_new();

    char* data = "abcd";

    for (size_t i = 0; i < strlen(data); ++i) {
        list_insert(l, i, data + i);
    }

    size_t i = 0;
    node_t* node = l->head;
    while (node != NULL) {
        assert(*((char*) (node->data)) == data[i++]);
        node = node->next;
    }

    list_free(l);
}

int main(int argc, char** argv) {
    list_test_insert();
}
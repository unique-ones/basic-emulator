#include "utility.h"

bool file_read(binary_buffer_t* buffer, const char* path) {
    FILE* file = fopen(path, "rb");
    if (!file) {
        return false;
    }

    fseek(file, 0, SEEK_END);
    s32 size = (s32) ftell(file);
    fseek(file, 0, SEEK_SET);

    buffer->data = (char*) malloc(size);
    if (buffer->data) {
        buffer->size = (u32) size;
        fread(buffer->data, sizeof(char), size, file);
        fclose(file);
        buffer->data[size - 1] = 0;
    }
    return true;
}

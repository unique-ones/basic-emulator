// Copyright (c) 2025 Elias Engelbert Plank

/// Reads the entire file into the specified buffer
bool file_read(BinaryBuffer *buffer, const char *path) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        return false;
    }

    fseek(file, 0, SEEK_END);
    u32 const size = ftell(file);
    fseek(file, 0, SEEK_SET);

    buffer->data = (char *) malloc(size + 1);
    if (buffer->data) {
        buffer->size = size;
        fread(buffer->data, sizeof(char), size, file);
        fclose(file);
        buffer->data[size] = 0;
    }
    return true;
}

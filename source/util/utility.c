//
// MIT License
//
// Copyright (c) 2023 Elias Engelbert Plank
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

#include "utility.h"

#include <stdio.h>
#include <stdlib.h>

/// Reads the entire file into the specified buffer
bool file_read(BinaryBuffer *buffer, const char *path) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        return false;
    }

    fseek(file, 0, SEEK_END);
    u32 const size = ftell(file);
    fseek(file, 0, SEEK_SET);

    buffer->data = (char *) malloc(size);
    if (buffer->data) {
        buffer->size = size;
        fread(buffer->data, sizeof(char), size, file);
        fclose(file);
        buffer->data[size - 1] = 0;
    }
    return true;
}

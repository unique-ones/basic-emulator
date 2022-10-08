#ifndef RETRO_UTILITY_H
#define RETRO_UTILITY_H

#include "types.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Reads the entire file into the specified buffer
 * @param buffer Buffer where the content of the file is copied to
 * @param path Path to the file
 * @return bool
 */
bool file_read(binary_buffer_t* buffer, const char* path);

#endif// RETRO_UTILITY_H

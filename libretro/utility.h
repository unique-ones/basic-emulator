#ifndef RETRO_UTILITY_H
#define RETRO_UTILITY_H

#include <stdio.h>
#include <stdlib.h>

#include "types.h"

/**
 * @brief reads the entire file into the specified buffer
 * @param buffer buffer where the content of the file is copied to
 * @param path path to the file
 * @return bool
 */
bool file_read(binary_buffer_t* buffer, const char* path);

#endif// RETRO_UTILITY_H

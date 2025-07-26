// Copyright (c) 2025 Elias Engelbert Plank

#ifndef RETRO_UTIL_UTILITY_H
#define RETRO_UTIL_UTILITY_H

/// Reads the entire file into the specified buffer
/// @param buffer The buffer where the content of the file is copied to
/// @param path The path to the file
/// @return Boolean value that indicates whether the file could be read
static b32 file_read(BinaryBuffer *buffer, const char *path);

#endif// RETRO_UTIL_UTILITY_H

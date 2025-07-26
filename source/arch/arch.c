// Copyright (c) 2025 Elias Engelbert Plank

#include "arch.h"

#define STRINGIFY_INTERNAL(x) #x
#define STRINGIFY(x) STRINGIFY_INTERNAL(x)
#define ARCH_PATH(file) STRINGIFY(LIBRETRO_PLATFORM/file)

#include ARCH_PATH(arch.c)

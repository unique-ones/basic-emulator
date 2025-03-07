// Copyright (c) 2025 Elias Engelbert Plank

#include <Windows.h>

#include "../time.h"

/// Sends the current thread of execution to sleep for the specified time
void time_sleep(u32 milliseconds) {
    Sleep(milliseconds);
}

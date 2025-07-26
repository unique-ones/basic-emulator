// Copyright (c) 2025 Elias Engelbert Plank

/// Sends the current thread of execution to sleep for the specified time
void time_sleep(u32 milliseconds) {
    usleep(milliseconds * 1000);
}

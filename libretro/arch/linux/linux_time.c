#include <unistd.h>

#include "../time.h"

void time_sleep(u32 milliseconds) {
    usleep(milliseconds * 1000);
}

#include <time.h>
#include <errno.h>

#include "../time.h"

void time_sleep(u32 milliseconds) {
    struct timespec ts;

    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;

    int res;
    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);
}
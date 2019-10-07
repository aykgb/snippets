#include <iostream>
#include <time.h>
#include <unistd.h>

int main()
{
    struct timespec ts_start, ts_end;

    clock_gettime(CLOCK_MONOTONIC, &ts_start);

    printf("%d.%03d\n", ts_start.tv_sec, ts_start.tv_nsec);

    usleep(10000);

    clock_gettime(CLOCK_MONOTONIC, &ts_end);

    printf("%d.%03d\n", ts_end.tv_sec, ts_end.tv_nsec);

    printf("%03d\n", ts_end.tv_nsec - ts_start.tv_nsec);
}

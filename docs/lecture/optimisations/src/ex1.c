#define _GNU_SOURCE
#include <stdio.h>
#include <time.h>

static void do_something() {}

int main()
{
    struct timespec t1;
    struct timespec t2;
    clock_gettime(CLOCK_MONOTONIC, &t1);
    do_something();
    clock_gettime(CLOCK_MONOTONIC, &t2);
    // compute elapsed time in nano-seconds
    long long int delta_t = (long long)(t2.tv_sec - t1.tv_sec) * 1000000000 +
                            t2.tv_nsec - t1.tv_nsec;
    printf("elapsed time: %lld [ns]\n", delta_t);
    return 0;
}

struct timespec t1, t2;
clock_gettime(CLOCK_MONOTONIC, &t1);
clock_gettime(CLOCK_MONOTONIC, &t2);
long long overhead =
    (long long)(t2.tv_sec - t1.tv_sec) * 1000000000 + t2.tv_nsec - t1.tv_nsec;
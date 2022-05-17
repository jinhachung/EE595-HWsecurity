#include "measure_time.h"

long elapsed_time_ns(const struct timespec after, const struct timespec before)
{
    // returns elapsed time in NANOSECONDS
    long sec = (long)after.tv_sec - (long)before.tv_sec;
    long nsec = (long)((long)after.tv_nsec - (long)before.tv_nsec);
    return (sec * 1000 * 1000 * 1000 + nsec);
}

long elapsed_time_us(const struct timespec after, const struct timespec before)
{
    // returns elapsed time in MICROSECONDS
    long sec = (long)after.tv_sec - (long)before.tv_sec;
    long usec = (long)(((long)after.tv_nsec - (long)before.tv_nsec) / ((long)1000));
    return (sec * 1000 * 1000 + usec);
}

long elapsed_time_ms(const struct timespec after, const struct timespec before)
{
    // returns elapsed time in MILLISECONDS
    long sec = (long)after.tv_sec - (long)before.tv_sec;
    long usec = (long)(((long)after.tv_nsec - (long)before.tv_nsec) / ((long)1000 * 1000));
    return (sec * 1000 + usec);
}

void print_current_time(const char *prefix_msg)
{
    // prints current time, with @prefix_msg as prefix
    struct timespec now;
    //clock_gettime(CLOCK_MONOTONIC, &now);
    clock_gettime(CLOCK_MONOTONIC_RAW, &now);
    //getrawmonotonic(&now);
    fprintf(stdout, "%s @ %lu.%09lu\n", prefix_msg, now.tv_sec, now.tv_nsec);
}


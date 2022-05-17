#ifndef MEASURE_TIME_H
#define MEASURE_TIME_H
#include <stdio.h>
#include <time.h>

long elapsed_time_ns(const struct timespec after, const struct timespec before);
long elapsed_time_us(const struct timespec after, const struct timespec before);
long elapsed_time_ms(const struct timespec after, const struct timespec before);
void print_current_time(const char *prefix_msg);

#endif // MEASURE_TIME_H

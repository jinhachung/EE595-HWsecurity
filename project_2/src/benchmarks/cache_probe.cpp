#include <iostream>
#include <cstdlib>
#include <assert.h>
#include <ctime>
#include <vector>
#include <bits/stdc++.h>

#include "measure_time.h"
#include "argparse.h"
#include <random>

#define _KB ((long)(1 << 10))
#define _MB ((long)(1 << 20))
#define _GB ((long)(1 << 30))

#define _CACHELINE_SIZE_BYTES (64)

int main(int argc, char *argv[]) {
    ArgParse parser;
    long cache_sz, arr_sz;
    int num_elem;
    struct timespec before, after;
    int *arrayA;
    int *arrayB;
    int tmp;
    long dummysum = 0;
    long duration;
    long memory_access_time_min = 10000000000;
    long memory_access_time_max = 0;
    long memory_access_time_total = 0;
    long cache_access_time_min = 10000000000;
    long cache_access_time_max = 0;
    long cache_access_time_total = 0;

    int int_per_cacheline = (int)(_CACHELINE_SIZE_BYTES / sizeof(int));

    // parse command line arguments
    parser.parse(argc, argv);

    // we want to measure how much time it takes for all data to be fetched into cache
    cache_sz = (long)parser.getIntFromKey("llc-size", 12); // 12MB
    arr_sz = cache_sz * _MB;
    num_elem = (arr_sz / (sizeof(int)));

    // print config
    std::cout << "******************** [BENCHMARK CONFIG] ************************************" << std::endl;
    std::cout << "*****\t[benchmark name] " << argv[0] << std::endl;
    std::cout << "*****\t[cache size] " << cache_sz << " MB" << std::endl;
    std::cout << "*****\t[iteration number] memory access: 1, cache access: 1" << std::endl;
    
    arrayA = (int *)calloc(num_elem, sizeof(int));
    if (!arrayA) {
        perror("calloc");
        goto ERROR_EXIT_A;
    }
    // initialize arrays
    for (long idx = 0; idx < num_elem; ++idx) {
        arrayA[idx] = (int)(idx % 10);
    }
    // flush caches
    // flushing caches seems difficult
    // "echo 3 > /proc/sys/vm/drop_caches" seems to only flush page cache and file-related objects from cache
    // so we just allocate a very very large array and go through them to flush cache
    // 4M elements = 16MB is more than enough to flush LLC
    std::cout << "******************** [FLUSHING CACHE] **************************************" << std::endl;
    arrayB = (int *)calloc(4 * _MB, sizeof(int));
    if (!arrayB) {
        perror("calloc");
        goto ERROR_EXIT_B;
    }
    // initialize arrayB so that arrayA will be flushed from cache (L1, L2, and LLC)
    for (long idx = 0; idx < 4 * _MB; ++idx) {
        arrayB[idx] = (int)(idx % 10);
    }
    free(arrayB);

    std::cout << "******************** [MEASURING MEMORY ACCESS] *****************************" << std::endl;
    
    for (long idx = 0; idx < num_elem; idx += int_per_cacheline) {
        clock_gettime(CLOCK_MONOTONIC, &before);
        tmp = arrayA[idx];
        clock_gettime(CLOCK_MONOTONIC, &after);
        dummysum += tmp;
        duration = elapsed_time_ns(after, before);
        memory_access_time_min = (duration < memory_access_time_min) ? duration : memory_access_time_min;;
        memory_access_time_max = (duration > memory_access_time_max) ? duration : memory_access_time_max;
        memory_access_time_total += duration;
    }

    std::cout << "******************** [RUNNING BENCHMARK] ***********************************" << std::endl;

    clock_gettime(CLOCK_MONOTONIC, &before);
    for (long idx = 0; idx < num_elem; idx += int_per_cacheline) {
        clock_gettime(CLOCK_MONOTONIC, &before);
        tmp = arrayA[idx];
        clock_gettime(CLOCK_MONOTONIC, &after);
        dummysum += tmp;
        duration = elapsed_time_ns(after, before);
        cache_access_time_min = (duration < cache_access_time_min) ? duration : cache_access_time_min;;
        cache_access_time_max = (duration > cache_access_time_max) ? duration : cache_access_time_max;
        cache_access_time_total += duration;
    }
    clock_gettime(CLOCK_MONOTONIC, &after);

    free(arrayA);

    // print results
    std::cout << "******************** [BENCHMARK RESULTS] ***********************************" << std::endl;
    std::cout << "*****\t[memory access] total: " << memory_access_time_total << " ns, average: " << memory_access_time_total / (num_elem / int_per_cacheline) << " ns" << std::endl;
    std::cout << "*****\t[memory access] minimum: " << memory_access_time_min << " ns, maximum: " << memory_access_time_max << " ns" << std::endl;
    std::cout << "*****\t[cache access] total: " << cache_access_time_total << " ns, average: " << cache_access_time_total / (num_elem / int_per_cacheline) << " ns" << std::endl;
    std::cout << "*****\t[cache access] minimum: " << cache_access_time_min << " ns, maximum: " << cache_access_time_max << " ns" << std::endl;
    std::cout << "****************************************************************************" << std::endl;

    return 0;

    // error exits
ERROR_EXIT_B:
    free(arrayA);
ERROR_EXIT_A:
    return -1;
}

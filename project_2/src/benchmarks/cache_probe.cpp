#include <iostream>
#include <cstdlib>
#include <assert.h>
#include <ctime>
#include <vector>
#include <bits/stdc++.h>

#include "measure_time.h"
#include "argparse.h"

#define USE_CLFLUSH
#define _CACHELINE_SIZE_BYTES (64)

#define _KB ((long)(1 << 10))
#define _MB ((long)(1 << 20))
#define _GB ((long)(1 << 30))

#ifdef USE_CLFLUSH
// memory access and cache flush for x86_64
static inline void memoryAccess(void *p) {
    asm volatile("movq (%0), %%rax\n" : : "c"(p) : "rax");
}

static void cacheFlush(void *p) {
    asm volatile("clflush 0(%0)\n" : : "c"(p) : "rax");
}

static inline uint64_t rdtsc() {
  	uint64_t a = 0, d = 0;
  	asm volatile("mfence");
#if defined(USE_RDTSCP) && defined(__x86_64__)
  	asm volatile("rdtscp" : "=a"(a), "=d"(d) :: "rcx");
#elif defined(USE_RDTSCP) && defined(__i386__)
  	asm volatile("rdtscp" : "=A"(a), :: "ecx");
#elif defined(__x86_64__)
    // it reaches here in our VM
  	asm volatile("rdtsc" : "=a"(a), "=d"(d));
#elif defined(__i386__)
  	asm volatile("rdtsc" : "=A"(a));
#endif
  a = (d << 32) | a;
  asm volatile("mfence");
  return a;
}
#endif // USE_CLFLUSH

int main(int argc, char *argv[]) {
    ArgParse parser;
    long cache_sz, arr_sz;
    int num_elem;
    int *arrayA;
#ifdef USE_CLFLUSH
    uint64_t start, end;
    uint64_t duration;
    uint64_t memory_access_time_min = 10000000000;
    uint64_t memory_access_time_max = 0;
    uint64_t memory_access_time_total = 0;
    uint64_t cache_access_time_min = 10000000000;
    uint64_t cache_access_time_max = 0;
    uint64_t cache_access_time_total = 0;
#else
    int tmp, tmptmp;
    long dummysum = 0;
    int *arrayB;
    struct timespec before, after;
    long duration;
    long memory_access_time_min = 10000000000;
    long memory_access_time_max = 0;
    long memory_access_time_total = 0;
    long cache_access_time_min = 10000000000;
    long cache_access_time_max = 0;
    long cache_access_time_total = 0;
#endif // USE_CLFLUSH

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
#ifdef USE_CLFLUSH
        cacheFlush(arrayA + idx);
#endif // USE_CLFLUSH
    }
    // flush caches
    // flushing caches seems difficult
    // "echo 3 > /proc/sys/vm/drop_caches" seems to only flush page cache and file-related objects from cache
    // so we just allocate a very very large array and go through them to flush cache
    // 4M elements = 16MB is more than enough to flush LLC
    std::cout << "******************** [FLUSHING CACHE] **************************************" << std::endl;
#ifndef USE_CLFLUSH
    arrayB = (int *)calloc(256 * _MB, sizeof(int));
    if (!arrayB) {
        perror("calloc");
        goto ERROR_EXIT_B;
    }
    // initialize arrayB so that arrayA will be flushed from cache (L1, L2, and LLC)
    for (long idx = 0; idx < 256 * _MB; ++idx) {
        tmp += arrayB[idx];
    }
    std::cout << tmp << std::endl;
    free(arrayB);
#endif // USE_CLFLUSH

    std::cout << "******************** [MEASURING MEMORY ACCESS] *****************************" << std::endl;
    
    for (long idx = 0; idx < num_elem; idx += int_per_cacheline) {
#ifdef USE_CLFLUSH
        start = rdtsc();
        memoryAccess(arrayA + idx);
        end = rdtsc();
        duration = end - start;
#else
        clock_gettime(CLOCK_MONOTONIC, &before);
        tmp = arrayA[idx];
        clock_gettime(CLOCK_MONOTONIC, &after);
        dummysum += tmp;
        duration = elapsed_time_ns(after, before);
#endif // USE_CLFLUSH
        memory_access_time_min = (duration < memory_access_time_min) ? duration : memory_access_time_min;;
        memory_access_time_max = (duration > memory_access_time_max) ? duration : memory_access_time_max;
        memory_access_time_total += duration;
    }

    std::cout << "******************** [MEASURING CACHE ACCESS] ******************************" << std::endl;

    for (long idx = 0; idx < num_elem; idx += int_per_cacheline) {
#ifdef USE_CLFLUSH
        start = rdtsc();
        memoryAccess(arrayA + idx);
        end = rdtsc();
        duration = end - start;
#else
        tmptmp = arrayA[idx];
        clock_gettime(CLOCK_MONOTONIC, &before);
        tmp = arrayA[idx];
        clock_gettime(CLOCK_MONOTONIC, &after);
        dummysum += (tmp + tmptmp);
        duration = elapsed_time_ns(after, before);
#endif // USE_CLFLUSH
        cache_access_time_min = (duration < cache_access_time_min) ? duration : cache_access_time_min;;
        cache_access_time_max = (duration > cache_access_time_max) ? duration : cache_access_time_max;
        cache_access_time_total += duration;
    }

    free(arrayA);

    // print results
    std::cout << "******************** [PROBE RESULTS] ***************************************" << std::endl;
#ifdef USE_CLFLUSH
    std::cout << "*****\t[memory access] total: " << memory_access_time_total << " cycles, average: " << memory_access_time_total / (num_elem / int_per_cacheline) << " cycles" << std::endl;
    std::cout << "*****\t[memory access] minimum: " << memory_access_time_min << " cycles, maximum: " << memory_access_time_max << " cycles" << std::endl;
    std::cout << "*****\t[cache access] total: " << cache_access_time_total << " cycles, average: " << cache_access_time_total / (num_elem / int_per_cacheline) << " cycles" << std::endl;
    std::cout << "*****\t[cache access] minimum: " << cache_access_time_min << " cycles, maximum: " << cache_access_time_max << " cycles" << std::endl;
#else
    std::cout << "*****\t[memory access] total: " << memory_access_time_total << " ns, average: " << memory_access_time_total / (num_elem / int_per_cacheline) << " ns" << std::endl;
    std::cout << "*****\t[memory access] minimum: " << memory_access_time_min << " ns, maximum: " << memory_access_time_max << " ns" << std::endl;
    std::cout << "*****\t[cache access] total: " << cache_access_time_total << " ns, average: " << cache_access_time_total / (num_elem / int_per_cacheline) << " ns" << std::endl;
    std::cout << "*****\t[cache access] minimum: " << cache_access_time_min << " ns, maximum: " << cache_access_time_max << " ns" << std::endl;
#endif // USE_CLFLUSH
    std::cout << "****************************************************************************" << std::endl;

    return 0;

    // error exits
#ifndef USE_CLFLUSH
ERROR_EXIT_B:
    free(arrayA);
#endif // USE_CLFLUSH
ERROR_EXIT_A:
    return -1;
}

#include <iostream>
#include <cstdlib>
#include <assert.h>
#include <ctime>
#include <vector>
#include <random>
#include <bits/stdc++.h>

#include "measure_time.h"
#include "argparse.h"

#define _CACHELINE_SIZE_BYTES (64)

#define _KB ((long)(1 << 10))
#define _MB ((long)(1 << 20))
#define _GB ((long)(1 << 30))

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

int main(int argc, char *argv[]) {
    ArgParse parser;
    long cache_sz, arr_sz;
    int num_elem;
    int *arrayA;
    int int_per_cacheline = (int)(_CACHELINE_SIZE_BYTES / sizeof(int));
    int total_access_count_cache;
    int total_access_count_memory;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> rng(0, _GB);
    uint64_t start, end;
    uint64_t duration;
    uint64_t memory_access_time_min = 10000000000;
    uint64_t memory_access_time_max = 0;
    uint64_t memory_access_time_total = 0;
    uint64_t cache_access_time_min = 10000000000;
    uint64_t cache_access_time_max = 0;
    uint64_t cache_access_time_total = 0;
    uint64_t threshold_cache_mem;
    uint64_t threshold_scheduled = 10000;
    int fast_memory_count = 0;
    int sched_memory_count = 0;
    int slow_cache_count = 0;
    int sched_cache_count = 0;

    // parse command line arguments
    parser.parse(argc, argv);

    // we want to measure how much time it takes for all data to be fetched into cache
    threshold_cache_mem = (uint64_t)parser.getIntFromKey("threshold", 105);
    cache_sz = (long)parser.getIntFromKey("llc-size", 12); // 12MB
    arr_sz = cache_sz * _MB;
    num_elem = (arr_sz / (sizeof(int)));
    total_access_count_memory = (num_elem / int_per_cacheline) * 4;;
    total_access_count_cache = (num_elem / int_per_cacheline) * 4;

    // print config
    std::cout << "******************** [BENCHMARK CONFIG] ************************************" << std::endl;
    std::cout << "*****\t[benchmark name] " << argv[0] << std::endl;
    std::cout << "*****\t[cache size] " << cache_sz << " MB" << std::endl;
    std::cout << "*****\t[total access count] memory access: " << total_access_count_memory << ", cache access: " << total_access_count_cache << std::endl;
    std::cout << "*****\t[timing] [0 <-- cache --> " << threshold_cache_mem << "] [" << threshold_cache_mem + 1 << " <-- memory --> " << threshold_scheduled << "] [" << threshold_scheduled + 1 << " <-- scheduled --> oo]" << std::endl;

    arrayA = (int *)calloc(num_elem, sizeof(int));
    if (!arrayA) {
        perror("calloc");
        goto ERROR_EXIT_A;
    }
    // initialize arrays
    for (long idx = 0; idx < num_elem; ++idx) {
        arrayA[idx] = (int)(idx % 10);
        cacheFlush(arrayA + idx);
    }
    
    std::cout << "******************** [MEASURING MEMORY ACCESS] *****************************" << std::endl;
    for (long i = 0; i < total_access_count_memory; ++i) {
        long idx = rng(gen) % num_elem;
        start = rdtsc();
        memoryAccess(arrayA + idx);
        end = rdtsc();
        cacheFlush(arrayA + idx);
        duration = end - start;
        if (duration <= threshold_scheduled) {
            memory_access_time_min = (duration < memory_access_time_min) ? duration : memory_access_time_min;
            memory_access_time_max = (duration > memory_access_time_max) ? duration : memory_access_time_max;
            memory_access_time_total += duration;
            if (duration <= threshold_cache_mem) {
                ++fast_memory_count;
            }
        } else {
            ++sched_memory_count;
        }
    }
    // because we flushed above for more precise measurements, refill cache
    for (long idx = 0; idx < num_elem; ++idx) {
        memoryAccess(arrayA + idx);
    }

    std::cout << "******************** [MEASURING CACHE ACCESS] ******************************" << std::endl;
    for (long i = 0; i < total_access_count_cache; ++i) {
        long idx = rng(gen) % num_elem;
        memoryAccess(arrayA + idx);
        start = rdtsc();
        memoryAccess(arrayA + idx);
        end = rdtsc();
        duration = end - start;
        if (duration <= threshold_scheduled) {
            cache_access_time_min = (duration < cache_access_time_min) ? duration : cache_access_time_min;;
            cache_access_time_max = (duration > cache_access_time_max) ? duration : cache_access_time_max;
            cache_access_time_total += duration;
            if (duration > threshold_cache_mem) {
                ++slow_cache_count;
            }
        } else {
            ++sched_cache_count;
        }
    }

    free(arrayA);

    // print results
    std::cout << "******************** [PROBE RESULTS] ***************************************" << std::endl;
    std::cout << "*****\t[memory access] total: " << memory_access_time_total << " cycles, average: " << memory_access_time_total / (total_access_count_memory - sched_memory_count) << " cycles" << std::endl;
    std::cout << "*****\t[memory access] minimum: " << memory_access_time_min << " cycles, maximum: " << memory_access_time_max << " cycles" << std::endl;
    std::cout << "*****\t[cache access] total: " << cache_access_time_total << " cycles, average: " << cache_access_time_total / (total_access_count_cache - sched_cache_count) << " cycles" << std::endl;
    std::cout << "*****\t[cache access] minimum: " << cache_access_time_min << " cycles, maximum: " << cache_access_time_max << " cycles" << std::endl;
    std::cout << "******************** [THRESHOLD CORRECTNESS] *******************************" << std::endl;
    std::cout << "*****\t[memory access] total: " << total_access_count_memory << ", too fast: " << fast_memory_count << ", scheduled: " << sched_memory_count << std::endl;
    std::cout << "*****\t[cache access] total: " << total_access_count_cache << ", too slow: " << slow_cache_count << ", scheduled: " << sched_cache_count << std::endl;
    std::cout << "****************************************************************************" << std::endl;

    return 0;

    // error exits
ERROR_EXIT_A:
    return -1;
}

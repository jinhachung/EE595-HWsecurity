#include <iostream>
#include <cstdlib>
#include <assert.h>
#include <ctime>
#include <vector>
#include <numeric> // for std::accumulate()
#include <bits/stdc++.h>

#include "measure_time.h"
#include "argparse.h"
#include <random>

#define _KB ((long)(1 << 10))
#define _MB ((long)(1 << 20))
#define _GB ((long)(1 << 30))

int main(int argc, char *argv[]) {
    ArgParse parser;
    int num_iter, num_warmup_iter;
    long cache_sz, arr_sz, average;
    std::vector<long> durationVector;
    struct timespec before, after;
    int *arrayA;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 15);

    // parse command line arguments
    parser.parse(argc, argv);

    num_iter = parser.getIntFromKey("num-iter", 100);
    num_warmup_iter = (int)(num_iter / 10);
    cache_sz = (long)parser.getIntFromKey("llc_size", 12); // 12MB
    arr_sz = cache_sz * _MB;

    if ((num_iter % 10) && (!num_iter)) {
        std::cerr << "Enter number of iterations (--num-iter) as a nonzero multiple of 10" << std::endl;
        goto ERROR_EXIT;
    }

    // print config
    std::cout << "******************** [BENCHMARK CONFIG] ************************************" << std::endl;
    std::cout << "*****\t[benchmark name] " << argv[0] << std::endl;
    // if (arr_sz_kb) std::cout << "*****\t[array size] " << arr_sz_kb << " KB" << std::endl;
    // else if (arr_sz_mb) std::cout << "*****\t[array size] " << arr_sz_mb << " MB" << std::endl;
    // else std::cout << "*****\t[array size] " << arr_sz_gb << " GB" << std::endl;
    std::cout << "*****\t[cache size] " << cache_sz << " MB" << std::endl;
    std::cout << "*****\t[iteration number] warmup: " << num_warmup_iter << ", measured runs: " << num_iter << std::endl;
    
    arrayA = (int *)calloc(arr_sz, sizeof(int));
    if (!arrayA) {
        perror("calloc");
        goto ERROR_EXIT_A;
    }
    // initialize arrays
    for (long idx = 0; idx < arr_sz; ++idx) {
        arrayA[idx] = (int)(idx % 10);
    }
    // calloc already initializes to 0
    //memset(arrayC, 0, arr_sz * sizeof(int));
    // no timer for warmup

    int tmp;

    std::cout << "******************** [BENCHMARK WARMUP] ************************************" << std::endl;
    
    for (int i = 0; i < 8 ; i++) {
        for (long idx = 0; idx < arr_sz; idx+=16) {
            tmp = arrayA[idx + dis(gen)];
        }
    }

    std::cout << "******************** [RUNNING BENCHMARK] ***********************************" << std::endl;

    for (int iter = 0; iter < num_iter; ++iter) {
        clock_gettime(CLOCK_MONOTONIC, &before);
        for (int i = 0; i < 8 ; i++) {
            for (long idx = 0; idx < arr_sz; idx+=16) {
                tmp = arrayA[idx + dis(gen)];
                tmp++;
                arrayA[idx + dis(gen)] = tmp;
            }
        }

        clock_gettime(CLOCK_MONOTONIC, &after);

        durationVector.push_back(elapsed_time_ns(after, before));
    }


    free(arrayA);
    // get average of durations
    //average = std::accumulate(durationVector.begin(), durationVector.end(), 0) / durationVector.size();
    average = 0;
    for (long val : durationVector) average += val;
    average = (long)(average / ((long)durationVector.size()));

    // print results
    std::cout << "******************** [BENCHMARK RESULTS] ***********************************" << std::endl;
    std::cout << "*****\t[average time] " << average << " ns" << std::endl;
    std::cout << "****************************************************************************" << std::endl;

    return 0;

    // error exits
ERROR_EXIT_A:
    free(arrayA);
ERROR_EXIT:
    return -1;
}

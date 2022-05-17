#include <iostream>
#include <cstdlib>
#include <assert.h>
#include <ctime>
#include <vector>
#include <numeric> // for std::accumulate()
#include <bits/stdc++.h>

#include "measure_time.h"
#include "argparse.h"

#define _KB ((long)(1 << 10))
#define _MB ((long)(1 << 20))
#define _GB ((long)(1 << 30))

int partition(int *arr, int start, int end) {
 
    int pivot = arr[start];
 
    int count = 0;
    for (int i = start + 1; i <= end; i++) {
        if (arr[i] <= pivot)
            count++;
    }
 
    // Giving pivot element its correct position
    int pivotIndex = start + count;
    std::swap(arr[pivotIndex], arr[start]);
 
    // Sorting left and right parts of the pivot element
    int i = start, j = end;
 
    while (i < pivotIndex && j > pivotIndex) {
 
        while (arr[i] <= pivot) {
            i++;
        }
 
        while (arr[j] > pivot) {
            j--;
        }
 
        if (i < pivotIndex && j > pivotIndex) {
            std::swap(arr[i++], arr[j--]);
        }
    }
 
    return pivotIndex;
}

void quickSort(int *arr, int start, int end) {
 
    // base case
    if (start >= end)
        return;
 
    // partitioning the array
    int p = partition(arr, start, end);
 
    // Sorting the left part
    quickSort(arr, start, p - 1);
 
    // Sorting the right part
    quickSort(arr, p + 1, end);
}

int main(int argc, char *argv[]) {
    ArgParse parser;
    int num_iter, num_warmup_iter;
    long arr_sz_kb, arr_sz_mb, arr_sz_gb, arr_sz, average;
    std::vector<long> durationVector;
    struct timespec before, after;
    int *arrayA;

    // parse command line arguments
    parser.parse(argc, argv);

    num_iter = parser.getIntFromKey("num-iter", 10);
    num_warmup_iter = (int)(num_iter / 10);
    arr_sz_kb = (long)parser.getIntFromKey("arr-sz-kb", 0);
    arr_sz_mb = (long)parser.getIntFromKey("arr-sz-mb", 0);
    arr_sz_gb = (long)parser.getIntFromKey("arr-sz-gb", 0);
    arr_sz = std::max(arr_sz_kb * _KB, std::max(arr_sz_mb * _MB, arr_sz_gb * _GB));

    if ((num_iter % 10) && (!num_iter)) {
        std::cerr << "Enter number of iterations (--num-iter) as a nonzero multiple of 10" << std::endl;
        goto ERROR_EXIT;
    }

    if (!arr_sz) {
        std::cerr << "Enter array size in either KB (--arr-sz-kb), MB (--arr-sz-mb), or GB (--arr-sz-gb)" << std::endl;
        goto ERROR_EXIT;
    }

    // print config
    std::cout << "******************** [BENCHMARK CONFIG] ************************************" << std::endl;
    std::cout << "*****\t[benchmark name] " << argv[0] << std::endl;
    if (arr_sz_kb) std::cout << "*****\t[array size] " << arr_sz_kb << " KB" << std::endl;
    else if (arr_sz_mb) std::cout << "*****\t[array size] " << arr_sz_mb << " MB" << std::endl;
    else std::cout << "*****\t[array size] " << arr_sz_gb << " GB" << std::endl;
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
    std::cout << "******************** [BENCHMARK WARMUP] ************************************" << std::endl;
    quickSort(arrayA, 0, arr_sz - 1);
    
    for (long idx = 0; idx < arr_sz; ++idx) {
        arrayA[idx] = (int)(idx % 10);
    }

    std::cout << "******************** [RUNNING BENCHMARK] ***********************************" << std::endl;

    for (int iter = 0; iter < num_iter; ++iter) {
        clock_gettime(CLOCK_MONOTONIC, &before);
        quickSort(arrayA, 0, arr_sz - 1);
        clock_gettime(CLOCK_MONOTONIC, &after);
        for (long idx = 0; idx < arr_sz; ++idx) {
            arrayA[idx] = (int)(idx % 10);
        }
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

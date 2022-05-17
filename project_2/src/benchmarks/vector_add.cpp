#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include "measure_time.h"

#define ARRAY_SIZE (1 << 28)

int main(int argc, char *argv[]) {
    int *arrayA = (int *)calloc(ARRAY_SIZE, sizeof(int));
    int *arrayB = (int *)calloc(ARRAY_SIZE, sizeof(int));
    int *arrayC = (int *)calloc(ARRAY_SIZE, sizeof(int));

    if (!arrayA || !arrayB || !arrayC) {
        perror("calloc");
        goto ERROR_EXIT;
    }

    if (argc != 2) {
        fprintf(stderr, "Specify how many times vector add will run\n");
        goto ERROR_EXIT;
    }

    free(arrayA);
    free(arrayB);
    free(arrayC);
    return 0;

ERROR_EXIT:
    free(arrayA);
    free(arrayB);
    free(arrayC);
    return -1;
}

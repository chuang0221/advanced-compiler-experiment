#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define ARR_SIZE 4096
#define NUM_RUNS 10

double benchmark(int is_row_major) {
    int (*A)[ARR_SIZE] = malloc(sizeof(int[ARR_SIZE][ARR_SIZE]));
    if (A == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    clock_t start = clock();

    if (is_row_major) {
        for (int i = 0; i < ARR_SIZE; i++) {
            for (int j = 0; j < ARR_SIZE; j++) {
                A[i][j] = 0;
            }
        }
    }
	else {
        for (int j = 0; j < ARR_SIZE; j++) {
            for (int i = 0; i < ARR_SIZE; i++) {
                A[i][j] = 0;
            }
        }
    }

    clock_t end = clock();
    free(A);

    return ((double) (end - start)) / CLOCKS_PER_SEC;
}

int main(void) {
    double row_major_total = 0, column_major_total = 0;

    for (int i = 0; i < NUM_RUNS; i++) {
        row_major_total += benchmark(1);
        column_major_total += benchmark(0);
    }

    printf("Average time for row-major: %f seconds\n", row_major_total / NUM_RUNS);
    printf("Average time for column-major: %f seconds\n", column_major_total / NUM_RUNS);

    return 0;
}

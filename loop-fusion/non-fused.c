#include <stdio.h>
#include <time.h>

#define ARR_SIZE 512 * 512

int main(void) {
	int X[ARR_SIZE], Y[ARR_SIZE], Z[ARR_SIZE];
	
	clock_t start, end, diff;

	for (int i = 0; i < ARR_SIZE; i++) {
		X[i] = i;
		Y[i] = ARR_SIZE - i;
	}

	start = clock();
	for (int i = 0; i < ARR_SIZE; i++) {
		Z[i] = X[i] - Y[i];
	}
	for (int i = 0; i < ARR_SIZE; i++) {
		Z[i] = Z[i] * Z[i];
	}

	end = clock();

	printf("It takes %f seconds\n", (float) (end - start) / CLOCKS_PER_SEC);
	return 0;
}

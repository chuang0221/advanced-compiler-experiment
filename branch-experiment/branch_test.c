#include <stdio.h>
#include <stdint.h>

extern void branch_experiment(uint64_t *taken, uint64_t *fall_through);

int main() {
    uint64_t taken_cycles, fall_through_cycles;
    branch_experiment(&taken_cycles, &fall_through_cycles);
    
    printf("Taken branch: %lu cycles\n", taken_cycles);
    printf("Fall-through branch: %lu cycles\n", fall_through_cycles);
    printf("Difference: %ld cycles\n", (int64_t)taken_cycles - (int64_t)fall_through_cycles);
    
    return 0;
}

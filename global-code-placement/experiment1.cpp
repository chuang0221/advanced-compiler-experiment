#include <iostream>
#include <chrono>
#include <vector>
#include <random>

const int ITERATIONS = 10000000;
const int ARRAY_SIZE = 1024 * 1024;

void flush_instruction_cache() {
    static std::vector<int> large_array(ARRAY_SIZE);
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        large_array[i] = i;
    }
}

int64_t measure_branch_performance(bool take_branch) {
    int64_t start, end;
    int result = 0;

    flush_instruction_cache();

    asm volatile (
        "mfence\n\t"
        "lfence\n\t"
        "rdtsc\n\t"
        "shl $32, %%rdx\n\t"
        "or %%rdx, %%rax\n\t"
        "mov %%rax, %0"
        : "=r" (start)
        :
        : "%rax", "%rdx"
    );

    for (int i = 0; i < ITERATIONS; ++i) {
        asm goto (
            "cmp $0, %0\n\t"
            "je %l1\n\t"
            ".space 1024, 0x90\n\t"
            "%l1:"
            : 
            : "r" (take_branch)
            : "cc"
            : taken_branch
        );
taken_branch:
        result += i;
    }

    asm volatile (
        "mfence\n\t"
        "lfence\n\t"
        "rdtsc\n\t"
        "shl $32, %%rdx\n\t"
        "or %%rdx, %%rax\n\t"
        "mov %%rax, %0"
        : "=r" (end)
        :
        : "%rax", "%rdx"
    );

    return end - start;
}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

    int64_t taken_cycles = measure_branch_performance(true);
    int64_t fall_through_cycles = measure_branch_performance(false);

    std::cout << "Taken branch cycles: " << taken_cycles << std::endl;
    std::cout << "Fall-through branch cycles: " << fall_through_cycles << std::endl;
    std::cout << "Difference (Taken - Fall-through): " << (taken_cycles - fall_through_cycles) << " cycles" << std::endl;

    return 0;
}

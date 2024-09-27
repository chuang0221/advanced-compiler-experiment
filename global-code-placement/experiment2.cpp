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

int64_t frequent_code(int x) {
    return x * x + x / 2;
}

int64_t infrequent_code(int x) {
    return x * x + x / 2;
}

int64_t measure_branch_performance(bool frequent_on_taken, float frequent_probability) {
    int64_t start, end;
    int64_t result = 0;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

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
        bool take_branch = (dis(gen) < frequent_probability) == frequent_on_taken;
        
        if (take_branch) {
            asm goto (
                "jmp %l0\n\t"
                ".space 1024, 0x90\n\t"  // 1KB of NOPs to simulate distance
                : : : : taken_branch
            );
        }
        result += infrequent_code(i);
        asm goto ("jmp %l0\n\t" : : : : end_branch);
taken_branch:
        result += frequent_code(i);
end_branch:
        {}
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
    float frequent_probability = 0.9999999999;  // 80% of the time we execute the frequent code

    int64_t frequent_on_taken_cycles = measure_branch_performance(true, frequent_probability);
    int64_t frequent_on_fallthrough_cycles = measure_branch_performance(false, frequent_probability);

    std::cout << "Cycles when frequent code is on taken branch: " << frequent_on_taken_cycles << std::endl;
    std::cout << "Cycles when frequent code is on fall-through branch: " << frequent_on_fallthrough_cycles << std::endl;
    std::cout << "Difference (Taken - Fall-through): " << (frequent_on_taken_cycles - frequent_on_fallthrough_cycles) << " cycles" << std::endl;
    
    float improvement_percentage = (float)(frequent_on_taken_cycles - frequent_on_fallthrough_cycles) / frequent_on_taken_cycles * 100;
    std::cout << "Performance improvement: " << improvement_percentage << "%" << std::endl;

    return 0;
}

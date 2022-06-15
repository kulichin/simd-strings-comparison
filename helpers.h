#include <Windows.h>
#include <stdio.h>
#include <stdint.h>

// https://stackoverflow.com/questions/5819227/profiling-simd-code
uint64_t start_cycles_count;
uint64_t end_cycles_count;

#define START_COUNTING_CPU_CYCLES() \
    start_cycles_count = __rdtsc();

#define END_COUNTING_CPU_CYCLES()                                       \
    end_cycles_count = __rdtsc();                                       \
    printf("Elapsed CPU cycles on string comparison: %i\n\n", (int)(end_cycles_count - start_cycles_count));

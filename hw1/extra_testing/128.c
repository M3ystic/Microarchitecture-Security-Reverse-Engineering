#include <stdio.h>
#include <stdint.h>
#include <immintrin.h>   // AVX/SSE intrinsics
#include <x86intrin.h>   // rdtscp
#include <unistd.h>      // usleep

#define EVENTS 1000000

int main()
{
    uint64_t start, finish;
    unsigned int temp;

    // 128-bit AVX vectors (4 floats, 16 bytes)
    __m128 evens = _mm_set_ps(2.0, 4.0, 6.0, 8.0);
    __m128 odds  = _mm_set_ps(1.0, 3.0, 5.0, 7.0);
    volatile __m128 result;

    FILE *file_pointer = fopen("avx128_timing_sleep.csv", "w");
    if (!file_pointer) {
        fprintf(stderr, "Could not open file\n");
        return 1;
    }

    fprintf(file_pointer, "before_sleep,after_sleep\n");

    for (unsigned int i = 0; i < EVENTS; i++) {
        // First multiplication timing
        _mm_mfence();
        start = __rdtscp(&temp);
        result = _mm_mul_ps(evens, odds);
        finish = __rdtscp(&temp);
        _mm_mfence();
        uint64_t before_sleep = finish - start;

        // Sleep for 100 ms
        usleep(50000); // 100,000 microseconds = 100 ms

        // Second multiplication timing
        _mm_mfence();
        start = __rdtscp(&temp);
        result = _mm_mul_ps(evens, odds);
        finish = __rdtscp(&temp);
        _mm_mfence();
        uint64_t after_sleep = finish - start;

        // Save both timings to CSV
        fprintf(file_pointer, "%llu,%llu\n", before_sleep, after_sleep);
    }

    fclose(file_pointer);
    return 0;
}

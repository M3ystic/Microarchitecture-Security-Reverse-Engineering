#include <stdio.h>
#include <stdint.h>
#include <immintrin.h>   // AVX-512 intrinsics, RDTSCP, MFENCE
#include <unistd.h>      // for usleep()

#define EVENTS 1000000

int main()
{
    uint64_t start, finish;
    unsigned int temp;

    __m512 evens = _mm512_set_ps(
        2.0, 4.0, 6.0, 8.0, 10.0, 12.0, 14.0, 16.0,
        18.0, 20.0, 22.0, 24.0, 26.0, 28.0, 30.0, 32.0
    );
    __m512 odds  = _mm512_set_ps(
        1.0, 3.0, 5.0, 7.0, 9.0, 11.0, 13.0, 15.0,
        17.0, 19.0, 21.0, 23.0, 25.0, 27.0, 29.0, 31.0
    );
    volatile __m512 result;

    FILE *file_pointer = fopen("avx512_timing_sleep.csv", "w");
    if (!file_pointer) {
        fprintf(stderr, "Could not open file\n");
        return 1;
    }

    fprintf(file_pointer, "before_sleep,after_sleep\n");

    for (unsigned int i = 0; i < EVENTS; i++) {
        // First multiplication timing
        _mm_mfence();
        start = __rdtscp(&temp);
        result = _mm512_mul_ps(evens, odds);
        finish = __rdtscp(&temp);
        _mm_mfence();
        uint64_t before_sleep = finish - start;

        // Sleep for 100 ms
        usleep(50000); // 100,000 microseconds = 100 ms

        // Second multiplication timing
        _mm_mfence();
        start = __rdtscp(&temp);
        result = _mm512_mul_ps(evens, odds);
        finish = __rdtscp(&temp);
        _mm_mfence();
        uint64_t after_sleep = finish - start;

        // Save both timings to CSV
        fprintf(file_pointer, "%llu,%llu\n", before_sleep, after_sleep);
    }

    fclose(file_pointer);
    return 0;
}

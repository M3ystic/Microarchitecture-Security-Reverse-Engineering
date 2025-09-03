#include <stdio.h>
#include <stdint.h>
#include <immintrin.h> 

#define EVENTS 1000000

int main()
{
    uint64_t start, finish;
    unsigned int temp;

    // AVX2 vectors (8 floats = 256 bits)
    __m256 evens = _mm256_set_ps(2.0, 4.0, 6.0, 8.0, 10.0, 12.0, 14.0, 16.0);
    __m256 odds  = _mm256_set_ps(1.0, 3.0, 5.0, 7.0, 9.0, 11.0, 13.0, 15.0);
    volatile __m256 result;

    FILE *file_pointer = fopen("avx2_timing.csv", "w");
    if (file_pointer == NULL) {
        fprintf(stderr, "Could not open file\n");
        return 1;
    }

    fprintf(file_pointer, "avx2_cycles\n");

    for (unsigned int i = 0; i < EVENTS; i++) {
        _mm_mfence();
        start = __rdtscp(&temp);

        // AVX2 operation to time
        result = _mm256_mul_ps(evens, odds);

        finish = __rdtscp(&temp);
        _mm_mfence();

        fprintf(file_pointer, "%llu\n", finish - start);
    }

    fclose(file_pointer);
    return 0;
}

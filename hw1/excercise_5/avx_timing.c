#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <immintrin.h>   // AVX intrinsics
#include <x86intrin.h>   // rdtscp
#include <unistd.h>      // usleep

#define EVENTS 1000000

// function to time one AVX multiplication
static inline uint64_t time_avx_mul() {
    unsigned int temp;
    __m128 evens = _mm_set_ps(2.0, 4.0, 6.0, 8.0);   // 128-bit: 4 floats
    __m128 odds  = _mm_set_ps(1.0, 3.0, 5.0, 7.0);
    volatile __m128 result;

    _mm_mfence();
    uint64_t start = __rdtscp(&temp);

    result = _mm_mul_ps(evens, odds);

    uint64_t finish = __rdtscp(&temp);
    _mm_mfence();

    return finish - start;
}

int main() {
    FILE *file_pointer = fopen("avx_timing.csv", "w");
    if (!file_pointer) {
        fprintf(stderr, "Error: could not open CSV file\n");
        return 1;
    }

    fprintf(file_pointer, "pre_cycles,post_cycles,delta_cycles\n");

    static uint64_t pre[EVENTS];
    static uint64_t post[EVENTS];

    // Pre-sleep timings
    for (int i = 0; i < EVENTS; i++) {
        pre[i] = time_avx_mul();
    }

    // Sleep once for 100 ms
    usleep(100000);

    // Post-sleep timings
    for (int i = 0; i < EVENTS; i++) {
        post[i] = time_avx_mul();
    }

    // Write results to CSV
    for (int i = 0; i < EVENTS; i++) {
        fprintf(file_pointer, "%llu,%llu,%llu\n",
                pre[i],
                post[i],
                (post[i] > pre[i]) ? post[i] - pre[i] : pre[i] - post[i]);
    }

    fclose(file_pointer);
    return 0;
}

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <immintrin.h>   // AVX-512 intrinsics
#include <x86intrin.h>   // rdtscp
#include <unistd.h>      // usleep

#define EVENTS 1000000

// function to time one AVX-512 multiplication
static inline uint64_t time_avx512_mul() {
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

    _mm_mfence();
    uint64_t start = __rdtscp(&temp);

    result = _mm512_mul_ps(evens, odds);

    uint64_t finish = __rdtscp(&temp);
    _mm_mfence();

    return finish - start;
}

int main() {
    FILE *file_pointer = fopen("avx512_timing.csv", "w");
    if (!file_pointer) {
        fprintf(stderr, "Error: could not open CSV file\n");
        return 1;
    }

    fprintf(file_pointer, "pre_cycles,post_cycles,delta_cycles\n");

    static uint64_t pre[EVENTS];
    static uint64_t post[EVENTS];

    // pre-sleep timings 
    for (int i = 0; i < EVENTS; i++) {
        pre[i] = time_avx512_mul();
    }

    // Sleep once for 100 ms 
    usleep(100000);

    //Collect post-sleep timings 
    for (int i = 0; i < EVENTS; i++) {
        post[i] = time_avx512_mul();
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

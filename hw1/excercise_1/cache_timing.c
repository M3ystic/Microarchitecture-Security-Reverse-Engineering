
#include <stdio.h>
#include <stdint.h>
#include <immintrin.h>

#define EVENTS 1000000


int main()
{ 
    volatile char secret_value[4096]__attribute__((aligned(4096)));
    uint64_t begin, finish;
    unsigned int temp;
    FILE *file_pointer = fopen("cache_timing.csv", "w");

    if (file_pointer == NULL)
    {
        fprintf(stderr, "Could not open file");
    }

    fprintf(file_pointer, "hit,miss\n");
    for (unsigned int i = 0; i < EVENTS; i++) 
    {
    secret_value[0] = 1;
    _mm_mfence();
    begin = __rdtscp(&temp);
    volatile char read = secret_value[0];
    finish = __rdtscp(&temp);
    _mm_mfence();
    uint64_t hit_cycles = finish - begin;

    _mm_mfence();
    _mm_clflush((const void*)&secret_value[0]);
    _mm_mfence();
    begin = __rdtscp(&temp);
    read = secret_value[0];
    finish = __rdtscp(&temp);
    _mm_mfence();
    uint64_t miss_cycles = finish - begin;

    fprintf(file_pointer, "%llu,%llu\n", hit_cycles, miss_cycles);
    }

    fclose(file_pointer);
    return 0;
}
#include <stdio.h>
#include <stdint.h>
#include <immintrin.h>  // AVX-512 intrinsics, RDTSCP, MFENCE

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

    // Open CSV file for writing
    FILE *file_pointer = fopen("avx512_timing.csv", "w");
    if (!file_pointer) {
        fprintf(stderr, "Could not open file\n");
        return 1;
    }

    fprintf(file_pointer, "avx512_cycles\n"); 

    for (unsigned int i = 0; i < EVENTS; i++) {
        _mm_mfence();              
        start = __rdtscp(&temp);    

        result = _mm512_mul_ps(evens, odds); 

        finish = __rdtscp(&temp);    
        _mm_mfence();               

        fprintf(file_pointer, "%llu\n", finish - start); 
    }

    fclose(file_pointer);
    return 0;
}

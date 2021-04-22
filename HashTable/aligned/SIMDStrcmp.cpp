#include "x86intrin.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

const char* string_1 = "first string ever";
const char* string_2 = "first string ever";

int SIMD_strcmp(const char* a, const char* b)
{
    __m128i first  = _mm_load_si128((const __m128i*) a);
    __m128i second = _mm_load_si128((const __m128i*) b);

    return _mm_cmpistri(first, second, _SIDD_CMP_EQUAL_EACH + _SIDD_NEGATIVE_POLARITY);
}

int main()
{
    char* al_1 = (char*)aligned_alloc(16, 50);
    strcpy(al_1, string_1);
    char* al_2 = (char*)aligned_alloc(16, 50);
    strcpy(al_2, string_2); 
    printf("%d\n", SIMD_strcmp(al_1, al_2));
    return 0;
}
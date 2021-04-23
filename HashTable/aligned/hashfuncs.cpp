#include "hashfuncs.h"

hash_t opt2_crc32(const __m128i* key, size_t n_blocks)
{
    hash_t crc = 0;

    char* current_char = (char*) key;

    while (*current_char && *(current_char+1) && *(current_char+2) && *(current_char+3) && *(current_char+4) && *(current_char+5) && *(current_char+6) && *(current_char+7))
    {
        crc = _mm_crc32_u64(crc, *((u_int64_t*)(current_char)));
        current_char+=8;
    }

    while (*current_char && *(current_char+1) && *(current_char+2) && *(current_char+3))
    {
        crc = _mm_crc32_u32(crc, *((unsigned int*)(current_char)));
        current_char+=4;
    }

    while (*current_char && *(current_char+1))
    {
        crc = _mm_crc32_u16(crc, *((unsigned short*)(current_char)));
        current_char+=2;
    }

    while (*current_char)
    {
        crc = _mm_crc32_u8(crc, *(current_char++));
    }

    return crc;
}

hash_t opt3_crc32(const __m128i* key, size_t n_blocks)
{
    hash_t crc = 0;

    hash_t* current_oct = (hash_t*) key;

    for (int n_block = 0; n_block < n_blocks; n_block++)
    {
        crc = _mm_crc32_u64(crc, *(current_oct++));
        crc = _mm_crc32_u64(crc, *(current_oct++));
    }
    return crc;
}
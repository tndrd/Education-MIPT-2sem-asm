#include "hashfuncs.h"

hash_t hash_1(const char* key)
{
    assert(key);
    return 1;
}

hash_t hash_ascii_0(const char* key)
{
    assert(key);
    return *key;
}

hash_t hash_len(const char* key)
{
    assert(key);
    return strlen(key);
}

hash_t hash_sum(const char* key)
{
    assert(key);
    hash_t sum = 0;
    for (int n_char = 0;;n_char++)
    {
        char current_char = key[n_char];
        if (!current_char) break;
        sum += current_char;
    }
    return sum;
}

hash_t hash_sum_len(const char* key)
{
    assert(key);
    hash_t sum = 0;
    int n_char = 0;

    for (;;n_char++)
    {
        char current_char = key[n_char];
        if (!current_char) break;
        sum += current_char;
    }
    return sum / (n_char+1);
}

hash_t ror(hash_t H) { return (H << (sizeof(H) * 8 - 1)) | (H >> 1) ;}

hash_t rol(hash_t H) { return (H >> (sizeof(H) * 8 - 1)) | (H << 1) ;}

hash_t ded_rol_hash(const char* key)
{
    assert(key);

    hash_t hash = 0;

    for (size_t n_char = 1;; n_char++)
    {
        char current_char = key[n_char];
        if (!current_char) break;

        hash = rol(hash) ^ current_char;
    }

    return hash;
}

hash_t ded_ror_hash(const char* key)
{
    assert(key);

    hash_t hash = 0;

    for (size_t n_char = 1;; n_char++)
    {
        char current_char = key[n_char];
        if (!current_char) break;

        hash = ror(hash) ^ current_char;
    }

    return hash;
}

hash_t MeowPurr_hash (const char* key)
{
    const unsigned int m = 0x5bd1e995;
    const unsigned int seed = 0;
    const int r = 24;
    size_t len = strnlen(key, MAX_KEY_LENGTH);

    unsigned int h = seed ^ len;

    const unsigned char * data = (const unsigned char *)key;
    unsigned int k;

    while (len >= 4)
    {
        k  = data[0];
        k |= data[1] << 8;
        k |= data[2] << 16;
        k |= data[3] << 24;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len -= 4;
    }

    switch (len)
    {
    case 3:
        h ^= data[2] << 16;
    case 2:
        h ^= data[1] << 8;
    case 1:
        h ^= data[0];
        h *= m;
    };

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h;
}

hash_t opt1_crc32(const char* key)
{
    hash_t crc = 0;

    char* current_char = (char*) key;

    while (*current_char)
    {
        crc = _mm_crc32_u8(crc, *(current_char++));
    }

    return crc;
}

hash_t opt2_crc32(const char* key)
{
    hash_t crc = 0;

    char* current_char = (char*) key;
    
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
#ifndef __HFUNCS__
#define __HFUNCS__

#include "stdlib.h"
#include "stdio.h"
#include "assert.h"
#include "assert.h"
#include "string.h"
#include "config.h"
#include "x86intrin.h"


hash_t hash_1(const char* key);

hash_t hash_ascii_0(const char* key);

hash_t hash_len(const char* key);

hash_t hash_sum(const char* key);

hash_t hash_sum_len(const char* key);

hash_t ror(hash_t H);

hash_t rol(hash_t H);

hash_t ded_rol_hash(const char* key);

hash_t ded_ror_hash(const char* key);

hash_t MeowPurr_hash (const char* key);

hash_t crc32(const char* key);

hash_t opt1_crc32(const char* key);

hash_t opt11_crc32(const char* key);

hash_t opt2_crc32(const char* key);

#endif
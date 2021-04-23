#ifndef __HFUNCS__
#define __HFUNCS__

#include "stdlib.h"
#include "stdio.h"
#include "assert.h"
#include "assert.h"
#include "string.h"
#include "../../../Education-MIPT/Onegin/onegin.h"
#include "config.h"
#include "x86intrin.h"

hash_t opt2_crc32(const __m128i* key, size_t n_blocks = 0);
hash_t opt3_crc32(const __m128i* key, size_t n_blocks);


#endif
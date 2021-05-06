inline int SIMD_strcmp(const __m128i* a, size_t a_n_blocks, const __m128i* b, size_t b_n_blocks)
{
    assert(a_n_blocks);
    assert(b_n_blocks);
    
    if (a_n_blocks != b_n_blocks) return -1;
    
    int res = ~0;

    for (int n_block = 0; n_block < a_n_blocks; n_block++)
    {
        __m128i first  = _mm_load_si128(a);
        __m128i second = _mm_load_si128(b);
        
        res &= _mm_movemask_epi8(_mm_cmpeq_epi8(first, second));

        if (res != 0xFFFF) return 1;

        a += 1;
        b += 1;
    }
    
    return res != 0xFFFF;
}
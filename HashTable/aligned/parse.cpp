#include "parse.h"

size_t countChars(char* buffer, char to_search)
{
    assert(to_search);
    assert(buffer);
    
    char* char_ptr = buffer;
    size_t n_found = 0;

    while (true)
    {
        char_ptr = strchr(char_ptr, to_search);
        if (!char_ptr) break;
        n_found++;
        char_ptr++;
    }
    return n_found;
}


size_t countKeys_ACSV(char* keys)
{
    assert(acsv);
    assert((long int)keys % 16 == 0);

    size_t n_key       = 0;
    char*  current_key = keys + 16;

    for (; *current_key; current_key += *(current_key-1) * 16) n_key++;

    printf("%d\n", n_key);
    return n_key;
}

WordList parseKeys_ACSV(char* keys)
{
    assert(keys);
    assert((long int)keys % 16 == 0);
    
    size_t n_words = countKeys_ACSV(keys) + 1;
    
    Word* words = (Word*)calloc(n_words, sizeof(Word));

    if (!words)
    {
        printf("Failed to generate words array: out of mem\n");
        return {nullptr, 0};
    }

    char* current_key   = keys + 16;

    size_t n_word = 0;

    for (unsigned char block_size = *(current_key - 1); *current_key; current_key += 16 * block_size, block_size = *(current_key - 1))
    {
        *(current_key - 1) = '\0';
        words[n_word]   .buff     = (const __m128i*) current_key;
        words[n_word++] .n_blocks =                  block_size;
    }

    return {words, n_word};
}
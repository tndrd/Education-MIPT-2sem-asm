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

WordList parseCSV(char* buffer)
{
    assert(buffer);
    size_t n_words = countChars(buffer, CSV_SEPARATOR) + 1;
    
    Word* words = (Word*)calloc(n_words, sizeof(Word));

    if (!words)
    {
        printf("Failed to generate words array: out of mem\n");
        return {nullptr, 0};
    }
    
    size_t n_word = 0;
    
    char* word_start = buffer;

    for (char* current_char = strchr(buffer, CSV_NEWLINE); current_char != nullptr; current_char = strchr(current_char, CSV_NEWLINE))
    {
        
        *current_char = '\0';
        current_char++;
        char* separator = strchr(word_start, CSV_SEPARATOR);
        if (!separator) throw std::runtime_error("Wrong file format: missing separator\n");

        *(separator++) = '\0';

        assert((separator - word_start) % 16 == 0);

        words[n_word]   .buff     = word_start;
        words[n_word++] .n_blocks = (separator - word_start) / 16;

        word_start      =  current_char;
    }

    return {words, n_word};
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
        words[n_word]   .buff     = current_key;
        words[n_word++] .n_blocks = block_size;
    }

    return {words, n_word};
}
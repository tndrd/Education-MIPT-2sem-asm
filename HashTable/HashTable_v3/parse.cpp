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
    size_t n_words = countChars(buffer, '=') + 1;
    
    char** words = (char**)calloc(n_words, sizeof(char*));

    if (!words)
    {
        printf("Failed to generate words array: out of mem\n");
        return {nullptr, 0};
    }
    
    size_t n_word = 0;
    
    char* word_start = buffer;

    for (char* current_char = strchr(buffer, CSV_NEWLINE); current_char != nullptr; current_char = strchr(current_char, '\n'))
    {
        
        *current_char = '\0';
        current_char++;

        char* separator = strchr(word_start, CSV_SEPARATOR);
        if (!separator) throw std::runtime_error("Wrong file format: missing separator\n");

        *(separator++) = '\0';


        words[n_word++] = word_start;
       
        word_start  =  current_char;
    }

    return {words, n_word};
}
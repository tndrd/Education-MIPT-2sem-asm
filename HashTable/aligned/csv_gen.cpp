#include "hashtable.h"

int main()
{
    WordList words = parseDict("dict.txt");
    
    #define SAVE_DISTRIBUTION(func_name, size)                 \
        HashTable table_ ## func_name (size, func_name);       \
        table_ ## func_name .loadWords(words);                 \
        table_ ## func_name.saveCSV("csv/" #func_name ".csv"); \

    SAVE_DISTRIBUTION(hash_1, 241);
    SAVE_DISTRIBUTION(hash_ascii_0, 241);
    SAVE_DISTRIBUTION(hash_len, 241);
    SAVE_DISTRIBUTION(hash_sum, 2347);
    SAVE_DISTRIBUTION(hash_sum_len, 2347);
    SAVE_DISTRIBUTION(ded_rol_hash, 2347);
    SAVE_DISTRIBUTION(ded_ror_hash, 2347);
    SAVE_DISTRIBUTION(MeowPurr_hash, 2347);
    

    free(words.words);
    return 0;
}
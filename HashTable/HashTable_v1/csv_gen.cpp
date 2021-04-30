#include "hashtable.h"

int main()
{
    char* dict = ReadFile("../Datasets/Csv/ded-dict.csv");
    WordList words = parseCSV(dict);
    printf("There are %d words in dict\n", words.quantity);

    #define SAVE_DISTRIBUTION(func_name, size)                 \
        HashTable table_ ## func_name (size, func_name);       \
        printf("Reading into %s...\n", #func_name);\
        table_ ## func_name .loadWords(words);                 \
        printf("Read %d words\n", table_ ## func_name .n_elements);     \
        table_ ## func_name.saveCSV("csv/" #func_name ".csv"); \

    SAVE_DISTRIBUTION(hash_1, 2347);
    SAVE_DISTRIBUTION(hash_ascii_0, 2347);
    SAVE_DISTRIBUTION(hash_len, 2347);
    SAVE_DISTRIBUTION(hash_sum, 2347);
    SAVE_DISTRIBUTION(hash_sum_len, 2347);
    SAVE_DISTRIBUTION(ded_rol_hash, 2347);
    SAVE_DISTRIBUTION(ded_ror_hash, 2347);
    SAVE_DISTRIBUTION(MeowPurr_hash, 2347);
    SAVE_DISTRIBUTION(crc32, 2347);
    

    free(dict);
    free(words.words);
    return 0;
}
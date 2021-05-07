#include "hashtable.h"

void LoadWords(WordList& words, HashTable& hashtable)
{
    for(int n_word = 0; n_word < words.quantity; n_word++)
    {
        hashtable.addElement(words.words[n_word], 0);
    }
}

int main()
{
    char* dict = ReadFile("../Datasets/Csv/ded-dict.csv");
    WordList words = parseCSV(dict);
    printf("There are %d words in dict\n", words.quantity);

    #define SAVE_DISTRIBUTION(func_name, size)                 \
        HashTable table_ ## func_name ## _ ## size (size, func_name);       \
        printf("Reading into %s...\n", #func_name);\
        LoadWords(words, table_ ## func_name ## _ ## size);                 \
        printf("Read %d words\n", table_ ## func_name ## _ ## size .getQuantity());     \
        table_ ## func_name ## _ ## size .saveDistribution_CSV("csv/" #func_name "_" #size ".csv"); \

    SAVE_DISTRIBUTION(hash_1, 2347);
    SAVE_DISTRIBUTION(hash_ascii_0, 2347);
    SAVE_DISTRIBUTION(hash_len, 2347);
    SAVE_DISTRIBUTION(hash_sum, 2347);
    SAVE_DISTRIBUTION(hash_sum_len, 2347);
    SAVE_DISTRIBUTION(ded_rol_hash, 2347);
    SAVE_DISTRIBUTION(ded_ror_hash, 2347);
    SAVE_DISTRIBUTION(MeowPurr_hash, 2347);
    SAVE_DISTRIBUTION(crc32, 2347);
    SAVE_DISTRIBUTION(crc32, 131137)
    

    free(dict);
    free(words.words);
    return 0;
}
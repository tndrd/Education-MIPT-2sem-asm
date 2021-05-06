#include "hashtable.h"

size_t countWords(WordList& wordlist, HashTable& table)
{
    size_t n_found = 0;
    for (int n_word = 0; n_word < wordlist.quantity; n_word++)
    {
        ListElement* found = table.getElement(wordlist.words[n_word].buff, wordlist.words[n_word].n_blocks);
        n_found += !!found;
    }

    return n_found;
}

int main()
{
    //printf("%d\n", sizeof(HashTableList));
    HashTable table (131137, opt3_crc32);

    char* keys   = ReadFile_aligned("../Datasets/Acsv/ded-dict.keys", 16);
    char* values = ReadFile_aligned("../Datasets/Acsv/ded-dict.values", 16);
    
    table.readACSV(keys, values);

    char* dict400k = ReadFile_aligned("../Datasets/Acsv/400k-words.keys", 16);
    WordList words = parseKeys_ACSV(dict400k);

    int n_read = 0;
    for (int i = 0; i < 10; i ++)
        n_read = countWords(words, table);
    printf("%d\n", n_read);

    free(dict400k);
    free(words.words);
    free(keys);
    free(values);

    return 0;
}
#include "hashtable.h"

size_t countWords(WordList& wordlist, HashTable& table)
{
    size_t n_found = 0;
    for (int n_word = 0; n_word < wordlist.quantity; n_word++)
    {
        ListElement* found = table.getElement(wordlist.words[n_word].buff, wordlist.words[n_word].n_blocks);
        //printf("searchng {%s} of size %d: %p\n", wordlist.words[n_word].buff, wordlist.words[n_word].n_blocks, found);
        //if (found) printf("{%s}[%d] == {%s}[%d]\n\n", wordlist.words[n_word].buff, wordlist.words[n_word].n_blocks, found -> key_, found -> n_blocks_);
        n_found += !!found;
    }

    return n_found;
}
/*
int main()
{
    HashTable table (131137, opt3_crc32);
    printf("%d\n", sizeof(HashTableList));

    char* ded_dict = ReadFile_aligned("Datasets/ded-dict/ded-dict.acsv", 16);
    table.readCSV(ded_dict);

    
    char* dict400k = ReadFile_aligned("Datasets/400k-words/400k-words.acsv", 16);
    WordList words = parseCSV(dict400k);

    int read = 0;
    for (int i = 0; i < 10; i ++)
        read = countWords(words, table);
    printf("%d\n", read);
    

    free(dict400k);
    free(words.words);
    free(ded_dict);
    return 0;
}
*/
/*
int validateKeyBuffer(char* keys)
{
    assert(keys);
    assert((long int)keys % 16 == 0);

    char*         current_key = keys + 16;
    unsigned char block_size  = *(current_key - 1);

    for (;*current_key; current_key += block_size * 16, block_size = *(current_key - 1))
    
}
*/

int main()
{
    HashTable table (131137, opt3_crc32);
    printf("%d\n", sizeof(HashTableList));

    char* keys   = ReadFile_aligned("Datasets/ded-dict/ded-dict.keys", 16);
    char* values = ReadFile_aligned("Datasets/ded-dict/ded-dict.values", 16);
    
    printf("%d %d\n",  table.n_elements, table.readACSV(keys, values));

    //table.dump();

    char* dict400k = ReadFile_aligned("Datasets/400k-words/400k-words.keys", 16);
    WordList words = parseKeys_ACSV(dict400k);

    int read = 0;
    for (int i = 0; i < 10; i ++)
        read = countWords(words, table);
    printf("%d\n", read);
    
    free(dict400k);
    free(words.words);
    free(keys);
    free(values);

    return 0;
}
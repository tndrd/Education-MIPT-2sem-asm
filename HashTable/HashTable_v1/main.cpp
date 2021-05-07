#include "hashtable.h"

size_t countWords(const WordList& wordlist, HashTable& table)
{
    size_t n_found = 0;

    for (int n_word = 0; n_word < wordlist.quantity; n_word++)
    {
        n_found += !!table.getElement(wordlist.words[n_word]);
    }

    return n_found;
}

int main()
{
    HashTable table (131137, crc32);

    char* ded_dict = ReadFile("../Datasets/Csv/ded-dict.csv");
    table.readCSV(ded_dict);

    char* dict400k = ReadFile("../Datasets/Csv/400k-words.csv");
    WordList words = parseCSV(dict400k);
    
    int n_read = 0;

    for (int i = 0; i < 10; i ++)
        n_read = countWords(words, table);
    printf("%d\n", n_read);
    
    free(ded_dict);
    free(dict400k);
    free(words.words);

    return 0;
}
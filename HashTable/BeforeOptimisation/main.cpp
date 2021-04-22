#include "hashtable.h"

size_t countWords(WordList& wordlist, HashTable& table)
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
    HashTable table (131137, MeowPurr_hash);
    
    char* ded_dict = ReadFile("Datasets/ded-dict/ded-dict.csv");
    table.readCSV(ded_dict);
    printf("%d\n", table.n_elements);
    char* dict400k = ReadFile("Datasets/400k-words/400k-words.csv");
    WordList words = parseCSV(dict400k);

    int read = 0;

    for (int i = 0; i < 10; i ++)
        read = countWords(words, table);
    
    printf("%d\n", read);

    free(ded_dict);
    free(dict400k);
    free(words.words);

    return 0;
}
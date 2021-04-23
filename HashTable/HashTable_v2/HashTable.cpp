#include "hashtable.h"

HashTable::HashTable(size_t list_buffer_size, hash_t  (*hashfunc)(const char*)):
    list_buffer ((HashTableList*)aligned_alloc(64, list_buffer_size * sizeof(HashTableList))),
    n_elements  (0),
    size        (list_buffer_size),
    hashfunc    (hashfunc),
    mem_manager ()
{ 
    for (int n_list = 0; n_list < size; n_list++)
    {
        assert(list_buffer);
        *(list_buffer + n_list) = HashTableList();
    }
}

HashTable::~HashTable()
{
    free(list_buffer);
}

size_t HashTable::getQuantity()
{
    return n_elements;
}

int HashTable::addElement(const char* key, Type value)
{
    HashTableList* element_list = getList(key);

    ListElement*   new_element  = mem_manager.getNewElement();
    new_element -> key_         = key;
    new_element -> value_       = value;

    element_list -> append(new_element);
    n_elements++;
    return 0;
}

void HashTable::dump(bool is_quite)
{   
    printf("Hashtable:\n");

    int n_errors = 0;

    for (int n_list = 0; n_list < size; n_list++)
    {
        HashTableList* current_list = list_buffer + n_list;

        if (current_list -> size)
        {
            printf("#%d ", n_list, current_list -> size);
            n_errors += (current_list -> print(is_quite));
        }
    }
    printf("\nErrors total: %d\n", n_errors);
}

ListElement* HashTable::getElement(const char* key)
{
    assert(key);
    HashTableList* list = getList(key); 
    return list -> getElement(key);
}

Type HashTable::getValue(const char* key)
{
    assert(key);
    HashTableList* list = getList(key); 
    return list -> getValue(key);
}

void HashTable::setValue(const char* key, Type value)
{
    assert(key);
    HashTableList* list = getList(key); 
    list -> setValue(key, value);
}

HashTableList* HashTable::getList(const char* key)
{
    return list_buffer + hashfunc(key) % size;
}

const char* HashTable::saveDistribution_CSV(const char* filename)
{
    FILE* fp = fopen(filename, "w");

    if (!fp) return nullptr;

    fprintf(fp, "hash,quantity\n");
    
    for (int n_list = 0; n_list < size; n_list++)
    {
        HashTableList* current_list = list_buffer + n_list;

        if (current_list -> size)
        {
            fprintf(fp, "%d,%d\n", n_list, current_list -> size);
        }
    }
    
    fclose(fp);

    return filename;
}

size_t HashTable::readCSV(char* buffer)
{

    assert(buffer);
    
    char*  word_start  = buffer;
    size_t words_read  = 0;

    for (char* current_char = strchr(buffer, CSV_NEWLINE); current_char != nullptr; current_char = strchr(current_char, '\n'))
    {
        *current_char = '\0';
        current_char++;

        char* separator = strchr(word_start, CSV_SEPARATOR);
        if (!separator) throw std::runtime_error("Wrong file format: missing separator\n");

        *(separator++) = '\0';


        addElement(word_start, separator);
        words_read++;
        word_start  =  current_char;
    }
    return words_read;
}
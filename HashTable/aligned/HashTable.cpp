#include "hashtable.h"

HashTable::HashTable(size_t list_buffer_size, hash_t  (*hashfunc)(const char*, size_t)):
    capacity    (INITIAL_HASHTABLE_CAPACITY),
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

int HashTable::addElement(const char* key, size_t n_blocks, Type value)
{
    #ifdef HASHTABLE_SAFE
    if (!key)       throw std::runtime_error("Wrong key");
    if (!n_blocks_) throw std::runtime_error("Wrong size");
    #endif

    HashTableList* element_list = getList(key, n_blocks);

    ListElement*   new_element  = mem_manager.getNewElement();
    new_element -> key_         = key;
    new_element -> value_       = value;
    new_element -> n_blocks_    = n_blocks;

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

ListElement* HashTable::getElement(const char* key, size_t n_blocks)
{
    assert(key);
    HashTableList* list = getList(key, n_blocks); 
    return list -> getElement(key, n_blocks);
}

Type HashTable::getValue(const char* key, size_t n_blocks)
{
    assert(key);
    HashTableList* list = getList(key, n_blocks); 
    return list -> getValue(key, n_blocks);
}

void HashTable::setValue(const char* key, size_t n_blocks, Type value)
{
    assert(key);
    HashTableList* list = getList(key, n_blocks); 
    list -> setValue(key, n_blocks, value);
}

HashTableList* HashTable::getList(const char* key, size_t n_blocks)
{
    return list_buffer + hashfunc(key, n_blocks) % size;
}

const char* HashTable::saveCSV(const char* filename)
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
    assert((long int)buffer % 16 == 0);
    
    char*  word_start  = buffer;
    size_t words_read  = 0;

    for (char* current_char = strchr(buffer, CSV_NEWLINE); current_char != nullptr; current_char = strchr(current_char, '\n'))
    {
        *current_char = '\0';
        current_char++;

        //printf("%d; %s\n", current_char - word_start, current_char - 14);

        char* separator = strchr(word_start, CSV_SEPARATOR);
        if (!separator) throw std::runtime_error("Wrong file format: missing separator\n");

        *(separator++) = '\0';

        assert((separator - word_start) % 16 == 0);

        addElement(word_start, (separator - word_start) / 16, separator);
        words_read++;
        word_start  =  current_char;
    }
    return words_read;
}

size_t HashTable::readACSV(char* keys, char* values)
{
    assert(keys);
    assert((long int)keys % 16 == 0);
    assert(values);
    
    char* current_key   = keys + 16;
    char* word_start    = values;
    char* word_end      = values;

    size_t words_read = 0;

    char* prev_key = keys;
    size_t prev_block_size = 0;
    char* prev_value = values;
    for (unsigned char block_size = *(current_key - 1); *current_key; current_key += 16 * block_size, block_size = *(current_key - 1))
    {

        *(current_key - 1) = '\0';

        word_end = strchr(word_start, '\n');
        
        if (!word_end) throw std::runtime_error("Keys and values amounts do not match\n");

        *word_end = '\0';

        addElement(prev_key, prev_block_size, prev_value);

        prev_key = current_key;
        prev_block_size = block_size;
        prev_value = word_start;

        word_start = word_end + 1;

        words_read++;

    }

    return words_read;
}
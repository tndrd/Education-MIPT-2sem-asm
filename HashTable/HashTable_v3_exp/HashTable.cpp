#include "hashtable.h"

HashTable::HashTable(size_t list_buffer_size, hash_t  (*hashfunc)(const __m128i*, size_t)):
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

int HashTable::addElement(const __m128i* key, size_t n_blocks, Type value)
{
    #ifdef HASHTABLE_SAFE
    if (!key)       throw std::runtime_error("Wrong key");
    if (!n_blocks_) throw std::runtime_error("Wrong size");
    #endif

    HashTableList* element_list = getList(key, n_blocks);

    ListElement*   new_element = nullptr;

    if (!element_list -> size)
    {
        new_element = &(element_list -> tail_el);
        element_list -> size++;
        new_element -> key_         = key;
        new_element -> value_       = value;
        new_element -> n_blocks_    = n_blocks;
        n_elements++;
        return 0;
    }

    new_element  = mem_manager.getNewElement();
    new_element -> key_         = key;
    new_element -> value_       = value;
    new_element -> n_blocks_    = n_blocks;

    element_list -> append(new_element);
    n_elements++;
    return 0;
}

void HashTable::dump(bool is_quiet)
{   
    printf("Hashtable:\n");

    int n_errors = 0;

    for (int n_list = 0; n_list < size; n_list++)
    {
        HashTableList* current_list = list_buffer + n_list;

        if (current_list -> size)
        {
            printf("#%d ", n_list, current_list -> size);
            n_errors += (current_list -> print(is_quiet));
        }
    }
    printf("\nErrors total: %d\n", n_errors);
}

ListElement* HashTable::getElement(const __m128i* key, size_t n_blocks)
{
    assert(key);
    HashTableList* list = getList(key, n_blocks); 
    return list -> getElement(key, n_blocks);
}

Type HashTable::getValue(const __m128i* key, size_t n_blocks)
{
    assert(key);
    HashTableList* list = getList(key, n_blocks); 
    return list -> getValue(key, n_blocks);
}

void HashTable::setValue(const __m128i* key, size_t n_blocks, Type value)
{
    assert(key);
    HashTableList* list = getList(key, n_blocks); 
    list -> setValue(key, n_blocks, value);
}

HashTableList* HashTable::getList(const __m128i* key, size_t n_blocks)
{
    return list_buffer + hashfunc(key, n_blocks) % size;
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

size_t HashTable::readACSV(char* keys, char* values)
{
    assert(keys);
    assert((long int)keys % 16 == 0);
    assert(values);
    
    char* current_key   = keys + 16;
    char* word_start    = values;
    char* word_end      = values;

    size_t words_read = 0;

    char*  prev_key        = keys;
    size_t prev_block_size = 0;
    char*  prev_value      = values;

    for (unsigned char block_size = *(current_key - 1); *current_key; current_key += 16 * block_size, block_size = *(current_key - 1))
    {

        *(current_key - 1) = '\0';

        word_end = strchr(word_start, '\n');
        
        if (!word_end) throw std::runtime_error("Keys and values amounts do not match\n");

        *word_end = '\0';

        addElement((const __m128i*)prev_key, prev_block_size, prev_value);

        prev_key = current_key;
        prev_block_size = block_size;
        prev_value = word_start;

        word_start = word_end + 1;

        words_read++;

    }

    return words_read;
}

size_t HashTable::validate()
{

    size_t n_errors = 0;

    for (int n_list = 0; n_list < size; n_list++)
    {
        HashTableList* current_list = list_buffer + n_list;

        if (current_list -> size)
        {
            n_errors += !!(current_list -> validate());
        }
    }
    return n_errors;
}
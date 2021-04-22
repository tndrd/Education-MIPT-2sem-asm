#include "hashtable.h"

HashTable::HashTable(size_t list_buffer_size, hash_t  (*hashfunc)(const char*)):
    capacity    (INITIAL_HASHTABLE_CAPACITY),
    data_buffer ((ListElement*)calloc(capacity + 1, sizeof(ListElement))),
    list_buffer ((HashTableList*)calloc(list_buffer_size, sizeof(HashTableList))),
    n_elements  (0),
    size        (list_buffer_size),
    hashfunc    (hashfunc)
{ 
    for (int n_list = 0; n_list < size; n_list++)
    {
        *(list_buffer + n_list) = HashTableList(&data_buffer);
    }
}

HashTable::~HashTable()
{
    free(data_buffer);
    free(list_buffer);
}

int HashTable::addElement(const char* key, Type value)
{
    hash_t hash = hashfunc(key) % size;
    HashTableList* element_list = list_buffer + hash;
    
    if (getElementInList(element_list, key))
    {
        //printf("%s already exists!\n");
        return 1;
    }
    
    if (n_elements == capacity - 1)
    {
        if(!resize())
        {
            printf("Failed to resize\n");
            return 2;
        }
    }

    ListElement*   new_element  = data_buffer + n_elements + 1;
    new_element -> key_         = key;
    new_element -> value_       = value;

    element_list -> append(n_elements + 1);
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

int HashTable::resize()
{
    size_t new_capacity = capacity * 2 + 1;
    
    ListElement* new_data_buffer = (ListElement*)realloc(data_buffer, new_capacity * sizeof(ListElement));

    if (!new_data_buffer) return 0;

    capacity    = new_capacity;
    data_buffer = new_data_buffer;
    return 1;
}

ListElement* HashTable::getElement(const char* key)
{
    hash_t hash = hashfunc(key) % size;
    HashTableList* element_list = list_buffer + hash;
    return getElementInList(element_list, key);
}

ListElement* HashTable::getElementInList(HashTableList* list, const char* key)
{

    ListElement* current_element = data_buffer + list -> tail;
    size_t list_size             = list -> size;

    for (int n_element = 0; n_element < list_size; n_element++)
    {
        if (!strncmp(key, current_element -> key_, MAX_KEY_LENGTH)) return current_element;

        current_element = data_buffer + current_element -> next_;
    }

    return nullptr;
}

Type HashTable::getValue(const char* key)
{
    ListElement* element = getElement(key);
    if (!element) throw std::logic_error("Element doesn't exist");

    return element -> value_;
}

void HashTable::setValue(const char* key, Type value)
{
    ListElement* element = getElement(key);
    if (!element) throw std::logic_error("Element doesn't exist");

    element -> value_ = value;
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

void HashTable::loadWords(WordList& words)
{
    for (int n_word = 0; n_word < words.quantity; n_word++)
    {
        addElement(words.words[n_word], 0);
    }
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
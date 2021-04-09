#include "stdlib.h"
#include "stdio.h"
#include "assert.h"
#include "math.h"
#include "assert.h"

#include "List.cpp"

const size_t INITIAL_HASHTABLE_CAPACITY = 3;
const size_t INITIAL_HASHTABLE_SIZE = 131;

#define Type int

unsigned char hashfunc(const char* key)
{
    assert(key);
    return *key;
}

//template<typename Type>
struct HashTable
{
    size_t n_elements;
    size_t capacity;
    size_t size = 0;
    ListElement* data_buffer;
    HashTableList* list_buffer;
    // hashfunc = ?

    HashTable(size_t list_buffer_size):
        list_buffer(new HashTableList[list_buffer_size]),
        n_elements(0),
        capacity(INITIAL_HASHTABLE_CAPACITY),
        data_buffer(new ListElement[capacity]),
        size(list_buffer_size)
    { }

    ~HashTable()
    {
        delete data_buffer;
        delete list_buffer;
    }

    void addElement(const char* key, Type value)
    {

        assert(data_buffer);
        
        if (n_elements == capacity)
        {
            printf("overflow");
            return;
        }

        ListElement*   new_element  = data_buffer + n_elements;
        new_element -> key_   = key;
        new_element -> value_ = value;

        unsigned int   element_hash = hashfunc(key);
        HashTableList* element_list = list_buffer + element_hash;

        element_list -> append(new_element);
        n_elements++;
        return;
    }

    void dump()
    {
        for (int n_list = 0; n_list < size; n_list++)
        {
            HashTableList* current_list = list_buffer + n_list;

            if (current_list -> size)
            {
                printf("#%d (%d): \n", n_list, current_list -> size);
                current_list -> dump();
            }
        }
    }

};
#ifndef __HTABLE__
#define __HTABLE__

#include "stdlib.h"
#include "stdio.h"
#include "assert.h"
#include "math.h"
#include "assert.h"
#include <exception>
#include <iostream>
#include "string.h"
#include "../../../Education-MIPT/Onegin/onegin.h"
#include "config.h"
#include "list.h"
#include "hashfuncs.h"
#include "htmemmanager.h"
#include "parse.h"

//template<typename Type>
struct HashTable
{
    private:
    size_t         n_elements = 0;
    size_t         size       = 0;
    
    HashTableMemoryManager mem_manager;

    HashTableList* list_buffer       = nullptr;
    hash_t  (*hashfunc)(const char*) = nullptr;

    HashTable (const HashTable& that)            = delete;
    HashTable& operator= (const HashTable& that) = delete;

    HashTableList* getList(const char* key);

    public:

    HashTable(size_t list_buffer_size, hash_t  (*hashfunc)(const char*));
    ~HashTable();

    int addElement(const char* key, Type value);

    void dump(bool is_quite = true);

    ListElement* getElement(const char* key);
    Type getValue(const char* key);

    void setValue(const char* key, Type value);

    const char* saveDistribution_CSV(const char* filename);
    size_t      readCSV(char* buffer);

    size_t getQuantity();
};

#endif
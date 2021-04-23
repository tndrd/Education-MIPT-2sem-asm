#ifndef __HTABLE__
#define __HTABLE__

#include "stdlib.h"
#include "stdio.h"
#include "assert.h"
#include "math.h"
#include "assert.h"
#include "coloring.h"
#include <exception>
#include <iostream>
#include "string.h"
#include "../../../Education-MIPT/Onegin/onegin.h"
#include "config.h"
#include "list.h"
#include "hashfuncs.h"
#include "htmemmanager.h"
#include "parse.h"
#include "x86intrin.h"

//template<typename Type>
struct HashTable
{
    private:
    size_t         n_elements  = 0;
    size_t         size        = 0;

    HashTableList* list_buffer = nullptr;

    HashTableMemoryManager mem_manager;

    hash_t  (*hashfunc)(const __m128i*, size_t) = nullptr;

    HashTable (const HashTable& that)            = delete;
    HashTable& operator= (const HashTable& that) = delete;

    HashTableList* getList(const __m128i* key, size_t n_blocks);

    public:

    HashTable(size_t list_buffer_size, hash_t  (*hashfunc)(const __m128i*, size_t));
    ~HashTable();

    int addElement(const __m128i* key, size_t n_blocks, Type value);

    size_t validate();
    void dump(bool is_quiet = true);

    size_t getQuantity();

    ListElement* getElement(const __m128i* key, size_t n_blocks);
    Type getValue(const __m128i* key, size_t n_blocks);

    void setValue(const __m128i* key, size_t n_blocks, Type value);

    const char*    saveDistribution_CSV(const char* filename);
    size_t         readACSV(char* keys, char* values);

};

#endif
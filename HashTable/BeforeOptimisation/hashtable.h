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
#include "parse.h"
#include "list.h"
#include "hashfuncs.h"

//template<typename Type>
struct HashTable
{
    size_t         n_elements = 0;
    size_t         capacity   = 0;
    size_t         size       = 0;
    ListElement*   data_buffer       = nullptr;
    HashTableList* list_buffer       = nullptr;
    hash_t  (*hashfunc)(const char*) = nullptr;

    HashTable(size_t list_buffer_size, hash_t  (*hashfunc)(const char*));

    HashTable (const HashTable& that)            = delete;
    HashTable& operator= (const HashTable& that) = delete;

    ~HashTable();

    int addElement(const char* key, Type value);

    void dump(bool is_quite = true);

    int resize();

    ListElement* getElementInList(HashTableList* list, const char* key);
    ListElement* getElement(const char* key);

    char* getValue(const char* key);

    void setValue(const char* key, char* value);
    

    const char* saveCSV(const char* filename);

    size_t      readCSV(char* buffer);

    void loadWords(WordList& words);

};

#endif
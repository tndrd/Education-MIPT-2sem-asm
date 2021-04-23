#ifndef __LIST__
#define __LIST__

#include "stdlib.h"
#include "stdio.h"
#include "assert.h"
#include "math.h"
#include "assert.h"
#include "config.h"

static char* Neutral();

struct ListElement
{
    size_t next_ = 0;
    size_t prev_ = 0;
    const char*  key_ = nullptr;
    
    Type value_;

    ListElement(const char* key = nullptr, Type value = Neutral(), size_t next = 0, size_t prev = 0);
    ListElement(const ListElement& that)              = delete;
    ListElement& operator= (const ListElement& that)  = delete;

    void print();
};

struct HashTableList
{
    size_t size = 0;
    size_t head = 0;
    size_t tail = 0;

    ListElement** data_buffer_ptr;

    public:

    enum LIST_CODES
    {
        #define CODE(codename) codename,
        #include "listcodes.h"
        #undef CODE
    };

    HashTableList(ListElement** data_buffer);
    HashTableList (const HashTableList& that)            = delete;
    //HashTableList& operator= (const HashTableList& that) = delete;

    LIST_CODES append(size_t new_element_index);
    int print(bool quiet = true);
    const char* getCodename(LIST_CODES code);
    LIST_CODES validate();

};

#endif
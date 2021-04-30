#ifndef __LIST__
#define __LIST__

#include "stdlib.h"
#include "stdio.h"
#include "assert.h"
#include "math.h"
#include "assert.h"
#include "config.h"
#include "string.h"
#include <stdexcept>

static char* Neutral();

struct ListElement
{
    ListElement* next_ = 0;
    ListElement* prev_ = 0;
    const char*  key_ = nullptr;
    Type value_;

    ListElement(const char* key = nullptr, Type value = Neutral(), ListElement* next = nullptr, ListElement* prev = nullptr);
    ListElement(const ListElement& that)              = delete;
    //ListElement& operator= (const ListElement& that)  = delete;

    void print();
};

struct HashTableList
{   
    ListElement* tail = 0;
    size_t size = 0;
    ListElement* head = 0;
    volatile size_t test = 0;

    ListElement tail_el;

    public:

    enum LIST_CODES
    {
        #define CODE(codename) codename,
        #include "listcodes.h"
        #undef CODE
    };

    HashTableList();
    HashTableList (const HashTableList& that)            = delete;
    //HashTableList& operator= (const HashTableList& that) = delete;

    LIST_CODES append(ListElement* new_element);
    int print(bool quiet = true);
    const char* getCodename(LIST_CODES code);
    LIST_CODES validate();

    ListElement* getElement(const char* key);
    Type getValue(const char* key);
    void setValue(const char* key, Type value);

};

#endif
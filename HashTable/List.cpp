#include "stdlib.h"
#include "stdio.h"
#include "assert.h"
#include "math.h"
#include "assert.h"

#define Type int


int    Neutral() { return 0; }
//template<> double Neutral() { return NAN; }

//template<typename Type>
struct ListElement
{

    ListElement* next_;
    ListElement* prev_;
    const char*  key_;
    
    Type value_;

    ListElement(const char* key = nullptr, Type value = Neutral(), ListElement* next = nullptr, ListElement* prev = nullptr):
        value_(value),
        key_(key),
        next_(next),
        prev_(prev) {}

    ListElement(const ListElement& that)             = delete;
    ListElement& operator= (const ListElement& that) = delete;

    void dump()
    {
        // TODO
        printf("%s: %d", key_, value_);
    }

};


//template<typename Type>
struct HashTableList
{
    size_t size       = 0;
    ListElement /*<Type>*/ * head = nullptr;
    ListElement /*<Type>*/ * tail = nullptr;

    public:

    enum LIST_CODES
    {
        LIST_OK,
        LIST_WRONG_SIZE,
        LIST_SIZE_AND_ELEMENTS_MISMATCH
    };

    HashTableList():
    size(0),
    head(nullptr),
    tail(nullptr)
    { }

    HashTableList (const HashTableList& that)            = delete;
    HashTableList& operator= (const HashTableList& that) = delete;

    void append(ListElement* new_element)
    {
        assert(new_element);
        
        if (!size)
        {
            head = new_element;
            tail = new_element;
            size++;
            return;
        }
        
        if (!head)
        {
            printf("head is zero while size more than zero");
            return;
        }

        head -> next_        = new_element;
        new_element -> prev_ = head;
        head                 = new_element;
        size++;
        return;
    }

    void dump()
    {
        int n_element = 0;
        ListElement* current_element = tail;

        for (;n_element < size; n_element++)
        {
            if (!current_element) break;

            printf("  ");
            current_element -> dump();
            printf("\n");
        }

        if (!current_element && (n_element != size))
        {
            printf("DUMP_ERROR");
            return;
        }
    }

};
//SRC 32
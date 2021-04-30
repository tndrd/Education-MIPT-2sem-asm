#ifndef __HT_MEM_MANAGER__
#define __HT_MEM_MANAGER__

#include "stdlib.h"
#include "stdio.h"
#include "list.h"
#include <stdexcept>
#include "malloc.h"

template <typename T>
struct Stack
{
    T* buffer = nullptr;
    size_t capacity = 0;
    size_t size     = 0;

    Stack(const Stack& that)              = delete;
    Stack& operator= (const Stack& that)  = delete;

    Stack(const size_t& capacity);

    ~Stack();

    T* push(T& value);
    T pop();

    T* getNewElement(); 

    private:
    void resizeUp();
    void resizeDown();

    //void _prohibit_resize() { throw std::runtime_error("Attempt to resize non-resizeable stack"); }

};

class HashTableMemoryManager
{

    Stack<ListElement>*  current_buffer;
    Stack< Stack<ListElement>* > allocd_element_bufs;

    HashTableMemoryManager(const HashTableMemoryManager& that)              = delete;
    HashTableMemoryManager& operator= (const HashTableMemoryManager& that)  = delete;
    void switchToNewBuffer();

    public:

    HashTableMemoryManager(size_t initial_capacity = INITIAL_HASHTABLE_CAPACITY);
    ~HashTableMemoryManager();
    ListElement* getNewElement();

};

#endif
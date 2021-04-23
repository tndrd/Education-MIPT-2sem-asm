#include "list.h"

static char* Neutral() { return 0; }

ListElement::ListElement(const char* key, Type value, ListElement* next, ListElement* prev, size_t n_blocks):
    value_(value),
    key_(key),
    next_(next),
    prev_(prev),
    n_blocks_(n_blocks)
    { }


void ListElement::print()
{
    printf("%s[%d]: %s", key_, n_blocks_, value_);
}

HashTableList::HashTableList():
    size(0),
    head(0),
    tail(0)
{ }

HashTableList::LIST_CODES HashTableList::append(ListElement* new_element)
{
    assert(new_element);
    if (!new_element)
    {
        return LIST_WRONG_POINTER_PARAM;
    }
    
    if (!size)
    {

        new_element -> next_ = nullptr;
        new_element -> prev_ = nullptr; 
        head = new_element;
        tail = new_element;
        size++;
        return LIST_OK;
    }

    head -> next_        = new_element;
    new_element -> prev_ = head;
    new_element -> next_ = nullptr;
    head = new_element;
    size++;
    return LIST_OK;
}

int HashTableList::print(bool quiet)
{

    LIST_CODES status = validate();
    
    printf("[%d](%s) (%p -> ... -> %p): \n", size, getCodename(status), tail, head);
    
    ListElement* current_element = tail;
    
    if (quiet && status == LIST_OK)
    {
        for (int n_element = 0; n_element < size; n_element++)
        {
            if (!current_element) break;

            printf("  ");
            current_element -> print();
            printf("\n");
            current_element = current_element -> next_;
        }
    }

    else
    {
        for (int n_element = 0; n_element < size; n_element++)
        {
            if (!current_element) break;
            
            ListElement* next            = current_element -> next_;
            ListElement* prev            = current_element -> prev_;

            printf("  ");
            current_element -> print();
            printf(" [ %p -> %p -> %p ]", prev, current_element, next);
            printf("\n");
            current_element = current_element -> next_;
        }
    }

    printf("\n");

    if (status == LIST_OK) return 0;
    else                   return 1;

}

const char* HashTableList::getCodename(LIST_CODES code)
{
    #define CODE(codename) case codename: return #codename;
    switch (code)
    {
        #include "listcodes.h"
        default: return "LIST_UNKNOWN_CODE";
    }
    #undef CODE 
}

int SIMD_strcmp(const char* a, size_t la, const char* b, size_t lb)
{
    assert(la);
    assert(lb);
    
    if (la != lb) return -1;
    
    int res = ~0;

    for (int n_block = 0; n_block < la; n_block++)
    {
        __m128i first  = _mm_load_si128((const __m128i*) a);
        __m128i second = _mm_load_si128((const __m128i*) b);
        
        res &= _mm_movemask_epi8(_mm_cmpeq_epi8(first, second));

        if (res != 0xFFFF) return 1;

        a += 16;
        b += 16;
    }
    
    return res != 0xFFFF;
}

ListElement* HashTableList::getElement(const char* key, size_t n_blocks)
{
    assert(key);
    
    register ListElement* current_element = tail;
    
    for (int n_element = 0; n_element < size; n_element++)
    {
        int result = SIMD_strcmp(key, n_blocks, current_element -> key_, current_element -> n_blocks_);
        if (!result) return current_element;

        current_element = current_element -> next_;
    }

    return nullptr;
}


Type HashTableList::getValue(const char* key, size_t n_blocks)
{
    ListElement* element = getElement(key, n_blocks);
    if (!element) throw std::runtime_error("Element doesn't exist");

    return element -> value_;
}

void HashTableList::setValue(const char* key, size_t n_blocks, Type value)
{
    ListElement* element = getElement(key, n_blocks);
    if (!element) throw std::runtime_error("Element doesn't exist");

    element -> value_ = value;
}

HashTableList::LIST_CODES HashTableList::validate()
{

    if (size < 0) return LIST_WRONG_SIZE_VALUE;

    if (size)
    {
        if (!head) return LIST_SIZE_AND_HEAD_MISMATCH;
        if (!tail) return LIST_SIZE_AND_TAIL_MISMATCH;
    }

    if (tail && tail -> prev_) return LIST_TAIL_IS_AHEAD;
    
    if (tail -> next_)
    {
        ListElement* current_element = tail -> next_;
        size_t n_element             = 1;

        do
        {
            if (!current_element -> prev_)         return LIST_BAD_LINKING;
            if (current_element -> next_ == tail)  return LIST_LOOPED;

            if (current_element -> next_) current_element = current_element -> next_;
            n_element++;

        } while (n_element < size && current_element -> next_);

        if (current_element != head) return LIST_HEAD_IS_BEHIND;
    }

    else if (head != tail) return LIST_BAD_LINKING;

    return LIST_OK;
}
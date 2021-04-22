#include "list.h"

//static int Neutral() { return 0; }
static char* Neutral() { return nullptr; }
//template<> double Neutral() { return NAN; }

//template<typename Type>

ListElement::ListElement(const char* key, Type value, size_t next, size_t prev):
    value_(value),
    key_(key),
    next_(next),
    prev_(prev) {}


void ListElement::print()
{
    printf("%s: %d", key_, value_);
}

HashTableList::HashTableList(ListElement** data_buffer):
    size(0),
    head(0),
    tail(0),
    data_buffer_ptr(data_buffer)
{ }


HashTableList::LIST_CODES HashTableList::append(size_t new_element_index)
{
    ListElement* data_buffer = *data_buffer_ptr;

    if (!data_buffer)
    {
        return LIST_WRONG_POINTER_PARAM;
    }
    
    if (!size)
    {
        head = new_element_index;
        tail = new_element_index;

        (data_buffer + new_element_index) -> next_ = 0;
        (data_buffer + new_element_index) -> prev_ = 0; 

        size++;
        return LIST_OK;
    }

    (data_buffer + head) -> next_              = new_element_index;
    (data_buffer + new_element_index) -> prev_ = head;
    (data_buffer + new_element_index) -> next_ = 0;
    head                                       = new_element_index;
    size++;
    return LIST_OK;
}

int HashTableList::print(bool quiet)
{
    ListElement* data_buffer = *data_buffer_ptr;

    LIST_CODES status = validate();
    
    printf("[%d](%s) (%d -> ... -> %d): \n", size, getCodename(status), tail, head);
    
    size_t current_element_index = tail;
    
    if (quiet && status == LIST_OK)
    {
        for (int n_element = 0; n_element < size; n_element++)
        {
            if (!current_element_index) break;

            printf("  ");
            (data_buffer + current_element_index) -> print();
            printf("\n");
            current_element_index = (data_buffer + current_element_index) -> next_;
        }
    }

    else
    {
        for (int n_element = 0; n_element < size; n_element++)
        {
            if (!current_element_index) break;
            
            ListElement* current_element = data_buffer + current_element_index;
            size_t next_index            = current_element -> next_;
            size_t prev_index            = current_element -> prev_;

            printf("  ");
            (data_buffer + current_element_index) -> print();
            printf(" [ %p (%d) -> %p (%d) -> %p (%d) ]", data_buffer + prev_index, prev_index, current_element, current_element_index, data_buffer + next_index, next_index);
            printf("\n");
            current_element_index = current_element -> next_;
        }
    }

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


HashTableList::LIST_CODES HashTableList::validate()
{
    ListElement* data_buffer = *data_buffer_ptr;

    if (size < 0)                       return LIST_WRONG_SIZE_VALUE;
    //if (size > parent_table.n_elements) return LIST_SIZE_GREATER_PARENT_TABLE_SIZE;

    if (size)
    {
        if (!head) return LIST_SIZE_AND_HEAD_MISMATCH;
        if (!tail) return LIST_SIZE_AND_TAIL_MISMATCH;
    }

    if (tail && (data_buffer + tail) -> prev_) return LIST_TAIL_IS_AHEAD;
    
    if ((data_buffer + tail) -> next_)
    {
        ListElement* current_element = data_buffer + ((data_buffer + tail) -> next_);
        size_t n_element             = 1;

        do
        {
            if (!current_element -> prev_)         return LIST_BAD_LINKING;
            if (current_element -> next_ == tail)  return LIST_LOOPED;

            if (current_element -> next_) current_element = data_buffer + (current_element -> next_);
            n_element++;

        } while (n_element < size && current_element -> next_);

        if (current_element != (data_buffer + head)) return LIST_HEAD_IS_BEHIND;
    }

    else if (head != tail) return LIST_BAD_LINKING;

    return LIST_OK;
}
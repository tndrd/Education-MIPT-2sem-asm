#include "htmemmanager.h"

template <typename T>
Stack<T>::Stack(const size_t& capacity):
    buffer((T*)aligned_alloc(64, (capacity * sizeof(T)) + ((capacity * sizeof(T)) % 64))),
    capacity(capacity),
    size(0)
{
    if (!buffer)
        throw std::runtime_error("Failed to create buffer\n");     
}

template <typename T>
Stack<T>::~Stack()
{
    free(buffer);
}

template <typename T>
T* Stack<T>::push(T& value)
{
    if (size == capacity) resizeUp();
    buffer[size++] = value;
    return buffer + size - 1;
}

template <typename T>
T* Stack<T>::getNewElement()
{
    if (size == capacity) resizeUp();
    return buffer + (size++);
}

template <typename T>
void Stack<T>::resizeUp()
{
    T* new_buffer = (T*)realloc(buffer, capacity * 2 * sizeof(T));

    if (!new_buffer) throw std::runtime_error("Failed to resize up\n");

    buffer = new_buffer;
    capacity *= 2;
}

template <typename T>
void Stack<T>::resizeDown()
{
    T* new_buffer = (T*)realloc(buffer, capacity / 2 * sizeof(T));

    if (!new_buffer) throw std::runtime_error("Failed to resize down\n");

    buffer = new_buffer;

    capacity /= 2;
}

template <typename T>
T Stack<T>::pop()
{
    if (!size) throw std::runtime_error("Attempt to pop empty stack\n");

    T value = buffer[--size];

    if (size == capacity / 2 && size > 1) resizeDown();

    return value;
}

HashTableMemoryManager::HashTableMemoryManager(size_t initial_capacity):
    current_buffer(new Stack<ListElement>(initial_capacity)),
    allocd_element_bufs(HT_MEM_STACK_INITIAL_CAPACITY)
    {  }

HashTableMemoryManager::~HashTableMemoryManager()
{
    delete current_buffer;

    size_t n_buffers = allocd_element_bufs.size;

    for (int n_elem_buf = 0; n_elem_buf < n_buffers; n_elem_buf++)
        delete allocd_element_bufs.pop();
}

ListElement* HashTableMemoryManager::getNewElement()
{
    if (current_buffer -> size == current_buffer -> capacity) switchToNewBuffer();
    return current_buffer -> getNewElement();
}


void HashTableMemoryManager::switchToNewBuffer()
{
    allocd_element_bufs.push(current_buffer);
    current_buffer = new Stack<ListElement>(current_buffer -> capacity * 2);
}

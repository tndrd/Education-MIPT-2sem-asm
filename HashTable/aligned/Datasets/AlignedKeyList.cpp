template<typename T>
struct AlignedKeyList
{
    size_t alignment;
    size_t n_elements;
    T*     data;
}


AlignedKeyList::AlignedKeyList(const char* buffer, size_t alignment)
    data(buffer),
    alignment(alignment)
    {
        if (!data)                 throw std::runtime_error("Wrong buffer");
        if (data % alignment != 0) throw std::runtime_error("Alignment and buffer adress do not match");

        ReadAlignedKeyList(buffer);
    }
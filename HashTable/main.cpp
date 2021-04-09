#include "HashTable.cpp"

int main()
{
    HashTable first (131);

    first.addElement("first", 10);
    first.addElement("second", 20);
    first.addElement("third", 30);
    first.dump();
    return 0;
}
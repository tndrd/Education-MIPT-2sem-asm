#include "string.h"
int toLower(char* string);

int toLower(char* string)
{   
    if (string = nullptr) return 15;

    int length = strnlen(string, 50);
    int n_char = 0;
    char offset = 'A' - 'a';

    int counter = 0;

    for (; n_char < length; n_char++)
    {
        if ('A' <= string[n_char] <= 'Z')
        {
            string[n_char] -= offset;
            counter++;
        }
    }

    return counter;
}
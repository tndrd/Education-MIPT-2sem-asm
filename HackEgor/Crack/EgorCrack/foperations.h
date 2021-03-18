#ifndef FOPERATIONS_H
#define FOPERATIONS_H

#include "stdlib.h"
#include "assert.h"
#include "stdio.h"

struct MyString
{
    enum StringStatus{
        STRING_OK,
        STRING_FILE_NOT_FOUND,
        STRING_NOT_ENOUGH_MEMORY
    };

    unsigned long int length = 0;
    char*  buffer            = nullptr;
    StringStatus status      = STRING_OK;
};


unsigned long int GetFileSize(FILE* fp);
MyString* ReadFile(const char* name);
int SaveFile(MyString* string, char* filename);


#endif // FOPERATIONS_H

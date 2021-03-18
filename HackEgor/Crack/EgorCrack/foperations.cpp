#include "foperations.h"

MyString* ReadFile(const char* name){

    assert(name);

    MyString* string = (MyString*)calloc(1, sizeof(MyString));

    assert(string);

    if (!string) return nullptr;

    FILE* fp = fopen(name, "r");

    if (!fp)
    {
        string -> status = MyString::STRING_FILE_NOT_FOUND;
        return string;
    }

    string -> length = GetFileSize(fp);
    string -> buffer = (char*)calloc(string -> length + 1, sizeof(char));

    if (!(string -> buffer))
    {
        string -> status = MyString::STRING_NOT_ENOUGH_MEMORY;
        return string;
    }

    fread(string -> buffer, sizeof(char), string -> length, fp);

    fclose(fp);
    return string;
}

int SaveFile(MyString* string, char* filename)
{

    assert(string);
    assert(filename);

    FILE* fp = fopen(filename, "w");

    if (!fp){
        return 1;
    }

    for (unsigned long int n_char = 0; n_char < string -> length; n_char++)
    {
        fprintf(fp, "%c", (string -> buffer)[n_char]);
    }

    fclose(fp);
    return 0;
}

unsigned long int GetFileSize(FILE* fp){

    int seekResult = fseek(fp, 0L, SEEK_END);
    assert(seekResult == 0);
    long int filesize = ftell(fp);
    rewind(fp);
    return filesize;
}

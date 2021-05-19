#include "foperations.h"

long int GetFileSize(FILE* fp){

    int seekResult = fseek(fp, 0L, SEEK_END);
    assert(seekResult == 0);
    long int filesize = ftell(fp);
    rewind(fp);
    return filesize;
}

char* ReadFile(const char* name, long int* filesize_ptr){

    assert(name);

    FILE* fp = fopen(name, "r");
    char* buffer = nullptr;

    if (fp==nullptr){
        printf("Error: unable to open file\n");
        exit(1);
    }

    long int filesize = GetFileSize(fp);
    *filesize_ptr = filesize;
    buffer = (char*)realloc(buffer, filesize + 1);
    int read = fread(buffer + 1, sizeof(char), filesize, fp);
    buffer = buffer + 1;
    fclose(fp);
    return buffer;
}
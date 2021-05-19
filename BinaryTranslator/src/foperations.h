#ifndef __FOPERATIONS__
#define __FOPERATIONS__

#include "stdlib.h"
#include "stdio.h"
#include "assert.h"

long int GetFileSize(FILE* fp);
char*    ReadFile(const char* name, long int* filesize_ptr);

#endif
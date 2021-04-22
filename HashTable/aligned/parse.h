#ifndef __PARSE__
#define __PARSE__

#include "stdlib.h"
#include "stdio.h"
#include "assert.h"
#include "math.h"
#include "assert.h"
#include "coloring.h"
#include <exception>
#include <iostream>
#include "string.h"
#include "../../../Education-MIPT/Onegin/onegin.h"
#include "config.h"
#include "list.h"

struct Word
{
    char*  buff     = nullptr;
    size_t n_blocks = 0;
};

struct WordList
{
    Word* words = nullptr;
    size_t quantity = 0;
};

size_t countChars(char* buffer, char to_search);
WordList parseCSV(char* buffer);
WordList parseKeys_ACSV(char* keys);
#endif
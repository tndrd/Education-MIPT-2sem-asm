#ifndef __PARSE__
#define __PARSE__

#include "stdlib.h"
#include "stdio.h"
#include "assert.h"
#include "math.h"
#include "assert.h"
#include <exception>
#include <iostream>
#include "string.h"
#include "../../../Education-MIPT/Onegin/onegin.h"
#include "config.h"

struct Word
{
    const __m128i*  buff     = nullptr;
    size_t       n_blocks = 0;
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
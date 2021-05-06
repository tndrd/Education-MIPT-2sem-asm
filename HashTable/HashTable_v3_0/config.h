#ifndef __HT_CONF__
#define __HT_CONF__

typedef char* Type;
typedef u_int64_t hash_t;

#include "list.h"

const size_t INITIAL_HASHTABLE_CAPACITY = 100;
const size_t MAX_KEY_LENGTH = 32;

const char CSV_SEPARATOR = ':';
const char CSV_NEWLINE   = '\n';

const size_t HT_MEM_STACK_INITIAL_CAPACITY = 3;

#endif
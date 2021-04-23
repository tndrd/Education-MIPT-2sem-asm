#ifndef __HT_CONF__
#define __HT_CONF__

typedef char* Type;
typedef u_int32_t hash_t;

const char CSV_SEPARATOR = '=';
const char CSV_NEWLINE   = '\n';

const size_t INITIAL_HASHTABLE_CAPACITY = 3;
const size_t MAX_KEY_LENGTH = 32;

#endif
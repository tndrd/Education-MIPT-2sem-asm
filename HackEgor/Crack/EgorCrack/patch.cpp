#include "patch.h"

unsigned long int HashSum(MyString* code);

const unsigned int      JMP_OP_ADDR   = 0x24;
const unsigned char     NEW_JMP_OFF   = 0x7E;
const long unsigned int COM_FILE_SIZE = 333;
const long unsigned int COM_FILE_HASH = 0x60F8EC6828EC9E8E;
const unsigned char     HASH_SEED     = 3;
const unsigned char     HASH_MLTR     = 5;
const unsigned char     HASH_SALT     = 2;

PATCH_RESULTS DoPatch(MyString* code)
{
    assert(code);
    assert(code -> buffer);

    if (code -> length != COM_FILE_SIZE)
        return PATCH_WRONG_FILESIZE;
    if (HashSum(code)  != COM_FILE_HASH)
        return PATCH_WRONG_HASH;

    // Patching
    (code -> buffer)[JMP_OP_ADDR + 1] = NEW_JMP_OFF;

    return PATCH_OK;
}

unsigned long int HashSum(MyString* code)
{
    assert(code);
    assert(code -> buffer);

    long unsigned int hashSum = HASH_SEED;

    for (unsigned long int n_char = 0; n_char < code -> length; n_char++){
        hashSum = (hashSum * HASH_MLTR) + (code -> buffer)[n_char] + HASH_SALT;
    }
    printf("hash: { %lX }\n", hashSum);
    return hashSum;
}

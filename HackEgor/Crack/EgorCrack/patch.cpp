#include "patch.h"

unsigned long int HashSum(MyString* code);

const unsigned int      NOP_START_ADDR = 0xF;
const unsigned char     NOP_QUANTITY   = 3;
const unsigned char     NOP_OPCODE     = 0x90;
const long unsigned int COM_FILE_SIZE  = 0x14F;
const long unsigned int COM_FILE_HASH  = 0x630B9F652226C859;
const unsigned char     HASH_SEED      = 3;
const unsigned char     HASH_MLTR      = 5;
const unsigned char     HASH_SALT      = 2;

PATCH_RESULTS DoPatch(MyString* code)
{
    assert(code);
    assert(code -> buffer);

    long unsigned int codeHash = HashSum(code);

    printf("Trying to patch file:\n  hash: %lX\n  length: %lX\n", codeHash, code->length);
    if (code -> length != COM_FILE_SIZE)
        return PATCH_WRONG_FILESIZE;
    if (codeHash  != COM_FILE_HASH)
        return PATCH_WRONG_HASH;

    // Patching
    for (int n_byte = 0; n_byte < NOP_QUANTITY; n_byte++)
        (code -> buffer)[NOP_START_ADDR + n_byte] = NOP_OPCODE;

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

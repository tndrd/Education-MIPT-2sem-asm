#include "stdlib.h"
#include "stdio.h" 
#include "assert.h"

#include "../lib/fileoperations.h"

const int   jmp_op_addr = 0x24;
const char  new_jmp_off = 0x7E;

int DoPatch();
{
    MyString* code = ReadFile("PASSWORD.COM");
    (code -> buffer)[jmp_op_addr + 1] = new_jmp_off;
    SaveFile(code, "patched.com");
    return 0;
}
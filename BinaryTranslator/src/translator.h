#ifndef __TRANSLATOR__
#define __TRANSLATOR__

#include "stdlib.h"
#include "stdio.h"
#include "string.h"

enum TRANSLATION_KEYS
{
    DISASSEMBLE = 0x01,
    TO_BINARY   = 0x02,
    TO_NASM     = 0x04,
};

const char RAM_BIT_MASK      = 0b10000000;
const char REGISTER_BIT_MASK = 0b01000000;
const char CONST_BIT_MASK    = 0b00100000;
const char COMMAND_NUM_MASK = 0b00011111;

const char TRANSLATION_MODE = DISASSEMBLE + TO_NASM;
#define MEMSPACE_LABEL "offset MemSpace"

const unsigned char OPCODE_SIZE = 1;
const unsigned char REGTAG_SIZE = 1;
const unsigned char CONST_SIZE  = sizeof(double); 
const unsigned int  HEADER_SIZE = 8;

void push2nasm(char** in_cursor, char** out_cursor);

#endif

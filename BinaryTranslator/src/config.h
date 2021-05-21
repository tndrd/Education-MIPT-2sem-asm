#ifndef __CONFIG__
#define __CONFIG__

const size_t IMM_STACK_INITIAL_CAPACITY = 10;
const size_t MAX_FILE_SIZE = 10000;


const char RAM_BIT_MASK      = 0b10000000;
const char REGISTER_BIT_MASK = 0b01000000;
const char CONST_BIT_MASK    = 0b00100000;
const char COMMAND_NUM_MASK  = 0b00011111;

const unsigned char OPCODE_SIZE = 1;
const unsigned char REGTAG_SIZE = 1;
const unsigned char CONST_SIZE  = sizeof(double); 
const unsigned int  HEADER_SIZE = 8;
const unsigned int  LABEL_SIZE  = sizeof(int); 

#endif
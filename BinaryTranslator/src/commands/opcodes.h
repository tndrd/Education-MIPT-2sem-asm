const char* PUSH_IMM  = "\xFF\x34\x25";

/*
const char* PUSH_RBX  = "\x53";
const char* PUSH_RCX  = "\x51";
const char* PUSH_RDX  = "\x52";
const char* PUSH_RSI  = "\x56";
 
const char* POP_RBX   = "\x5B";
const char* POP_RCX   = "\x59";
const char* POP_RDX   = "\x5A";
const char* POP_RSI   = "\x5E";
*/


const char* PUSH_R10  = "\x41\x52";
const char* PUSH_R11  = "\x41\x53";
const char* PUSH_R12  = "\x41\x54";
const char* PUSH_R13  = "\x41\x55";
 
const char* POP_R10   = "\x41\x5A";
const char* POP_R11   = "\x41\x5B";
const char* POP_R12   = "\x41\x5C";
const char* POP_R13   = "\x41\x5D";



const char* OP_FLD     = "\xDD\x04\x24";
const char* OP_FST     = "\xDD\x1C\x24";
const char* OP_SUB     = "\x48\x83\xEC\x08";
const char* OP_ADD     = "\x48\x83\xC4\x08";
const char* OP_FCOMPP  = "\xDE\xD9";
const char* OP_FSTSW   = "\x9B\xDF\xE0";
const char* OP_SAHF    = "\x9E";
const char* OP_RETURN  = "\xC3";

const char* OP_FADD    = "\xDE\xC1";
const char* OP_FSUB    = "\xDE\xE9";
const char* OP_FMUL    = "\xDE\xC9";
const char* OP_FDIV    = "\xDE\xF9";
const char* OP_FCHS    = "\xD9\xE0";
const char* OP_FSQRT   = "\xD9\xFA";

const char* OP_JMP     = "\xE9";
const char* OP_CALL    = "\xE8";
const char* OP_JA      = "\x0F\x87";
const char* OP_JB      = "\x0F\x82";
const char* OP_JAE     = "\x0F\x83";
const char* OP_JBE     = "\x0F\x86";

const char* OP_JE      = "\x0F\x84";
const char* OP_JNE     = "\x0F\x85";

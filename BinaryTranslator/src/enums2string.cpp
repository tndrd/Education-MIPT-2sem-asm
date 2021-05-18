#include "enums2string.h"

const char* getEnum_Operation(Operation operation)
{
    #define ENUM_CASE(enum_name) case enum_name: return #enum_name;

    switch(operation)
    {
        ENUM_CASE(OP_EMPTY);
        ENUM_CASE(PUSH);
        ENUM_CASE(POP);
        ENUM_CASE(JMP);
        ENUM_CASE(ARITHMETIC);
        ENUM_CASE(STDLIB);
        ENUM_CASE(RETURN);
        ENUM_CASE(CALL);
        ENUM_CASE(COMPARE);

        default: return "UNKNOWN";
    }
     
}

const char* getEnum_RegName(RegName reg)
{
    #define ENUM_CASE(enum_name) case enum_name: return #enum_name;

    switch(reg)
    {
        ENUM_CASE(RAX);
        ENUM_CASE(RBX);
        ENUM_CASE(RCX);
        ENUM_CASE(RDX);
        ENUM_CASE(REG_NONE);
        default: return "UNKNOWN";
    }
     
}

const char* getEnum_ArithmeticOperation(ArithmeticOperation operation)
{
    #define ENUM_CASE(enum_name) case enum_name: return #enum_name;

    switch(operation)
    {
        ENUM_CASE(ADD);
        ENUM_CASE(SUB);
        ENUM_CASE(MUL);
        ENUM_CASE(DIV);
        default: return "UNKNOWN";
    }
     
}

const char* getEnum_JumpOperation(JumpOperation operation)
{
    #define ENUM_CASE(enum_name) case enum_name: return #enum_name;

    switch(operation)
    {
        ENUM_CASE(JA);
        ENUM_CASE(JAE);
        ENUM_CASE(JB);
        ENUM_CASE(JBE);
        ENUM_CASE(JE);
        ENUM_CASE(JNE);
        default: return "UNKNOWN";
    }
     
}

const char* GetOperationName(Operation op_type, OperationName name)
{
    switch(op_type)
    {
        case ARITHMETIC: return getEnum_ArithmeticOperation(name.ari_op);
        case JMP       : return getEnum_JumpOperation(name.jmp_op);
        default:         return getEnum_Operation(op_type);
    }
}
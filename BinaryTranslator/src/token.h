#ifndef __TOKEN__
#define __TOKEN__

#include "stdlib.h"
#include "stdio.h"
#include "string.h"

enum Operation
{
    OP_EMPTY,
    PUSH,
    POP,
    JMP,
    ARITHMETIC,
    STDLIB,
    RETURN,
    CALL,
    COMPARE
};

enum RegName
{
    REG_NONE = 0,
    RAX,
    RBX,
    RCX,
    RDX
};

enum ArithmeticOperation
{
    ADD,
    SUB,
    MUL,
    DIV
};

enum JumpOperation
{
    JA,
    JAE,
    JB,
    JBE,
    JE,
    JNE
};

enum GeneralOperation
{
    NO_SUBNAME
};

enum OperandType
{
    OPERAND_EMPTY = 0,
    r64,
    m64,
    imm64
};

union OperationName
{
    GeneralOperation    gen_op = NO_SUBNAME;
    ArithmeticOperation ari_op;
    JumpOperation       jmp_op;
};

#include "enums2string.h"

struct Operand
{
    OperandType type = OPERAND_EMPTY;

    union
    {
        double     cst = 0; // for double constants
        u_int64_t mem;     // for jmp offsets and memory adresses
        RegName    reg;     // for register operands
    };
};

struct Token
{
    Operation op_type = OP_EMPTY;

    OperationName op_name;

    Operand a = {};
    Operand b = {};

    Token* next = nullptr;
    Token* prev = nullptr;
};

Token* newToken(Operation op_type, OperationName op_name, Token* next, Token* prev);

void DeleteToken(Token* token);

Token* assignOperands(Token* thou, Operand* a, Operand* b);

Operand* AssignRegOperand(Operand* operand, RegName reg_name);

Operand* AssignCstOperand(Operand* operand, double val);

Operand* AssignAdrOperand(Operand* operand, u_int64_t val);

Operand* AssignEmpOperand(Operand* operand);

Token* printToken(Token* token);

Operand* printOperand(FILE* fp, Operand* operand);

#endif
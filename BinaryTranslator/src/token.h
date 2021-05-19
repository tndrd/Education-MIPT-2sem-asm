#ifndef __TOKEN__
#define __TOKEN__

#include "stdlib.h"
#include "stdio.h"
#include "string.h"

enum Operation
{
    OP_EMPTY,
    FLD,
    FST,
    PUSH,
    MOV,
    POP,
    JUMP,
    IN,
    OUT,
    HLT,
    FCOMPP,
    FSTSW,
    SAHF,
    ARITHMETIC,
    STDLIB,
    RETURN,
    COMPARE
};

enum RegName
{
    REG_NONE = 0,
    RAX = 'a',
    RBX = 'b',
    RCX = 'c',
    RDX = 'd',
    AX  = 'A'
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
    JMP,
    CALL,
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
    SPEC_NAME,
    TOKEN_REF,
    r64,
    m64,
    imm64
};

enum SpecName
{
    VALUE_BUFFER
};

union OperationName
{
    GeneralOperation    gen_op = NO_SUBNAME;
    ArithmeticOperation ari_op;
    JumpOperation       jmp_op;
};

#include "enums2string.h"

struct Token;

struct Operand
{
    OperandType type = OPERAND_EMPTY;

    union
    {
        double        cst = 0;   // for double constants
        u_int64_t     mem;       // for jmp offsets and memory adresses
        RegName       reg;       // for register operands
        SpecName      name; 
        unsigned char label;     // for token references
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

Token* newToken(Operation op_type = OP_EMPTY, OperationName op_name = {NO_SUBNAME}, Token* next = nullptr, Token* prev = nullptr);

void DeleteToken(Token* token);

Token* assignOperands(Token* thou, Operand* a, Operand* b = nullptr);

Operand* AssignRegOperand(Operand* operand, RegName reg_name);

Operand* AssignCstOperand(Operand* operand, double val);

Operand* AssignAdrOperand(Operand* operand, u_int64_t val);

Operand* AssignSpecOperand(Operand* operand, SpecName name);

Operand* AssignEmpOperand(Operand* operand);

Token* printToken(Token* token);

Operand* printOperand(FILE* fp, Operand* operand);

Operand* AssignLabelOperand(Operand* operand, unsigned char label);
#endif
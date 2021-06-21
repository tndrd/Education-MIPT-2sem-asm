#ifndef __TOKEN__
#define __TOKEN__

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "assert.h"
#include "config.h"

enum Operation
{
    OP_EMPTY,
    FLD,
    FST,
    PUSH,
    MOV,
    POP,
    SUB,
    JUMP,
    ADD,
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
    AX  = 1,
    RAX = 2,
    
    
    RBX = 4,
    RCX = 5,
    RDX = 6,
    RSI = 7,
    
    R10 = 'a',
    R11 = 'b',
    R12 = 'c',
    R13 = 'd',
    
    RSP = 3
};

enum ArithmeticOperation
{
    FADD,
    FSUB,
    FMUL,
    FDIV,
    FCHS,
    FSQRT
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
    reg2mem64,
    int64,
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
        unsigned int  imm = 0;   // for double constants
        u_int64_t     int64;
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

    Operand operand_a = {};
    Operand operand_b = {};

    Token* next = nullptr;
    Token* prev = nullptr;

    char is_label = 0;
    unsigned char n_label = 0;
};

struct ImmediateStack
{
    double* buffer = nullptr;
    size_t size = 0;
    size_t capacity = 0;
};

Token* newToken(Operation op_type = OP_EMPTY, OperationName op_name = {NO_SUBNAME}, Token* next = nullptr, Token* prev = nullptr);

void DeleteToken(Token* token);

Token* assignOperands(Token* thou, Operand* operand_a, Operand* operand_b = nullptr);

Operand* AssignRegOperand(Operand* operand, RegName reg_name);

Operand* AssignCstOperand(ImmediateStack* imms, Operand* operand, double val);

Operand* AssignAdrOperand(Operand* operand, u_int64_t val);

Operand* AssignSpecOperand(Operand* operand, SpecName name);

Operand* AssignEmpOperand(Operand* operand);

Token* printToken(Token* token);

Operand* printOperand(FILE* fp, Operand* operand);

Operand* AssignLabelOperand(Operand* operand, unsigned char label);


Operand* AssignRegToMemOperand(Operand* operand, RegName reg);
Operand* AssignIntgOperand(Operand* operand, u_int64_t val);

void resize(ImmediateStack* stack);
size_t push(ImmediateStack* stack, double value);
double pop(ImmediateStack* stack);
void SetImmStack(ImmediateStack* stack);

#endif
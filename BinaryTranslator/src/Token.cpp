#include "token.h"

Token* newToken(Operation op_type, OperationName op_name, Token* next, Token* prev)
{
    Token* new_token = (Token*)calloc(1, sizeof(Token));
    if (!new_token) return nullptr;

    new_token -> op_type = op_type;
    new_token -> op_name = op_name;
    new_token -> next    = next;
    new_token -> prev    = prev;
    return new_token; 
}

void DeleteToken(Token* token)
{
    free(token);
}

Token* assignOperands(Token* thou, Operand* a, Operand* b)
{
    if (!thou || !a) return nullptr;

    if (b)
        thou -> b = *b;
    thou -> a = *a;
    

    return thou;
}

Operand* AssignRegOperand(Operand* operand, RegName reg_name)
{
    if (!operand) return nullptr;
    
    operand -> type = r64;
    operand -> reg  = reg_name;
    return operand;
}


Operand* AssignCstOperand(Operand* operand, double val)
{
    if (!operand) return nullptr;
    
    operand -> type  = imm64;
    operand -> cst   = val;
    return operand;
}


Operand* AssignAdrOperand(Operand* operand, u_int64_t val)
{
    if (!operand) return nullptr;
    
    operand -> type = m64;
    operand -> mem  = val;
    return operand;
}

Operand* AssignEmpOperand(Operand* operand)
{
    if (!operand) return nullptr;
    
    operand -> type = OPERAND_EMPTY;
    return operand;
}

Operand* AssignSpecOperand(Operand* operand, SpecName name)
{
    if (!operand) return nullptr;
    
    operand -> type = SPEC_NAME;
    operand -> name = name;
    return operand;
}

Operand* AssignLabelOperand(Operand* operand, unsigned char label)
{
    if (!operand) return nullptr;

    operand -> type      = TOKEN_REF;
    operand -> label     = label; 
    return operand;
}

Token* printToken(Token* token)
{
    if (!token) return nullptr;

    printf("%s ", GetOperationName(token -> op_type, token -> op_name));
    
    if ((token -> a).type) printOperand(stdout, &(token -> a));
    if ((token -> b).type)
    {
        printf(", ");
        printOperand(stdout, &(token -> b));
    }
    
    printf("\n");

    return token;
}

Operand* printOperand(FILE* fp, Operand* operand)
{
    if (!operand) return nullptr;

    //printf("fucking ");

    switch (operand -> type)
    {
        case OPERAND_EMPTY: fprintf(fp, "NONE"); break;
        case SPEC_NAME    : fprintf(fp, "%s",  getEnum_SpecName(operand -> name)); break;
        case r64          : fprintf(fp, "%s",  getEnum_RegName(operand -> reg)); break;
        case m64          : fprintf(fp, "[%x]",  operand -> mem); break;
        case imm64        : fprintf(fp, "%lf", operand -> cst); break;
        case TOKEN_REF    : fprintf(fp, "label_%d", operand -> label); break;
    }

    return operand;
}
#include "token.h"

Token* newToken(Operation op_type, OperationName op_name, Token* next, Token* prev)
{
    Token* new_token = (Token*)calloc(1, sizeof(Token));
    if (!new_token) return nullptr;

    new_token -> op_type = op_type;
    new_token -> op_name = op_name;
    new_token -> next    = next;
    new_token -> prev    = prev;

    new_token -> is_label = 0;
    new_token -> n_label  = 0;
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


Operand* AssignCstOperand(ImmediateStack* imms, Operand* operand, double val)
{
    if (!operand) return nullptr;
    
    operand -> type  = imm64;
    operand -> imm   = push(imms, val);
    return operand;
}


Operand* AssignAdrOperand(Operand* operand, u_int64_t val)
{
    if (!operand) return nullptr;
    
    operand -> type = m64;
    operand -> mem  = val;
    return operand;
}

Operand* AssignRegToMemOperand(Operand* operand, RegName reg)
{
    if (!operand) return nullptr;
    
    operand -> type = reg2mem64;
    operand -> reg  = reg;
    return operand;
}

Operand* AssignIntgOperand(Operand* operand, u_int64_t val)
{
    if (!operand) return nullptr;
    
    operand -> type  = int64;
    operand -> int64 = val;
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
        case imm64        : fprintf(fp, "imm%d", operand -> imm); break;
        case int64        : fprintf(fp, "%d", operand -> int64); break;
        case TOKEN_REF    : fprintf(fp, "label_%d", operand -> label); break;
        case reg2mem64    : fprintf(fp, "[%s]",  getEnum_RegName(operand -> reg)); break;
    }

    return operand;
}

void resize(ImmediateStack* stack)
{
    assert(stack);
    stack -> buffer = (double*)realloc(stack -> buffer, stack -> capacity * 2 * sizeof(double));
    assert(stack -> buffer);
}


size_t push(ImmediateStack* stack, double value)
{
    assert(stack);
    
    if (stack -> size == stack -> capacity) resize(stack);

    (stack -> buffer)[stack -> size ++] = value;

    return stack -> size - 1;
}

double pop(ImmediateStack* stack)
{
    assert(stack -> size > 0);

    return (stack -> buffer)[-- stack -> size];
}

void SetImmStack(ImmediateStack* stack)
{
    stack -> size     = 0;
    stack -> capacity = IMM_STACK_INITIAL_CAPACITY;
    stack -> buffer   = (double*)calloc(IMM_STACK_INITIAL_CAPACITY, sizeof(double));
}
#include "translation.h"

void Translate(TokenList* tlist, char* buffer)
{
    
    char* cursor = buffer;
    
    for (Token* current_token = tlist -> tail; current_token; current_token = current_token -> next)
    {
        TranslateOperation(current_token, &cursor);
        
        Operand* a = &(current_token -> a);
        Operand* b = &(current_token -> b);

        if (a -> type)
        {
            sprintf(cursor, " "); cursor+=1;
            TranslateOperand(a, &cursor);

            if (b -> type)
            {
                sprintf(cursor, ", "); cursor+=2;
                TranslateOperand(b, &cursor);
            }

        }

        sprintf(cursor, "\n"); cursor += 1;  
    }

}

void TranslateOperand(Operand* operand, char** cursor)
{
    assert(operand);
    assert(cursor);

    switch (operand -> type)
    {
        case OPERAND_EMPTY:                                              return;
        case r64:           TranslateRegister (operand -> reg,  cursor); return;
        case imm64:         TranslateImmediate(operand -> cst,  cursor); return;
        case SPEC_NAME:     TranslateSpecial  (operand -> name, cursor); return;

        default:            printf(RED_CLR "Unknown operand: " END_CLR);
                            printOperand(stdout, operand); printf("\n"); return;
    }
}

void TranslateRegister(RegName reg, char** cursor)
{
    assert(cursor);

    size_t offset = 0;

    switch (reg)
    {
        #define REGISTER(name, code, translation) \
        case name: sprintf(*cursor, "%s%n", translation, &offset); break;

        #define OPERATION(name, translation) ;
        #define ARITHMETIC_OP(name, translation) ;
        #define JUMP_OP(name, translation) ;

        #include "commands/asm_commands.h"

        #undef REGISTER
        #undef OPERATION
        #undef ARITHMETIC_OP
        #undef JUMP_OP
    }

    (*cursor) += offset;
}

void TranslateImmediate(double cst, char** cursor)
{
    assert(cursor);

    size_t offset = 0;

    sprintf(*cursor, "%lf%n", cst, &offset);

    (*cursor) += offset;
}

void TranslateSpecial(SpecName name, char** cursor)
{
    assert(cursor);

    size_t offset = 0;

    switch (name)
    {
        case VALUE_BUFFER: sprintf(*cursor, "qword [value_buffer]%n", &offset);
    }

    (*cursor) += offset;
}

void TranslateOperation(Token* token, char** cursor)
{
    assert(token);
    assert(cursor);

    size_t offset = 0;

    switch (token -> op_type)
    {
        #define OPERATION(name, translation) \
        case name: sprintf(*cursor, "%s%n", translation, &offset); break;
        
        #define ARITHMETIC_OP(name, translation)       ;
        #define REGISTER(name, code, translation) ;
        #define JUMP_OP(name, translation)       ;

        #include "commands/asm_commands.h"

        #undef ARITHMETIC_OP
        #undef REGISTER
        #undef JUMP_OP
        #undef OPERATION

        case ARITHMETIC: TranslateArithmetic((token -> op_name).ari_op, cursor); break;
        case JUMP:       TranslateJump      ((token -> op_name).jmp_op, cursor); break;

        default:            printf(RED_CLR "Unknown operand: " END_CLR);
                            printf("%s\n", GetOperationName(token -> op_type, token -> op_name)); break;       break;
    }

    (*cursor) += offset;
}

void TranslateArithmetic(ArithmeticOperation operation, char** cursor)
{
    assert(cursor);

    size_t offset = 0;

    switch (operation)
    {
        #define ARITHMETIC_OP(name, translation) \
        case name: sprintf(*cursor, "%s%n", translation, &offset); break;
        
        #define OPERATION(name, translation)       ;
        #define REGISTER(name, code, translation) ;
        #define JUMP_OP(name, translation)       ;

        #include "commands/asm_commands.h"

        #undef ARITHMETIC_OP
        #undef REGISTER
        #undef JUMP_OP
        #undef OPERATION
    }

    (*cursor) += offset;
}

void TranslateJump(JumpOperation operation, char** cursor)
{
    assert(cursor);

    size_t offset = 0;

    switch (operation)
    {
        #define JUMP_OP(name, translation) \
        case name: sprintf(*cursor, "%s%n", translation, &offset); break;

        #define ARITHMETIC_OP(name, translation)       ;
        #define OPERATION(name, translation)       ;
        #define REGISTER(name, code, translation) ;

        #include "commands/asm_commands.h"

        #undef ARITHMETIC_OP
        #undef REGISTER
        #undef JUMP_OP
        #undef OPERATION
    }

    (*cursor) += offset;
}
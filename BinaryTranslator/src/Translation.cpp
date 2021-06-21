#include "translation.h"

void Translate(TokenList* tlist, char* buffer)
{

    char* cursor = buffer;

    PutHeader(tlist, &cursor);

    for (Token* current_token = tlist -> tail; current_token; current_token = current_token -> next)
    {
        
        if (current_token -> is_label)
        {
            size_t offset = 0;
            sprintf(cursor, "\nlabel_%d:\n%n", current_token -> n_label, &offset);
            cursor += offset;
        }
        
        TranslateOperation(current_token, &cursor);
        
        Operand* operand_a = &(current_token -> operand_a);
        Operand* operand_b = &(current_token -> operand_b);

        if (operand_a -> type)
        {
            sprintf(cursor, " "); cursor+=1;
            TranslateOperand(operand_a, &cursor);

            if (operand_b -> type)
            {
                sprintf(cursor, ", "); cursor+=2;
                TranslateOperand(operand_b, &cursor);
            }

        }

        sprintf(cursor, "\n"); cursor += 1;  
    }

}

void PutHeader(TokenList* tlist, char** cursor)
{
    assert(cursor);

    long int offset = 0;
    char*  header = ReadFile("src/nasm_header.txt", &offset);
    
    assert(header);

    sprintf(*cursor, "%s%n", header, &offset);
    *cursor += offset;

    ShitImmsOut(tlist, cursor);

    sprintf(*cursor, "section .bss\nsection .text\n\n_start:%n", &offset);
    *cursor += offset;
}

void ShitImmsOut(TokenList* tlist, char** cursor)
{
    size_t offset = 0;

    int n_imm = (tlist -> imms).size - 1;
    while (n_imm >= 0)
    {
        sprintf(*cursor, "    imm%d dq %lf\n%n", n_imm, ((tlist -> imms).buffer)[n_imm], &offset);
        n_imm--;
        (*cursor+=offset);
    }

    sprintf(*cursor, "\n");
    (*cursor)++;
}

void TranslateOperand(Operand* operand, char** cursor)
{
    assert(operand);
    assert(cursor);

    size_t offset = 0;

    switch (operand -> type)
    {
        case OPERAND_EMPTY:                                              return;
        case r64:           TranslateRegister (operand -> reg,  cursor); return;
        case imm64:         TranslateImmediate(operand -> imm,  cursor); return;
        case SPEC_NAME:     TranslateSpecial  (operand -> name, cursor); return;
        
        case TOKEN_REF:     sprintf(*cursor, "label_%d%n", operand -> label, &offset);
                            (*cursor) += offset; return;
        
        case int64:         sprintf(*cursor, "%d%n", operand -> int64, &offset);
                            (*cursor) += offset; return;          

        case reg2mem64:     sprintf(*cursor, "qword ["); (*cursor)+=7;
                            TranslateRegister(operand -> reg, cursor); 
                            sprintf(*cursor,       "]"); (*cursor)++;
                            return;

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

void TranslateImmediate(unsigned int imm, char** cursor)
{
    assert(cursor);

    size_t offset = 0;

    sprintf(*cursor, "qword [imm%d]%n", imm, &offset);

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
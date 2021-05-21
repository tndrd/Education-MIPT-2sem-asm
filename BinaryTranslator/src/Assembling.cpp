#include "assembling.h"
#include "commands/opcodes.h"
#include "elf.h"

size_t Assemble(TokenList* tlist, char* buffer)
{
    tlist -> buffer = nullptr;
    char* cursor = buffer;
    WriteImms(tlist, &cursor);

    for (Token* current_token = tlist -> tail; current_token; current_token = current_token -> next)
    {
        if (current_token -> is_label)
        {
            (tlist -> label_list).label_offsets[current_token -> n_label] = cursor;
        }
        
        switch (current_token -> op_type)
        {
            #define OPERATION(name, translation) \
            case name: assemble ## name (tlist, current_token, &cursor); break;

            #define ARITHMETIC_OP(name, translation)       ;
            #define REGISTER(name, code, translation) ;
            #define JUMP_OP(name, translation)       ;

            #include "commands/asm_commands.h"

            #undef ARITHMETIC_OP
            #undef REGISTER
            #undef JUMP_OP
            #undef OPERATION

            case ARITHMETIC: assembleARITHMETIC (tlist, current_token, &cursor); break;
            case JUMP:       assembleJUMP       (tlist, current_token, &cursor); break;

            default:            printf(RED_CLR "Unknown operand: " END_CLR);
                                printf("%s\n", GetOperationName(current_token -> op_type, current_token -> op_name)); break;       break;
        }
    }

    return cursor - buffer;
}

u_int32_t GetImmAddress(TokenList* tlist, size_t n_imm)
{
    return 0x400080 + sizeof(OP_JMP) + sizeof(u_int32_t) - 4 + 8 * ((tlist -> imms).size - n_imm - 1);
}

void assemblePUSH(TokenList* tlist, Token* token, char** cursor)
{
    assert(token);
    assert(cursor);
    assert(token -> op_type == PUSH);

    Operand operand = token -> a;

    size_t offset = 0;

    switch(operand.type)
    {
        case imm64: {sprintf(*cursor, "%s%n", PUSH_IMM, &offset);
                    (*cursor) += offset;
                    u_int32_t imm = GetImmAddress(tlist, operand.imm);
                    
                    //char* imm_address = tlist -> buffer + imm;
                    
                    memcpy(*cursor, &imm, sizeof(imm));
                    (*cursor) += sizeof(imm); break;}

        case r64:   switch (operand.reg)
                    {
                        case R10: sprintf(*cursor, PUSH_R10); (*cursor)+=2; break;
                        case R11: sprintf(*cursor, PUSH_R11); (*cursor)+=2; break;
                        case R12: sprintf(*cursor, PUSH_R12); (*cursor)+=2; break;
                        case R13: sprintf(*cursor, PUSH_R13); (*cursor)+=2; break;
                        default:  printf(RED_CLR "PUSH error: %s is not suitable\n" END_CLR, getEnum_RegName(operand.reg));
                        break;
                    }   break;

        default: printOperand(stdout, &operand); printf(RED_CLR " push mode is not supported\n" END_CLR);
    }
}

void assemblePOP(TokenList* tlist, Token* token, char** cursor)
{
    assert(token);
    assert(cursor);
    assert(token -> op_type == POP);

    Operand operand = token -> a;

    size_t offset = 0;

    switch(operand.type)
    {
        case r64:   switch(operand.reg)
                    {
                        case R10: sprintf(*cursor, POP_R10); (*cursor)+=2; break;
                        case R11: sprintf(*cursor, POP_R11); (*cursor)+=2; break;
                        case R12: sprintf(*cursor, POP_R12); (*cursor)+=2; break;
                        case R13: sprintf(*cursor, POP_R13); (*cursor)+=2; break;
                        default:  printf(RED_CLR "POP error: %s is not suitable\n" END_CLR, getEnum_RegName(operand.reg));
                        break;
                    }   break;
        default: printOperand(stdout, &operand); printf(RED_CLR " pop mode is not supported\n" END_CLR);


    }
}

void assembleARITHMETIC(TokenList* tlist, Token* token, char** cursor)
{
    assert(token);                                                          
    assert(cursor);
    assert(token -> op_type == ARITHMETIC);

    switch ((token -> op_name).ari_op)
    {
        #define ARITHMETIC_OP(NAME, translation) \
        case NAME: assemble ## NAME(tlist, token, cursor); break;
        
        #define OPERATION(name, translation)       ;
        #define REGISTER(name, code, translation) ;
        #define JUMP_OP(name, translation)       ;

        #include "commands/asm_commands.h"

        #undef ARITHMETIC_OP
        #undef REGISTER
        #undef JUMP_OP
        #undef OPERATION

        default: printf(RED_CLR "%s arithmetic is not supported\n" END_CLR, GetOperationName(token -> op_type, token -> op_name));
    }
}

#define ASSEMBLE_INSTRUCTION_WITH_FIXED_OPCODE(OP_NAME)                     \
void assemble ## OP_NAME (TokenList* tlist, Token* token, char** cursor)    \
{                                                                           \
    assert(cursor);                                                         \
                                                                            \
    size_t offset = 0;                                                      \
                                                                            \
    sprintf(*cursor, "%s%n", OP_ ## OP_NAME, &offset);                      \
    (*cursor) += offset;                                                    \
}                                                                           \

ASSEMBLE_INSTRUCTION_WITH_FIXED_OPCODE(FLD);
ASSEMBLE_INSTRUCTION_WITH_FIXED_OPCODE(FST);
ASSEMBLE_INSTRUCTION_WITH_FIXED_OPCODE(FCOMPP);
ASSEMBLE_INSTRUCTION_WITH_FIXED_OPCODE(FSTSW);
ASSEMBLE_INSTRUCTION_WITH_FIXED_OPCODE(SAHF);
ASSEMBLE_INSTRUCTION_WITH_FIXED_OPCODE(RETURN);
ASSEMBLE_INSTRUCTION_WITH_FIXED_OPCODE(FADD);
ASSEMBLE_INSTRUCTION_WITH_FIXED_OPCODE(FSUB);
ASSEMBLE_INSTRUCTION_WITH_FIXED_OPCODE(FMUL);
ASSEMBLE_INSTRUCTION_WITH_FIXED_OPCODE(FDIV);
ASSEMBLE_INSTRUCTION_WITH_FIXED_OPCODE(FCHS);
ASSEMBLE_INSTRUCTION_WITH_FIXED_OPCODE(FSQRT);
ASSEMBLE_INSTRUCTION_WITH_FIXED_OPCODE(SUB);
ASSEMBLE_INSTRUCTION_WITH_FIXED_OPCODE(ADD);

ASSEMBLE_INSTRUCTION_WITH_FIXED_OPCODE(JMP);
ASSEMBLE_INSTRUCTION_WITH_FIXED_OPCODE(CALL);
ASSEMBLE_INSTRUCTION_WITH_FIXED_OPCODE(JA);
ASSEMBLE_INSTRUCTION_WITH_FIXED_OPCODE(JAE);
ASSEMBLE_INSTRUCTION_WITH_FIXED_OPCODE(JB);
ASSEMBLE_INSTRUCTION_WITH_FIXED_OPCODE(JBE);
ASSEMBLE_INSTRUCTION_WITH_FIXED_OPCODE(JE);
ASSEMBLE_INSTRUCTION_WITH_FIXED_OPCODE(JNE);

void assembleIN(TokenList* tlist, Token* token, char** cursor)
{
    assembleCALL(tlist, token, cursor);
    CalculateLabelOffset(((tlist -> label_list).stdlib)._in_offset_, cursor);
}

void assembleOUT(TokenList* tlist, Token* token, char** cursor)
{
    assembleCALL(tlist, token, cursor);
    CalculateLabelOffset(((tlist -> label_list).stdlib)._out_offset_, cursor);
}

void assembleHLT(TokenList* tlist, Token* token, char** cursor)
{
    assembleCALL(tlist, token, cursor);
    CalculateLabelOffset(((tlist -> label_list).stdlib)._hlt_offset_, cursor);
}

void assembleJUMP(TokenList* tlist, Token* token, char** cursor)
{
    assert(token);                                                          
    assert(cursor);
    assert(token -> op_type  == JUMP);
    assert((token -> a).type == TOKEN_REF);

    switch ((token -> op_name).jmp_op)
    {

        #define JUMP_OP(NAME, translation) \
        case NAME: assemble ## NAME(tlist, token, cursor); break;

        #define ARITHMETIC_OP(name, translation)       ;
        #define OPERATION(name, translation)       ;
        #define REGISTER(name, code, translation) ;

        #include "commands/asm_commands.h"

        #undef ARITHMETIC_OP
        #undef REGISTER
        #undef JUMP_OP
        #undef OPERATION

        default: printf(RED_CLR "%s jump is not supported\n" END_CLR, GetOperationName(token -> op_type, token -> op_name));
    }

    CalculateLabelOffset((tlist -> label_list).label_offsets[(token -> a).label], cursor);

}

void CalculateLabelOffset(char* label, char** cursor)
{
    int32_t label_offset = label - *cursor - 4;
    memcpy(*cursor, &label_offset, sizeof(int32_t));
    (*cursor) += sizeof(int32_t);
}
/*
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
*/

void WriteImms(TokenList* tlist, char** cursor)
{
    char* old_cursor = *cursor;
    assembleJMP(tlist, nullptr, cursor);

    u_int32_t* entry_offset = (u_int32_t*)(*cursor);

    (*cursor) += sizeof(u_int32_t);

    (*cursor)[0] = (*cursor)[1] = (*cursor)[2] = 0;

    (*cursor) +=  + 3;

    int n_imm = (tlist -> imms).size - 1; 

    while (n_imm >= 0)
    {
        double imm_val = ((tlist -> imms).buffer[n_imm--]);
        memcpy(*cursor, &imm_val, sizeof(imm_val));
        (*cursor) += sizeof(imm_val);
    }

    LoadStdlib(tlist, cursor);

    *entry_offset = *cursor - old_cursor - 5;
}

void LoadStdlib(TokenList* tlist, char** cursor)
{

    StdLib* stdlib = &((tlist -> label_list).stdlib);

    LoadFunc("src/stdlib/in",  &(stdlib -> _in_offset_),  cursor);
    LoadFunc("src/stdlib/out", &(stdlib -> _out_offset_), cursor);
    LoadFunc("src/stdlib/hlt", &(stdlib -> _hlt_offset_), cursor);
}

void LoadFunc(const char* filename, char** dst_addr, char** cursor)
{
    
    long int filesize = 0;
    char*    binary   = ReadFile(filename, &filesize);

    if (!binary)
    {
        printf(RED_CLR "Failed to load _stdlib_%s_\n" END_CLR, filename);
        return;
    }
    *dst_addr = *cursor;

    memcpy(*cursor, binary, filesize);

    (*cursor += filesize);
}
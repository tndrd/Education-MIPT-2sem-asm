#include "tokenization.h"

enum CPU_OPERATIONS
{
    #define DEF_CMD(name, code) CPU_ ## name = code,
    #include "commands/cpu_commands.h"
    #undef  DEF_CMD
};

size_t GetCodeStartOffset(const char* code)
{
    return HEADER_SIZE + code[HEADER_SIZE - 1] * LABEL_SIZE; 
}

TokenList* Tokenize(char* code, long int filesize)
{
    TokenList* tlist = (TokenList*)calloc(1, sizeof(TokenList));
    SetLabelList(&(tlist -> label_list), code);

    size_t code_offset = GetCodeStartOffset(code);
    
    char* cursor = code + code_offset;

    unsigned char current_label_idx = 0;

    while (cursor - code < filesize)
    {   
        Token* new_token  = nullptr;
        char*  old_cursor = cursor; 

        switch(*cursor & COMMAND_NUM_MASK)
        {
            #define DEF_CMD(NAME, CODE) case CPU_ ## NAME : new_token = tokenize ## NAME (tlist, &cursor); break;

            #include "commands/cpu_commands.h"

            #undef DEF_CMD
        }

        if (old_cursor - code_offset - code == (tlist -> label_list).in_labels[current_label_idx])
        {
            (tlist -> label_list).labels[current_label_idx++] = new_token;            
        }
        
    }

    return tlist;
}


Token* tokenizePUSH(TokenList* tlist, char** in_cursor)
{

    if (!tlist || !in_cursor || !(*(in_cursor))) return nullptr;

    char* current_command   = *in_cursor;

    const unsigned char opcode = *current_command;

    unsigned char RAM_BIT    = opcode & RAM_BIT_MASK;
    unsigned char REG_BIT    = opcode & REGISTER_BIT_MASK;
    unsigned char CONST_BIT  = opcode & CONST_BIT_MASK;
    
    unsigned char       in_offset  = 1;
    
    Operand operand_1 = {};
    Operand operand_2 = {};

    Token* push_token = newToken(FLD);
    AssignSpecOperand(&operand_1, VALUE_BUFFER);
    assignOperands(push_token, &operand_1);

    Token* val_token  = newToken();

    if (RAM_BIT)
    {
        fflush(stdout);
        printf("NO RAM!\n"); exit(0);
    }

    if (REG_BIT)
    {
       
       RegName regname = (RegName)(*(current_command + in_offset));
       
       val_token -> op_type = MOV;
       AssignSpecOperand(&operand_1, VALUE_BUFFER);
       AssignRegOperand(&operand_2, regname);
       assignOperands(val_token, &operand_1, &operand_2);

       in_offset += REGTAG_SIZE;
    }

    else if (CONST_BIT)
    {   
        double val = *((double*)(current_command + in_offset));

        val_token -> op_type = MOV;
       
       AssignSpecOperand(&operand_1, VALUE_BUFFER);
       AssignCstOperand(&operand_2, val);
       assignOperands(val_token, &operand_1, &operand_2);

        in_offset  += CONST_SIZE;
    }

    *in_cursor  = *in_cursor  + in_offset;

    append(tlist, val_token);
    append(tlist, push_token);
    
    return val_token;
}

Token* tokenizePOP(TokenList* tlist, char** in_cursor)
{

    if (!tlist || !in_cursor || !(*(in_cursor))) return nullptr;

    char* current_command   = *in_cursor;

    const unsigned char opcode = *current_command;

    unsigned char RAM_BIT    = opcode & RAM_BIT_MASK;
    unsigned char REG_BIT    = opcode & REGISTER_BIT_MASK;
    unsigned char CONST_BIT  = opcode & CONST_BIT_MASK;
    
    unsigned char       in_offset  = 1;
    
    Operand operand_1 = {};
    Operand operand_2 = {};

    if (RAM_BIT)
    {
        fflush(stdout);
        printf("NO RAM!\n"); exit(0);
    }

    if (CONST_BIT)
    {
        fflush(stdout);
        printf("POP DOESN't LIKE CONSTANTS!\n"); exit(0);
    }

    if (!REG_BIT)
    {
        fflush(stdout);
        printf("EMPTY POP!\n"); exit(0);
    }

    Token* pop_token = newToken(FST);
    AssignSpecOperand(&operand_1, VALUE_BUFFER);
    assignOperands(pop_token, &operand_1);
    
    RegName regname = (RegName)(*(current_command + in_offset));
    

    Token* val_token  = newToken();
    
    val_token -> op_type = MOV;
    AssignSpecOperand(&operand_2, VALUE_BUFFER);
    AssignRegOperand(&operand_1, regname);
    assignOperands(val_token, &operand_1, &operand_2);

    in_offset += REGTAG_SIZE;


    *in_cursor  = *in_cursor  + in_offset;

    append(tlist, pop_token);
    append(tlist, val_token);

    return pop_token;
}

Token* tokenizeIN(TokenList* tlist, char** in_cursor)
{
    if (!tlist || !in_cursor || !(*(in_cursor))) return nullptr;

    Token* new_token = newToken(IN);
    append(tlist, new_token);


    (*in_cursor)++;
    return new_token;
}

Token* tokenizeOUT(TokenList* tlist, char** in_cursor)
{
    if (!tlist || !in_cursor || !(*(in_cursor))) return nullptr;

    Token* new_token = newToken(OUT);
    append(tlist, new_token);

    (*in_cursor)++;
    return new_token;
}


Token* tokenizeHLT(TokenList* tlist, char** in_cursor)
{
    if (!tlist || !in_cursor || !(*(in_cursor))) return nullptr;

    Token* new_token = newToken(HLT);
    append(tlist, new_token);

    (*in_cursor)++;
    return new_token;
}


Token* tokenizeSUB(TokenList* tlist, char** in_cursor)
{
    if (!tlist || !in_cursor || !(*(in_cursor))) return nullptr;

    OperationName operation = {};
    operation.    ari_op    = FSUB;

    Token* new_token = newToken(ARITHMETIC, operation);
    append(tlist, new_token);

    (*in_cursor)++;
    return new_token;
}


Token* tokenizeADD(TokenList* tlist, char** in_cursor)
{
    if (!tlist || !in_cursor || !(*(in_cursor))) return nullptr;

    OperationName operation = {};
    operation.    ari_op    = FADD;

    Token* new_token = newToken(ARITHMETIC, operation);
    append(tlist, new_token);

    (*in_cursor)++;
    return new_token;
}


Token* tokenizeMUL(TokenList* tlist, char** in_cursor)
{
    if (!tlist || !in_cursor || !(*(in_cursor))) return nullptr;

    OperationName operation = {};
    operation.    ari_op    = FMUL;

    Token* new_token = newToken(ARITHMETIC, operation);
    append(tlist, new_token);

    (*in_cursor)++;
    return new_token;
}

Token* tokenizeDIV(TokenList* tlist, char** in_cursor)
{
    if (!tlist || !in_cursor || !(*(in_cursor))) return nullptr;

    OperationName operation = {};
    operation.    ari_op    = FDIV;

    Token* new_token = newToken(ARITHMETIC, operation);
    append(tlist, new_token);

    (*in_cursor)++;
    return new_token;
}

Token* tokenizeNEG(TokenList* tlist, char** in_cursor)
{
    if (!tlist || !in_cursor || !(*(in_cursor))) return nullptr;

    OperationName operation = {};
    operation.    ari_op    = FCHS;

    Token* new_token = newToken(ARITHMETIC, operation);
    append(tlist, new_token);

    (*in_cursor)++;
    return new_token;
}

Token* tokenizeJUMP(TokenList* tlist, char** in_cursor, JumpOperation jump_op)
{
    OperationName name = {};
    name.jmp_op        = jump_op;
    
    Token* new_token = newToken(JUMP, name);
    
    Operand label_op = {};

    unsigned char label  = GetLabel(&(tlist -> label_list), *(*(in_cursor) + 1));

    (*in_cursor) += 2;

    AssignLabelOperand(&label_op, label);
    assignOperands(new_token, &label_op);

    append(tlist, new_token);

    return new_token;
}

Token* tokenizeJMP(TokenList* tlist, char** in_cursor)
{
    return tokenizeJUMP(tlist, in_cursor, JMP);
}

Token* tokenizeCompare(TokenList* tlist)
{
    Token* fcompp = newToken(FCOMPP);
    Token* fstsw = newToken(FSTSW);
    Operand dest = {};
    AssignRegOperand(&dest, AX);
    assignOperands(fstsw, &dest);

    Token* sahf = newToken(SAHF);

    append(tlist, fcompp);
    append(tlist, fstsw);
    append(tlist, sahf);

    return fcompp;
}

Token* tokenizeCALL(TokenList* tlist, char** in_cursor)
{
    return tokenizeJUMP(tlist, in_cursor, CALL);
}

Token* tokenizeRETURN(TokenList* tlist, char** in_cursor)
{
    if (!tlist || !in_cursor || !(*(in_cursor))) return nullptr;
    Token* new_token = newToken(RETURN);
    append(tlist, new_token);

    (*in_cursor)++;
    return new_token;
}


#define TOKENIZE_CDTDL_JUMP_TEMPLATE(COMMAND)                   \
Token* tokenize ## COMMAND (TokenList* tlist, char** in_cursor) \
{                                                               \
    Token* compare = tokenizeCompare(tlist);                    \
    tokenizeJUMP(tlist, in_cursor, COMMAND);                    \
    return compare;                                             \
}                                                               \

TOKENIZE_CDTDL_JUMP_TEMPLATE(JA);
TOKENIZE_CDTDL_JUMP_TEMPLATE(JAE);
TOKENIZE_CDTDL_JUMP_TEMPLATE(JB);
TOKENIZE_CDTDL_JUMP_TEMPLATE(JBE);
TOKENIZE_CDTDL_JUMP_TEMPLATE(JE);
TOKENIZE_CDTDL_JUMP_TEMPLATE(JNE);
#include "translator.h"

char* CURRENT_COMMAND = nullptr;
char* RIP             = nullptr;

void push2nasm(char** in_cursor, char** out_cursor)
{

    #define END_TRANSLATION RAM_BIT=REG_BIT=CONST_BIT=0

    char* out_command_start = *out_cursor;
    char* current_command   = *in_cursor;

    const unsigned char opcode = *current_command;
    
    unsigned char RAM_BIT    = opcode & RAM_BIT_MASK;
    unsigned char REG_BIT    = opcode & REGISTER_BIT_MASK;
    unsigned char CONST_BIT  = opcode & CONST_BIT_MASK;
    
    unsigned char       in_offset  = 0;
    unsigned int        out_offset = 0;
    unsigned int        delta      = 0;

    in_offset++;
    
    if (!RAM_BIT && REG_BIT && CONST_BIT)
    {
        unsigned char reg_name  = *(current_command + 1);
        double        const_val = *(double*)(current_command + 2);
        sprintf(out_command_start, "add r%cx, %lf\npush r%cx%n", reg_name, const_val, reg_name, &delta);
        out_offset += delta;
        in_offset  += REGTAG_SIZE + CONST_SIZE;
        
        END_TRANSLATION;
    }

    else
    {
        sprintf(out_command_start + out_offset, "push %n", &delta);
        out_offset += delta;
    }
    
    if (RAM_BIT)
    {
        sprintf(out_command_start + out_offset, "[ " MEMSPACE_LABEL " + %n", &delta);
        out_offset += delta;
    }

    if (REG_BIT)
    {

        sprintf(out_command_start + out_offset, "r%cx%n", *(current_command + in_offset), &delta);
        in_offset  += REGTAG_SIZE;
        out_offset += delta;
    }

    if (CONST_BIT)
    {

        if (REG_BIT)
        {
            sprintf(out_command_start + out_offset, " + ");
            out_offset += 3;
        }

        sprintf(out_command_start + out_offset, "%lld%n", (long int)(*(double*)(current_command + in_offset)), &delta);
        in_offset  += CONST_SIZE;
        out_offset += delta;
    }

    if (RAM_BIT)
    {
        sprintf(out_command_start + out_offset, " ]%n", &delta);
        out_offset += delta;
    }

    sprintf(out_command_start + out_offset, "\n\n%n", &delta);
    out_offset+=delta;

    *in_cursor  = *in_cursor  + in_offset;
    *out_cursor = *out_cursor + out_offset;

    #undef END_TRANSLATION   
}

void pop2nasm(char** in_cursor, char** out_cursor)
{

    #define END_TRANSLATION RAM_BIT=REG_BIT=CONST_BIT=0

    char* out_command_start = *out_cursor;
    char* current_command   = *in_cursor;

    const unsigned char opcode = *current_command;
    
    unsigned char RAM_BIT    = opcode & RAM_BIT_MASK;
    unsigned char REG_BIT    = opcode & REGISTER_BIT_MASK;
    unsigned char CONST_BIT  = opcode & CONST_BIT_MASK;
    
    unsigned char       in_offset  = 0;
    unsigned int        out_offset = 0;
    unsigned int        delta      = 0;

    sprintf(out_command_start + out_offset, "push %n", &delta);
    in_offset++;
    out_offset += delta;

    
    if (RAM_BIT)
    {
        sprintf(out_command_start + out_offset, "[ " MEMSPACE_LABEL " + %n", &delta);
        out_offset += delta;
    }

    if (REG_BIT)
    {

        sprintf(out_command_start + out_offset, "r%cx%n", *(current_command + in_offset), &delta);
        in_offset  += REGTAG_SIZE;
        out_offset += delta;
    }

    if (CONST_BIT)
    {

        if (REG_BIT)
        {
            sprintf(out_command_start + out_offset, " + ");
            out_offset += 3;
        }

        sprintf(out_command_start + out_offset, "%lld%n", (long int)(*(double*)(current_command + in_offset)), &delta);
        in_offset  += CONST_SIZE;
        out_offset += delta;
    }

    if (RAM_BIT)
    {
        sprintf(out_command_start + out_offset, " ]%n", &delta);
        out_offset += delta;
    }

    sprintf(out_command_start + out_offset, "\n\n%n", &delta);
    out_offset+=delta;

    *in_cursor  = *in_cursor  + in_offset;
    *out_cursor = *out_cursor + out_offset;

    #undef END_TRANSLATION   
}
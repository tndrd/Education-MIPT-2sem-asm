#include "elf.h"

void WriteElf(const char* filename, char* code_buffer, size_t filesize)
{
    FILE* elf = fopen(filename, "wb");
    
    ProgramHeader pHeader   = {};
    ELFHeader     elfHeader = {};
    pHeader.P_FILES = pHeader.P_MEMSZ = sizeof(ELFHeader) + sizeof(ProgramHeader) + filesize;

    printf("%d %d\n", pHeader.P_FILES, pHeader.P_MEMSZ);
    fwrite (&elfHeader, sizeof (elfHeader), 1              , elf);
    fwrite (&pHeader  , sizeof (pHeader)  , 1              , elf);

    fwrite (code_buffer , sizeof (char)     , filesize    , elf);

    fclose (elf);
}
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "tokenization.h"
#include "translation.h"
#include "assembling.h"
#include "sys/mman.h"
#include "elf.h"



// Not working
void Execute(char* code_buffer, const size_t len)
{
    assert(code_buffer);
    assert(len > 0);

    mprotect(code_buffer, len, PROT_EXEC);

    void (*func)(void) = (void(*)(void))code_buffer;

    func();
}

int Code2nasm(TokenList* token_list, char* code, const long int filesize)
{
    assert(token_list);
    assert(code);
    assert(filesize > 0);

    char* out_buffer = (char*)aligned_alloc(4096, MAX_FILE_SIZE * sizeof(char));
    if  (!out_buffer)  return 1;

    Translate(token_list, out_buffer); 

    FILE* fp = fopen(DEFAULT_ASM_NAME, "w");
    fprintf(fp, "%s", out_buffer);

    free(out_buffer);
    fclose(fp);

    return 0;

}

int Code2Elf(TokenList* token_list, char* code, const long int filesize)
{
    assert(token_list);
    assert(code);
    assert(filesize > 0);
    
    char* out_buffer = (char*)aligned_alloc(4096, MAX_FILE_SIZE * sizeof(char));
    if  (!out_buffer)  return 1;
    
    size_t bin_size = 0;

    bin_size = Assemble(token_list, out_buffer); // double-passing  
    bin_size = Assemble(token_list, out_buffer); //

    WriteElf(DEFAULT_ELF_NAME, out_buffer, bin_size);

    free(out_buffer);

    return 0;
}

int main(int argc, char* argv[])
{

    switch (argc)
    {
        case 1:  printf("Error: specify input CPU file\n"); exit(0);
        default: printf("Error: too much arguments");       exit(0);
        case 2:  break;
    }


    long  int filesize = 0;
    char*     code = ReadFile(argv[1], &filesize);
    
    if (!code)
    {
        printf("Error: failed to open file \"%s\"\n", argv[1]);
    }
    else
    {
        printf("Tokenizing \"%s\"...", argv[1]);
    }

    TokenList* token_list = Tokenize(code, filesize);

    if (token_list)
    {
        printf(GRN_CLR "Done!\n\n" END_CLR);
    }
    else
    {
        printf(RED_CLR "Failed to tokenize. Stopping program.\n" END_CLR);
        return 1;
    }

    printf("Translating into nasm...");
    if(Code2nasm(token_list, code, filesize) != 0)
    {
        printf(RED_CLR " Failed to translate!\n\n" END_CLR);
    }
    else
    {
        printf(GRN_CLR " Done!\n" END_CLR "> See %s.\n\n", DEFAULT_ASM_NAME);
    }

    printf("Assembling...");
    if(Code2Elf(token_list, code, filesize) != 0)
    {
        printf(RED_CLR " Failed to assemble!\n" END_CLR);
    }
    else
    {
        printf(GRN_CLR " Done!\n" END_CLR);
        printf("> How to run: $ chmod +x %s\n"
               "              $ ./%s\n\n", DEFAULT_ELF_NAME, DEFAULT_ELF_NAME);
    }

    return 0;
}
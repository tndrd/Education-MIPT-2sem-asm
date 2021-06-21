#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "token.h"
#include "tokenlist.h"
#include "tokenization.h"
#include "translation.h"
#include "assembling.h"
#include "sys/mman.h"
#include "elf.h"

void Execute(char* code_buffer, const size_t len)
{
    mprotect(code_buffer, len, PROT_EXEC);

    void (*func)(void) = (void(*)(void))code_buffer;

    func();
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
        printf("Tokenizing \"%s\"...\n", argv[1]);
    }

    TokenList* token_list = Tokenize(code, filesize);

    drawTokens(token_list);

    char* out_buffer = (char*)aligned_alloc(4096, MAX_FILE_SIZE * sizeof(char));

    printf("Translating into nasm...\n");

    Translate(token_list, out_buffer);

    FILE* fp = fopen("result.asm", "w");
    fprintf(fp, "%s", out_buffer);

    free(out_buffer);
    fclose(fp);

    printf("Assembling...\n");

    out_buffer = (char*)aligned_alloc(4096, MAX_FILE_SIZE * sizeof(char));

    size_t bin_size = 0;

    bin_size = Assemble(token_list, out_buffer);
    bin_size = Assemble(token_list, out_buffer);

    WriteElf("result", out_buffer, bin_size);

    printf("Done! How to run: $ chmod +x result\n"
           "                  $ ./result\n\n");

    free(out_buffer);
}
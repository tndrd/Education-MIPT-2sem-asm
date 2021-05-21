#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "tests.h"
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

int main()
{
    runTests();

    printf("%d\n", sizeof(ELFHeader) + sizeof(ProgramHeader));

    long  int filesize = 0;
    char* code = ReadFile("pushtest", &filesize);
    
    TokenList* token_list = Tokenize(code, filesize);

    printTokens(token_list);
    drawTokens(token_list);

    printf(H_BAR);

    char* out_buffer = (char*)aligned_alloc(4096, 10000 * sizeof(char));

    Translate(token_list, out_buffer);
    printf(H_BAR);
    printf("%s", out_buffer);

    FILE* fp = fopen("result.asm", "w");
    fprintf(fp, "%s", out_buffer);

    free(out_buffer);
    fclose(fp);

    out_buffer = (char*)aligned_alloc(4096, 10000 * sizeof(char));

    size_t bin_size = 0;

    bin_size = Assemble(token_list, out_buffer);
    bin_size = Assemble(token_list, out_buffer);
    printf(H_BAR);

    //Execute(out_buffer, bin_size);

    fp = fopen("binary", "w");
    fwrite(out_buffer, 1, bin_size, fp);

    WriteElf("result", out_buffer, bin_size);


    free(out_buffer);
    fclose(fp);
}
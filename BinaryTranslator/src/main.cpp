#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "tests.h"
#include "token.h"
#include "tokenlist.h"
#include "tokenization.h"
#include "translation.h"

int main()
{
    runTests();


    long  int filesize = 0;
    char* code = ReadFile("pushtest", &filesize);
    

    TokenList* token_list = Tokenize(code, filesize);

    printTokens(token_list);
    drawTokens(token_list);

    printf(H_BAR);

    char* out_buffer = (char*)calloc(5096, sizeof(char));

    Translate(token_list, out_buffer);
    printf(H_BAR);
    printf("%s", out_buffer);

    free(out_buffer);
}
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "tests.h"
#include "token.h"
#include "tokenlist.h"
#include "tokenization.h"


int main()
{
    runTests();


    long  int filesize = 0;
    char* code = ReadFile("pushtest", &filesize);
    

    TokenList* token_list = Tokenize(code, filesize);

    printTokens(token_list);
    drawTokens(token_list);
    /*
    AssignAdrOperand(&a, 0x532);
    AssignCstOperand(&b, 239);
    assignOperands(new_token, &a, &b);

    printToken(new_token);
    */
}
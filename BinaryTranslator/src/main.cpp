#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "tests.h"
#include "token.h"
#include "tokenlist.h"


int main()
{
    runTests();

    TokenList* token_list = newTokenList();

    Operand a = {};
    Operand b = {};

    Token* first = newToken(PUSH, {NO_SUBNAME}, nullptr, nullptr);
    AssignAdrOperand(&a, 0x532);
    AssignRegOperand(&b, RAX);
    assignOperands(first, &a, &b);
    append(token_list, first);

    Token* second = newToken(PUSH, {NO_SUBNAME}, nullptr, nullptr);
    AssignAdrOperand(&a, 0x532);
    AssignRegOperand(&b, RAX);
    assignOperands(second, &a, &b);
    append(token_list, second);

    Token* third = newToken(PUSH, {NO_SUBNAME}, nullptr, nullptr);
    AssignAdrOperand(&a, 0x532);
    AssignEmpOperand(&b);
    assignOperands(third, &a, &b);
    append(token_list, third);

    printTokens(token_list);
    //drawTokens(token_list);
    /*
    AssignAdrOperand(&a, 0x532);
    AssignCstOperand(&b, 239);
    assignOperands(new_token, &a, &b);

    printToken(new_token);
    */
}
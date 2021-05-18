#ifndef __TOKENLIST__
#define __TOKENLIST__

#include "token.h"

struct TokenList
{
    Token* head = nullptr;
    Token* tail = nullptr;
    
    size_t size = 0;    
};

TokenList* newTokenList();

void DeleteTokenList(TokenList* tlist);

Token* appendFirst(TokenList* tlist, Token* token);

Token* append(TokenList* tlist, Token* token);

TokenList* printTokens(TokenList* tlist);

Token* drawToken(FILE* fp, Token* token);

TokenList* drawTokens(TokenList* tlist);

#endif
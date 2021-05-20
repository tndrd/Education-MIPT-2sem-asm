#ifndef __TOKENLIST__
#define __TOKENLIST__

#include "token.h"
#include "config.h"
#include "assert.h"
#include "coloring.h"

struct LabelList
{
    unsigned char n_labels   = 0;
    char*   map              = nullptr;
    int*    in_labels        = nullptr;
    Token** labels           = nullptr;
};

struct TokenList
{
    Token* head = nullptr;
    Token* tail = nullptr;

    LabelList label_list = {};

    ImmediateStack imms = {};

    size_t size = 0;    
};

TokenList* newTokenList();

void DeleteTokenList(TokenList* tlist);

Token* appendFirst(TokenList* tlist, Token* token);

Token* append(TokenList* tlist, Token* token);

TokenList* printTokens(TokenList* tlist);

Token* drawToken(FILE* fp, TokenList* tlist, Token* token);

TokenList* drawTokens(TokenList* tlist);

LabelList* SetLabelList(LabelList* label_list, char* code);

LabelList* SortLabels(LabelList* label_list);

void PrintLabels(LabelList* label_list);

unsigned char GetLabel(LabelList* label_list, unsigned char label_index);

#endif
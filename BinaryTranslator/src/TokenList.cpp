#include "tokenlist.h"


TokenList* newTokenList()
{
    TokenList* new_tlist = (TokenList*)calloc(1, sizeof(TokenList));
    new_tlist -> size = 0;
    return new_tlist;   
}

void DeleteTokenList(TokenList* tlist)
{
    Token* current_element = tlist -> tail;
    Token* next_element    = nullptr;

    while (current_element)
    {
        next_element    = current_element -> next;
        free(current_element);
        current_element = next_element;
    }
}


Token* appendFirst(TokenList* tlist, Token* token)
{
    if (!tlist || !token) return nullptr;

    tlist -> head = token;
    tlist -> tail = token;
    tlist -> size++;

    token -> next = nullptr;
    token -> prev = nullptr;

    return token;
}

Token* append(TokenList* tlist, Token* token)
{
    if (!tlist || !token) return nullptr;

    if (!tlist -> size)
    {
        appendFirst(tlist, token);
        return token;
    }

    token -> prev = tlist -> head;
    token -> next = nullptr;
    
    (tlist -> head) -> next = token;
    (tlist -> head) =  token;

    tlist -> size++;

    return token;
}

TokenList* printTokens(TokenList* tlist)
{
    if (!tlist) return nullptr;

    Token* current_token = tlist -> tail;

    while (current_token)
    {
        printToken(current_token);
        current_token = current_token -> next;
    }

    return tlist;
}

Token* drawToken(FILE* fp, Token* token)
{
    if (!token || !token) return nullptr;

    const char* operation = GetOperationName(token -> op_type, token -> op_name);
    fprintf(fp, "%d [shape=record, fillcolor=green rank = same style=filled label=\"    %s | {", token, operation);
    printOperand(fp, &(token -> a));
    fprintf(fp, " | ");
    printOperand(fp, &(token -> b));
    fprintf(fp, " }\" ];\n");
    fprintf(fp, "%d -> %d\n", token, token -> next);
    return token;
}

TokenList* drawTokens(TokenList* tlist)
{
    if (!tlist) return nullptr;

    FILE* fp = fopen("show.dot", "w");
    if  (!fp)  return nullptr;

    Token* current_token = tlist -> tail;

    fprintf(fp, "digraph structs {\nrankdir=LR rank=same;\n");

    while (current_token)
    {
        drawToken(fp, current_token);
        current_token = current_token -> next;
    }

    fprintf(fp, "}");
    fclose(fp);
    
    system("dot -Tpng show.dot -n -o show.png");
    system("viewnior show.png");

    return tlist;
}
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

Token* drawToken(FILE* fp, TokenList* tlist, Token* token)
{
    if (!token || !token) return nullptr;

    const char* operation = GetOperationName(token -> op_type, token -> op_name);
    fprintf(fp, "%d [shape=record, fillcolor=green rank = same style=filled label=\"    %s ", token, operation);
    
    OperandType a_op = token -> operand_a.type;
    OperandType b_op = token -> operand_b.type;
    
    if (a_op)
    {
        fprintf (fp, " | {");
        printOperand(fp, &(token -> operand_a));

        if (b_op)
        {
            fprintf(fp, " | ");
            printOperand(fp, &(token -> operand_b));
        }
    
    fprintf(fp, " }");
    }
    
    fprintf(fp, "\" ];\n");
    fprintf(fp, "%d -> %d\n", token, token -> next);
    
    if (token -> op_type == JUMP)
    {
        fprintf(fp, "%d -> %d\n", token, (tlist -> label_list).labels[(token -> operand_a).label]);
    }
    
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
        drawToken(fp, tlist, current_token);
        current_token = current_token -> next;
    }

    for (int n_label = 0; n_label < (tlist -> label_list).n_labels; n_label++)
    {
        fprintf(fp, "label_%d -> %d\n", n_label, (tlist -> label_list).labels[n_label]);
    }

    fprintf(fp, "}");
    fclose(fp);
    
    system("dot -Tpng show.dot -n -o show.png");
    system("viewnior show.png");

    return tlist;
}

LabelList* SetLabelList(LabelList* label_list, char* code)
{
    if (!label_list || !code) return nullptr;

    char n_labels = *(code + HEADER_SIZE - 1);

    label_list -> n_labels  = n_labels;
    label_list -> in_labels = (int*)(code + HEADER_SIZE);

    label_list -> map           = (char*) calloc(n_labels, sizeof(char));
    label_list -> labels        = (Token**)calloc(n_labels, sizeof(Token*));
    label_list -> label_offsets = (char**)calloc(n_labels, sizeof(char*));

    (label_list -> stdlib)._out_offset_ = nullptr;
    (label_list -> stdlib)._in_offset_ = nullptr;
    (label_list -> stdlib)._hlt_offset_ = nullptr;

    label_list -> entry = nullptr;

    for (int n_label = 0; n_label < n_labels; n_label++)
    {
        (label_list -> map)[n_label] = n_label;
    }

    for (int n_label = 0; n_label < n_labels; n_label++)
    {
        (label_list -> label_offsets)[n_label] = nullptr;
    }
    
    return label_list;   
}

LabelList* SortLabels(LabelList* label_list)
{
    if (!label_list || !label_list -> map || !label_list -> in_labels) return nullptr;

    for (int i = 0; i < label_list -> n_labels; i++)
        for (int j = 1; j < label_list -> n_labels; j++)
            if ((label_list -> in_labels)[j - 1] > (label_list -> in_labels)[j])
            {
                char t_idx                 = (label_list -> map)[j - 1];
                (label_list -> map)[j - 1] = (label_list -> map)[j];     
                (label_list -> map)[j]     = t_idx;

                int t_lab                        = (label_list -> in_labels)[j - 1];
                (label_list -> in_labels)[j - 1] = (label_list -> in_labels)[j];     
                (label_list -> in_labels)[j]     = t_lab;

            } 

    return label_list;
}

void PrintLabels(LabelList* label_list)
{
    printf("%d labels:\n", label_list -> n_labels);

    for (int n_label = 0; n_label < label_list -> n_labels; n_label++)
    {
        printf("%d [%p]\n", label_list -> in_labels[n_label], label_list -> label_offsets[n_label]); 
    }
}

unsigned char GetLabel(LabelList* label_list, unsigned char label_index)
{   
    assert(label_list);
    
    assert(label_index < label_list -> n_labels);
    
    for (int n_label = 0; n_label < label_list -> n_labels; n_label++)
    {
        if ((label_list -> map)[n_label] == label_index) return n_label; 
    }

    printf("HOLY SHIT!\n");

    return 0;
}
#ifndef __TOKENIZATION__
#define __TOKENIZATION__

#include "tokenlist.h"
#include "config.h"

TokenList* Tokenize(char* code, long int filesize);

Token* tokenizePUSH (TokenList* tlist, char** in_cursor);
Token* tokenizePOP  (TokenList* tlist, char** in_cursor);
Token* tokenizeIN   (TokenList* tlist, char** in_cursor);
Token* tokenizeOUT  (TokenList* tlist, char** in_cursor);
Token* tokenizeHLT  (TokenList* tlist, char** in_cursor);

Token* tokenizeDIV  (TokenList* tlist, char** in_cursor);
Token* tokenizeMUL  (TokenList* tlist, char** in_cursor);
Token* tokenizeADD  (TokenList* tlist, char** in_cursor);
Token* tokenizeSUB  (TokenList* tlist, char** in_cursor);

Token* tokenizeJMP  (TokenList* tlist, char** in_cursor);
Token* tokenizeJA   (TokenList* tlist, char** in_cursor);
Token* tokenizeJAE  (TokenList* tlist, char** in_cursor);
Token* tokenizeJB   (TokenList* tlist, char** in_cursor);
Token* tokenizeJBE  (TokenList* tlist, char** in_cursor);
Token* tokenizeJE   (TokenList* tlist, char** in_cursor);
Token* tokenizeJNE  (TokenList* tlist, char** in_cursor);
 
Token* tokenizeCALL (TokenList* tlist, char** in_cursor);
Token* tokenizeRETURN(TokenList* tlist, char** in_cursor);

#endif
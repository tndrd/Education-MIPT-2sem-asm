#ifndef __ASSEMBLING__
#define __ASSEMBLING__

#include "tokenlist.h"
#include "assert.h"
#include "foperations.h"

size_t Assemble(TokenList* tlist, char* buffer);

void assembleFLD    (TokenList* tlist, Token* token, char** cursor);
void assembleFST    (TokenList* tlist, Token* token, char** cursor);
void assembleFCOMPP (TokenList* tlist, Token* token, char** cursor);
void assembleFSTSW  (TokenList* tlist, Token* token, char** cursor);
void assembleSAHF   (TokenList* tlist, Token* token, char** cursor);
void assembleRETURN (TokenList* tlist, Token* token, char** cursor);
void assembleFADD   (TokenList* tlist, Token* token, char** cursor);
void assembleFSUB   (TokenList* tlist, Token* token, char** cursor);
void assembleFMUL   (TokenList* tlist, Token* token, char** cursor);
void assembleFDIV   (TokenList* tlist, Token* token, char** cursor);
void assembleFCHS   (TokenList* tlist, Token* token, char** cursor);
void assembleFSQRT  (TokenList* tlist, Token* token, char** cursor);
void assembleSUB    (TokenList* tlist, Token* token, char** cursor);
void assembleADD    (TokenList* tlist, Token* token, char** cursor);
void assembleJMP    (TokenList* tlist, Token* token, char** cursor);
void assembleCALL   (TokenList* tlist, Token* token, char** cursor);
void assembleJA     (TokenList* tlist, Token* token, char** cursor);
void assembleJAE    (TokenList* tlist, Token* token, char** cursor);
void assembleJB     (TokenList* tlist, Token* token, char** cursor);
void assembleJBE    (TokenList* tlist, Token* token, char** cursor);
void assembleJE     (TokenList* tlist, Token* token, char** cursor);
void assembleJNE    (TokenList* tlist, Token* token, char** cursor);

void assemblePUSH   (TokenList* tlist, Token* token, char** cursor);
void assemblePOP    (TokenList* tlist, Token* token, char** cursor);
void assembleOUT    (TokenList* tlist, Token* token, char** cursor);
void assembleIN     (TokenList* tlist, Token* token, char** cursor);
void assembleHLT    (TokenList* tlist, Token* token, char** cursor);

void assembleJUMP       (TokenList* tlist, Token* token, char** cursor);
void assembleARITHMETIC (TokenList* tlist, Token* token, char** cursor);

void CalculateLabelOffset(char* label, char** cursor);

void WriteImms(TokenList* tlist, char** cursor);
void LoadFunc(const char* filename, char** dst_addr, char** cursor);

void LoadStdlib(TokenList* tlist, char** cursor);
#endif
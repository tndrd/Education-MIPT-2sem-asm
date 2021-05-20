#ifndef __TRANSLATION__
#define __TRANSLATION__

#include "tokenlist.h"
#include "assert.h"

void Translate           (TokenList* tlist, char* buffer);
void TranslateOperand    (Operand* operand, char** cursor);
void TranslateRegister   (RegName reg, char** cursor);
void TranslateImmediate  (double cst, char** cursor);
void TranslateOperation  (Token* token, char** cursor);
void TranslateArithmetic (ArithmeticOperation operation, char** cursor);
void TranslateSpecial(SpecName name, char** cursor);
void TranslateJump       (JumpOperation operation, char** cursor);


#endif
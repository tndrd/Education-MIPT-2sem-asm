#ifndef __ENUMS2STRING__
#define __ENUMS2STRING__

#include "token.h"
#include "coloring.h"

const char* getEnum_Operation(Operation operation);
const char* getEnum_RegName(RegName reg);
const char* getEnum_ArithmeticOperation(ArithmeticOperation operation);
const char* getEnum_JumpOperation(JumpOperation operation);
const char* GetOperationName(Operation op_type, OperationName name);

#endif
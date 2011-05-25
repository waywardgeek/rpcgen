// Function methods.
#include "co.h"

// Create a command.
coFunction coFunctionCreate(
    utSym name,
    coTyperef returnType,
    coTyperef parameterType)
{
    coFunction function = coFunctionAlloc();

    coFunctionSetSym(function, name);
    coFunctionSetReturnTyperef(function, returnType);
    coFunctionSetParameterTyperef(function, parameterType);
    coRootAppendFunction(coTheRoot, function);
    return function;
}

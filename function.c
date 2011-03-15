// Function methods.
#include "co.h"

// Create a command.
coFunction coFunctionCreate(
    utSym name,
    coType returnType,
    coType parameterType)
{
    coFunction function = coFunctionAlloc();

    coFunctionSetSym(function, name);
    coFunctionSetReturnType(function, returnType);
    coFunctionSetParameterType(function, parameterType);
    coRootAppendFunction(coTheRoot, function);
    return function;
}

// Function methods.
#include "co.h"

// Create a command.
coFunction coFunctionCreate(
    utSym name,
    coType returnType,
    coType parameterType)
{
    coFunction command = coFunctionAlloc();

    coFunctionSetSym(command, name);
    coFunctionSetReturnType(command, returnType);
    coFunctionSetParameterType(command, parameterType);
    return command;
}

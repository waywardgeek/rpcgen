// Command methods.
#include "co.h"

// Create a command.
coCommand coCommandCreate(
    utSym name,
    coType returnType,
    coType parameterType)
{
    coCommand command = coCommandAlloc();

    coCommandSetSym(command, name);
    coCommandSetReturnType(command, returnType);
    coCommandSetParameterType(command, parameterType);
    return command;
}

// Typedef methods.
#include "co.h"

// Create a typedef object.
coTypedef coTypedefCreate(
    utSym name,
    coType type)
{
    coTypedef typeDef = coTypedefAlloc();

    coTypedefSetSym(typeDef, name);
    coTypedefSetType(typeDef, type);
    coRootInsertTypedef(coTheRoot, typeDef);
    return typeDef;
}

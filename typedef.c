// Typedef methods.
#include "co.h"

// Create a typedef object.
coTypedef coTypedefCreate(
    utSym name,
    coTyperef typeref)
{
    coTypedef typeDef = coTypedefAlloc();

    coTypedefSetSym(typeDef, name);
    coTypedefSetTyperef(typeDef, typeref);
    coRootAppendTypedef(coTheRoot, typeDef);
    return typeDef;
}

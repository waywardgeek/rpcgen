// Enum and entry methods.
#include "co.h"

// Create an enum object.
coEnum coEnumCreate(
    utSym name)
{
    coEnum Enum = coEnumAlloc();

    coEnumSetSym(Enum, name);
    coRootAppendEnum(coTheRoot, Enum);
    return Enum;
}

// Create an entry in an enum.
coEntry coEntryCreate(
    coEnum Enum,
    utSym name)
{
    coEntry entry = coEntryAlloc();

    coEntrySetSym(entry, name);
    coEnumAppendEntry(Enum, entry);
    coRootAppendEntry(coTheRoot, entry);
    return entry;
}

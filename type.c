// Type methods.
#include "co.h"

// Create a basic value type.
coType coBasicTypeCreate(
    coValtype valtype)
{
    coType type = coTypeAlloc();

    coTypeSetValtype(type, valtype);
    return type;
}

// Create an unbound identifier type, which will later be bound to an enum or
// typedef.
coType coIdentTypeCreate(
    utSym name)
{
    coType type = coBasicTypeCreate(CO_UNBOUND);

    coTypeSetNameVal(type, name);
    return type;
}

// Create a tuple type.
coType coTupleTypeCreate(void)
{
    return coBasicTypeCreate(CO_TUPLE);
}

// Add a field to the tuple type.
void coAddFieldToTuple(
    coType tuple,
    coType type,
    utSym name,
    coValue defaultValue)
{
    coField field = coFieldAlloc();

    coFieldSetSym(field, name);
    coFieldSetType(field, type);
    coFieldSetDefaultValue(field, defaultValue);
    coTypeAppendField(tuple, field);
}

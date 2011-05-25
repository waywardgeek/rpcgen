// Type methods.  Note that we should come up with a better hash function to
// make it less likely we get different types with the same 64-bit hash key,
// which will cause errors.
#include "co.h"

// Create a type.
static coType typeCreate(
    coValtype valtype,
    uint64 hashKey)
{
    coType type = coRootFindType(coTheRoot, hashKey);

    if(type == coTypeNull) {
        type = coTypeAlloc();
        coTypeSetValtype(type, valtype);
        coTypeSetHashKey(type, hashKey);
        coRootAppendType(coTheRoot, type);
    }
    return type;
}

// Create a new typeref.
static coTyperef typerefCreate(
    coType type)
{
    coTyperef typeref = coTyperefAlloc();

    coTyperefSetType(typeref, type);
    return typeref;
}

// Create a basic value type.
coTyperef coBasicTypeCreate(
    coValtype valtype)
{
    uint64 hashKey = valtype;
    coType type = typeCreate(valtype, hashKey);

    return typerefCreate(type);
}

// Create a basic value type.
coTyperef coIntTypeCreate(
    coValtype valtype,
    uint8 width)
{
    uint64 hashKey = utHashValues(width, valtype);
    coType type = typeCreate(valtype, hashKey);

    coTypeSetWidth(type, width);
    return typerefCreate(type);
}

// Create an unbound identifier type, which will later be bound to an enum or
// typedef.
coTyperef coIdentTypeCreate(
    utSym name)
{
    uint64 hashKey = utHashValues(utSym2Index(name), CO_IDENT);
    coType type = typeCreate(CO_IDENT, hashKey);

    coTypeSetNameVal(type, name);
    return typerefCreate(type);
}

// Create a tuple type.  We don't hash this until after fields are all added.
coType coTupleTypeCreate(void)
{
    coType type = coTypeAlloc();
    
    coTypeSetValtype(type, CO_TUPLE);
    return type;
}

// Add a field to the tuple type.
void coAddFieldToTuple(
    coType tuple,
    coTyperef typeref,
    utSym name,
    coValue defaultValue)
{
    coField field = coFieldAlloc();

    coFieldSetSym(field, name);
    coFieldSetTyperef(field, typeref);
    coFieldSetDefaultValue(field, defaultValue);
    coTypeAppendField(tuple, field);
}

// Once all the fields are added to a tupple, we need to hash it and see if we
// already have this type.  If so, delete the type passed, and return the old
// one.  If it is new, add it to the hash table and return it.
coTyperef coHashTupleType(
    coType type)
{
    coTyperef typeref = coTyperefAlloc();
    coField field;
    coType fieldType;
    coType oldType;
    uint64 hashKey = CO_TUPLE;

    coForeachTypeField(type, field) {
        fieldType = coTyperefGetType(coFieldGetTyperef(field));
        hashKey = utHashValues(coTypeGetHashKey(fieldType), hashKey);
    } coEndTypeField;
    oldType = coRootFindType(coTheRoot, hashKey);
    if(oldType == coTypeNull) {
        coTypeSetHashKey(type, hashKey);
        coRootAppendType(coTheRoot, type);
    } else {
        coTypeDestroy(type);
        type = oldType;
    }
    coTyperefSetType(typeref, type);
    return typeref;
}

// Create an array type.
coTyperef coArrayTypeCreate(
    coTyperef typeref)
{
    uint64 hashKey = utHashValues(coTypeGetHashKey(coTyperefGetType(typeref)), CO_ARRAY);
    coType arrayType = typeCreate(CO_ARRAY, hashKey);

    coTypeSetTyperefVal(arrayType, typeref);
    return typerefCreate(arrayType);
}

// Create a pointer type, as in a C pointer.
coTyperef coPointerTypeCreate(
    coTyperef typeref)
{
    uint64 hashKey = utHashValues(coTypeGetHashKey(coTyperefGetType(typeref)), CO_POINTER);
    coType pointerType = typeCreate(CO_POINTER, hashKey);

    coTypeSetTyperefVal(pointerType, typeref);
    return typerefCreate(pointerType);
}

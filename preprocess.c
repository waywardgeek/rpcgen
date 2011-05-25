// Preprocess data types.  For now, this is just declaring structures for each
// array type so we can make them length encoded.
#include "co.h"

// Convert tuple types to typedefs and declare them.
static coTyperef declareUnnamedTypedef(
    utSym typeName,
    coTyperef typeref)
{
    coTypedefCreate(typeName, typeref);

    return coIdentTypeCreate(typeName);
}

// Declare typedefs for all unnamed tuples in the function.
static void declareUnnamedTypedefs(
    coFunction function)
{
    coField field;
    coTyperef returnType, paramType;
    utSym retTypeName, paramTypeName;

    returnType = coFunctionGetReturnTyperef(function);
    if(returnType != coTyperefNull && coTypeGetValtype(coTyperefGetType(returnType)) == CO_TUPLE) {
        retTypeName = utSymCreateFormatted("%sRetval",
            coCapitalize(coFunctionGetName(function)));
        returnType = declareUnnamedTypedef(retTypeName, returnType);
        coFunctionSetReturnTyperef(function, returnType);
    }
    coForeachTypeField(coTyperefGetType(coFunctionGetParameterTyperef(function)), field) {
        paramType = coFieldGetTyperef(field);
        if(coTypeGetValtype(coTyperefGetType(paramType)) == CO_TUPLE) {
            paramTypeName = utSymCreateFormatted("%s%sVal",
                coCapitalize(coFunctionGetName(function)),
                coCapitalize(coFieldGetName(field)));
            paramType = declareUnnamedTypedef(paramTypeName, paramType);
            coFieldSetTyperef(field, paramType);
        }
    } coEndTypeField;
}

// Declare functions unnamed types.
static void declareFunctionTypes(void)
{
    coFunction function;

    coForeachRootFunction(coTheRoot, function) {
        declareUnnamedTypedefs(function);
    } coEndRootFunction;
}

// Array types need to be declared as structs with a length and pointer.
static void declareArrayTypes(void)
{
    coTyperef typeref, widthTyperef, pointerTyperef;
    coType type;

    coSafeForeachRootType(coTheRoot, type) {
        if(coTypeGetValtype(type) == CO_ARRAY) {
            typeref = coTypeGetTyperefVal(type);
            coRootRemoveType(coTheRoot, type);
            coTypeSetValtype(type, CO_TUPLE);
            widthTyperef = coIntTypeCreate(CO_UINT, 32);
            coAddFieldToTuple(type, widthTyperef, utSymCreate("length"), coValueNull);
            pointerTyperef = coPointerTypeCreate(typeref);
            coAddFieldToTuple(type, pointerTyperef, utSymCreate("data"), coValueNull);
            typeref = coHashTupleType(type);
            coTyperefDestroy(typeref);
        }
    } coEndSafeRootType;
}

// Preprocess data types.
void coPreprocessDataTypes(void)
{
    declareArrayTypes();
    declareFunctionTypes();
}

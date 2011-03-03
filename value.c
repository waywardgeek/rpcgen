// Value methods.
#include "co.h"

static inline coValue valueCreate(
   coValtype valtype)
{
    coValue value = coValueAlloc();

    coValueSetType(value, valtype);
    return value;
}

// Create an integer value.
coValue coIntValueCreate(
    int64 val)
{
    coValue value = valueCreate(CO_INT);

    coValueSetIntVal(value, val);
    return value;
}

// Create a string value.
coValue coStringValueCreate(
    utSym val)
{
    coValue value = valueCreate(CO_STRING);
    coString string = coStringAlloc();
    char *s = utSymGetName(val);

    coStringSetValue(string, s, strlen(s) + 1);
    coValueSetStringVal(value, string);
    return value;
}

// Create a floating point value.
coValue coFloatValueCreate(
    double val)
{
    coValue value = valueCreate(CO_FLOAT);

    coValueSetFloatVal(value, val);
    return value;
}

// Create a Boolean point value.
coValue coBoolValueCreate(
    bool val)
{
    coValue value = valueCreate(CO_BOOL);

    coValueSetBoolVal(value, val);
    return value;
}

// Create an entry value.
coValue coEntryValueCreate(
    utSym val)
{
    coValue value = valueCreate(CO_ENTRY);

    coValueSetEntryVal(value, val);
    return value;
}

// Create a tuple value from an array of values.
coValue coTupleValueCreate(
    coValueArray vals)
{
    coValue value = valueCreate(CO_TUPLE);

    coValueSetTupleVal(value, vals);
    return value;
}

// Create an array value from an array of values.
coValue coArrayValueCreate(
    coValueArray vals)
{
    coValue value = valueCreate(CO_ARRAY);

    coValueSetArrayVal(value, vals);
    return value;
}


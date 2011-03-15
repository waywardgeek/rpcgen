// Generate a .c file for parsing commands, and a .h file for functions the user
// must provide to process them.
#include "co.h"

#define CO_WRAP_COLUMN 80
static uint32 coLinePos;
static char *coPrefix;

// Write text to coFile.  Print a new line if there is not enough space.
void coPrintW(
    char *newLineText,
    char *format,
    ...)
{
    char *buf;
    va_list ap;
    uint32 length;

    va_start(ap, format);
    buf = utVsprintf(format, ap);
    length = strlen(buf);
    if(length + coLinePos > CO_WRAP_COLUMN) {
        coLinePos = fprintf(coFile, "\n%s", newLineText);
    }
    coLinePos += fprintf(coFile, "%s", buf);
}

// Call coPrint, and print a new line.
void coPrintlnW(
    char *newLineText,
    char *format,
    ...)
{
    char *buf;
    va_list ap;
    uint32 length;

    va_start(ap, format);
    buf = utVsprintf(format, ap);
    length = strlen(buf);
    if(length + coLinePos > CO_WRAP_COLUMN) {
        fprintf(coFile, "\n%s", newLineText);
    }
    fprintf(coFile, "%s\n", buf);
    coLinePos = 0;
}

// Write text to coFile.
void coPrint(
    char *format,
    ...)
{
    char *buf;
    va_list ap;

    va_start(ap, format);
    buf = utVsprintf(format, ap);
    coLinePos += fprintf(coFile, "%s", buf);
}

// Call coPrint, and print a new line.
void coPrintln(
    char *format,
    ...)
{
    char *buf;
    va_list ap;

    va_start(ap, format);
    buf = utVsprintf(format, ap);
    fprintf(coFile, "%s\n", buf);
    coLinePos = 0;
}

// Declare enumerated types.
static void declareEnums(void)
{
    coEnum theEnum;
    coEntry entry;

    coForeachRootEnum(coTheRoot, theEnum) {
        coPrint("typedef enum {");
        coForeachEnumEntry(theEnum, entry) {
            coPrintW("    ", "%s", coEntryGetName(entry));
            if(coEntryGetNextEnumEntry(entry) != coEntryNull) {
                coPrint(", ");
            }
        } coEndEnumEntry;
        coPrintlnW("    ", "} %s%s;", coPrefix, coEnumGetName(theEnum));
    } coEndRootEnum;
}

static void writeType(coType type);

// Write the tuple type.
static void writeTupleFields(
    coType type)
{
    coField field;

    coForeachTypeField(type, field) {
        if(field != coTypeGetFirstField(type)) {
            coPrint(", ");
        }
        writeType(coFieldGetType(field));
        coPrintW("    ", " %s", coFieldGetName(field));
    } coEndTypeField;
}

// Write the type definition.
static void writeType(
    coType type)
{
    switch(coTypeGetValtype(type)) {
    case CO_INT:
        coPrintW("    ", "int%u", coTypeGetWidth(type));
        break;
    case CO_UINT:
        coPrintW("    ", "int%u", coTypeGetWidth(type));
        break;
    case CO_STRING:
        coPrintW("    ", "char *");
        break;
    case CO_FLOAT:
        coPrintW("    ", "float");
        break;
    case CO_DOUBLE:
        coPrintW("    ", "double");
        break;
    case CO_BOOL:
        coPrintW("    ", "bool");
        break;
    case CO_UNBOUND:
        coPrintW("    ", "%s", utSymGetName(coTypeGetNameVal(type)));
        break;
    case CO_TUPLE:
        coPrintW("    ", "struct {");
        writeTupleFields(type);
        coPrint("}");
        break;
    default:
        utExit("unknown type");
    }
    if(coTypeArray(type)) {
        coPrint(" *");
    }
}

// Declare a typedef.
static void declareTypedef(
    coTypedef theTypedef)
{
    coPrint("typedef ");
    writeType(coTypedefGetType(theTypedef));
    coPrintlnW("   ", " %s%s;", coPrefix, coTypedefGetName(theTypedef));
}

// Declare the named typedefs.
static void declareNamedTypedefs(void)
{
    coTypedef theTypedef;

    coForeachRootTypedef(coTheRoot, theTypedef) {
        declareTypedef(theTypedef);
    } coEndRootTypedef;
}

// Convert tuple types to typedefs and declare them.
static coType declareUnnamedTypedef(
    utSym typeName,
    coType type)
{
    coTypedef theTypedef = coTypedefCreate(typeName, type);
    coType newType = coIdentTypeCreate(typeName);

    declareTypedef(theTypedef);
    return newType;
}

// Declare typedefs for all unnamed tuples in the function.
static void declareUnnamedTypedefs(
    coFunction function)
{
    coField field;
    coType returnType, paramType;
    utSym retTypeName, paramTypeName;

    returnType = coFunctionGetReturnType(function);
    if(returnType != coTypeNull && coTypeGetValtype(returnType) == CO_TUPLE) {
        retTypeName = utSymCreateFormatted("%sRetval",
            coCapitalize(coFunctionGetName(function)));
        returnType = declareUnnamedTypedef(retTypeName, returnType);
        coFunctionSetReturnType(function, returnType);
    }
    coForeachTypeField(coFunctionGetParameterType(function), field) {
        paramType = coFieldGetType(field);
        if(coTypeGetValtype(paramType) == CO_TUPLE) {
            paramTypeName = utSymCreateFormatted("%s%sVal",
                coCapitalize(coFunctionGetName(function)),
                coCapitalize(coFieldGetName(field)));
            paramType = declareUnnamedTypedef(paramTypeName, paramType);
            coFieldSetType(field, paramType);
        }
    } coEndTypeField;
}

// Declare a function.
static void declareFunction(
    coFunction function)
{
    coType returnType = coFunctionGetReturnType(function);
    coType paramType = coFunctionGetParameterType(function);

    if(returnType == coTypeNull) {
        coPrint("void");
    } else {
        writeType(returnType);
    }
    coPrint(" %s(", coFunctionGetName(function));
    if(coTypeGetFirstField(paramType) == coFieldNull) {
        coPrint("void");
    } else {
        writeTupleFields(paramType);
    }
    coPrintln(");");
}

// Declare functions.
static void declareFunctions(void)
{
    coFunction function;

    coForeachRootFunction(coTheRoot, function) {
        coPrintln("/* %s */", coFunctionGetDescription(function));
        declareUnnamedTypedefs(function);
        declareFunction(function);
    } coEndRootFunction;
}

// Generate the header file for functions the user must provide.
static void generateHFile(void)
{
    declareEnums();
    declareNamedTypedefs();
    declareFunctions();
}

// Generate code to parse commands and call the related function.
static void generateCFile()
{
    //TODO: do this
}

// Generate both the hearder file and command parser.
void coGenerateCommandParser(
    char *fileName,
    char *prefix)
{
    char *name = utSprintf("%s%s.h", prefix, fileName);

    coPrefix = prefix;
    coFile = fopen(name, "w");
    if(coFile == NULL) {
        utExit("Unable to open file %s for writing", name);
    }
    coLinePos = 0;
    generateHFile();
    fclose(coFile);
    name = utSprintf("%s%s.c", prefix, fileName);
    coFile = fopen(name, "w");
    if(coFile == NULL) {
        utExit("Unable to open file %s for writing", name);
    }
    coLinePos = 0;
    generateCFile();
    fclose(coFile);
}

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
        coPrintln("");
    } coEndRootEnum;
}

static void writeType(coTyperef typeref, uint16 depth);

// Indent by the number of depth levels.
static void indent(
    uint16 depth)
{
    while(depth--) {
        coPrint("    ");
    }
}

// Write the tuple type.
static void writeTupleFields(
    coType type,
    uint16 depth)
{
    coField field;

    coForeachTypeField(type, field) {
        indent(depth);
        writeType(coFieldGetTyperef(field), depth);
        coPrintln(" %s;", coFieldGetName(field));
    } coEndTypeField;
}

// Write function parameters.
static void writeParameters(
    coType type)
{
    coField field;

    coForeachTypeField(type, field) {
        if(coTypeGetFirstField(type) != field) {
            coPrintW("    ", ", ");
        }
        writeType(coFieldGetTyperef(field), 1);
        coPrintW("    ", " %s", coFieldGetName(field));
    } coEndTypeField;
}

// Write the type definition.
static void writeType(
    coTyperef typeref,
    uint16 depth)
{
    coType type = coTyperefGetType(typeref);

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
    case CO_IDENT:
        coPrintW("    ", "%s%s", coPrefix, utSymGetName(coTypeGetNameVal(type)));
        break;
    case CO_TUPLE:
        coPrintln("struct {");
        writeTupleFields(type, depth + 1);
        indent(depth);
        coPrint("}");
        break;
    case CO_POINTER:
        writeType(coTypeGetTyperefVal(type), depth);
        coPrintW("    ", " *");
        break;
    default:
        utExit("unknown type");
    }
}

// Declare a typedef.
static void declareTypedef(
    coTypedef theTypedef)
{
    coPrint("typedef ");
    writeType(coTypedefGetTyperef(theTypedef), 0);
    coPrintlnW("   ", " %s%s;", coPrefix, coTypedefGetName(theTypedef));
    coPrintln("");
}

// Declare the typedefs.
static void declareTypedefs(void)
{
    coTypedef theTypedef;

    coForeachRootTypedef(coTheRoot, theTypedef) {
        declareTypedef(theTypedef);
    } coEndRootTypedef;
}

// Declare a function.
static void declareFunction(
    coFunction function)
{
    coTyperef returnType = coFunctionGetReturnTyperef(function);
    coType paramType = coTyperefGetType(coFunctionGetParameterTyperef(function));

    if(returnType == coTyperefNull) {
        coPrint("void");
    } else {
        writeType(returnType, 0);
    }
    coPrint(" %s(", coFunctionGetName(function));
    if(coTypeGetFirstField(paramType) == coFieldNull) {
        coPrint("void");
    } else {
        writeParameters(paramType);
    }
    coPrintln(");");
    coPrintln("");
}

// Declare functions.
static void declareFunctions(void)
{
    coFunction function;

    coForeachRootFunction(coTheRoot, function) {
        coPrintln("/* %s */", coFunctionGetDescription(function));
        declareFunction(function);
    } coEndRootFunction;
}

// Generate the header file for functions the user must provide.
static void generateHFile(void)
{
    declareEnums();
    declareTypedefs();
    declareFunctions();
}

// Create a wrapper for the function.  These things take argc argv, and fill out
// the various structures needed by the user's function.
static void writeFunctionWrapper(
    coFunction function)
{
    
}

// Generate code to parse commands and call the related function.
static void generateCFile(
    char *HName)
{
    coFunction function;

    coPrintln("#include <ddutil.h>");
    coPrintln("#include \"%s\"", HName);
    coPrintln("#include \"rpclite.h\"\n");
    coForeachRootFunction(coTheRoot, function) {
        writeFunctionWrapper(function);
    } coEndRootFunction;
    coPrint(
        "/* Register functions */\n"
        "void rlRegisterFunctions(\n"
        "    rlServer server)\n"
        "{\n");
    coForeachRootFunction(coTheRoot, function) {
        coPrintln("    rlServerRegisterFunction(server, \"%s\", coFunctionGetName(function));");
    } coEndRootFunction;
    coPrintln("}");
}

// Generate both the hearder file and command parser.
void coGenerateCommandParser(
    char *fileName,
    char *prefix)
{
    char *HName = utAllocString(utReplaceSuffix(fileName, ".h"));
    char *CName = utAllocString(utReplaceSuffix(fileName, ".c"));

    coPrefix = prefix;
    coFile = fopen(HName, "w");
    if(coFile == NULL) {
        utExit("Unable to open file %s for writing", HName);
    }
    coLinePos = 0;
    generateHFile();
    fclose(coFile);
    coFile = fopen(CName, "w");
    if(coFile == NULL) {
        utExit("Unable to open file %s for writing", CName);
    }
    coLinePos = 0;
    generateCFile(HName);
    fclose(coFile);
    utFree(HName);
    utFree(CName);
}

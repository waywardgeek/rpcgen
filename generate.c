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
        coPrintln("// %s enumerated type", coEnumGetName(theEnum));
        coPrintln("typedef enum {");
        coForeachEnumEntry(theEnum, entry) {
            coPrint("    %s", coEntryGetName(entry));
            if(coEntryGetNextEnumEntry(entry) != coEntryNull) {
                coPrint(",");
            }
            coPrintln("");
        } coEndEnumEntry;
        coPrintln("} %s%s;\n\n", coPrefix, coEnumGetName(theEnum));
    } coEndRootEnum;
}


// Declare the typedefs.
//declareTypedefs();

//declareFunctions();

// Generate the header file for functions the user must provide.
static void generateHFile(void)
{
    declareEnums();
    //declareTypedefs();
    //declareUnnamedTypedefs();
    //declareFunctions();
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

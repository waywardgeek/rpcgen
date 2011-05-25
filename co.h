#include <ddutil.h>
#include "codatabase.h"

// Main routines
bool coParseCommandFile(char *fileName);
void coGenerateCommandParser(char *fileName, char *prefix);
void coPreprocessDataTypes(void);

// Shortcuts and utilities
char *coCapitalize(char *string);

// Section methods.
coSection coSectionCreate(utSym name);

// Typedef methods.
coTypedef coTypedefCreate(utSym name, coTyperef typeref);

// Type methods.
coTyperef coBasicTypeCreate(coValtype valtype);
coTyperef coIntTypeCreate(coValtype valtype, uint8 width);
coTyperef coIdentTypeCreate(utSym name);
coType coTupleTypeCreate(void);
coTyperef coArrayTypeCreate(coTyperef typeref);
coTyperef coPointerTypeCreate(coTyperef typeref);
void coAddFieldToTuple(coType tuple, coTyperef typeref, utSym name, coValue defaultValue);
coTyperef coHashTupleType(coType type);

// Enum and entry methods.
coEnum coEnumCreate(utSym name);
coEntry coEntryCreate(coEnum Enum, utSym name);

// Value methods.
coValue coIntValueCreate(int64 val);
coValue coStringValueCreate(utSym val);
coValue coFloatValueCreate(double val);
coValue coBoolValueCreate(bool val);
coValue coEntryValueCreate(utSym val);
coValue coTupleValueCreate(coValueArray vals);
coValue coArrayValueCreate(coValueArray vals);

// Function methods.
coFunction coFunctionCreate(utSym name, coTyperef returnType, coTyperef parameterType);

extern coRoot coTheRoot;

// Lex, Yacc stuff */
extern FILE *coFile;
extern uint32 coFileSize, coLineNum;
extern int coparse();
extern int colex();
extern void coerror(char *message, ...);
extern char *cotext;

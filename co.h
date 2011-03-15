#include <ddutil.h>
#include "codatabase.h"

// Main routines
bool coParseCommandFile(char *fileName);
void coGenerateCommandParser(char *fileName, char *prefix);

// Shortcuts and utilities
char *coCapitalize(char *string);

// Section methods.
coSection coSectionCreate(utSym name);

// Typedef methods.
coTypedef coTypedefCreate(utSym name, coType type);

// Type methods.
coType coBasicTypeCreate(coValtype valtype);
coType coIdentTypeCreate(utSym name);
coType coTupleTypeCreate(void);
void coAddFieldToTuple(coType tuple, coType type, utSym name, coValue defaultValue);

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
coFunction coFunctionCreate(utSym name, coType returnType, coType parameterType);

extern coRoot coTheRoot;

// Lex, Yacc stuff */
extern FILE *coFile;
extern uint32 coFileSize, coLineNum;
extern int coparse();
extern int colex();
extern void coerror(char *message, ...);
extern char *cotext;

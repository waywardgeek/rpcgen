#include <ddutil.h>
#include "codatabase.h"

// Section methods
coSection coSectionCreate(utSym name);

// Lex, Yacc stuff */
extern FILE *coFile;
extern uint32 coFileSize, coLineNum;
extern int coparse();
extern int colex();
extern void coerror(char *message, ...);
extern char *cotext;

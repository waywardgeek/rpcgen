#include "co.h"

coRoot coTheRoot;

// Main entry point.  Process parameters and call the generators.
int main(
    int argc,
    char *argv[])
{
    char *exeName;

    if(argc != 3) {
        fprintf(stderr, "Usage: %s prefix commandFile\n", argv[0]);
        return 1;
    }
    utStart();
    exeName = utReplaceSuffix(utBaseName(argv[0]), "");
    utInitLogFile(utSprintf("%s.log", exeName));
    coDatabaseStart();
    coTheRoot = coRootAlloc();
    coParseCommandFile(argv[2]);
    coPreprocessDataTypes();
    coGenerateCommandParser(argv[2], argv[1]);
    coDatabaseStop();
    utStop(false);
    return 0;
}

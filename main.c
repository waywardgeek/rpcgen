#include "co.h"

coRoot coTheRoot;

// Main entry point.  Process parameters and call the generators.
int main(
    int argc,
    char *argv[])
{
    if(argc != 2) {
        fprintf(stderr, "Usage: %s commandFile\n", argv[0]);
        return 1;
    }
    utStart();
    coDatabaseStart();
    coTheRoot = coRootAlloc();
    coParseCommandFile(argv[1]);
    coDatabaseStop();
    utStop(false);
    return 0;
}

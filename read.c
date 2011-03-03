#include "co.h"

FILE *coFile;
uint32 coFileSize, coLineNum;

// Parse a command definition file.
bool coParseCommandFile(
    char *fileName)
{
    coFile = fopen(fileName, "r");

    coLineNum = 1;
    if(coFile == NULL) {
        fprintf(stderr, "Unable to open file %s\n", fileName);
        return false;
    }
    if(coparse()) {
        fclose(coFile);
        return false;
    }
    fclose(coFile);
    return true;
}

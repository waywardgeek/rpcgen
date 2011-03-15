// Shortcuts and utilities.
#include "co.h"

// Just capaitalize the first letter of the string.
char *coCapitalize(
    char *string)
{
    char *copy = utCopyString(string);
    char c = *copy;
    if(c >= 'a' && c <= 'z')
        *copy += 'A' - 'a';
    return copy;
}

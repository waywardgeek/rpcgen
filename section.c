// Section methods.
#include "co.h"

// Create a section, which is used to group commands in help.
coSection coSectionCreate(
    utSym name)
{
    coSection section = coSectionAlloc();

    coSectionSetSym(section, name);
    coRootAppendSection(coTheRoot, section);
    return section;
}

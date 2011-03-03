/*
 * Copyright (C) 2011 Bill Cox
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program; if not, write to the Free Software 
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111 USA
 */

%{

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "co.h"
#include "coparse.h"

#define YY_NEVER_INTERACTIVE 1
#define YY_INPUT(buf,result,max_size) \
    if((result = coRead((char *) buf, max_size )) < 0 ) \
        YY_FATAL_ERROR( "input in flex scanner failed" );

static inline int cogetc(void)
{
    return getc(coFile);
}

static int coRead(char *buf, int maxSize)
{
    int numChars = 0;
    char c;

    do {
        c = cogetc();
        if(c != EOF && c != '\r') {
            *buf++ = c;
            numChars++;
        }
    } while(numChars < maxSize && c != EOF);
    return numChars;
}

static char *unmunge(
    char *string)
{
    char *buffer = utMakeString(strlen(string));
    char *p = buffer;

    string++; /* Skip " */
    while(*string != '\0') {
        if(*string == '\\') {
            string++;
        }
        *p++ = *string++;
    }
    *--p = '\0'; /* Overwrite terminating " */
    return buffer;
}

int cowrap(void)
{
    return 1;
}

#ifdef CO_DEBUG
#define myDebug utDebug
#define retKey(key) myDebug("key"); return key
#else
#define myDebug noDebug
#define retKey(key) return key
#endif
#undef YYLMAX
#define YYLMAX 4096

static void noDebug(char *foo, ...) {}

%}

%option nounput
%option noinput

%%
^[ \t]*\n                       { coLineNum++; }
[ \t]+                          ;

"bool"                          {retKey(KWBOOL);}
"char"                          {retKey(KWCHAR);}
"enum"                          {retKey(KWENUM);}
"int"                           {retKey(KWINT);}
"float"                         {retKey(KWFLOAT);}
"string"                        {retKey(KWSTRING);}
"typedef"                       {retKey(KWTYPEDEF);}
"section"                       {retKey(KWSECTION);}
"true"                          {retKey(KWTRUE);}
"false"                         {retKey(KWFALSE);}

\n                              { coLineNum++; retKey('\n'); }
[0-9]+                          { colval.intVal = atoll(cotext);
                                  myDebug("INTEGER %u\n", colval.intVal);
                                  return INTEGER; }
[0-9]+(\.[0-9]*)?([eE][+-]?[0-9]+)? { colval.floatVal = atof(cotext);
                                  myDebug("FLOAT %f\n", colval.floatVal);
                                  return FLOAT; }
\.[0-9]+([eE][+-]?[0-9]+)?      { colval.floatVal = atof(cotext);
                                  myDebug("FLOAT %f\n", colval.floatVal);
                                  return FLOAT; }
[a-zA-Z][a-zA-Z0-9_]*           { myDebug("IDENT %s\n", cotext);
                                  colval.symVal = utSymCreate(cotext);
                                  return IDENT; }
\"([^"]|\\.)*\"                 { myDebug("STRING %s\n", cotext);
                                  colval.stringVal = utSymCreate(unmunge(cotext));
                                  return STRING; }
.                               { myDebug("Char '%c'\n", cotext[0]);
                                  return cotext[0]; }

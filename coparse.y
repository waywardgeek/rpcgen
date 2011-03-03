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

#include "co.h"

static coSection coCurrentSection;

/*--------------------------------------------------------------------------------------------------
  Provide yyerror function capability.
--------------------------------------------------------------------------------------------------*/
void coerror(
    char *message,
    ...)
{
    char *buff;
    va_list ap;

    va_start(ap, message);
    buff = utVsprintf(message, ap);
    va_end(ap);
    utError("Line %d, token \"%s\": %s", coLineNum, cotext, buff);
}

%}

%union {
    utSym symVal;
    utSym stringVal;
    int64 intVal;
    double floatVal;
};

%token <symVal> IDENT
%token <stringVal> STRING
%token <intVal> INTEGER
%token <floatVal> FLOAT

%token KWBOOL
%token KWCHAR
%token KWENUM
%token KWFLOAT
%token KWINT
%token KWSTRING
%token KWTYPEDEF
%token KWSECTION
%token KWTRUE
%token KWFALSE

%%

goal: initialize commands
;

initialize: /* Empty */
{
    coCurrentSection = coSectionNull;
}

commands: /* Empty */
| commands command
;

command: section
| typedef
| enum
| function
| '\n'
;

section: KWSECTION STRING '\n'
//{
//    coCurrentSection = coSectionCreate($2);
//}

typedef: KWTYPEDEF type IDENT '\n'

enum: KWENUM IDENT '(' entries ')' '\n'
;

entries: // Empty
| oneOrMoreEntries
;

oneOrMoreEntries: IDENT
| oneOrMoreEntries ',' optNewline IDENT
;

optNewline: // Empty
| '\n'
;

function: STRING '\n' functionDef '\n'
;

functionDef: optType IDENT '(' parameterList ')'
;

optType: // Empty
| type
;

parameterList: // Empty
| oneOrMoreParameters
;

oneOrMoreParameters: parameter
| oneOrMoreParameters ',' optNewline parameter
;

parameter: type IDENT optDefaultValue
;

optDefaultValue: // Empty
| '=' value
;

type: IDENT // Enum or Typedef
| KWINT
| KWSTRING
| KWFLOAT
| KWBOOL
| tupleType
| arrayType

tupleType: '{' namedTypeList '}'
;

namedTypeList: type IDENT
| namedTypeList ',' optNewline type IDENT
;

arrayType: '[' type ']'
;

value: INTEGER
| STRING
| FLOAT
| KWTRUE
| KWFALSE
| IDENT // Entry value
| '{' valueList '}'
| '[' valueList ']'
;

valueList: value
| valueList ',' optNewline value
;

%%

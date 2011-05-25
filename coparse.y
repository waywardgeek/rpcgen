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
static coEnum coCurrentEnum;
static coFunction coCurrentFunction;

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
    utExit("Line %d, token \"%s\": %s", coLineNum, cotext, buff);
}

%}

%union {
    utSym symVal;
    utSym stringVal;
    int64 intVal;
    double floatVal;
    coTyperef typerefVal;
    coType typeVal;
    coValue valueVal;
    coValueArray valueArrayVal;
};

%token <symVal> IDENT
%token <stringVal> STRING
%token <intVal> INTEGER KWINT KWUINT
%token <floatVal> FLOAT

%type <typerefVal> type tupleType arrayType fieldList
%type <typeVal> oneOrMoreFields
%type <valueVal> value optDefaultValue
%type <valueArrayVal> valueList

%token KWBOOL
%token KWCHAR
%token KWENUM
%token KWFLOAT
%token KWDOUBLE
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
    coCurrentEnum = coEnumNull;
    coCurrentFunction = coFunctionNull;
}

commands: /* Empty */
| commands command
;

command: section
| typedef
| enum
| function
;

section: KWSECTION STRING
{
   coCurrentSection = coSectionCreate($2);
}

typedef: KWTYPEDEF type IDENT
{
    coTypedefCreate($3, $2);
}

enum: enumHeader '(' entries ')'
{
    coCurrentEnum = coEnumNull;
}
;


enumHeader:  KWENUM IDENT
{
    coCurrentEnum = coEnumCreate($2);
}

entries: // Empty
| oneOrMoreEntries
;

oneOrMoreEntries: IDENT
{
    coEntryCreate(coCurrentEnum, $1);
}
| oneOrMoreEntries ',' IDENT
{
    coEntryCreate(coCurrentEnum, $3);
}
;

function: STRING functionDef
{
    char *description = utSymGetName($1);
    coFunctionSetDescription(coCurrentFunction, description, strlen(description) + 1);
    coCurrentFunction = coFunctionNull;
}
;

functionDef: IDENT '(' fieldList ')'
{
    coCurrentFunction = coFunctionCreate($1, coTyperefNull, $3);
}
| type IDENT '(' fieldList ')'
{
    coCurrentFunction = coFunctionCreate($2, $1, $4);
}
;

optDefaultValue: // Empty
{
    $$ = coValueNull;
}
| '=' value
{
    $$ = $2;
}
;

type: IDENT // Enum or Typedef
{
    $$ = coIdentTypeCreate($1);
}
| KWINT
{
    $$ = coIntTypeCreate(CO_INT, $1);
}
| KWUINT
{
    $$ = coIntTypeCreate(CO_UINT, $1);
}
| KWSTRING
{
    $$ = coBasicTypeCreate(CO_STRING);
}
| KWFLOAT
{
    $$ = coBasicTypeCreate(CO_FLOAT);
}
| KWDOUBLE
{
    $$ = coBasicTypeCreate(CO_DOUBLE);
}
| KWBOOL
{
    $$ = coBasicTypeCreate(CO_BOOL);
}
| tupleType
| arrayType
;

tupleType: '{' fieldList '}'
{
    $$ = $2;
}
;

fieldList: // Empty
{
    coType type = coTupleTypeCreate();
    $$ = coHashTupleType(type);
}
| oneOrMoreFields
{
    $$ = coHashTupleType($1);
}
;

oneOrMoreFields: type IDENT optDefaultValue
{
    $$ = coTupleTypeCreate();
    coAddFieldToTuple($$, $1, $2, $3);
}
| oneOrMoreFields ',' type IDENT optDefaultValue
{
    coAddFieldToTuple($1, $3, $4, $5);
    $$ = $1
}
;

arrayType: '[' type ']'
{
    $$ = coArrayTypeCreate($2);
}
;

value: INTEGER
{
    $$ = coIntValueCreate($1);
}
| STRING
{
    $$ = coStringValueCreate($1);
}
| FLOAT
{
    $$ = coFloatValueCreate($1);
}
| KWTRUE
{
    $$ = coBoolValueCreate(true);
}
| KWFALSE
{
    $$ = coBoolValueCreate(false);
}
| IDENT // Entry value
{
    $$ = coEntryValueCreate($1);
}
| '{' valueList '}'
{
    $$ = coTupleValueCreate($2);
}
| '[' valueList ']'
{
    $$ = coArrayValueCreate($2);
}
;

valueList: value
{
    $$ = coValueArrayAlloc();
    coValueArrayAppendValue($$, $1);
}
| valueList ',' value
{
    coValueArrayAppendValue($1, $3);
}
;

%%

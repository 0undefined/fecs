%{
#include <stdlib.h>
#include <string.h>

#include "tokens.h"
%}

%%

"struct"    { return STRUCT; }
"component" { return COMPONENT; }
"type"      { return TYPE; }
"function"  { return FUNCTION; }
"system"    { return SYSTEM; }

"("         { return LPAR; }
")"         { return RPAR; }
"{"         { return LCURLY; }
"}"         { return RCURLY; }
"["         { return LBRACK; }
"]"         { return RBRACK; }

":"           	{ return COLON; }
"="           	{ return EQUAL; }

"++"          	{ return CONCAT; }
"+"           	{ return PLUS; }
"-"           	{ return MINUS; }
"*"           	{ return ASTERISK; }
"/"           	{ return SLASH; }
"."           	{ return DOT; }

INT           	{ return INT; }
FLOAT         	{ return FLOAT; }
BOOL          	{ return BOOL; }
DATATYPE      	{ return DATATYPE; }

VNAME         	{ return VNAME; }
%%

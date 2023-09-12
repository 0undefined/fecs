/* Disable some functions */
%option nounput noinput noyywrap
%option noyyalloc noyyrealloc noyyfree

/* Replace "yy" prefix with "fecs" in exported functions */
%option prefix="fecs"

/* Generate thread safe code */
%option reentrant bison-bridge

%{
#include <stdlib.h>
#include <string.h>

#include "parser.tab.h"

#define YY_EXIT_FAILURE ((void)yyscanner, EXIT_FAILURE)

#define _XOPEN_SOURCE 600
#include <limits.h>
#include <stdlib.h>
#include <string.h>

// Maybe unnecessary?
#define YYSTYPE FECSSTYPE

int lisperror(const char *msg);
%}

%%

"("         { return LPAR; }
")"         { return RPAR; }
"{"         { return LCURLY; }
"}"         { return RCURLY; }
"["         { return LBRACK; }
"]"         { return RBRACK; }

"//".*         { }

[ \r\t]     { /* do nothing */ }

"struct"    { return STRUCT; }
"component" { return COMPONENT; }
"type"      { return TYPE; }
"function"  { return FUNCTION; }
"system"    { return SYSTEM; }

("/\\"|"&&") { return LAND; }
("\\/"|"||") { return LOR; }
("!"|"¬")    { return LNEG; }

"++"          	{ return CONCAT; }

":"           	{ return COLON; }
"="           	{ return EQUAL; }

"+"           	{ return PLUS; }
"-"           	{ return MINUS; }
"*"           	{ return ASTERISK; }
"/"           	{ return SLASH; }
"."           	{ return DOT; }

"true"          	{ return TRUE; }
"false"          	{ return FALSE; }

[a-zA-Z_][a-zA-Z0-9_']* { yylval->vname = strdup(yytext); return VNAME; }

[\n]+         { /* Ignored */ }
%%

// TODO:  Move these patterns back up

/*-?[0-9]+\.[0-9]+ { yylval.floating = strdup(yytext); return FLOAT;  }*/
/*-?[0-9]+        { yylval.num = strdup(yytext); return INT; }*/



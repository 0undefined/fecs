/* Disable some functions */
%option noyywrap nounput noinput
/*%option noyyalloc noyyrealloc noyyfree*/

/* Replace "yy" prefix with "fecs" in exported functions */
%option prefix="fecs"

/* Generate thread safe code */
/*%option reentrant bison-bridge*/

%option   8bit reentrant bison-bridge
%option   warn nodefault
%option   yylineno
%option   outfile="lex.fecs.c" header-file="lex.fecs.h"


%{
#include "fecs.tab.h"

//#define YY_EXIT_FAILURE ((void)yyscanner, EXIT_FAILURE)

#define _XOPEN_SOURCE 600
#include <limits.h>
#include <stdlib.h>
#include <string.h>

// Maybe unnecessary?
#define YYSTYPE FECSSTYPE

int fecserror(const char *msg);
void set_input_string(const char* in, yyscan_t scanner);
void end_lexical_scan(YY_BUFFER_STATE b, yyscan_t scanner);

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

"struct"    { return STRUCTDEF; }
"component" { return COMPONENTDEF; }
"type"      { return TYPEDEF; }
"function"  { return FUNCTIONDEF; }
"system"    { return SYSTEMDEF; }

("/\\"|"&&") { return LAND; }
("\\/"|"||") { return LOR; }
("!"|"¬")    { return LNEG; }

"++"         { return CONCAT; }

"#!"         { return SHEBANG; }
"#include"   { return PRAGMA_IMPORT; /* #include C-library */ }
"#load"      { return PRAGMA_LOAD; /* Load other fecs source */ }

":"          { return COLON; }
"="          { return EQUAL; }

"+"          { return PLUS; }
"-"          { return MINUS; }
"*"          { return ASTERISK; }
"/"          { return SLASH; }
"."          { return DOT; }

"true"       { return TRUE; }
"false"      { return FALSE; }

"i8"         { return I8;    }
"i16"        { return I16;   }
"i32"        { return I32;   }
"i64"        { return I64;   }
"u8"         { return U8;    }
"u16"        { return U16;   }
"u32"        { return U32;   }
"u64"        { return U64;   }
"f32"        { return F32;   }
"f64"        { return F64;   }
"usize"      { return USIZE; }
"isize"      { return ISIZE; }
"bool"       { return BOOL;  }

[a-zA-Z_][a-zA-Z0-9_']* { yylval->vname = strdup(yytext); return VNAME; }

[\n]+         { /* Ignored */ }

%%

void
set_input_string(const char* in, yyscan_t scanner) {
  yy_scan_string(in, scanner);
}

void
end_lexical_scan(YY_BUFFER_STATE b, yyscan_t scanner) {
  yy_delete_buffer(b, scanner);
}

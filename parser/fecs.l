/* Disable some functions */
%option noyywrap nounput noinput
/*%option noyyalloc noyyrealloc noyyfree*/

/* Replace "yy" prefix with "fecs" in exported functions */
%option prefix="fecs"

/* Generate thread safe code */
%option reentrant bison-bridge

%option   8bit reentrant bison-bridge
%option   warn nodefault
%option   yylineno
%option   outfile="lex.fecs.c" header-file="lex.fecs.h"

%{
#include "fecs.tab.h"
#include "util.h"

#define YY_EXIT_FAILURE ((void)yyscanner, EXIT_FAILURE)

#define _XOPEN_SOURCE 600
#include <limits.h>
#include <stdlib.h>
#include <string.h>

// Maybe unnecessary?
#define YYSTYPE FECSSTYPE

int fecserror(const char *msg);
void set_input_string(const char* in, yyscan_t scanner);
void end_lexical_scan(YY_BUFFER_STATE b, yyscan_t scanner);
Value categorize_int(char* valuestring, size_t len);
FECSTOKENTYPE valuetypetoken_from_value(Value v);

%}

%%

"#!".*"\n"  { puts("shebang"); return SHEBANG; }
"#include"  { puts("include"); return PRAGMA_IMPORT; /* #include C-library */ }
"#load"     { puts("load"); return PRAGMA_LOAD; /* Load other fecs source */ }

"->"        { puts("ARROW"); return ARROW; }

"("         { puts("LPAR"); return LPAR; }
")"         { puts("RPAR"); return RPAR; }
"{"         { puts("LBRACE"); return LBRACE; }
"}"         { puts("RBRACE"); return RBRACE; }
"["         { puts("LBRACKET"); return LBRACKET; }
"]"         { puts("RBRACKET"); return RBRACKET; }
"<"         { puts("LT"); return LT; }
">"         { puts("GT"); return GT; }

"//".*\n      { /* comment */ }

"struct"    { puts("STRUCTDEF"); return STRUCTDEF; }
"component" { puts("COMPONENTDEF"); return COMPONENTDEF; }
"type"      { puts("TYPEDEF"); return TYPEDEF; }
"function"  { puts("FUNCTIONDEF"); return FUNCTIONDEF; }
"system"    { puts("SYSTEMDEF"); return SYSTEMDEF; }

("/\\"|"&&") { puts("LAND"); return LAND; }
("\\/"|"||") { puts("LOR"); return LOR; }
("!"|"¬")    { puts("NEG"); return NEG; }

"++"         { puts("CONCAT"); return CONCAT; }

":"          { puts("COLON"); return COLON; }
";"          { puts("SEMICOLON"); return SEMICOLON; }
"="          { puts("EQUAL"); return EQUAL; }

"+"          { puts("PLUS"); return PLUS; }
"-"          { puts("MINUS"); return MINUS; }
"*"          { puts("ASTERISK"); return ASTERISK; }
"/"          { puts("SLASH"); return SLASH; }
"."          { puts("DOT"); return DOT; }

"true"       { puts("TRUE"); return TRUE; }
"false"      { puts("FALSE"); return FALSE; }

"i8"         { puts("I8"); return I8;     }
"i16"        { puts("I16"); return I16;    }
"i32"        { puts("I32"); return I32;    }
"i64"        { puts("I64"); return I64;    }
"u8"         { puts("U8"); return U8;     }
"u16"        { puts("U16"); return U16;    }
"u32"        { puts("U32"); return U32;    }
"u64"        { puts("U64"); return U64;    }
"f32"        { puts("F32"); return F32;    }
"f64"        { puts("F64"); return F64;    }
"bool"       { puts("BOOL"); return BOOL;   }
"usize"      { puts("USIZE"); return USIZE;  }
"isize"      { puts("ISIZE"); return ISIZE;  }
"string"     { puts("STRING"); return STRING; }
"list"       { puts("LIST"); return LIST;   }

"-"?[0-9]([_0-9]+)?([ui](8|16|32|64))? { puts("TYPEVAL"); yylval->value = categorize_int(yytext, yyleng); return valuetypetoken_from_value(yylval->value); }

[a-zA-Z_][a-zA-Z0-9_']* { puts("VNAME"); yylval->vname = strdup(yytext); return VNAME; }

[ \r\t\n]+     { printf(" S ");/* do nothing */ }

%%

void
set_input_string(const char* in, yyscan_t scanner) {
  yy_scan_string(in, scanner);
}

void
end_lexical_scan(YY_BUFFER_STATE b, yyscan_t scanner) {
  yy_delete_buffer(b, scanner);
}

Value categorize_int(char* valuestring, size_t len) {
  Value v;
  bool signed_type = valuestring[0] == '-';

  /* TOOD: stop this lazy bs, assume a lower type, unless specific type-suffix
   * is given. */
  if (signed_type) {
    v.type = Type_i64;
    v.value.i64_t.v = atoll(valuestring);
  }
  else {
    v.type = Type_u64;
    v.value.i64_t.v = strtoull(valuestring, valuestring + len, 10);
  }

  return v;
}

FECSTOKENTYPE valuetypetoken_from_value(Value v) {
  switch (v.type) {
    case Type_i8:        return     I8VAL;
    case Type_i16:       return     I16VAL;
    case Type_i32:       return     I32VAL;
    case Type_i64:       return     I64VAL;
    case Type_u8:        return     U8VAL;
    case Type_u16:       return     U16VAL;
    case Type_u32:       return     U32VAL;
    case Type_u64:       return     U64VAL;
    case Type_f32:       return     F32VAL;
    case Type_f64:       return     F64VAL;

    case Type_isize:     return     ISIZEVAL;
    case Type_usize:     return     USIZEVAL;

    default: die("We fucked up parsing!");
  }
  return FECSerror;
}

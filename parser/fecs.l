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

"#!".*"\n"  { return SHEBANG; }
"#include"  { return PRAGMA_IMPORT; /* #include C-library */ }
"#load"     { return PRAGMA_LOAD; /* Load other fecs source */ }

"->"        { return ARROW; }

"("         { return LPAR; }
")"         { return RPAR; }
"{"         { return LBRACE; }
"}"         { return RBRACE; }
"["         { return LBRACKET; }
"]"         { return RBRACKET; }
"<"         { return LT; }
">"         { return GT; }

"//".*\n      { /* comment */ }

"struct"    { return STRUCTDEF; }
"component" { return COMPONENTDEF; }
"type"      { return TYPEDEF; }
"function"  { return FUNCTIONDEF; }
"system"    { return SYSTEMDEF; }

("/\\"|"&&") { return LAND; }
("\\/"|"||") { return LOR; }
("!"|"¬")    { return NEG; }

"++"         { return CONCAT; }

":"          { return COLON; }
";"          { return SEMICOLON; }
"="          { return EQUAL; }

"+"          { return PLUS; }
"-"          { return MINUS; }
"*"          { return ASTERISK; }
"/"          { return SLASH; }
"."          { return DOT; }

"true"       { return TRUE; }
"false"      { return FALSE; }

"i8"         { return I8;     }
"i16"        { return I16;    }
"i32"        { return I32;    }
"i64"        { return I64;    }
"u8"         { return U8;     }
"u16"        { return U16;    }
"u32"        { return U32;    }
"u64"        { return U64;    }
"f32"        { return F32;    }
"f64"        { return F64;    }
"bool"       { return BOOL;   }
"usize"      { return USIZE;  }
"isize"      { return ISIZE;  }
"string"     { return STRING; }
"list"       { return LIST;   }

[0-9]+(i8|i16|i32|i64) { yylval->value = categorize_int(yytext, yyleng); return valuetypetoken_from_value(yylval->value); }

[a-zA-Z_][a-zA-Z0-9_']* { yylval->vname = strdup(yytext); return VNAME; }

[ \r\t\n]+     { /* printf(" S "); */  /* do nothing */ }

%%

//"-"?[0-9]([_0-9]+)?([ui](8|16|32|64))? { printf("TYPEVAL(%s)", yytext); yylval->value = categorize_int(yytext, yyleng); return valuetypetoken_from_value(yylval->value); }

void
set_input_string(const char* in, yyscan_t scanner) {
  yy_scan_string(in, scanner);
}

void
end_lexical_scan(YY_BUFFER_STATE b, yyscan_t scanner) {
  yy_delete_buffer(b, scanner);
}

/* Returns the index of `needle`, if needle is a suffix of src */
int is_suffix(char *restrict src, const size_t srclen, char* needle) {
  isize nlen = strlen(needle);
  if (srclen < nlen) return -1;

  isize i = 0;
  while (i < nlen && src[srclen - i] == needle[nlen - i]) {
    i++;
  }

  if (i == nlen) return srclen - nlen;

  return -1;
}

Value assign(Types_t t, char* val) {
  switch (t) {
  case Type_i8:   return (Value){.type = t, .value.i8_t.v  = atoi(val)};
  case Type_i16:  return (Value){.type = t, .value.i16_t.v = atoi(val)};
  case Type_i32:  return (Value){.type = t, .value.i32_t.v = atoi(val)};
  case Type_i64:  return (Value){.type = t, .value.i64_t.v = atoll(val)};

  case Type_u8:   return (Value){.type = t, .value.u8_t.v  = atoi(val)};
  case Type_u16:  return (Value){.type = t, .value.u16_t.v = atoi(val)};
  case Type_u32:  return (Value){.type = t, .value.u32_t.v = atoi(val)};
  case Type_u64:  return (Value){.type = t, .value.u64_t.v = atoll(val)};
  default:
    die("Assign not implemented for type");
  }
}

Value categorize_int(char* valuestring, size_t len) {
  Value v = (Value){.type = Type_untyped};
  bool signed_type = valuestring[0] == '-';

  /* Check if `valuestring` has a suffix of a type. */
  {
    int i = 0;
    int needle = -1;
    do {
      needle = is_suffix(valuestring, len, Types_str[++i]);
    } while (needle < 0 && i < Type_MAX);

    if (needle >= 0) {
      valuestring[needle] = '\0';
      v = assign(i, valuestring);
    }
  }

  /* For now, just accept the type, if given. */
  /* TODO: Verify the range of the value. */
  if (v.type == Type_untyped) {
  if (signed_type) {
    v.type = Type_i64;
    v.value.i64_t.v = 42; //atoll(valuestring);
  }
  else {
    v.type = Type_u64;
    v.value.i64_t.v = 43; //strtoull(valuestring, valuestring + len, 10);
  }
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

/* Disable public variables cluttering everything up */
%define api.pure true

/* Rename prefix to avoid clashes */
%define api.prefix {fecs}

/* More usefull error messages*/
%define parse.error verbose

%code top {

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wchar.h>
#include <string.h>
#include <ctype.h>

//#include "ast.h"
//
//void yyerror(char* e);
//int yylex();
//int err = 0;

//Spec *specification = NULL;

#ifndef YYNOMEM
#define YYNOMEM goto yyexhaustedlab
#endif

}

%code requires {
#include "ast.h"
}

%parse-param { Spec **result }
%param { void *scanner }

%code provides {
  void spec_free(Spec *s);
}

%code {
 int fecserror(void *foo, char const *msg, const void *s);
 int fecslex(FECSSTYPE *lval , void *s);
}

%union {
  bool cst;
  DExpr def_expr;
  char* vname;
  Types_t type;
  Struct_t* struct_t;
  Attribute* attribute_t;
  Spec* specification;
}

%type <specification> start ast
%type <type> TYPE
%type <def_expr> dexpr
%type <struct_t> SDef
%type <attribute_t> Attribute
%type <vname> VNAME

%destructor { free($$); } <vname>
//%destructor { spec_free($$); } <specification>



%token SHEBANG PRAGMA_IMPORT PRAGMA_LOAD

%token LPAR RPAR LCURLY RCURLY LBRACK RBRACK COLON

%token LAND LOR LNEG

%token CONCAT
%left PLUS MINUS
%left ASTERISK SLASH DOT
%left EQUAL
%left STRUCTDEF COMPONENTDEF TYPEDEF FUNCTIONDEF SYSTEMDEF

%token I8 I16 I32 I64 U8 U16 U32 U64 F32 F64 USIZE ISIZE BOOL TUPLE RECORD
%token VNAME

%token <cst> TRUE FALSE

%%
start: ast {*result = $$ = $1; return 0;}
     ;

ast: ast dexpr { $$ = spec_push($1, $2); }
   | dexpr { Spec* tt = spec_new(); $$ = spec_push(tt, $1); }
   ;

dexpr: STRUCTDEF VNAME LCURLY SDef RCURLY
   { $$ = (DExpr){
       .type = DExpr_struct,
       .name = strdup($2),
       .exp.struct_t = $4,
     };
     /*printf("vname:%s\n", $2);*/
   }
   ;

Attribute:
  TYPE VNAME { $$ = attribute_new($1, $2); /*printf("attribute: "); print_attrib($$);*/ }
  ;

TYPE:
    I8      { $$ = Type_i8;     /* printf("Found \"%s\"\n", Types_str[$$]); */}
  | I16     { $$ = Type_i16;    /* printf("Found \"%s\"\n", Types_str[$$]); */}
  | I32     { $$ = Type_i32;    /* printf("Found \"%s\"\n", Types_str[$$]); */}
  | I64     { $$ = Type_i64;    /* printf("Found \"%s\"\n", Types_str[$$]); */}
  | U8      { $$ = Type_u8;     /* printf("Found \"%s\"\n", Types_str[$$]); */}
  | U16     { $$ = Type_u16;    /* printf("Found \"%s\"\n", Types_str[$$]); */}
  | U32     { $$ = Type_u32;    /* printf("Found \"%s\"\n", Types_str[$$]); */}
  | U64     { $$ = Type_u64;    /* printf("Found \"%s\"\n", Types_str[$$]); */}
  | F32     { $$ = Type_f32;    /* printf("Found \"%s\"\n", Types_str[$$]); */}
  | F64     { $$ = Type_f64;    /* printf("Found \"%s\"\n", Types_str[$$]); */}
  | BOOL    { $$ = Type_bool;   /* printf("Found \"%s\"\n", Types_str[$$]); */}
  | USIZE   { $$ = Type_usize;  /* printf("Found \"%s\"\n", Types_str[$$]); */}
  | ISIZE   { $$ = Type_isize;  /* printf("Found \"%s\"\n", Types_str[$$]); */}
  | TUPLE   { $$ = Type_tuple;  /* printf("Found \"%s\"\n", Types_str[$$]); */}
  | RECORD  { $$ = Type_record; /* printf("Found \"%s\"\n", Types_str[$$]); */}
  | VNAME   { $$ = Type_alias;  /* printf("Found \"%s\"\n", Types_str[$$]); */}
  ;





SDef:
    SDef Attribute { $$ = struct_add_attrib($1, $2); }
  | Attribute { $$ = struct_add_attrib(NULL, $1); }
  ;

%%

int
fecserror(void *yylval, char const *msg, const void *s) {
  //fprintf(stderr, "Error %d: %s\n", yylineno, s);
  (void)yylval;
  (void)s;
  fprintf(stderr, "Error %s\n", msg);
}

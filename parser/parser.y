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

#include "ast.h"

void yyerror(char* e);
int yylex();
int err = 0;

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
 int fecslex(void *lval, const void *s);
}

%union {
  bool cst;
  DExpr def_expr;
  char* vname;
  Struct_t* struct_t;
  Attribute* attribute_t;
  Spec* specification;
}

%type <specification> start ast
%type <def_expr> dexpr
%type <struct_t> SDef
%type <attribute_t> Attribute
%type <vname> VNAME TYPENAME

%destructor { free($$); } <vname>
//%destructor { spec_free($$); } <specification>



%token LPAR RPAR LCURLY RCURLY LBRACK RBRACK COLON

%token LAND LOR LNEG

%token CONCAT
%token EQUAL PLUS MINUS ASTERISK SLASH DOT
%left EQUAL
%left STRUCT COMPONENT TYPE FUNCTION SYSTEM

%token VNAME TYPENAME

%token <cst> TRUE FALSE

%%
start: ast {*result = $$ = $1; return 0;}

ast: dexpr { $$ = spec_new(NULL, $1); }
   | ast dexpr { $$ = spec_push($1, $2); };

dexpr: STRUCT VNAME LCURLY SDef RCURLY
   { $$ = (DExpr){
       .type = DExpr_struct,
       .name = strdup($2),
       .exp.struct_t = $4,
     };
   };

Attribute:
  TYPENAME VNAME { $$ = attribute_new($2); }
  ;

SDef:
    Attribute SDef { $$ = struct_add_attrib($2, $1); }
  | { $$ = struct_new(); }
  ;

%%

int
fecserror(void *yylval, char const *msg, const void *s) {
  //fprintf(stderr, "Error %d: %s\n", yylineno, s);
  (void)yylval;
  (void)s;
  fprintf(stderr, "Error %s\n", msg);
}

/* Disable public variables cluttering everything up */
%define api.pure true

/* Rename prefix to avoid clashes */
%define api.prefix {fecs}

/* More usefull error messages*/
%define parse.error verbose

%code top {

#define YYERROR_VERBOSE 1
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

%parse-param { Spec** result }

%param { void *scanner }

%code provides {
  void spec_free(Spec *s);
}

%code {
 int fecserror(Spec **result, void *scanner, const char *msg);
 int fecslex(FECSSTYPE *lval , void *s);
}

%union {
  bool cst;
  DExpr def_expr;
  char* vname;
  Types_t type;
  Value value;
  Struct_t* struct_t;
  Declaration* declaration_t;
  Spec* specification;
}

%type <specification> start ast
%type <type> TYPE PTYPE OTYPE ATYPE
%type <def_expr> dexpr
%type <struct_t> SDef
%type <declaration_t> Decl DeclTYPE
%type <vname> VNAME
%type <value> NUMBER

%destructor { free($$); } <vname>
//%destructor { spec_free($$); } <specification>



%token VNAME

%token LAND LOR NEG

%token CONCAT
%left PLUS MINUS
%left SLASH DOT
%left STRUCTDEF COMPONENTDEF TYPEDEF FUNCTIONDEF SYSTEMDEF


// Typenames
%token I8    I16    I32    I64    U8    U16    U32    U64    F32    F64    BOOL    USIZE    ISIZE    STRING    LIST
// Typevalues
%token <value> I8VAL I16VAL I32VAL I64VAL U8VAL U16VAL U32VAL U64VAL F32VAL F64VAL BOOLVAL USIZEVAL ISIZEVAL STRINGVAL LISTVAL

%left ARROW ASTERISK AMPERSAND BAR SEMICOLON COLON EQUAL COMMA

%token LPAR RPAR LBRACE RBRACE LBRACKET RBRACKET LT GT
%token <cst> TRUE FALSE

%token SHEBANG PRAGMA_IMPORT PRAGMA_LOAD

%%
start:
    SHEBANG ast { *result = $$ = $2; return 0; }
  |         ast { *result = $$ = $1; return 0; }
  ;

ast: ast dexpr { $$ = spec_push($1, $2); }
   | dexpr { Spec* tt = spec_new(); $$ = spec_push(tt, $1); }
   ;

/* Defining expression */
dexpr: STRUCTDEF VNAME LBRACE SDef RBRACE
   { $$ = (DExpr){
       .type = DExpr_struct,
       .name = strdup($2),
       .exp.struct_t = $4,
     };
   }
   ;

DeclTYPE:
    VNAME { $$ = declaration_new_open($1); }
  | VNAME COLON TYPE { $$ = declaration_new_typed($1, $3); }
  ;

// Declarations can just be typed, or given concrete values
Decl:
    DeclTYPE { $$ = $1; }
  // TODO Expand this to expressions in general
  | VNAME EQUAL NUMBER { $$ = declaration_new_untyped($1, $3); }
  | VNAME COLON TYPE EQUAL NUMBER { $$ = declaration_new($1, $3, $5); }
  ;

TYPE:
    I8      { $$ = Type_i8;     }
  | I16     { $$ = Type_i16;    }
  | I32     { $$ = Type_i32;    }
  | I64     { $$ = Type_i64;    }
  | U8      { $$ = Type_u8;     }
  | U16     { $$ = Type_u16;    }
  | U32     { $$ = Type_u32;    }
  | U64     { $$ = Type_u64;    }
  | F32     { $$ = Type_f32;    }
  | F64     { $$ = Type_f64;    }
  | BOOL    { $$ = Type_bool;   }
  | USIZE   { $$ = Type_usize;  }
  | ISIZE   { $$ = Type_isize;  }

  | LPAR TYPE COMMA TYPE RPAR   { $$ = Type_tuple;  }
  //| STRUCT        { /*tbd*/ $$ = Type_struct; }
  | TYPE BAR TYPE { $$ = Type_union; }

  //| LBRACKET AExpr RBRACKET TYPE { $$ = Type_array; }

  | LIST TYPE        { $$ = Type_list; }

  | VNAME            { $$ = Type_alias;  }

  | TYPE ARROW TYPE { $$ = Type_function; }
  ;

ATYPE: PTYPE { $$ = $1; } | OTYPE { $$ = $1; } | TYPE { $$ = $1; };
PTYPE: ASTERISK TYPE { $$ = Type_pointer; } ;
OTYPE: AMPERSAND TYPE { $$ = Type_owner; } ;

// Empty structs are ill-formed.
//STRUCT_TYPEo: DeclTYPE | DeclTYPE STRUCT_TYPEo ;
//STRUCT_TYPE: LBRACE STRUCT_TYPEo RBRACE ;

NUMBER:
    I8VAL | I16VAL | I32VAL | I64VAL
  | U8VAL | U16VAL | U32VAL | U64VAL
  | F32VAL | F64VAL
  | BOOLVAL
  | USIZEVAL | ISIZEVAL
  ;
  // STRINGVAL
  // LISTVAL


AExpr:
    AExpr PLUS AExpr {}
  | NUMBER
  ;

SDef:
    SDef Decl SEMICOLON { $$ = struct_add_attrib($1, $2); }
  | Decl SEMICOLON { $$ = struct_add_attrib(NULL, $1); }
  ;

%%

int
fecserror(Spec **result, void *scanner, const char *msg) {
  int line = fecsget_lineno(scanner);
  int column = fecsget_column(scanner);

  //if (filename == NULL) {
    fprintf(stderr, "(input):%d:%d: parse error: %s\n", line, column, msg);
  //} else {
  //  if (filename->filename == NULL) {
  //    fprintf(stderr, "(input):%d:%d: parse error: %s\n", line, column, msg);
  //  } else {
  //    fprintf(stderr, "%s:%d:%d: parse error: %s\n", filename->filename, line, column, msg);
  //  }
  //}

  return -1;
}

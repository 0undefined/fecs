/* Disable public variables cluttering everything up */
%define api.pure full

/* Rename prefix to avoid clashes */
%define api.prefix {fecs}

%define api.location.type {location_t}

/* More usefull error messages*/
%define parse.error verbose

%locations

%code top {

#define YYERROR_VERBOSE 1
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wchar.h>
#include <string.h>
#include <ctype.h>

#ifndef YYNOMEM
#define YYNOMEM goto yyexhaustedlab
#endif

}

%code requires {
#include "ast.h"

/* Define custom location type */
typedef struct location_t
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
  char *sourcefile;
} location_t;
typedef struct location_t YYLTYPE;

}

%parse-param { char *file_name }
%parse-param { Spec** result }

%initial-action
{
  @$.sourcefile = file_name;
};

%param { void *scanner }

%code provides {
  void spec_free(Spec *s);
}

%code {
 int fecserror(location_t *loc, char* file_name, Spec **result, void *scanner, const char *msg);
 int fecslex(FECSSTYPE *lval, location_t *loc, void *s);
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

char *fecsget_text ( void* yyscanner );

void
print_location(FILE *__restrict fd, location_t *loc) {
  int line_start = 1 + loc->first_line;
  int line_end = 1 + loc->last_line;
  int col_start = loc->first_column;
  int col_end = loc->last_column;

  fprintf(fd, "%s:%d:%d-%d:%d: ",
    loc->sourcefile != NULL ? loc->sourcefile : "(unknown source)",
    line_start, col_start,
    line_end, col_end);
}

int
fecserror(location_t *loc, char* file_name, Spec **result, void *scanner, const char *msg) {
  FILE* fd = stderr;

  print_location(fd, loc);

  fprintf(fd, "%s, \"%s\"\n", msg, fecsget_text(scanner));

  return -1;
}

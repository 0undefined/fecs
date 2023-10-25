#include "ast.h"

static isize GLOBAL_tag_counter = 0;

LinkedList_Implementation(DExpr)
LinkedList_Implementation(Declaration)

const char* Types_str[] = {
  [Type_internal_error] = "internal_error",
  [Type_untyped]        = "untyped",

  [Type_i8]       = "i8",
  [Type_i16]      = "i16",
  [Type_i32]      = "i32",
  [Type_i64]      = "i64",

  [Type_u8]       = "u8",
  [Type_u16]      = "u16",
  [Type_u32]      = "u32",
  [Type_u64]      = "u64",

  [Type_f32]      = "f32",
  [Type_f64]      = "f64",

  [Type_bool]     = "bool",

  [Type_usize]    = "usize",
  [Type_isize]    = "isize",

  [Type_string]    = "string",

  /* Combinatory types, and their syntax. */
  [Type_tuple]    = "tuple",
  [Type_struct]   = "record",
  [Type_union]    = "union",
  [Type_array]    = "array",
  [Type_list]     = "list",

  /* Pointer types */
  [Type_pointer]  = "pointer",
  [Type_owner]    = "owner",

  [Type_function] = "function",

  [Type_alias]  = "alias",

  [Type_MAX] = "MAXTYPE",
};

// Supertyped
Types_t ZTYPE(RawTypes_t t) {
  return (Types_t){
    .type = t,
      .subtype_1 = NULL,
      .subtype_2 = NULL,
      .type_name = NULL
  };
}
// unary typed
Types_t UTYPE(RawTypes_t t, Types_t *a) {
  return (Types_t){
    .type = t,
      .subtype_1 = type_dup(a),
      .subtype_2 = NULL,
      .type_name = NULL
  };
}
// binary typed
Types_t BTYPE(RawTypes_t t, Types_t *a, Types_t *b) {
  return (Types_t){
    .type = t,
      .subtype_1 = type_dup(a),
      .subtype_2 = type_dup(b),
      .type_name = NULL
  };
}
// named type (aka, alias & array)
Types_t NTYPE(RawTypes_t t, char* name) {
  return (Types_t){
    .type = t,
      .subtype_1 = NULL,
      .subtype_2 = NULL,
      .type_name = name
  };
}

// Types needs to be duplicated s.t. they're not cleaned up from the parser.
// t is assumed that it is _not_ allocated here, and is safe to discard the
// reference, in regard to memory leakage.
Types_t *type_dup(Types_t *t) {

  Types_t *res = NULL;

  Types_t *t1 = NULL;
  Types_t *t2 = NULL;

  if (t == NULL) return t;

  if (t->subtype_2 != NULL) {
    t2 = type_dup(t->subtype_2);
    t1 = type_dup(t->subtype_1);
  }
  else if (t->subtype_1 != NULL) {
    t1 = type_dup(t->subtype_1);
  }

  res = malloc(sizeof(Types_t));
  res->type = t->type;
  res->subtype_1 = t1;
  res->subtype_2 = t2;

  if (t->type_name != NULL) {
    res->type_name = strdup(t->type_name); // this might get nasty, since we also
                                           // strdup when aliasing already
    // free t->type_name ??
  }

  return res;
}

/* For the declaration constructors, assume that `vname` already has allocated
 * dedicated memory */

/* Create a untyped, no-value declaration. */
Declaration* declaration_new_open(char *vname) {
  Declaration* ret = (Declaration*)calloc(1, sizeof(Declaration));
  ret->vname = (VName){.tag = GLOBAL_tag_counter++, .name=vname};
  ret->has_type = None;
  ret->has_default_value = None;
  ret->value.type = ZTYPE(Type_untyped);
  return ret;
}

/* Create a typed, no-value declaration, assume default zero-element. */
Declaration* declaration_new_typed(char *vname, Types_t t) {
  Declaration* ret = (Declaration*)calloc(1, sizeof(Declaration));
  ret->vname = (VName){.tag = GLOBAL_tag_counter++, .name=vname};
  ret->has_type = Some;
  ret->has_default_value = None;
  ret->value.type = t;
  // TODO: Create function to return, or lookup default values.
  //ret->value.value = (Value){0};
  return ret;
}

/* Create a untyped declaration. The type is inferred from the default value. */
Declaration* declaration_new_untyped(char *vname, Value v) {
  Declaration* ret = (Declaration*)calloc(1, sizeof(Declaration));
  ret->vname = (VName){.tag = GLOBAL_tag_counter++, .name=vname};
  ret->has_type = None;
  ret->has_default_value = Some;
  // TODO: Mayhaps create the lexer such that it parses specific values and
  // attaches a assumed type pairs? Then we can "lift" the type if necessary.
  ret->value = v;
  return ret;
}

/* The way god intended declarations to be defined */
//Declaration* declaration_new(void *scanner, char *vname, Types_t t, Value v) {
Declaration* declaration_new(char *vname, Types_t t, Value v) {

  //const int line = fecsget_lineno(scanner);
  //const int column = fecsget_column(scanner);


  Declaration* ret = (Declaration*)calloc(1, sizeof(Declaration));
  if (t.type != v.type.type) {
    printf("WARNING: type mismatch! %s != %s\n", Types_str[t.type], Types_str[v.type.type]);
  }
  ret->vname = (VName){.tag = GLOBAL_tag_counter++, .name=vname};
  ret->value.type = t;
  return ret;
}

Struct_t* struct_new() {
  return (Struct_t*)calloc(1, sizeof(Struct_t));
}

Struct_t* struct_add_attrib(Struct_t* s, Declaration *a) {
  if (s == NULL) s = struct_new();

  if (s->declarations == NULL) {
    s->declarations = LinkedList_Declaration_new(*a);
  } else {
    s->declarations = LinkedList_Declaration_append(s->declarations, *a);
  }

  s->num_declarations++;
  return s;
}

Spec *spec_new() {
  return (Spec*)calloc(1, sizeof(Spec));
}

Spec* spec_push(Spec *s, DExpr e) {
  if (s == NULL) s = spec_new();

  if (s->definitions == NULL) {
    s->definitions = LinkedList_DExpr_new(e);
  } else {
    s->definitions = LinkedList_DExpr_append(s->definitions, e);
  }

  s->num_definitions++;
  return s;
}

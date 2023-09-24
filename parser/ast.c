#include "ast.h"

static isize GLOBAL_tag_counter = 0;

LinkedList_Implementation(DExpr);
LinkedList_Implementation(Declaration);

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
};

/* For the declaration constructors, assume that `vname` already has allocated
 * dedicated memory */

/* Create a untyped, no-value declaration. */
Declaration* declaration_new_open(char *vname) {
  Declaration* ret = (Declaration*)calloc(1, sizeof(Declaration));
  ret->vname = (VName){.tag = GLOBAL_tag_counter++, .name=vname};
  ret->has_type = None;
  ret->has_default_value = None;
  ret->value.type = Type_untyped;
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
Declaration* declaration_new(char *vname, Types_t t, Value v) {
  Declaration* ret = (Declaration*)calloc(1, sizeof(Declaration));
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

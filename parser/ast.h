#ifndef AST_H
#define AST_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "types.h"
#include "algo.h"

// Top level definitions
typedef enum DExpr_t {
  /* Structs are program-level type definitions used to group related values
   * together. */
  DExpr_struct,
  /* A component, like a struct, groups values together, but has some
   * restrictions imposed upon them. They are used for the produced ECS. */
  DExpr_Component,
  /* A type is a composition of components, used to efficiently create entities
   * with several components */
  DExpr_Type,
  /* Program-level function. */
  DExpr_Function,
  /* Like functions, but can only be used in the resulting ECS. They also has
   * some restrictions, such as their first argument, and return-value, must
   * both be of the same type/component. */
  DExpr_System,
  /* An entrypoint is a program entry-point. If no entrypoint is specified, it
   * is assumed that the compilation should result in a library. If function
   * `main` exists, and no entry-point is specified, it is assumed that main is
   * the entrypoint. */
  DExpr_Entry,
} DExpr_t;

extern const char* Types_str[];

// Primary types
typedef enum Types_t {
  Type_internal_error,
  Type_untyped,

  Type_i8,
  Type_i16,
  Type_i32,
  Type_i64,

  Type_u8,
  Type_u16,
  Type_u32,
  Type_u64,

  Type_f32,
  Type_f64,

  Type_bool,

  Type_usize,
  Type_isize,

  Type_string,

  /* Combinatory types, and their syntax. */
  Type_tuple,    // ( a, b, c )
  Type_struct,   // { k0; k1 : a; k2 = v0; k3 : b = v1; }
  Type_union,    //  a | b | c
  Type_array,    // [n]a
  Type_list,     // list a

  /* Pointer types */
  Type_pointer,  // *a
  Type_owner,    // &a

  Type_function, // a -> b

  Type_alias,    // typedef TYPE VNAME;
} Types_t;

typedef struct Value {
  Types_t type;


  union {
    struct {   i8 v;}   i8_t;
    struct {  i16 v;}  i16_t;
    struct {  i32 v;}  i32_t;
    struct {  i64 v;}  i64_t;
    struct {   u8 v;}   u8_t;
    struct {  u16 v;}  u16_t;
    struct {  u32 v;}  u32_t;
    struct {  u64 v;}  u64_t;
    struct {  f32 v;}  f32_t;
    struct {  f64 v;}  f64_t;

    struct { isize v;}  isize_t;
    struct { usize v;}  usize_t;

    struct { char *typestr; Types_t v; } alias_t;
  } value;
} Value;

typedef enum Maybe {
  None,
  Some,
} Maybe;

typedef struct VName {
  isize tag;
  char* name;
} VName;

/* Declarations are of the form `VARIABLENAME [":" TYPE] ["=" VALUE];`.
 * If no type is given, the most restrictive type that can be assumed from the
 * value is taken.
 * If no value is given, the zero-element of the type is assumed.
 * If neither is given, we pray that we can assume some value later by observing
 * the usage of VARIABLENAME. */
typedef struct Declaration {
  VName vname;
  Maybe has_type;
  Maybe has_default_value;
  Value value;
} Declaration;

LinkedList(Declaration)

typedef struct Struct_t {
  isize num_declarations;
  LinkedList_Declaration* declarations;
} Struct_t;

typedef struct DExpr {
  DExpr_t type;
  char *name;

  union {
    Struct_t *struct_t;
  } exp;
} DExpr;


LinkedList(DExpr)

typedef struct Spec {
  isize num_definitions;
  LinkedList_DExpr *definitions;
  //DExpr* definitions;
} Spec;

Declaration* declaration_new_open(char *vname);
Declaration* declaration_new_typed(char *vname, Types_t t);
Declaration* declaration_new_untyped(char *vname, Value v);
Declaration* declaration_new(char *vname, Types_t t, Value v);

Struct_t* struct_new();
Struct_t* struct_add_attrib(Struct_t* s, Declaration *a);

Spec *spec_new();
Spec* spec_push(Spec *s, DExpr e);

#endif

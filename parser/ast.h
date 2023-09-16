#ifndef AST_H
#define AST_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "types.h"
#include "algo.h"

#define die(...) {perror("%s:%d: Error! " FIRST(__VA_ARGS__) "\n", __FILE__, __LINE__, REST(__VA_ARGS__)); exit(EXIT_FAILURE);}

#define FIRST(...) FIRST_HELPER(__VA_ARGS__, throwaway)
#define FIRST_HELPER(first, ...) first

#define REST(...) REST_HELPER(NUM(__VA_ARGS__), __VA_ARGS__)
#define REST_HELPER(qty, ...) REST_HELPER2(qty, __VA_ARGS__)
#define REST_HELPER2(qty, ...) REST_HELPER_##qty(__VA_ARGS__)
#define REST_HELPER_ONE(first)
#define REST_HELPER_TWOORMORE(first, ...) , __VA_ARGS__
#define NUM(...) \
    SELECT_10TH(__VA_ARGS__, TWOORMORE, TWOORMORE, TWOORMORE, TWOORMORE,\
                TWOORMORE, TWOORMORE, TWOORMORE, TWOORMORE, ONE, throwaway)
#define SELECT_10TH(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, ...) a10


// Top level definitions
typedef enum DExpr_t {
	DExpr_struct,
	DExpr_Component,
	DExpr_Type,
	DExpr_Function,
	DExpr_System,
} DExpr_t;

// Primary types
typedef enum Types_t {
	Type_internal_error,
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

	Type_tuple,
	Type_record,

	Type_alias,
} Types_t;

extern const char* Types_str[];

typedef struct Var {
	Types_t type;
	char *typestr;

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


		struct { Types_t v;} alias_t;
	} value;
} Var;

typedef struct Attribute {
	char* vname;
	bool has_default_value;
	Var var;
} Attribute;

LinkedList(Attribute);

typedef struct Struct_t {
	isize num_attributes;
	LinkedList_Attribute* attributes;
} Struct_t;

typedef struct DExpr {
	DExpr_t type;
	char *name;

	union {
		Struct_t *struct_t;
	} exp;
} DExpr;


LinkedList(DExpr);

typedef struct Spec {
	isize num_definitions;
	LinkedList_DExpr *definitions;
	//DExpr* definitions;
} Spec;

Attribute* attribute_new(Types_t t, char *vname);

Struct_t* struct_new();
Struct_t* struct_add_attrib(Struct_t* s, Attribute *a);

Spec *spec_new();
Spec* spec_push(Spec *s, DExpr e);

#endif

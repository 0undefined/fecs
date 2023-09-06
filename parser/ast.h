#ifndef AST_H
#define AST_H

#include <stdbool.h>
#include <stdint.h>

#include "types.h"

#define die(fmt, ...) {perror("%s:%d: Error! " fmt "\n", __FILE__, __LINE__, ...); exit(EXIT_FAILURE);}

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

	Type_alias,
} Types_t;

typedef struct Var {
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


		struct { Types_t v;} alias_t;
	} value;
} Var;

typedef struct Attribute {
	const char* vname;
	bool has_default_value;
	Var var;
} Attribute;

typedef struct Struct_t {
	const isize num_attributes;
	Attribute* attributes;
} Struct_t;

typedef struct DExpr {
	DExpr_t type;
	const char *name;

	union {
		Struct_t struct_t;
	} exp;
} DExpr;

typedef struct Spec {
	const isize num_definitions;
	DExpr* definitions;
} Spec;



Struct_t struct_new() {
	return (Struct_t){.num_attributes = 0, attributes = NULL};
}

Struct_t* struct_add_attrib(Struct_t* s, Attribute a) {
	if (s == NULL) die("Got nil!");

	if (s->attributes == NULL) {
		s->attributes = malloc(sizeof(Attribute) * 8);
	}

	if (s->num_attributes > 0 && s->num_attributes % 8 == 0) {
		s->attributes = realloc(s->attributes, sizeof(Attribute) * (s->num_attributes + 8));
	}

	s->attributes[s->num_attributes++] = a;
	return s;
}

Spec spec_new() {
	return (Spec){.num_definitions = 0, definitions = NULL};
}

Spec spec_push(Spec *s, DExpr e) {
	if (s.definitions == NULL) {
		s.definitions = malloc(sizeof(DExpr) * 8);
	}

	if (s->definitions > 0 && s->num_definitions % 8 == 0) {
		s->definitions = realloc(s->definitions, sizeof(DExpr) * (s->num_definitions + 8));
	}

	s->definitions[s->num_definitions++] = v;
	return s;
}

#endif

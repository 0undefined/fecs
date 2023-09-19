#include "ast.h"

LinkedList_Implementation(DExpr);
LinkedList_Implementation(Declaration);

const char* Types_str[] = {
	[Type_internal_error] = "internal_error",
	[Type_untyped]        = "untyped",

	[Type_i8]     = "i8",
	[Type_i16]    = "i16",
	[Type_i32]    = "i32",
	[Type_i64]    = "i64",

	[Type_u8]     = "u8",
	[Type_u16]    = "u16",
	[Type_u32]    = "u32",
	[Type_u64]    = "u64",

	[Type_f32]    = "f32",
	[Type_f64]    = "f64",

	[Type_bool]   = "bool",

	[Type_usize]  = "usize",
	[Type_isize]  = "isize",

	[Type_tuple]  = "tuple",
	[Type_record] = "record",
	[Type_sum]    = "sum",
	[Type_array]  = "array",

	[Type_alias]  = "alias",
};

Declaration* declaration_new(Types_t t, char *vname) {
	Declaration* ret = (Declaration*)calloc(1, sizeof(Declaration));
	ret->vname = vname;
	ret->var.type = t;
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

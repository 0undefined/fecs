#include "ast.h"

Attribute* attribute_new(char *vname) {
	Attribute* ret = (Attribute*)calloc(1, sizeof(Attribute));
	ret->vname = vname;
	return ret;
}

Struct_t* struct_new() {
	return (Struct_t*)calloc(1, sizeof(Struct_t));
}

Struct_t* struct_add_attrib(Struct_t* s, Attribute *a) {
	if (s == NULL) exit(EXIT_FAILURE); //die("Got nil!");

	if (s->attributes == NULL) {
		s->attributes = (Attribute**)calloc(8, sizeof(Attribute*));
	}

	if (s->num_attributes > 0 && s->num_attributes % 8 == 0) {
		s->attributes = (Attribute**)realloc(s->attributes, sizeof(Attribute*) * (s->num_attributes + 8));
	}

	s->attributes[s->num_attributes++] = a;
	return s;
}

Spec *spec_new() {
	return (Spec*)calloc(1, sizeof(Spec));
}

Spec* spec_push(Spec *s, DExpr e) {
	if (s == NULL) {
		s = spec_new();
	}
	if (s->definitions == NULL) {
		s->definitions = LinkedList_DExpr_new(e);
	} else {
		s->definitions = LinkedList_DExpr_append(s->definitions, e);
	}

	s->num_definitions++;
	return s;
}

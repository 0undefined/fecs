#include "ast.h"
#include "ast_print.h"

void print_ast(Spec* s) {
	if (s == NULL) {printf("<empty ast>\n"); return;}
	//printf("Num constructs: %lu\n", s->num_definitions);
	LinkedList_DExpr *head = s->definitions;

	while (head != NULL) {
		printf("def %s [%lu]{\n", head->value.name, head->value.exp.struct_t->num_declarations);
		LinkedList_Declaration* head1 = head->value.exp.struct_t->declarations;

		while (head1 != NULL) {
			printf("  ");
			print_attrib(&head1->value);
			head1 = head1->next;
		}

		printf("}\n");

		head = head->next;
		if (head != NULL) printf("\n");
	}
}

void print_word(const char* w) {
	puts(w);
}

void print_attrib(Declaration* a) {
	Types_t t = a->var.type;
	printf("%s :: %s\n", a->vname, Types_str[t]);
}

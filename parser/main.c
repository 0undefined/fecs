#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define YYSTYPE FECSSTYPE
#include "fecs.tab.h"
#include "lex.fecs.h"

LinkedList_Implementation(DExpr);
LinkedList_Implementation(Attribute);

//int tokenize_callback(char *, void (*)(const char *));

void print_word(const char* w) {
	puts(w);
}

void print_attrib(Attribute* a) {
	Types_t t = a->var.type;
	printf("%s :: %s\n", a->vname, Types_str[t]);
}

void print_ast(Spec* s) {
	if (s == NULL) {printf("<empty ast>"); return;}
	//printf("Num constructs: %lu\n", s->num_definitions);
	LinkedList_DExpr *head = s->definitions;

	while (head != NULL) {
		printf("def %s [%lu]{\n", head->value.name, head->value.exp.struct_t->num_attributes);
		LinkedList_Attribute* head1 = head->value.exp.struct_t->attributes;

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

int main () {
	int i = 0;
	Spec* result = NULL;
	yyscan_t scanner;

	if ((i = fecslex_init(&scanner)) != 0)
		exit(i);

	clock_t start = clock(), diff;

	int e = fecsparse(&result, scanner);
	printf("Code = %d\n", e);
	if (e == 0) {
		printf("\n---\n");
		print_ast(result);
		printf("---\n");
		/* TODO free */
	}

	diff = clock() - start;
	int diff_ms = diff * 1000 / CLOCKS_PER_SEC;
	printf("Parsing took %ds %dms!\n", diff_ms / 1000, diff_ms % 1000);

	fecslex_destroy(scanner);

	return 0;
}

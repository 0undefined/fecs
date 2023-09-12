#include <stdio.h>
#include <stdlib.h>

#define YYSTYPE FECSSTYPE
#include "fecs.tab.h"
#include "lex.fecs.h"

LinkedList_Implementation(DExpr);

//int tokenize_callback(char *, void (*)(const char *));

void print_word(const char* w) {
	puts(w);
}

void print_ast(Spec* s) {
	if (s == NULL) {printf("<empty ast>"); return;}
	printf("Num constructs: %lu\n", s->num_definitions);
	LinkedList_DExpr *head = s->definitions;
	while (head != NULL) {
		printf("def %s {..}\n", head->value.name);

		head = head->next;
	}
}

int main () {
	int i = 0;
	Spec* result = NULL;
	yyscan_t scanner;

	if ((i = fecslex_init(&scanner)) != 0)
		exit(i);

	int e = fecsparse(&result, scanner);
	printf("Code = %d\n", e);
	if (e == 0) {
		printf("\n---\n");
		print_ast(result);
		printf("\n---\n");
		/* TODO free */
	}

	fecslex_destroy(scanner);

	return 0;
}

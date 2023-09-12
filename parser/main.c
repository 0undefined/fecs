#include <stdio.h>
#include <stdlib.h>

#define YYSTYPE FECSSTYPE
#include "fecs.tab.h"
#include "fecs.lex.h"

LinkedList_Implementation(DExpr);

//int tokenize_callback(char *, void (*)(const char *));

void print_word(const char* w) {
	puts(w);
}

int main () {
	int i = 0;
	Spec* result = NULL;
	//yyscan_t scanner;

	//if ((i = fecslex_init(&scanner)) != 0)
		exit(i);

	return 0;
}

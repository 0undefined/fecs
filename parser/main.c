#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define YYSTYPE FECSSTYPE
#include "fecs.tab.h"
#include "lex.fecs.h"
#include "ast.h"
#include "ast_print.h"
#include "util.h"

LinkedList_Implementation(DExpr);
LinkedList_Implementation(Declaration);

//int tokenize_callback(char *, void (*)(const char *));

void print_help();

int parse_file(Spec** result, FILE* f);

int main (int argc, char* argv[]) {

	if (argc <= 1) {
		print_help();
		exit(EXIT_FAILURE);
	}

	// For now, just assume the file is the first argument
	FILE* src = fopen(argv[1], "r");

	Spec* result = NULL;

	clock_t start = clock(), diff;

	int e = parse_file(&result, src);

	//int e = fecsparse(&result, scanner);
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

	//fecslex_destroy(scanner);

	//free(mainsource);
	return 0;
}

void print_help() {
	printf(
			"Usage: fecs [options] FILE\n"
			"Options:\n");
}

int parse_file(Spec** result, FILE* f) {
	yyscan_t scanner;
	int i = 0;

	if (!f) {
		die("Failed to read file");
	}

	fseek(f, 0, SEEK_END);
	usize len = ftell(f);
	rewind(f);

	if ((i = fecslex_init(&scanner)) != 0)
		exit(i);

	char *src = readfile_fd(f);
	set_input_string(src, scanner);

	YY_BUFFER_STATE s = fecs_scan_string ( src, scanner );

	i = fecsparse(result, scanner);

  end_lexical_scan(s, scanner);
	//fecs_delete_buffer(s, scanner);
	fecslex_destroy(scanner);
	return i;
}

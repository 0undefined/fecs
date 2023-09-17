#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define YYSTYPE FECSSTYPE
#include "fecs.tab.h"
#include "lex.fecs.h"
#include "ast.h"

LinkedList_Implementation(DExpr);
LinkedList_Implementation(Attribute);

//int tokenize_callback(char *, void (*)(const char *));

void print_word(const char* w);
void print_attrib(Attribute* a);
void print_ast(Spec* s);
void print_help();

char* readfile(const char *pathname);
char* readfile_fd(FILE* file);

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

void die_imp(const char *fmt, const char *file, int line, ...) {
	const int len = strlen(fmt);
	const char* prelude = "%s:%d: Error: ";
	char *final_fmt = (char*)calloc(strlen(prelude) + len + 1 + 2, sizeof(char));
	strcat(final_fmt, prelude);
	strcat(final_fmt, fmt);
	strcat(final_fmt, "\n");

	va_list args;
	va_start(args,line);
	fprintf(stderr, final_fmt, file, line, args);
  va_end(args);
	exit(EXIT_FAILURE);
}

char* readfile(const char *pathname) {
	return readfile_fd(fopen(pathname, "r"));
}

char* readfile_fd(FILE* file) {
	usize len = 0;
	char* buf = NULL;

	if (!file) {
		die("Failed to read file");
	}

	fseek(file, 0, SEEK_END);
	len = ftell(file);
	rewind(file);

	buf = calloc(len, sizeof(char));

	fread(buf, sizeof(char), len, file);

	fclose(file);

	return buf;
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

void print_ast(Spec* s) {
	if (s == NULL) {printf("<empty ast>\n"); return;}
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

void print_word(const char* w) {
	puts(w);
}

void print_attrib(Attribute* a) {
	Types_t t = a->var.type;
	printf("%s :: %s\n", a->vname, Types_str[t]);
}

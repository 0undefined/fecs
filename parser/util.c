#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "util.h"

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


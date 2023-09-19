#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>

char* readfile(const char *pathname);
char* readfile_fd(FILE* file);

#define die(fmt, ...) die_imp(fmt, __FILE__, __LINE__ __VA_ARGS__)
void die_imp(const char *fmt, const char *file, int line, ...);

#endif

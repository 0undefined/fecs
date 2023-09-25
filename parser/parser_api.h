#ifndef PARSER_API_H
#define PARSER_API_H

#define YYSTYPE FECSSTYPE

#include "ast.h"

int parse_file(Spec** result, char *restrict filename);

#endif

#include "parser_api.h"

#include "fecs.tab.h"
#include "lex.fecs.h"
#include "ast.h"
#include "ast_print.h"
#include "util.h"

/* Defined in fecs.l */
void set_input_string(const char* in, yyscan_t scanner);
void end_lexical_scan(YY_BUFFER_STATE b, yyscan_t scanner);

int parse_file(Spec** result, char *restrict filename) {
  // Duplicate filename in case it is wiped
  FILE* f = fopen(filename, "r");
  char *sourcefile = strdup(filename);
  yyscan_t scanner;
  i32 i = 0;

  if (!f) {
    die("Failed to read file");
  }

  if ((i = fecslex_init(&scanner)) != 0)
    exit(i);

  char *src = readfile_fd(f);

  set_input_string(src, scanner);

  YY_BUFFER_STATE s = fecs_scan_string ( src, scanner );

  i = fecsparse(sourcefile, result, scanner);

  end_lexical_scan(s, scanner);
  fecslex_destroy(scanner);
  free(sourcefile);
  return i;
}

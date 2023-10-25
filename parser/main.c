#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "parser_api.h"
#include "util.h"
#include "ast_print.h"

//int tokenize_callback(char *, void (*)(const char *));

void print_help();

int main (int argc, char* argv[]) {

  if (argc <= 1) {
    print_help();
    exit(EXIT_FAILURE);
  }

  // For now, just assume the file is the first argument

  Spec* result = NULL;

  clock_t start = clock(), diff;

  int e = parse_file(&result, argv[1]);

  if (e != 0) {
    return e;
  }

  printf("\n---\n");
  print_ast(result);
  printf("---\n");
  /* TODO free */

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

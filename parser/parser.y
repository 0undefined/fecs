%{

void yyerror(char* e);
int yylex();

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "symboltype.h"

int err = 0;

%}

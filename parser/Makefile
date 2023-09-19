# We do not support other parser generators et al
LEX=flex
YACC=bison

default: main
lex.fecs.c: fecs.l fecs.tab.h
	$(LEX) -P fecs fecs.l

fecs.tab.c fecs.tab.h: parser.y
	$(YACC) -d -b fecs parser.y

lex.fecs.o: lex.fecs.c
	gcc -g -Og -c lex.fecs.c

fecs.tab.o: fecs.tab.c fecs.tab.h
	gcc -g -Og -c fecs.tab.c

ast.o: ast.c ast.h
	gcc -g -Og -c ast.c

ast_print.o: ast_print.c ast_print.h ast.c ast.h
	gcc -g -Og -c ast_print.c

util.o: util.c util.h
	gcc -g -Og -c util.c

parser_api.o: parser_api.c parser_api.h
	gcc -g -Og -c parser_api.c

main: main.c fecs.tab.o lex.fecs.o fecs.tab.h ast.o ast_print.o util.o parser_api.o
	cc -g -Og -o main main.c fecs.tab.o lex.fecs.o ast.o ast_print.o util.o parser_api.o

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

main: main.c fecs.tab.o lex.fecs.o fecs.tab.h ast.o
	cc -g -Og -o main main.c fecs.tab.o lex.fecs.o ast.o

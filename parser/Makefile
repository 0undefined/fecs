LEX=flex
YACC=bison

default: main
lex.fecs.c: fecs.l fecs.tab.h
	flex -P fecs fecs.l

lex.fecs.o: lex.fecs.c
	gcc -c lex.fecs.c

fecs.tab.o: fecs.tab.c fecs.tab.h
	gcc -c fecs.tab.c

fecs.tab.c fecs.tab.h: parser.y
	bison -d -b fecs parser.y

ast.o: ast.c ast.h
	gcc -c ast.c

main: main.c fecs.tab.o lex.fecs.o fecs.tab.h ast.o
	cc -o main main.c fecs.tab.o lex.fecs.o ast.o

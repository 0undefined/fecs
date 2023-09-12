LEX=flex
YACC=bison

default: main
lex.fecs.c: fecs.l fecs.tab.h
	flex fecs.l

lex.fecs.o: lex.fecs.c
	gcc -c lex.fecs.c

fecs.tab.o: fecs.tab.c
	gcc -c fecs.tab.c

fecs.tab.c fecs.tab.h: parser.y
	bison -d -b fecs parser.y

ast.o: ast.c ast.h
	gcc -c ast.c

main: fecs.tab.o lex.fecs.o fecs.tab.h ast.o
	gcc -o main -ly -ll main.c fecs.tab.o lex.fecs.o ast.o

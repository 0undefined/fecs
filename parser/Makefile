LEX=flex
YACC=bison

default: main
fecs.yy.c: fecs.l parser.tab.h
	flex fecs.l

fecs.yy.o: fecs.yy.c
	gcc -c fecs.yy.c

parser.tab.o: parser.tab.c
	gcc -c parser.tab.c

parser.tab.c parser.tab.h: parser.y
	bison -d -b fecs parser.y


main: parser.tab.o fecs.yy.o parser.tab.h
	gcc -o main -ly -ll main.c parser.tab.o fecs.yy.o

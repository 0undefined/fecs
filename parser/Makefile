fecs.yy.c: fecs.l
	flex -o fecs.yy.c fecs.l

fecs.yy.o: fecs.yy.c
	gcc -c fecs.yy.c

parser.tab.c:
	bison -d parser.y

parser.tab.h:
	bison -d parser.y

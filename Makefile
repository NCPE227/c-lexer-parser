regex: parser.y lexer.l main.c
	bison -d parser.y
	flex lexer.l
	gcc -o parse main.c parser.tab.c lex.yy.c -lfl
parse: parser.y lexer.l main.c AST.c
	bison -d parser.y -Wcounterexamples
	flex lexer.l
	gcc -o parse main.c parser.tab.c lex.yy.c AST.c -lfl
parse: parser.y lexer.l main.c AST.c
	bison -d parser.y -Wcounterexamples
	flex lexer.l
	gcc -o parse main.c parser.tab.c lex.yy.c AST.c -lfl

generate: parser.y lexer.l main.c AST.c
	bison -d parser.y
	flex lexer.l
	gcc -o generate main.c parser.tab.c lex.yy.c AST.c generators.c -lfl

rexec: rexec.c
	gcc -o rexec rexec.c

clean:
	rm -f generate parser.tab.* lex.yy.c rexec.c
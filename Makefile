token.o : token.c token.h
	clang -c -Wall token.c 

compiler.o : compiler.c compiler.h
	clang -c -g -Wall compiler.c 

jc.o : jc.c
	clang -c -g -Wall jc.c

jc : token.o compiler.o jc.o
	clang -o jc token.o compiler.o jc.o

clean :
	rm jc *.o
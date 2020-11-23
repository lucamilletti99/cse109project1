all: prog1

prog1: prog1.o prog1_funcs.o
	gcc -o prog1 prog1.o prog1_funcs.o

prog1.o: prog1_funcs.h prog1.c
	gcc -g -Wall -Wextra -Wwrite-strings -c prog1.c

prog1_funcs.o: prog1_funcs.h prog1_funcs.c
	gcc -g -Wall -Wextra -Wwrite-strings -c prog1_funcs.c

clean:
	-rm -f prog1.o prog1_funcs.o prog1 

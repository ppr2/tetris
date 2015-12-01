# NOTICE: carefull here, you need to use real TABs instead of spaces as separators

STRUCTURES=stack.o state.o

fitting_test: fitting_test.o fitting.o 
	mpicc -Wall -o fitting_test fitting_test.o fitting.o

token_test: token_test.o
	mpicc -Wall -o token_test token_test.o
token_test.o: tests/token_test.c
	mpicc -c -Wall tests/token_test.c

stack_test: stack_test.o stack.o state.o
	mpicc -Wall -o stack_test stack_test.o stack.o state.o

all: tetris

tetris: tetris.o fitting.o ${STRUCTURES}
	mpicc -Wall -o tetris tetris.o fitting.o ${STRUCTURES}

tetris.o: tetris.c stack.h state.h fitting.h
	mpicc -c -Wall tetris.c

fitting_test.o: tests/fitting_test.c tests/fitting_test.h fitting.h
	mpicc -c -Wall tests/fitting_test.c

stack_test.o: tests/stack_test.c tests/stack_test.h stack.h state.h
	mpicc -c -Wall tests/stack_test.c

fitting.o: fitting.c fitting.h
	mpicc -c -Wall fitting.c

state.o: state.c state.h
	mpicc -c -Wall state.c

stack.o: stack.c stack.h
	mpicc -c -Wall stack.c

clean:
	rm -f tetris fitting_test stack_test *.o

clean_o:
	rm -f *.o
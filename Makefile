# NOTICE: carefull here, you need to use real TABs instead of spaces as separators

STRUCTURES=stack.o state.o

fitting_test: fitting_test.o fitting.o 
	gcc -Wall -o fitting_test fitting_test.o fitting.o

# TODO create tetris executable

tetris.o: tetris.c tetris.h stack.h state.h
	gcc -c -Wall tetris.c

fitting_test.o: tests/fitting_test.c tests/fitting_test.h fitting.h
	gcc -c -Wall tests/fitting_test.c

fitting.o: fitting.c fitting.h tetris.h
	gcc -c -Wall fitting.c

state.o: state.c state.h
	gcc -c -Wall state.c

stack.o: stack.c stack.h
	gcc -c -Wall stack.c

clean:
	rm -f tetris fitting_test *.o

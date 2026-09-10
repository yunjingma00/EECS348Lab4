
prog: main.o isOdd.o
	gcc main.o isOdd.o -o prog

main.o: main.c isOdd.h
	gcc -c main.c

is_odd.o: isOdd.c isOdd.h
	gcc -c isOdd.c

clean:
	rm -f main.o isOdd.o prog

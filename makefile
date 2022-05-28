all:  main
main: main.o  Queue.a Queue.h 
	gcc -Wall -g -o main main.o Queue.a 

Queue:Queue.a


Queue.a: Queue.o
	ar -rcs Queue.a Queue.o

Queue.o: Queue.c Queue.h
	gcc -Wall -g -c Queue.c

main.o: main.c Queue.h 
	gcc -Wall -g -c main.c 

.PHONY: clean#.PHONY means that clean is not a file.
clean:
	rm -f *.o *.a  main 


all:  main main1 client
main: main.o  Queue.a Queue.h 
	g++ -Wall -g -o main main.o Queue.a 

main1: main1.o Queue.a Queue.h AO.a AO.h
	g++ -Wall -g -o main1 main1.o Queue.a AO.a -lpthread

Queue:Queue.a

AO:AO.a

Queue.a: Queue.o
	ar -rcs Queue.a Queue.o

AO.a: AO.o
	ar -rcs AO.a AO.o

Queue.o: Queue.c Queue.h
	g++ -Wall -g -c Queue.c

AO.o: AO.c AO.h Queue.h
	g++ -Wall -g -c AO.c -lpthread

client: client.cpp 
	g++ -Wall -g client.cpp -o client -lpthread

main1.o: main1.cpp Queue.h AO.h
	g++ -Wall -g -c main1.cpp -lpthread

main.o: main.c Queue.h 
	g++ -Wall -g -c main.c 

.PHONY: clean#.PHONY means that clean is not a file.
clean:
	rm -f *.o *.a  main main1 client


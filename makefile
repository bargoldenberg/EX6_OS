all: reactor.o main1 client guard singleton pollserver 

main1: main1.o Queue.a Queue.h AO.a AO.h
	g++ -Wall -g -o main1 main1.o Queue.a AO.a -lpthread

pollserver: pollserver.o reactor.a reactor.h
	g++ -Wall -g -o pollserver pollserver.o reactor.a -lpthread

guard: guard.o
	g++ -Wall -g -o guard guard.o -lpthread

singleton: singleton.o	
	g++ -Wall -g -o singleton singleton.o -lpthread

Queue:Queue.a

AO:AO.a

libdesign.so: guard.o singleton.o reactor.o Queue.o AO.o
	g++ -shared -fPIC -o libdesign.so guard.o singleton.o reactor.o Queue.o AO.o

Queue.a: Queue.o
	ar -rcs Queue.a Queue.o

AO.a: AO.o
	ar -rcs AO.a AO.o

reactor.a:
	ar -rcs reactor.a reactor.o

Queue.o: Queue.cpp Queue.h
	g++ -Wall -g -c Queue.cpp

guard.o: guard.cpp
	g++ -Wall -g -c guard.cpp

reactor.o: reactor.c reactor.h
	g++ -Wall -g -c reactor.c -lpthread

singleton.o: singleton.cpp
	g++ -Wall -g -c singleton.cpp

AO.o: AO.c AO.h Queue.h
	g++ -Wall -g -c AO.c -lpthread

client: client.cpp 
	g++ -Wall -g client.cpp -o client -lpthread

main1.o: main1.cpp Queue.h AO.h
	g++ -Wall -g -c main1.cpp -lpthread

pollserver.o: pollserver.cpp reactor.h
	g++ -Wall -g -c pollserver.cpp -lpthread 

.PHONY: clean#.PHONY means that clean is not a file.
clean:
	rm -f *.o *.a  main1 client guard singleton pollserver


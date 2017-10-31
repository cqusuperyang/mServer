build:main.o mserver.o
	g++ -pthread -o build main.o mserver.o
main.o:main.cpp mserver.h
	g++ -g -c main.cpp
mserver.o:mserver.cpp mserver.h 
	g++ -g -c mserver.cpp
clean:
	rm *.o
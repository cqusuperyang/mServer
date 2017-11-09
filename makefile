start:./src/main.o ./src/mserver.o
	g++ -pthread -o ./build/start ./src/main.o ./src/mserver.o
main.o:./src/main.cpp ./src/mserver.h
	g++ -g -c ./src/main.cpp
mserver.o:./src/mserver.cpp ./src/mserver.h 
	g++ -g -c ./src/mserver.cpp
clean:
	rm ./src/*.o
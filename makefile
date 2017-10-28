build:main.o
	g++ -o build main.o
main.o:main.cpp
	g++ -g -c main.cpp
clean:
	rm main.o
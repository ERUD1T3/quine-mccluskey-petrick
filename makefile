all: build debug

build:
	g++ -Wall -o ./bin/qmp main.cpp ./includes/quine-mccluskey-petrick.cpp

run: build
	./bin/qmp

debug:
	g++ -std=c++11 -Wall -ggdb main.cpp ./includes/quine-mccluskey-petrick.cpp
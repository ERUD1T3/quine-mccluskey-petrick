all: build

build:
	g++ -Wall -o ./bin/qmp main.cpp ./includes/quine-mccluskey-petrick.cpp

run:
	./bin/qmp input.txt

debug:
	g++ -std=c++11 -Wall -ggdb main.cpp ./includes/quine-mccluskey-petrick.cpp
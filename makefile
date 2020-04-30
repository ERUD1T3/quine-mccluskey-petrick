all: build debug

build:
	g++ -Wall -ggdb -o ./bin/qm quine-mcclusky.cpp ./includes/qmutils.cpp

run: build
	./bin/qm

debug:
	g++ -std=c++11 -Wall -ggdb quine-mcclusky.cpp ./includes/qmutils.cpp
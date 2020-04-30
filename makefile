all: build 

build:
	g++ -Wall -ggdb -o ./bin/qm quine-mcclusky.cpp ./includes/qmutils.cpp

run: build
	./bin/qm

debug:
	g++ -Wall -ggdb quine-mcclusky.cpp ./includes/qmutils.cpp
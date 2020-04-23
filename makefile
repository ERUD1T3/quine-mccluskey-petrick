all: build run

build:
	g++ -Wall -o ./bin/qm quine-mcclusky.cpp ./includes/qmutils.cpp

run:
	./bin/qm
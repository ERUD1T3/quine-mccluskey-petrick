all: build run

build:
	g++ -Wall -o ./bin/qm quine-mcclusky.cpp ./includes/qmutils.cpp ./includes/fileparser.cpp

run:
	./bin/qm
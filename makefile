all: build 

build:
	g++ -Wall -o ./bin/qm quine-mcclusky.cpp ./includes/qmutils.cpp

run: build
	./bin/qm
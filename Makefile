compiler = g++
flags = -Wall -o
program = main.out
files = main.cpp InputFunctions.cpp Functions.cpp
all: ${program}

${program}: ${files}
	${compiler} ${files} ${flags} ${program}

run: ${program}
	./${program}

clean:
	rm *.out
compiler = g++
flags = -Wall -o
program = main.out
files = main.cpp ValidatingFunctions.cpp InputFunctions.cpp SolvingFunctions.cpp

all: ${program}

${program}: ${files}
	${compiler} ${files} ${flags} ${program}

run: ${program}
	./${program}

clean:
	rm *.out
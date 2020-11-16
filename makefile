assembler: assembler.o errors.o files.o functions.o instructions.o parse.o pass1_functions.o pass_one.o pass_two.o symbols.o
	gcc -g -ansi -Wall -pedantic assembler.o errors.o files.o functions.o instructions.o parse.o pass1_functions.o pass_one.o pass_two.o symbols.o -o assembler

assembler.o: assembler.c assembler.h
	gcc -c -ansi -Wall -pedantic assembler.c -o assembler.o

errors.o: errors.c assembler.h
	gcc -c -ansi -Wall -pedantic errors.c -o errors.o

files.o: files.c assembler.h
	gcc -c -ansi -Wall -pedantic files.c -o files.o

functions.o: functions.c assembler.h
	gcc -c -ansi -Wall -pedantic functions.c -o functions.o

instruction.o: instruction.c assembler.h
	gcc -c -ansi -Wall -pedantic instruction.c -o instruction.o

parse: parse.c assembler.h
	gcc -c -ansi -Wall -pedantic parse.c -o parse.o

pass1_functions: pass1_functions.c assembler.h
	gcc -c -ansi -Wall -pedantic pass1_functions.c -o pass1_functions.o

pass_one.o: pass_one.c assembler.h
	gcc -c -ansi -Wall -pedantic pass_one.c -o pass_one.o

pass_two.o: pass_two.c assembler.h
	gcc -c -ansi -Wall -pedantic pass_two.c -o pass_two.o

symbols.o: symbols.c assembler.h
	gcc -c -ansi -Wall -pedantic symbols.c -o symbols.o



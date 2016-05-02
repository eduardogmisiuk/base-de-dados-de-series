PROGRAM_EXEC=build/main
CFLAGS=-g -Wall
VALGRINDFLAGS=--leak-check=full
CASE=< test.in
ERROR=2> err.log

all: clean compile run

clean:
	@find -name "*~" -exec rm -rf {} \;
	@find -name "vgcore.*" -exec rm -rf {} \;
	@clear

compile:
	@gcc -o $(PROGRAM_EXEC) src/*.c -I./includes $(CFLAGS)

runtest: clean compile
	@echo "Press any key to clear the terminal and run the program..."
	@read
	@clear
	@valgrind $(VALGRINDFLAGS) ./$(PROGRAM_EXEC) $(CASE) $(ERROR)

run:
	@valgrind $(VALGRINDFLAGS) ./$(PROGRAM_EXEC)

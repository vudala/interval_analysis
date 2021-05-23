CC = gcc
LIBS = -lm
SRC = main.c
OUTPUT = program
TEST_INPUT = input.txt

all:
	$(CC) $(SRC) -o $(OUTPUT) $(LIBS) 

purge:
	rm $(OUTPUT)

test:
	./$(OUTPUT) < $(TEST_INPUT)
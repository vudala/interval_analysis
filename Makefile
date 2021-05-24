CC = gcc
LIBS = -lm
SRC = main.c
OBJS = interval.o
OUTPUT = program

all: $(OBJS)
	$(CC) $(SRC) $(OBJS) -o $(OUTPUT) $(LIBS)

interval.o: interval.c
	gcc -c interval.c $(LIBS)

clean:
	rm $(OBJS)

purge: clean
	rm $(OUTPUT)
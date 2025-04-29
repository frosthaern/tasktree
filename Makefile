CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c23 -pedantic -O2 -g
LIBS = -lraylib

src/draw.o: src/draw.c src/raygui.c
	$(CC) $(CFLAGS) -c src/draw.c -o src/draw.o

src/main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c -o src/main.o

src/helper.o: src/helper.c
	$(CC) $(CFLAGS) -c src/helper.c -o src/helper.o

src/todo.o: src/todo.c
	$(CC) $(CFLAGS) -c src/todo.c -o src/todo.o

main: src/main.o src/draw.o
	$(CC) $(CFLAGS) src/main.o src/draw.o $(LIBS) -o main

clean:
	rm -f *.o main

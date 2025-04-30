CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c23 -pedantic -O2 -g
LIBS = -lraylib

object/draw.o: src/draw.c src/raygui.c include/tasktree.h
	$(CC) $(CFLAGS) -c src/draw.c -o object/draw.o

object/main.o: src/main.c include/tasktree.h
	$(CC) $(CFLAGS) -c src/main.c -o object/main.o

object/helper.o: src/helper.c include/tasktree.h
	$(CC) $(CFLAGS) -c src/helper.c -o object/helper.o

object/todo.o: src/todo.c include/tasktree.h
	$(CC) $(CFLAGS) -c src/todo.c -o object/todo.o

main: object/main.o object/draw.o object/helper.o object/todo.o include/tasktree.h
	$(CC) $(CFLAGS) object/main.o object/draw.o object/helper.o object/todo.o $(LIBS) -o main

clean:
	rm -f *.o main

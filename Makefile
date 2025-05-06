CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c2x -pedantic -O2 -g
LIBS = -lraylib -lm -lpthread -ldl -lrt -lX11 -lsqlite3

OBJECTS = object/main.o object/draw.o object/helper.o object/todo.o object/raygui.o object/sql.o

all: dir tasktree

dir:
	mkdir -p object

object/raygui.o: src/raygui.c
	$(CC) -c src/raygui.c -DRAYGUI_IMPLEMENTATION -o object/raygui.o

object/draw.o: src/draw.c include/tasktree.h
	$(CC) $(CFLAGS) -c src/draw.c -o object/draw.o

object/main.o: src/main.c include/tasktree.h
	$(CC) $(CFLAGS) -c src/main.c -o object/main.o

object/helper.o: src/helper.c include/tasktree.h
	$(CC) $(CFLAGS) -c src/helper.c -o object/helper.o

object/todo.o: src/todo.c include/tasktree.h
	$(CC) $(CFLAGS) -c src/todo.c -o object/todo.o

object/sql.o: src/sql.c include/tasktree.h
	$(CC) $(CFLAGS) -c src/sql.c -o object/sql.o

tasktree: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) -o tasktree

clean:
	rm -f object/*.o

#ifndef TASKTREE_H
#define TASKTREE_H

#include "../src/raygui.c"
#include <raylib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// From helper.c
void init_rand();
int32_t random_number();

typedef struct Todo {
    int32_t id;
    char *title;
    bool completed;
    Rectangle bounds;
    int32_t parent_id;
    struct Todo **children;
    int32_t num_children;
    int32_t capacity;
    bool expanded;
} Todo;

Todo *createTodo(char *title);
void destroyTodo(Todo *todo);
void addChild(Todo *parent, Todo *child);
int32_t calculateLayout(Todo *root, int32_t x, int32_t y);

// From draw.c
Rectangle newRectangle(int x, int y, int width, int height);
Vector2 newVector2(float x, float y);
void drawLayout(Todo *todo, Font *f);

#endif // TASKTREE_H

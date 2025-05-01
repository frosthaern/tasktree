#ifndef TASKTREE_H
#define TASKTREE_H

char *takeTodoTitle();

#include "raygui.h"
#define RAYGUI_IMPLEMENTATION
#include <raylib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void init_rand();
int32_t random_number();

typedef struct Todo {
    int32_t id;
    char *title;
    bool completed;
    Rectangle bounds;
    int32_t parent_id;
    struct Todo *parent;
    struct Todo **children;
    int32_t depth;
    int32_t num_children;
    int32_t capacity;
    bool expanded;
} Todo;

extern bool show_input_modal;
extern char input_buffer[50];
extern struct Todo *pending_parent;

Todo *newTodo(char *title);
void destroyTodo(Todo *todo);
void removeChildFromParent(Todo *parent, Todo *child);
void addChild(Todo *parent, Todo *child);
int32_t calculateLayout(Todo *root, int32_t x, int32_t y);
void moveTodoDown(Todo *todo);
void moveTodoUp(Todo *todo);
char *takeTodoTitle();
int32_t calcTotalHeight(Todo *root);
char *intToString(int32_t num);

Rectangle newRectangle(int x, int y, int width, int height);
Vector2 newVector2(float x, float y);
void drawLayout(Todo *todo, Font *font);

Font *loadFontWithGlyph();
#endif // TASKTREE_H

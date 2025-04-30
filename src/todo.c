#include "../include/tasktree.h"

const int32_t TODO_WIDTH = 400;
const int32_t TODO_HEIGHT = 100;

Todo *createTodo(char *title) {
    Todo *todo = malloc(sizeof(Todo));
    todo->id = random_number();
    todo->title = title;
    todo->completed = false;
    todo->bounds = (Rectangle){0, 0, 0, 0};
    todo->parent_id = 0;
    todo->children = NULL;
    todo->num_children = 0;
    todo->capacity = 0;
    return todo;
}

void destroyTodo(Todo *todo) {
    free(todo->title);
    for (int i = 0; i < todo->num_children; ++i) {
        destroyTodo(todo->children[i]);
    }
    free(todo->children);
    free(todo);
}

void addChild(Todo *parent, Todo *child) {
    if (parent->num_children == parent->capacity) {
        parent->capacity = parent->capacity == 0 ? 1 : parent->capacity * 2;
        parent->children = realloc(parent->children, parent->capacity * sizeof(Todo *));
        if (parent->children == NULL) {
            printf("Memory allocation failed\n");
            exit(1);
        }
        child->parent_id = parent->id;
    }
    parent->children[parent->num_children++] = child;
}

int32_t calculateLayout(Todo *root, int32_t x, int32_t y) {
    root->bounds.x = x;
    root->bounds.y = y;
    root->bounds.width = TODO_WIDTH;
    root->bounds.height = TODO_HEIGHT;
    int32_t child_y = y + TODO_HEIGHT;
    if (root->expanded) {
        for (int32_t i = 0; i < root->num_children; ++i) {
            child_y += calculateLayout(root->children[i], x, child_y);
        }
    }
    return child_y;
}

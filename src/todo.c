#include "../include/tasktree.h"

const int32_t TODO_WIDTH = 800;
const int32_t TODO_HEIGHT = 125;

Todo *newTodo(char *title) {
    Todo *todo = malloc(sizeof(Todo));
    todo->id = random_number();
    todo->title = strdup(title);
    todo->completed = false;
    todo->bounds = (Rectangle){0, 0, 0, 0};
    todo->parent_id = 0;
    todo->children = NULL;
    todo->parent = NULL;
    todo->num_children = 0;
    todo->depth = 0;
    todo->capacity = 0;
    todo->expanded = true;
    return todo;
}

void destroyTodo(Todo *todo) {
    if (!todo) return;
    free(todo->title);
    for (int i = 0; i < todo->num_children; ++i) {
        destroyTodo(todo->children[i]);
    }
    free(todo->children);
    if (todo->parent) removeChildFromParent(todo->parent, todo);
    free(todo);
}

void removeChildFromParent(Todo *parent, Todo *child) {
    int32_t j = 0;
    if (parent->children != NULL) {
        for (int32_t i = 0; i < parent->num_children; ++i) {
            if (parent->children[i] != child) {
                parent->children[j++] = parent->children[i];
            }
        }
        parent->num_children--;
    }
}

void addChild(Todo *parent, Todo *child) {
    if (parent->num_children == parent->capacity) {
        parent->capacity = parent->capacity == 0 ? 1 : parent->capacity * 2;
        parent->children = realloc(parent->children, parent->capacity * sizeof(Todo *));
        if (parent->children == NULL) {
            printf("Memory allocation failed in addChild\n");
            exit(1);
        }
    }
    child->parent_id = parent->id;
    child->depth = parent->depth + 1;
    child->parent = parent;
    parent->children[parent->num_children++] = child;
}

int32_t calculateLayout(Todo *root, int32_t x, int32_t y) {
    root->bounds.x = x;
    root->bounds.y = y;
    root->bounds.width = TODO_WIDTH;
    root->bounds.height = TODO_HEIGHT;
    int32_t child_y = y + TODO_HEIGHT;
    if (root->children && root->expanded) {
        for (int32_t i = 0; i < root->num_children; ++i) {
            child_y = calculateLayout(root->children[i], x, child_y);
        }
    }
    return child_y;
}

void moveTodoUp(Todo *todo) {
    if (!todo || !todo->parent || todo->parent->num_children <= 1) return;
    Todo *parent = todo->parent;
    for (int32_t i = 1; i < parent->num_children; ++i) {
        if (parent->children[i] == todo) {
            Todo *prev = parent->children[i - 1];
            parent->children[i - 1] = parent->children[i];
            parent->children[i] = prev;
            return;
        }
    }
}

void moveTodoDown(Todo *todo) {
    if (!todo || !todo->parent || todo->parent->num_children <= 1) return;
    Todo *parent = todo->parent;
    int32_t idx = -1;
    for (int32_t i = 0; i < parent->num_children - 1; ++i) {
        if (parent->children[i] == todo) {
            idx = i;
            break;
        }
    }
    if (idx == -1) return;
    printf("Before move:");
    for (int i = 0; i < parent->num_children; i++)
        printf(" [%d]%s", i, parent->children[i]->title);

    Todo *next = parent->children[idx + 1];
    parent->children[idx + 1] = parent->children[idx];
    parent->children[idx] = next;

    printf("\nAfter move:");
    for (int i = 0; i < parent->num_children; i++)
        printf(" [%d]%s", i, parent->children[i]->title);
}

int32_t calcTotalHeight(Todo *root) {
    int32_t total_height = 0;
    if (root->expanded) {
        for (int32_t i = 0; i < root->num_children; ++i) {
            total_height += calcTotalHeight(root->children[i]);
        }
    }
    return total_height + TODO_HEIGHT;
}

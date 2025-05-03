#include "../include/tasktree.h"
#include <sqlite3.h>

void saveTodos(sqlite3 *db, Todo *root) {
    if (root->id != 0) {
        char *sql = "INSERT INTO todos (id, title, completed, parent_id) VALUES (?, ?, ?, ?)";
        sqlite3_stmt *stmt;
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
            fprintf(stderr, "Failed to prepare SQL statement: %s\n", sqlite3_errmsg(db));
            exit(EXIT_FAILURE);
        }
        sqlite3_bind_int(stmt, 1, root->id);
        sqlite3_bind_text(stmt, 2, root->title, -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 3, root->completed);
        sqlite3_bind_int(stmt, 4, root->parent_id);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    for (int32_t i = 0; i < root->num_children; i++) {
        saveTodos(db, root->children[i]);
    }
}

Todo **loadTodosArray(sqlite3 *db, int32_t *number_of_todos) {
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM todos", -1, &stmt, NULL);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        *number_of_todos = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    Todo **todoArray = malloc(sizeof(Todo *) * *number_of_todos);
    if (!todoArray) {
        fprintf(stderr, "Memory allocation failed in loadTodosArray function\n");
        exit(EXIT_FAILURE);
    }
    if (sqlite3_prepare_v2(db, "SELECT * FROM todos", -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare SQL statement: %s\n", sqlite3_errmsg(db));
        exit(EXIT_FAILURE);
    }
    int32_t i = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Todo *todo = malloc(sizeof(Todo));
        if (!todo) {
            fprintf(stderr, "Memory allocation failed in loadTodosArray function in todo\n");
            exit(EXIT_FAILURE);
        }
        todo->id = sqlite3_column_int(stmt, 0);
        todo->title = strdup((char *)sqlite3_column_text(stmt, 1));
        todo->completed = sqlite3_column_int(stmt, 2);
        todo->parent_id = sqlite3_column_int(stmt, 3);
        todo->parent = NULL;
        todo->children = NULL;
        todo->depth = 0;
        todo->num_children = 0;
        todo->capacity = 0;
        todo->expanded = false;
        todoArray[i++] = todo;
    }
    todoArray = realloc(todoArray, sizeof(Todo *) * i);
    sqlite3_finalize(stmt);
    *number_of_todos = i;
    return todoArray;
}

void loadTodos(sqlite3 *db, Todo *todo, Todo **todoArray, int32_t *no_of_todos) {
    int32_t number_of_children = 0;
    for (int32_t i = 0; i < *no_of_todos; ++i) {
        if (todoArray[i]->parent_id == todo->id) {
            number_of_children++;
        }
    }
    if (number_of_children == 0) {
        todo->children = NULL;
        todo->num_children = 0;
        todo->capacity = 0;
        return;
    }
    Todo **children = malloc(sizeof(Todo *) * number_of_children);
    if (!children) {
        fprintf(stderr, "Memory allocation failed in loadTodosArray function in children\n");
        exit(EXIT_FAILURE);
    }
    int32_t j = 0;
    for (int32_t i = 0; i < *no_of_todos; ++i) {
        if (todoArray[i]->parent_id == todo->id) {
            children[j++] = todoArray[i];
            todoArray[i]->parent = todo;
            todoArray[i]->parent_id = todo->id;
            todoArray[i]->depth = todo->depth + 1;
            todoArray[i]->children = NULL;
            todoArray[i]->num_children = 0;
            todoArray[i]->capacity = 0;
        }
    }
    todo->children = children;
    todo->num_children = number_of_children;
    todo->capacity = number_of_children;
    for (int32_t i = 0; i < number_of_children; ++i) {
        loadTodos(db, children[i], todoArray, no_of_todos);
    }
}

// TODO: and i will be calling the recursive function in one of the other functions so that i can return the root which will go to main function
// TODO: a function to recursive function to start from parent_id 0, get all todos and go through children each and do the same

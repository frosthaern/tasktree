#include "../include/tasktree.h"

const int32_t MAIN_WINDOW_WIDTH = 900;
const int32_t MAIN_WINDOW_HEIGHT = 900;
const char *MAIN_WINDOW_TITLE = "tasktree";

int main() {
    init_rand();
    Font f = LoadFont("/usr/share/fonts/TTF/JetBrainsMono-Regular.ttf");
    SetTargetFPS(10);
    InitWindow(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, MAIN_WINDOW_TITLE);

    // Create root todo and children
    Todo *root = createTodo("Root Todo");
    root->expanded = true;
    Todo *child1 = createTodo("Child 1");
    Todo *child2 = createTodo("Child 2");
    Todo *child3 = createTodo("Child 3");
    addChild(root, child1);
    addChild(root, child2);
    addChild(root, child3);
    child2->completed = true;

    // Layout the todos
    calculateLayout(root, 20, 20);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        drawLayout(root, &f);
        EndDrawing();
    };

    destroyTodo(root);
    CloseWindow();
    UnloadFont(f);
    return 0;
}

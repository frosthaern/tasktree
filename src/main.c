#include "../include/tasktree.h"
#include <raylib.h>

const char *MAIN_WINDOW_TITLE = "tasktree";
const int32_t SCROLL_MULTIPLYER = 20;

int main() {
    int32_t scroll_offset = -1;
    int32_t MAIN_WINDOW_WIDTH = 650;
    int32_t MAIN_WINDOW_HEIGHT = 1000;
    init_rand();
    Font f = LoadFont("/usr/share/fonts/TTF/JetBrainsMono-Regular.ttf");
    SetTargetFPS(60);
    InitWindow(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, MAIN_WINDOW_TITLE);

    Todo *root = newTodo("Root Todo");
    root->expanded = true;
    Todo *child1 = newTodo("Child 1");
    Todo *child2 = newTodo("Child 2");
    Todo *child3 = newTodo("Child 3");
    addChild(root, child1);
    addChild(root, child2);
    addChild(root, child3);
    child2->completed = true;

    while (!WindowShouldClose()) {
        scroll_offset += GetMouseWheelMove() * SCROLL_MULTIPLYER;
        printf("scroll_offset: %d\n", scroll_offset);
        int32_t total_height = calcTotalHeight(root);
        if (total_height < MAIN_WINDOW_HEIGHT) {
            scroll_offset = 0;
            SetWindowSize(MAIN_WINDOW_WIDTH, total_height + 25);
        }
        if (scroll_offset > total_height - MAIN_WINDOW_HEIGHT) scroll_offset = total_height - MAIN_WINDOW_HEIGHT;
        if (scroll_offset < 0) scroll_offset = 0;
        calculateLayout(root, 20, 20 - scroll_offset);
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

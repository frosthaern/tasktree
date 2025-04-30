#include "../include/tasktree.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TODO_TITLE_MAX_LEN 70

char *takeTodoTitle() {
    const int popupWidth = 500, popupHeight = 180;
    const int screenWidth = 600, screenHeight = 300;
    InitWindow(screenWidth, screenHeight, "New Todo Title");
    SetTargetFPS(60);
    char buffer[TODO_TITLE_MAX_LEN + 1] = {0};
    int letterCount = 0;
    bool done = false, canceled = false;
    while (!done && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawRectangle((screenWidth - popupWidth) / 2, (screenHeight - popupHeight) / 2, popupWidth, popupHeight, LIGHTGRAY);
        DrawRectangleLines((screenWidth - popupWidth) / 2, (screenHeight - popupHeight) / 2, popupWidth, popupHeight, DARKGRAY);
        DrawText("Enter todo title (max 70 chars):", (screenWidth - popupWidth) / 2 + 20, (screenHeight - popupHeight) / 2 + 20, 20, BLACK);
        DrawRectangle((screenWidth - popupWidth) / 2 + 20, (screenHeight - popupHeight) / 2 + 60, popupWidth - 40, 32, WHITE);
        DrawText(buffer, (screenWidth - popupWidth) / 2 + 28, (screenHeight - popupHeight) / 2 + 66, 20, BLACK);
        Rectangle okBtn = {(screenWidth - popupWidth) / 2 + 60, (screenHeight - popupHeight) / 2 + 110, 100, 40};
        Rectangle cancelBtn = {(screenWidth - popupWidth) / 2 + 240, (screenHeight - popupHeight) / 2 + 110, 100, 40};
        DrawRectangleRec(okBtn, DARKGREEN);
        DrawText("OK", okBtn.x + 35, okBtn.y + 10, 20, WHITE);
        DrawRectangleRec(cancelBtn, DARKGRAY);
        DrawText("Cancel", cancelBtn.x + 15, cancelBtn.y + 10, 20, WHITE);
        EndDrawing();
        // Handle input
        int key = GetCharPressed();
        while (key > 0) {
            if (((key >= 32) && (key <= 125)) && (letterCount < TODO_TITLE_MAX_LEN)) {
                buffer[letterCount++] = (char)key;
                buffer[letterCount] = '\0';
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && letterCount > 0) {
            buffer[--letterCount] = '\0';
        }
        // Mouse click handling
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();
            if (CheckCollisionPointRec(mouse, okBtn)) {
                done = true;
            } else if (CheckCollisionPointRec(mouse, cancelBtn)) {
                canceled = true;
                done = true;
            }
        }
        // Enter key as OK
        if (IsKeyPressed(KEY_ENTER)) {
            done = true;
        }
        // Escape key as Cancel
        if (IsKeyPressed(KEY_ESCAPE)) {
            canceled = true;
            done = true;
        }
    }
    CloseWindow();
    if (canceled || letterCount == 0) return NULL;
    return strdup(buffer);
}

void init_rand() { srand(time(NULL)); }

int32_t random_number() { return (int32_t)rand(); }

char *intToString(int32_t n) {
    int32_t depth_length = snprintf(NULL, 0, "%d", n);
    char *depth = malloc(sizeof(char) * (depth_length + 1));
    snprintf(depth, depth_length + 1, "%d", n);
    return depth;
}

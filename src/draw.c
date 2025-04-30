#include "../include/tasktree.h"

const int32_t SQUARE_BUTTON_SIZE = 20;
const int32_t SQUARE_BUTTON_PADDING = 8;

Rectangle newRectangle(int x, int y, int width, int height) {
    Rectangle rect = {x, y, width, height};
    return rect;
}

Vector2 newVector2(float x, float y) {
    Vector2 vec = {x, y};
    return vec;
}

void drawLayout(Todo *todo, Font *f) {
    // buttons for expand contract, delete, add, done and notdone
    Rectangle todo_border_bounds = newRectangle(todo->bounds.x + 1, todo->bounds.y + 1, todo->bounds.width - 2, todo->bounds.height - 2);
    Rectangle todo_rectangle_bounds = newRectangle(todo->bounds.x + 2, todo->bounds.y + 2, todo->bounds.width - 4, todo->bounds.height - 4);
    DrawRectangleLinesEx(todo_border_bounds, 1, BLACK);
    DrawRectangleRec(todo_rectangle_bounds, LIGHTGRAY);
    Vector2 todo_title_coords = newVector2(todo_rectangle_bounds.x + 2, todo_rectangle_bounds.y + 2);
    DrawTextEx(*f, todo->title, todo_title_coords, 18, 0, BLACK);
    const int32_t num_buttons = 4;

    int32_t start_x = todo->bounds.x + SQUARE_BUTTON_PADDING;
    int32_t start_y = todo->bounds.y + todo->bounds.height - SQUARE_BUTTON_SIZE - SQUARE_BUTTON_PADDING;

    const char *labels[4] = {"+", "-", "✓", ">"};
    if (todo->completed) labels[2] = "✗";
    if (todo->expanded) labels[3] = "v";

    for (int i = 0; i < num_buttons; ++i) {
        Rectangle btn = newRectangle(start_x + i * (SQUARE_BUTTON_SIZE + SQUARE_BUTTON_PADDING), start_y, SQUARE_BUTTON_SIZE, SQUARE_BUTTON_SIZE);
        DrawRectangleLinesEx(btn, 2, BLACK);
        if (GuiButton(btn, labels[i])) {
            switch (i) {
            // case 0: for adding todo
            // case 1: for deleting todo
            case 2:
                todo->completed = !todo->completed;
                break;
            case 3:
                todo->expanded = !todo->expanded;
                break;
            }
            int text_width = MeasureText(labels[i], 18);
            int text_x = btn.x + (int32_t)((SQUARE_BUTTON_SIZE - text_width) / 2);
            int text_y = btn.y + (int32_t)((SQUARE_BUTTON_SIZE - 18) / 2);
            DrawText(labels[i], text_x, text_y, 18, WHITE);
        }
    }
}

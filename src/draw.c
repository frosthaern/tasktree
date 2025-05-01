#include "../include/tasktree.h"
#include <raylib.h>
#include <stdio.h>

const int32_t SQUARE_BUTTON_SIZE = 40;
const int32_t SQUARE_BUTTON_PADDING = 10;

Rectangle newRectangle(int x, int y, int width, int height) {
    Rectangle rect = {x, y, width, height};
    return rect;
}

Vector2 newVector2(float x, float y) {
    Vector2 vec = {x, y};
    return vec;
}

void drawLayout(Todo *todo, Font *font) {
    Rectangle todo_border_bounds = newRectangle(todo->bounds.x + 5, todo->bounds.y + 5, todo->bounds.width - 10, todo->bounds.height - 10);
    Rectangle todo_rectangle_bounds = newRectangle(todo->bounds.x + 10, todo->bounds.y + 10, todo->bounds.width - 20, todo->bounds.height - 20);
    DrawRectangleLinesEx(todo_border_bounds, 3, BLACK);
    DrawRectangleRec(todo_rectangle_bounds, LIGHTGRAY);

    if (todo->parent != NULL) {
        Vector2 todo_depth_coords = newVector2(todo_rectangle_bounds.x + 10, todo_rectangle_bounds.y + 10);
        char *depth = intToString(todo->depth);
        DrawTextEx(*font, depth, todo_depth_coords, FONTSIZE, 0, BLACK);
        Rectangle todo_depth_border_bounds = newRectangle(todo_depth_coords.x - 5, todo_depth_coords.y - 5, (strlen(depth) + 1) * 13, 36);
        DrawRectangleLinesEx(todo_depth_border_bounds, 2, BLACK);
        Vector2 todo_title_coords = newVector2(todo_depth_border_bounds.x + todo_depth_border_bounds.width + 20, todo_depth_border_bounds.y);
        DrawTextEx(*font, todo->title, todo_title_coords, FONTSIZE, 0, BLACK);
        if (todo->completed) {
            int32_t textWidth = MeasureText(todo->title, FONTSIZE);
            int32_t line_y_coords = todo_title_coords.y + (int32_t)(FONTSIZE / 2);
            // DrawLine(todo_title_coords.x, line_y_coords, todo_title_coords.x + textWidth, line_y_coords, BLACK);
            DrawLineEx((Vector2){todo_title_coords.x, line_y_coords}, (Vector2){todo_title_coords.x + textWidth, line_y_coords}, 3, BLACK);
        }
        const int32_t num_buttons = 6;

        int32_t start_x = todo_rectangle_bounds.x + SQUARE_BUTTON_PADDING;
        int32_t start_y = todo_rectangle_bounds.y + todo_rectangle_bounds.height - SQUARE_BUTTON_SIZE - SQUARE_BUTTON_PADDING;

        const char *labels[6] = {
            "\u22a0", // Add
            "\uf1f8", // Remove
            "\uf096", // Unchecked
            "\u25b6", // Expand
            "\u25b2", // Up
            "\u25bc"  // Down
        };
        if (todo->completed) labels[2] = "\u25a0"; // Checked
        if (todo->expanded) labels[3] = "\u25bc";
        for (int i = 0; i < num_buttons; ++i) {
            Rectangle btn = newRectangle(start_x + i * (SQUARE_BUTTON_SIZE + SQUARE_BUTTON_PADDING), start_y, SQUARE_BUTTON_SIZE, SQUARE_BUTTON_SIZE);
            if (GuiButton(btn, labels[i])) {
                switch (i) {
                case 0:
                    show_input_modal = true;
                    pending_parent = todo;
                    input_buffer[0] = '\0';
                    return;
                case 1:
                    destroyTodo(todo);
                    return;
                case 2:
                    todo->completed = !todo->completed;
                    return;
                case 3:
                    todo->expanded = !todo->expanded;
                    return;
                case 4:
                    moveTodoUp(todo);
                    return;
                case 5:
                    moveTodoDown(todo);
                    return;
                }
                return;
            }
        }
    } else {
        if (GuiButton(todo_rectangle_bounds, "+")) {
            show_input_modal = true;
            pending_parent = todo;
            input_buffer[0] = '\0';
        }
    }

    if (todo->expanded) {
        for (int32_t i = 0; i < todo->num_children; ++i) {
            if (todo->children[i] == NULL) {
                printf("  ERROR: child %d is NULL!\n", i);
                continue;
            }
            drawLayout(todo->children[i], font);
        }
    }
}

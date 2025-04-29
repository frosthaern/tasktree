#include <stdint.h>
#include <raylib.h>

const int32_t MAIN_WINDOW_WIDTH = 900;
const int32_t MAIN_WINDOW_HEIGHT = 900;
const char* MAIN_WINDOW_TITLE = "tasktree";

int main() {
    Font f = LoadFont("/usr/share/fonts/TTF/JetBrainsMono-Regular.ttf");
    SetTargetFPS(10);
    InitWindow(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, MAIN_WINDOW_TITLE);
    while (!WindowShouldClose()){
        BeginDrawing();

        EndDrawing();
    };
}

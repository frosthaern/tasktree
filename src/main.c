#include "../include/font.h"
#include "../include/tasktree.h"

const char *MAIN_WINDOW_TITLE = "tasktree";
const int32_t SCROLL_MULTIPLYER = 50;
static const int32_t MAIN_WINDOW_WIDTH = 800;
static const int32_t MAIN_WINDOW_HEIGHT = 800;
const int32_t INPUT_BOX_WIDTH = 750;
const int32_t INPUT_BOX_HEIGHT = 100;
const int32_t FONTSIZE = 24;
bool show_input_modal = false;
char input_buffer[INPUT_MODEL_BUFFER_SIZE] = "";
struct Todo *pending_parent = NULL;
const int32_t NUM_ASCII = 95;
const int32_t NUM_EXTRA = 7;

int main() {
    int32_t scroll_offset = -1;
    init_rand();
    InitWindow(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, MAIN_WINDOW_TITLE);
    Font *f = loadFontWithGlyph();
    GuiSetFont(*f);
    GuiSetStyle(DEFAULT, TEXT_SIZE, FONTSIZE);
    SetTargetFPS(120);
    SetExitKey(0);
    sqlite3 *db;
    sqlite3_open("todos.db", &db);
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS todos (id INTEGER PRIMARY KEY, title TEXT NOT NULL, completed INTEGER NOT NULL DEFAULT 0, parent_id INTEGER NOT NULL);", 0, 0, 0);
    Todo *root = newTodo("Root Todo");
    root->id = 0;
    root->capacity = 0;
    root->depth = 0;
    root->parent = NULL;
    root->expanded = true;
    int32_t numTodos = 0;
    Todo **todoArray = loadTodosArray(db, &numTodos);
    loadTodos(db, root, todoArray, &numTodos);
    if (sqlite3_exec(db, "DELETE FROM todos;", 0, 0, 0) != SQLITE_OK) {
        printf("[ERROR]: error deleting todos in main file %s\n", sqlite3_errmsg(db));
    }
    while (!WindowShouldClose()) {
        scroll_offset += GetMouseWheelMove() * SCROLL_MULTIPLYER;
        int32_t total_height = calcTotalHeight(root);
        if (scroll_offset > total_height - MAIN_WINDOW_HEIGHT) scroll_offset = total_height - MAIN_WINDOW_HEIGHT;
        if (scroll_offset < 0) scroll_offset = 0;
        calculateLayout(root, 0, 0 - scroll_offset);
        BeginDrawing();
        ClearBackground(RAYWHITE);
        drawLayout(root, f);
        if (show_input_modal) {
            Rectangle input_box_bounds = newRectangle(((MAIN_WINDOW_WIDTH / 2) - (INPUT_BOX_WIDTH / 2)), ((MAIN_WINDOW_HEIGHT / 2) - (INPUT_BOX_HEIGHT / 2)), INPUT_BOX_WIDTH, INPUT_BOX_HEIGHT);
            GuiTextBox(input_box_bounds, input_buffer, INPUT_MODEL_BUFFER_SIZE, true);
            if (IsKeyPressed(KEY_ENTER) && input_buffer[0] != '\0') {
                Todo *new_child_parent = newTodo(strdup(input_buffer));
                addChild(pending_parent, new_child_parent);
                show_input_modal = false;
            }
            if (IsKeyPressed(KEY_ESCAPE)) {
                show_input_modal = false;
            }
        }
        EndDrawing();
    };
    saveTodos(db, root);
    if (sqlite3_close(db) == SQLITE_OK) {
        printf("database closed properly");
    }
    destroyTodo(root);
    CloseWindow();
    return 0;
}

Font *loadFontWithGlyph() {
    int codepoints[NUM_ASCII + NUM_EXTRA];
    for (int i = 0; i < NUM_ASCII; i++) {
        codepoints[i] = 0x20 + i;
    }
    codepoints[NUM_ASCII + 0] = 0x22a0;
    codepoints[NUM_ASCII + 1] = 0xf1f8;
    codepoints[NUM_ASCII + 2] = 0xf096;
    codepoints[NUM_ASCII + 3] = 0x25a0;
    codepoints[NUM_ASCII + 4] = 0x25b6;
    codepoints[NUM_ASCII + 5] = 0x25bc;
    codepoints[NUM_ASCII + 6] = 0x25b2;
    Font *f = malloc(sizeof(Font));
    *f = LoadFontFromMemory(".ttf", trimmed_ttf, trimmed_ttf_len, FONTSIZE, codepoints, NUM_ASCII + NUM_EXTRA);
    return f;
}

#include "../include/tasktree.h"

void init_rand() { srand(time(NULL)); }

int32_t random_number() { return (int32_t)rand(); }

char *intToString(int32_t n) {
    int32_t depth_length = snprintf(NULL, 0, "%d", n);
    char *depth = malloc(sizeof(char) * (depth_length + 1));
    snprintf(depth, depth_length + 1, "%d", n);
    return depth;
}

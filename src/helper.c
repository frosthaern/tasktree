#include <stdint.h>
#include <stdlib.h>
#include <time.h>

void init_rand() { srand(time(NULL)); }

int32_t random_number() { return (int32_t)rand(); }

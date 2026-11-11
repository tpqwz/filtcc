#include <stdlib.h>
void *aligned_alloc(size_t, size_t);
int main(void) {
    char *p = aligned_alloc(64, 64);
    if (!p)
        return 2;
    p[64] = 1;
    return 0;
}

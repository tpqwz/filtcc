#include <stdint.h>
#include <stdlib.h>
void *aligned_alloc(size_t, size_t);
int main(void) {
    char *p = aligned_alloc(64, 128);
    if (!p)
        return 2;
    if (((uintptr_t)p & 63) != 0)
        return 3;
    p[0] = 1;
    p[127] = 2;
    free(p);
    return 0;
}

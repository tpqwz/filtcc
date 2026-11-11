#include <stdint.h>
#include <stdlib.h>
void *aligned_alloc(size_t, size_t);
int main(void) {
    void *(*f)(size_t, size_t) = aligned_alloc;
    char *p = f(256, 256);
    if (!p)
        return 2;
    if (((uintptr_t)p & 255) != 0)
        return 3;
    p[255] = 1;
    free(p);
    return 0;
}

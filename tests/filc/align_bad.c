#include <stdlib.h>
void *aligned_alloc(size_t, size_t);
int main(void) {
    if (aligned_alloc(24, 48))
        return 2;
    if (aligned_alloc(64, 32))
        return 3;
    if (aligned_alloc(0, 0))
        return 4;
    return 0;
}

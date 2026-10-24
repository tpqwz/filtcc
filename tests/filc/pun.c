#include <stdint.h>
int main(void) {
    int x = 11;
    int y = 22;
    int *p = &x;
    uintptr_t *u = (uintptr_t *)&p;
    *u = (uintptr_t)&y;
    return *p;
}

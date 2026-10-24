#include <stdlib.h>
#include <stdint.h>
int main(void) {
    int *p = malloc(sizeof(int));
    int *q = (int *)(uintptr_t)p;
    *p = 1;
    return *q;
}

#include <stdlib.h>
int main(void) {
    char *p = malloc(4);
    if (!p)
        return 2;
    p[4] = 1;
    return 0;
}

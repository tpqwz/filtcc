#include <stdlib.h>
int main(void)
{
    char *p = malloc(16);
    char *q;
    if (!p)
        return 2;
    q = realloc(p, 32);
    if (!q)
        return 3;
    p[0] = 1;
    return q[0];
}

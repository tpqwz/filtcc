#include <stdlib.h>
int main(void)
{
    int x = 4, y = 7;
    int **p, **q, **r;
    void *(*rf)(void *, size_t) = realloc;
    int *z;
    free(0);
    p = realloc(0, 2 * sizeof(int *));
    if (!p)
        return 1;
    p[0] = &x;
    p[1] = &y;
    if (*p[0] != 4 || *p[1] != 7)
        return 2;
    q = realloc(p, sizeof(int *));
    if (!q || q[0] != &x || *q[0] != 4)
        return 3;
    r = realloc(q, 2 * sizeof(int *));
    if (!r || r[0] != &x || *r[0] != 4)
        return 4;
    r[1] = &y;
    if (*r[1] != 7)
        return 5;
    free(r);
    z = rf(0, sizeof(int));
    if (!z)
        return 6;
    *z = 9;
    if (*z != 9)
        return 7;
    free(z);
    return 0;
}

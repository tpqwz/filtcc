#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int hits;
static void *mine(size_t n)
{
    hits++;
    return malloc(n);
}
int main(void)
{
    void *(*mf)(size_t) = malloc;
    void *(*cp)(void *, const void *, size_t) = memcpy;
    char *(*sf)(const char *, int) = strchr;
    char *(*df)(const char *) = strdup;
    int (*pf)(const char *, ...) = printf;
    void *(*uf)(size_t) = mine;
    char *p, buf[8], *c, *d, *u;
    p = mf(8);
    if (!p)
        return 1;
    p[0] = 'a';
    p[7] = 'z';
    cp(buf, p, 8);
    if (buf[0] != 'a' || buf[7] != 'z')
        return 2;
    c = sf("abc", 'b');
    if (!c || c[0] != 'b' || c[1] != 'c')
        return 3;
    d = df("hi");
    if (!d || d[0] != 'h' || d[1] != 'i' || d[2] != 0)
        return 4;
    d[0] = 'H';
    if (d[0] != 'H')
        return 5;
    u = uf(4);
    if (!hits || !u)
        return 6;
    u[0] = 1;
    u[3] = 2;
    if (pf("ind %d\n", 3) < 0)
        return 7;
    free(p);
    free(d);
    return 0;
}

#include <stdlib.h>
int read_at(int *p) { return *p; }
int *id(int *p) { return p; }
int main(void) {
    int *p = malloc(sizeof(int));
    int x = 5;
    int a[4];
    struct { int *p; } s, t;
    int *q;
    if (!p) return 9;
    *p = 7;
    if (read_at(p) != 7) return 1;
    if (*id(p) != 7) return 1;
    q = p;
    if (*q != 7) return 3;
    free(p);
    s.p = &x;
    t = s;
    if (*t.p != 5) return 2;
    a[0] = 1;
    a[3] = 4;
    if (a[0] + a[3] != 5) return 4;
    return 0;
}

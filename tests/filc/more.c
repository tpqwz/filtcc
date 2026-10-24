#include <stdlib.h>
#include <string.h>
int read_at(int *p) { return *p; }
int main(void) {
    int a[4];
    int x = 11;
    int *p, *q, *r;
    int (*fp)(int *);
    char buf[8];
    char *s;
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    a[3] = 4;
    p = &a[1];
    if (p[0] != 2) return 1;
    if (p[2] != 4) return 2;
    q = a + 4;
    if (q - a != 4) return 3;
    if (p == 0) return 4;
    memcpy(buf, "abcd", 4);
    buf[4] = 0;
    if (buf[0] != 'a' || buf[3] != 'd') return 5;
    if (strlen(buf) != 4) return 6;
    s = malloc(8);
    if (!s) return 9;
    memcpy(s, buf, 5);
    if (s[0] != 'a' || s[4] != 0) return 7;
    if (strlen(s) != 4) return 8;
    free(s);
    r = &x;
    memcpy(&q, &r, sizeof q);
    if (*q != 11) return 10;
    fp = read_at;
    if (fp(&x) != 11) return 11;
    if (strlen("hi") != 2) return 12;
    return 0;
}

#include <string.h>
#include <stdlib.h>
int main(void)
{
    char *(*c1)(char *, const char *) = strcpy;
    char *(*c2)(char *, const char *, size_t) = strncpy;
    char *(*cat)(char *, const char *) = strcat;
    size_t (*len)(const char *, size_t) = strnlen;
    int (*cmp)(const void *, const void *, size_t) = memcmp;
    char *(*dup)(const char *, size_t) = strndup;
    char buf[16];
    char *p, *s;
    p = c1(buf, "xy");
    if (p != buf || p[0] != 'x' || p[2] != 0)
        return 1;
    c2(buf, "ab", 4);
    if (buf[0] != 'a' || buf[2] != 0)
        return 2;
    cat(buf, "z");
    if (buf[0] != 'a' || buf[1] != 'b' || buf[2] != 'z' || buf[3] != 0)
        return 3;
    s = malloc(2);
    s[0] = 'q';
    s[1] = 0;
    if (len(s, 8) != 1)
        return 4;
    if (cmp(buf, "abz", 3) != 0)
        return 5;
    p = dup("hello", 4);
    if (!p || p[0] != 'h' || p[4] != 0)
        return 6;
    p[0] = 'H';
    free(p);
    free(s);
    return 0;
}

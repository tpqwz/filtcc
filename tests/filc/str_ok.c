#include <string.h>
#include <stdlib.h>

int main(void)
{
    char buf[16];
    char raw[4];
    char out[4];
    char *p, *d, *src;
    long slots_s[2], slots_d[2];
    char *sp, *dp;
    int x, i;
    int *tmp;
    long bits;
    int *loaded;

    p = strcpy(buf, "hi");
    if (p != buf || p[0] != 'h' || p[1] != 'i' || p[2] != 0)
        return 1;
    p[0] = 'H';
    if (buf[0] != 'H')
        return 2;

    /* Interior return keeps the object's capability. */
    p = strcpy(buf + 1, "ab");
    if (p != buf + 1 || p[0] != 'a' || p[1] != 'b' || p[2] != 0)
        return 3;
    p[-1] = 'Q';
    if (buf[0] != 'Q')
        return 4;

    src = malloc(3);
    if (!src)
        return 5;
    src[0] = 'a';
    src[1] = 'b';
    src[2] = 0;
    strncpy(buf, src, 8);
    if (buf[0] != 'a' || buf[1] != 'b' || buf[2] != 0 || buf[7] != 0)
        return 6;

    raw[0] = 'w';
    raw[1] = 'x';
    raw[2] = 'y';
    raw[3] = 'z';
    strncpy(out, raw, 4);
    if (out[0] != 'w' || out[3] != 'z')
        return 7;

    strcpy(buf, "ab");
    strcat(buf, "cd");
    if (strcmp(buf, "abcd") != 0)
        return 8;
    strncat(buf, "efghij", 2);
    if (strcmp(buf, "abcdef") != 0)
        return 9;

    if (strnlen(src, 10) != 2 || strnlen(src, 1) != 1 || strnlen(src, 0) != 0)
        return 10;
    if (memcmp(buf, "abcdef", 6) != 0 || memcmp(buf, buf, 0) != 0)
        return 11;
    if (memcmp("ab", "ac", 2) >= 0)
        return 12;

    d = strndup("hello", 3);
    if (!d || d[0] != 'h' || d[1] != 'e' || d[2] != 'l' || d[3] != 0)
        return 13;
    d[0] = 'H';
    if (d[0] != 'H')
        return 14;

    /* strcpy moves an aligned capability that lives in the copied bytes. */
    x = 42;
    tmp = &x;
    sp = (char *)slots_s;
    dp = (char *)slots_d;
    memcpy(sp, &tmp, 8);
    bits = *(long *)sp;
    for (i = 0; i < 8; i++) {
        if (sp[i] == 0)
            sp[i] = 1;
    }
    sp[8] = 0;
    strcpy(dp, sp);
    *(long *)dp = bits;
    loaded = *(int **)dp;
    if (loaded != &x || *loaded != 42)
        return 15;

    free(src);
    free(d);
    return 0;
}

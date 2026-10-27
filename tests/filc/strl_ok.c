#include <string.h>
#include <stdlib.h>

int main(void)
{
    char buf[4];
    char full[4];
    size_t (*cp)(char *, const char *, size_t);
    size_t (*ct)(char *, const char *, size_t);
    int x;
    int *tmp;
    long slots_s[2], slots_d[2], slots_c[2];
    char *sp, *dp, *cpb;
    long bits;
    int i;
    int *loaded;

    if (strlcpy(buf, "hello", 4) != 5)
        return 2;
    if (buf[0] != 'h' || buf[1] != 'e' || buf[2] != 'l' || buf[3] != 0)
        return 3;
    buf[0] = 'Q';
    if (strlcpy(buf, "ab", 0) != 2 || buf[0] != 'Q')
        return 4;
    if (strlcpy(0, "ab", 0) != 2)
        return 5;

    buf[0] = 'x';
    buf[1] = 'y';
    buf[2] = 0;
    if (strlcat(buf, "hello", 4) != 7)
        return 6;
    if (buf[0] != 'x' || buf[1] != 'y' || buf[2] != 'h' || buf[3] != 0)
        return 7;

    full[0] = 'a';
    full[1] = 'b';
    full[2] = 'c';
    full[3] = 'd';
    if (strlcat(full, "Z", 4) != 5 || full[0] != 'a' || full[3] != 'd')
        return 8;
    if (strlcat(0, "Z", 0) != 1)
        return 9;

    buf[0] = 0;
    if (strlcat(buf, "ab", 4) != 2 || buf[0] != 'a' || buf[1] != 'b'
        || buf[2] != 0)
        return 10;

    cp = strlcpy;
    ct = strlcat;
    if (cp(buf, "z", 4) != 1 || buf[0] != 'z' || buf[1] != 0)
        return 11;
    if (ct(buf, "y", 4) != 2 || buf[0] != 'z' || buf[1] != 'y'
        || buf[2] != 0)
        return 12;

    /* A source that fits, including its NUL, moves an aligned cap. */
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
    if (strlcpy(dp, sp, 16) != 8)
        return 13;
    *(long *)dp = bits;
    loaded = *(int **)dp;
    if (loaded != &x || *loaded != 42)
        return 14;

    cpb = (char *)slots_c;
    cpb[0] = 0;
    if (strlcat(cpb, sp, 16) != 8)
        return 15;
    *(long *)cpb = bits;
    loaded = *(int **)cpb;
    if (loaded != &x || *loaded != 42)
        return 16;
    return 0;
}

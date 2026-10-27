#include <string.h>

int main(void)
{
    char buf[4];
    char *p;
    void *(*fn)(void *, const void *, size_t);
    int x;
    int *tmp;
    long slots_s[2], slots_d[2];
    char *sp, *dp;
    long bits;
    int i;
    int *loaded;
    void *end;

    p = mempcpy(buf, "ab", 2);
    if (p != buf + 2 || buf[0] != 'a' || buf[1] != 'b')
        return 2;
    *p = 'Z';
    if (buf[2] != 'Z')
        return 3;
    buf[0] = 'Q';
    p = mempcpy(buf, "ab", 0);
    if (p != buf || buf[0] != 'Q')
        return 4;

    fn = mempcpy;
    p = fn(buf, "xy", 2);
    if (p != buf + 2 || buf[0] != 'x' || buf[1] != 'y')
        return 5;

    /* Eight aligned bytes move the capability. The return is one past. */
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
    end = mempcpy(dp, sp, 8);
    if (end != dp + 8)
        return 6;
    *(long *)dp = bits;
    loaded = *(int **)dp;
    if (loaded != &x || *loaded != 42)
        return 7;
    return 0;
}

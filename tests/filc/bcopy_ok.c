#include <string.h>

int main(void)
{
    char dst[8];
    char src[8];
    char buf[8];
    void (*fn)(const void *, void *, size_t);
    int x;
    int *tmp;
    long slots_s[2], slots_d[2];
    char *sp, *dp;
    long bits;
    int i;
    int *loaded;

    memset(dst, 'x', 8);
    memcpy(src, "abcdefgh", 8);
    bcopy(src, dst, 4);
    if (memcmp(dst, "abcdxxxx", 8) != 0 || memcmp(src, "abcdefgh", 8) != 0)
        return 2;
    dst[0] = 'Q';
    bcopy(src, dst, 0);
    if (dst[0] != 'Q')
        return 3;
    bcopy("ab", dst, 2);
    if (dst[0] != 'a' || dst[1] != 'b' || dst[2] != 'c')
        return 4;

    memcpy(buf, "abcdefg", 8);
    bcopy(buf, buf + 2, 5);
    if (memcmp(buf, "ababcde", 7) != 0)
        return 5;
    memcpy(buf, "abcdefg", 8);
    bcopy(buf + 2, buf, 4);
    if (memcmp(buf, "cdefefg", 7) != 0)
        return 6;

    fn = bcopy;
    memset(dst, 'x', 8);
    fn(src, dst, 4);
    if (memcmp(dst, "abcdxxxx", 8) != 0 || memcmp(src, "abcdefgh", 8) != 0)
        return 7;

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
    bcopy(sp, dp, 8);
    *(long *)dp = bits;
    loaded = *(int **)dp;
    if (loaded != &x || *loaded != 42)
        return 8;
    return 0;
}

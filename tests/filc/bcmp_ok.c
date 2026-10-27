#include <string.h>

int main(void)
{
    char a[4] = {'a', 'b', 'c', 'd'};
    char b[4] = {'a', 'b', 'c', 'd'};
    char c[4] = {'a', 'b', 'X', 'd'};
    char raw[2];
    int (*fn)(const void *, const void *, size_t);

    if (bcmp(a, b, 4) != 0 || bcmp(a, a, 4) != 0)
        return 2;
    if (bcmp(a, c, 4) == 0 || bcmp(a, c, 4) != memcmp(a, c, 4))
        return 3;
    if (bcmp(c, a, 4) != memcmp(c, a, 4))
        return 4;
    if (bcmp(a, c, 0) != 0 || bcmp("ab", "zz", 0) != 0)
        return 5;
    if (bcmp("ab", "ac", 2) >= 0 || bcmp("ac", "ab", 2) <= 0)
        return 6;
    raw[0] = 'Q';
    raw[1] = 'Q';
    if (bcmp(raw, "QQ", 2) != 0)
        return 7;
    fn = bcmp;
    if (fn(a, c, 3) != bcmp(a, c, 3))
        return 8;
    return 0;
}

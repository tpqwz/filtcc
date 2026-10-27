#include <string.h>

int main(void)
{
    char buf[8];
    char slot;
    char a[8];
    char b[8];
    int (*cfn)(const char *, const char *);

    if (strcoll("a", "b") >= 0 || strcoll("b", "a") <= 0)
        return 2;
    if (strcoll("a", "a") != 0 || strcoll("", "") != 0)
        return 3;
    if (strxfrm(0, "ab", 0) != 2)
        return 4;
    if (strxfrm(buf, "ab", 8) != 2 || buf[0] != 'a' || buf[1] != 'b'
        || buf[2] != 0)
        return 5;
    slot = 99;
    if (strxfrm(&slot, "ab", 0) != 2 || slot != 99)
        return 6;
    if (strxfrm(a, "a", 8) != 1 || strxfrm(b, "b", 8) != 1
        || strcmp(a, b) >= 0)
        return 7;
    cfn = strcoll;
    if (cfn("a", "a") != 0)
        return 8;
    return 0;
}

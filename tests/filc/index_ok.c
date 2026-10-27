#include <string.h>

int main(void)
{
    char buf[8];
    char raw[1];
    char *p;
    char *(*fi)(const char *, int);
    char *(*fr)(const char *, int);

    memcpy(buf, "hello", 6);
    p = index(buf, 'e');
    if (p != buf + 1)
        return 2;
    *p = 'E';
    if (buf[1] != 'E')
        return 3;
    p = index(buf, 'z');
    if (p != 0)
        return 4;
    p = index(buf, 0);
    if (p != buf + 5 || *p != 0)
        return 5;
    p = index(buf, 'h' + 256);
    if (p != buf)
        return 6;

    raw[0] = 'A';
    p = index(raw, 'A');
    if (p != raw)
        return 7;
    *p = 'B';
    if (raw[0] != 'B')
        return 8;

    memcpy(buf, "ababa", 6);
    p = rindex(buf, 'a');
    if (p != buf + 4)
        return 9;
    *p = 'Z';
    if (buf[4] != 'Z')
        return 10;
    p = rindex(buf, 'q');
    if (p != 0)
        return 11;
    p = rindex(buf, 0);
    if (p != buf + 5)
        return 12;

    fi = index;
    fr = rindex;
    memcpy(buf, "hello", 6);
    p = fi(buf, 'l');
    if (p != buf + 2)
        return 13;
    p = fr(buf, 'l');
    if (p != buf + 3 || *p != 'l')
        return 14;
    return 0;
}

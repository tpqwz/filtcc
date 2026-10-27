#include <string.h>

int main(void)
{
    char buf[8];
    char raw[1];
    char *p;
    char *(*fn)(const char *, int);

    memcpy(buf, "hello", 6);
    p = strchrnul(buf, 'e');
    if (p != buf + 1)
        return 2;
    *p = 'E';
    if (buf[1] != 'E')
        return 3;
    p = strchrnul(buf, 'z');
    if (p != buf + 5 || *p != 0)
        return 4;
    *p = 'Q';
    if (buf[5] != 'Q')
        return 5;
    p = strchrnul(buf, 'h' + 256);
    if (p != buf || *p != 'h')
        return 6;

    buf[0] = 0;
    p = strchrnul(buf, 'a');
    if (p != buf || *p != 0)
        return 7;

    raw[0] = 'A';
    p = strchrnul(raw, 'A');
    if (p != raw)
        return 8;
    *p = 'B';
    if (raw[0] != 'B')
        return 9;

    fn = strchrnul;
    memcpy(buf, "hello", 6);
    p = fn(buf, 0);
    if (p != buf + 5 || *p != 0)
        return 10;
    p = fn(buf, 256);
    if (p != buf + 5)
        return 11;
    return 0;
}

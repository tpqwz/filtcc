#include <wchar.h>

int main(void)
{
    wchar_t buf[8];
    wchar_t one;
    wchar_t *end;
    wchar_t *(*cp)(wchar_t *, const wchar_t *);
    wchar_t *(*cn)(wchar_t *, const wchar_t *, size_t);

    end = wcpcpy(buf, L"");
    if (end != buf || *end != 0)
        return 2;
    end = wcpcpy(buf, L"ab");
    if (end != buf + 2 || *end != 0 || buf[0] != L'a' || buf[1] != L'b')
        return 3;
    *end = L'Z';
    if (buf[2] != L'Z')
        return 4;

    buf[5] = 99;
    end = wcpncpy(buf, L"hello", 8);
    if (end != buf + 5 || buf[0] != L'h' || buf[4] != L'o' || buf[5] != 0)
        return 5;
    *end = L'Q';
    if (buf[5] != L'Q')
        return 6;

    end = wcpncpy(buf, L"abcdef", 4);
    if (end != buf + 4 || buf[0] != L'a' || buf[3] != L'd')
        return 7;

    buf[0] = 7;
    one = L'A';
    end = wcpncpy(buf, &one, 0);
    if (end != buf || buf[0] != 7)
        return 8;

    cp = wcpcpy;
    cn = wcpncpy;
    end = cp(buf, L"xy");
    if (end != buf + 2 || buf[0] != L'x' || *end != 0)
        return 9;
    end = cn(buf, L"z", 4);
    if (end != buf + 1 || buf[0] != L'z' || buf[1] != 0 || buf[3] != 0)
        return 10;
    return 0;
}

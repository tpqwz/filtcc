#include <wchar.h>

int main(void)
{
    wchar_t buf[4];
    wchar_t full[4];
    size_t (*cp)(wchar_t *, const wchar_t *, size_t);
    size_t (*ct)(wchar_t *, const wchar_t *, size_t);

    if (wcslcpy(buf, L"hello", 4) != 5)
        return 2;
    if (buf[0] != L'h' || buf[1] != L'e' || buf[2] != L'l' || buf[3] != 0)
        return 3;
    buf[0] = L'Q';
    if (wcslcpy(buf, L"ab", 0) != 2 || buf[0] != L'Q')
        return 4;
    if (wcslcpy(0, L"ab", 0) != 2)
        return 5;

    buf[0] = L'x';
    buf[1] = L'y';
    buf[2] = 0;
    if (wcslcat(buf, L"hello", 4) != 7)
        return 6;
    if (buf[0] != L'x' || buf[1] != L'y' || buf[2] != L'h' || buf[3] != 0)
        return 7;

    full[0] = L'a';
    full[1] = L'b';
    full[2] = L'c';
    full[3] = L'd';
    if (wcslcat(full, L"Z", 4) != 5 || full[0] != L'a' || full[3] != L'd')
        return 8;
    if (wcslcat(0, L"Z", 0) != 1)
        return 9;

    buf[0] = 0;
    if (wcslcat(buf, L"ab", 4) != 2 || buf[0] != L'a' || buf[1] != L'b'
        || buf[2] != 0)
        return 10;

    cp = wcslcpy;
    ct = wcslcat;
    if (cp(buf, L"z", 4) != 1 || buf[0] != L'z' || buf[1] != 0)
        return 11;
    if (ct(buf, L"y", 4) != 2 || buf[0] != L'z' || buf[1] != L'y'
        || buf[2] != 0)
        return 12;
    return 0;
}

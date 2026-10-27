#include <stdio.h>
#include <string.h>
#include <wchar.h>

int main(void)
{
    wchar_t buf[16];
    wchar_t w[8];
    wchar_t one[1];
    wchar_t scanned[4];
    wchar_t ch;
    char out[32];
    char bytes[16];
    wchar_t *p;
    size_t n;
    size_t (*wlen)(const wchar_t *) = wcslen;
    int x;
    int *tmp;
    long slots[2];
    long bits;

    if (wcslen(L"hi") != 2)
        return 1;
    p = wcscpy(buf, L"hi");
    if (p != buf || buf[0] != L'h' || buf[1] != L'i' || buf[2] != 0)
        return 2;
    p[0] = L'H';
    if (buf[0] != L'H')
        return 3;

    p = wcscpy(buf + 1, L"cd");
    if (p != buf + 1 || p[0] != L'c' || p[1] != L'd' || p[2] != 0)
        return 4;
    p[-1] = L'Q';
    if (buf[0] != L'Q')
        return 5;

    wcscpy(buf, L"ab");
    if (wcscat(buf, L"xy") != buf || wcscmp(buf, L"abxy") != 0)
        return 6;
    if (wcsncat(buf, L"efghij", 2) != buf || wcscmp(buf, L"abxyef") != 0)
        return 7;

    if (wcsnlen(L"hi", 10) != 2 || wcsnlen(L"hi", 1) != 1 || wcsnlen(L"hi", 0) != 0)
        return 8;
    if (wcsncmp(L"ab", L"ac", 1) != 0 || wcsncmp(L"ab", L"ac", 2) >= 0)
        return 9;
    if (wcsncmp(L"ab", L"ab", 0) != 0)
        return 10;

    {
        wchar_t raw[4];
        wchar_t dest[4];
        wchar_t a[1], b[1];
        raw[0] = L'w';
        raw[1] = L'x';
        raw[2] = L'y';
        raw[3] = L'z';
        wcsncpy(dest, raw, 4);
        if (dest[0] != L'w' || dest[3] != L'z')
            return 11;
        if (wcsncmp(raw, dest, 4) != 0)
            return 12;
        a[0] = L'Q';
        b[0] = L'Q';
        if (wcsncmp(a, b, 1) != 0)
            return 13;
        dest[0] = L'K';
        if (wcsncpy(dest, a, 0) != dest || dest[0] != L'K')
            return 14;
    }

    wcsncpy(buf, L"ab", 4);
    if (buf[0] != L'a' || buf[1] != L'b' || buf[2] != 0 || buf[3] != 0)
        return 15;
    wcscpy(buf, L"ab");
    one[0] = L'Z';
    if (wcsncat(buf, one, 0) != buf || wcscmp(buf, L"ab") != 0)
        return 16;

    n = mbstowcs(w, "ab", 8);
    if (n != 2 || w[0] != L'a' || w[1] != L'b' || w[2] != 0)
        return 17;
    if (mbstowcs(NULL, "ab", 0) != 2)
        return 18;
    if (wcstombs(bytes, w, 8) != 2 || strcmp(bytes, "ab") != 0)
        return 19;
    if (wcstombs(NULL, L"ab", 0) != 2)
        return 20;

    if (sprintf(out, "%ls", L"xy") != 2 || strcmp(out, "xy") != 0)
        return 21;
    if (sprintf(out, "%.1ls", one) != 1 || strcmp(out, "Z") != 0)
        return 22;
    if (sprintf(out, "%.0ls%s", one, "z") != 1 || strcmp(out, "z") != 0)
        return 23;
    if (sprintf(out, "%lc%s", (wint_t)L'Q', "z") != 2 || strcmp(out, "Qz") != 0)
        return 24;
    if (sprintf(out, "%S", L"ab") != 2 || strcmp(out, "ab") != 0)
        return 25;

    if (sscanf("ab", "%2ls", scanned) != 1)
        return 26;
    if (scanned[0] != L'a' || scanned[1] != L'b' || scanned[2] != 0)
        return 27;
    if (sscanf("Q", "%lc", &ch) != 1 || ch != L'Q')
        return 28;

    if (wlen(L"hi") != 2)
        return 29;

    /* Integer fill leaves a capability that shares these bytes. */
    x = 42;
    tmp = &x;
    *(int **)slots = tmp;
    bits = slots[0];
    wcscpy((wchar_t *)slots, L"a");
    slots[0] = bits;
    if (*(int **)slots != &x || **(int **)slots != 42)
        return 30;

    return 0;
}

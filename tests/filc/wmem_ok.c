#include <wchar.h>

int main(void)
{
    wchar_t buf[8];
    wchar_t src[8];
    wchar_t *p;
    int x, y;
    long slots[2];
    long bits;
    long src_slots[2];
    long dst_slots[2];
    long dbits;
    wchar_t *(*cp)(wchar_t *, const wchar_t *, size_t);

    src[0] = L'a';
    src[1] = L'b';
    src[2] = L'c';
    src[3] = L'd';
    src[4] = L'e';
    src[5] = L'f';

    p = wmemcpy(buf, src, 6);
    if (p != buf || buf[0] != L'a' || buf[5] != L'f')
        return 1;
    p[0] = L'A';
    if (buf[0] != L'A')
        return 2;

    p = wmemcpy(buf + 1, src, 2);
    if (p != buf + 1 || p[0] != L'a' || p[1] != L'b')
        return 3;
    p[-1] = L'Q';
    if (buf[0] != L'Q')
        return 4;

    buf[0] = L'a';
    buf[1] = L'b';
    buf[2] = L'c';
    buf[3] = L'd';
    buf[4] = L'e';
    buf[5] = L'f';
    p = wmemmove(buf + 2, buf, 4);
    if (p != buf + 2)
        return 5;
    if (buf[0] != L'a' || buf[1] != L'b' || buf[2] != L'a' ||
        buf[3] != L'b' || buf[4] != L'c' || buf[5] != L'd')
        return 6;

    p = wmemset(buf, L'Z', 3);
    if (p != buf || buf[0] != L'Z' || buf[1] != L'Z' || buf[2] != L'Z')
        return 7;
    if (buf[3] != L'b')
        return 8;

    if (wmemcmp(src, src, 6) != 0)
        return 9;
    {
        wchar_t a[2], b[2];
        a[0] = L'a';
        a[1] = L'b';
        b[0] = L'a';
        b[1] = L'c';
        if (wmemcmp(a, b, 1) != 0)
            return 10;
        if (wmemcmp(a, b, 2) == 0)
            return 11;
    }

    {
        wchar_t one[1];
        wchar_t dest[1];
        one[0] = L'Q';
        dest[0] = L'K';
        if (wmemcpy(dest, one, 0) != dest || dest[0] != L'K')
            return 12;
        if (wmemmove(dest, one, 0) != dest || dest[0] != L'K')
            return 13;
        if (wmemset(dest, L'Z', 0) != dest || dest[0] != L'K')
            return 14;
        if (wmemcmp(one, dest, 0) != 0)
            return 15;
    }

    /* Integer fill leaves a capability that shares these bytes. */
    x = 42;
    *(int **)slots = &x;
    bits = slots[0];
    wmemcpy((wchar_t *)slots, src, 2);
    slots[0] = bits;
    if (*(int **)slots != &x || **(int **)slots != 42)
        return 16;

    /* A source capability must not replace the destination's. */
    y = 7;
    *(int **)src_slots = &y;
    *(int **)dst_slots = &x;
    dbits = dst_slots[0];
    wmemcpy((wchar_t *)dst_slots, (wchar_t *)src_slots, 2);
    dst_slots[0] = dbits;
    if (*(int **)dst_slots != &x || **(int **)dst_slots != 42)
        return 17;

    cp = wmemcpy;
    if (cp(buf, src, 2) != buf || buf[0] != L'a' || buf[1] != L'b')
        return 18;

    return 0;
}
